#include "kernel.hpp"

static gemm_opt_o16_t saturate_i16(int64_t value) {
#pragma HLS INLINE
    if (value > 32767) {
        return 32767;
    }
    if (value < -32768) {
        return -32768;
    }
    return static_cast<gemm_opt_o16_t>(value);
}

static gemm_opt_o16_t requantize_i16(gemm_opt_acc_t acc, int32_t scale_q15, int32_t shift) {
#pragma HLS INLINE
    int64_t scaled = static_cast<int64_t>(acc) * static_cast<int64_t>(scale_q15);
    if (shift > 0) {
        const int64_t rounding = static_cast<int64_t>(1) << (shift - 1);
        if (scaled >= 0) {
            scaled = (scaled + rounding) >> shift;
        } else {
            scaled = -(((-scaled) + rounding) >> shift);
        }
    }
    return saturate_i16(scaled);
}

extern "C" void int8_gemm_opt_kernel(
    const gemm_opt_i8_t input[GEMM_OPT_M][GEMM_OPT_K],
    const gemm_opt_i8_t weight[GEMM_OPT_K][GEMM_OPT_N],
    const gemm_opt_acc_t bias[GEMM_OPT_N],
    gemm_opt_o16_t output[GEMM_OPT_M][GEMM_OPT_N],
    int32_t scale_q15,
    int32_t shift) {
#pragma HLS INTERFACE m_axi port=input offset=slave bundle=gmem0 depth=GEMM_OPT_M * GEMM_OPT_K max_widen_bitwidth=512
#pragma HLS INTERFACE m_axi port=weight offset=slave bundle=gmem1 depth=GEMM_OPT_K * GEMM_OPT_N max_widen_bitwidth=512
#pragma HLS INTERFACE m_axi port=bias offset=slave bundle=gmem2 depth=GEMM_OPT_N max_widen_bitwidth=512
#pragma HLS INTERFACE m_axi port=output offset=slave bundle=gmem3 depth=GEMM_OPT_M * GEMM_OPT_N max_widen_bitwidth=512
#pragma HLS INTERFACE s_axilite port=input bundle=control
#pragma HLS INTERFACE s_axilite port=weight bundle=control
#pragma HLS INTERFACE s_axilite port=bias bundle=control
#pragma HLS INTERFACE s_axilite port=output bundle=control
#pragma HLS INTERFACE s_axilite port=scale_q15 bundle=control
#pragma HLS INTERFACE s_axilite port=shift bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    gemm_opt_i8_t input_buf[GEMM_OPT_K];
#pragma HLS ARRAY_PARTITION variable=input_buf cyclic factor=GEMM_OPT_K_PAR dim=1

    gemm_opt_i8_t weight_buf[GEMM_OPT_K][GEMM_OPT_N];
#pragma HLS BIND_STORAGE variable=weight_buf type=ram_2p impl=bram
#pragma HLS ARRAY_PARTITION variable=weight_buf cyclic factor=GEMM_OPT_OUT_PAR dim=2

    gemm_opt_acc_t bias_buf[GEMM_OPT_N];
#pragma HLS BIND_STORAGE variable=bias_buf type=ram_2p impl=bram
#pragma HLS ARRAY_PARTITION variable=bias_buf cyclic factor=GEMM_OPT_OUT_PAR dim=1

    gemm_opt_acc_t acc[GEMM_OPT_OUT_PAR];
#pragma HLS ARRAY_PARTITION variable=acc complete dim=1

load_bias_loop:
    for (int n = 0; n < GEMM_OPT_N; ++n) {
#pragma HLS PIPELINE II=1
        bias_buf[n] = bias[n];
    }

load_weight_k_loop:
    for (int k = 0; k < GEMM_OPT_K; ++k) {
    load_weight_n_loop:
        for (int n = 0; n < GEMM_OPT_N; ++n) {
#pragma HLS PIPELINE II=1
            weight_buf[k][n] = weight[k][n];
        }
    }

row_loop:
    for (int m = 0; m < GEMM_OPT_M; ++m) {
    load_input_loop:
        for (int k = 0; k < GEMM_OPT_K; ++k) {
#pragma HLS PIPELINE II=1
            input_buf[k] = input[m][k];
        }

    out_tile_loop:
        for (int n0 = 0; n0 < GEMM_OPT_N; n0 += GEMM_OPT_OUT_PAR) {
        init_acc_loop:
            for (int op = 0; op < GEMM_OPT_OUT_PAR; ++op) {
#pragma HLS UNROLL
                acc[op] = bias_buf[n0 + op];
            }

        k_loop:
            for (int k = 0; k < GEMM_OPT_K; ++k) {
#pragma HLS PIPELINE II=1
                const gemm_opt_acc_t a_value = static_cast<gemm_opt_acc_t>(input_buf[k]);
            compute_op_loop:
                for (int op = 0; op < GEMM_OPT_OUT_PAR; ++op) {
#pragma HLS UNROLL
                    acc[op] += a_value * static_cast<gemm_opt_acc_t>(weight_buf[k][n0 + op]);
                }
            }

        store_loop:
            for (int op = 0; op < GEMM_OPT_OUT_PAR; ++op) {
#pragma HLS UNROLL
                output[m][n0 + op] = requantize_i16(acc[op], scale_q15, shift);
            }
        }
    }
}
