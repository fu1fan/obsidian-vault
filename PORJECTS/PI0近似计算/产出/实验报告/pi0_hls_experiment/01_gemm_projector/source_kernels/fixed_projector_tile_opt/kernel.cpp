#include "kernel.hpp"

extern "C" void fixed_projector_tile_opt_kernel(
    const proj_opt_t input[PROJ_TOKENS][PROJ_IN_DIM],
    const proj_opt_t weight[PROJ_IN_DIM][PROJ_OUT_DIM],
    const proj_opt_t bias[PROJ_OUT_DIM],
    proj_opt_t output[PROJ_TOKENS][PROJ_OUT_DIM]) {
#pragma HLS INTERFACE m_axi port=input offset=slave bundle=gmem0 depth=PROJ_TOKENS * PROJ_IN_DIM max_widen_bitwidth=512
#pragma HLS INTERFACE m_axi port=weight offset=slave bundle=gmem1 depth=PROJ_IN_DIM * PROJ_OUT_DIM max_widen_bitwidth=512
#pragma HLS INTERFACE m_axi port=bias offset=slave bundle=gmem2 depth=PROJ_OUT_DIM max_widen_bitwidth=512
#pragma HLS INTERFACE m_axi port=output offset=slave bundle=gmem3 depth=PROJ_TOKENS * PROJ_OUT_DIM max_widen_bitwidth=512
#pragma HLS INTERFACE s_axilite port=input bundle=control
#pragma HLS INTERFACE s_axilite port=weight bundle=control
#pragma HLS INTERFACE s_axilite port=bias bundle=control
#pragma HLS INTERFACE s_axilite port=output bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    proj_opt_t input_buf[PROJ_TOKENS][PROJ_IN_DIM];
#pragma HLS BIND_STORAGE variable=input_buf type=ram_2p impl=bram
#pragma HLS ARRAY_PARTITION variable=input_buf cyclic factor=PROJ_OPT_K_PAR dim=2

    proj_opt_acc_t acc_buf[PROJ_TOKENS][PROJ_OPT_OUT_PAR];
#pragma HLS ARRAY_PARTITION variable=acc_buf complete dim=2

    proj_opt_t weight_tile[PROJ_OPT_K_PAR][PROJ_OPT_OUT_PAR];
#pragma HLS ARRAY_PARTITION variable=weight_tile complete dim=0

load_input_token_loop:
    for (int t = 0; t < PROJ_TOKENS; ++t) {
    load_input_k_loop:
        for (int k = 0; k < PROJ_IN_DIM; ++k) {
#pragma HLS PIPELINE II=1
            input_buf[t][k] = input[t][k];
        }
    }

out_block_loop:
    for (int ob = 0; ob < PROJ_OUT_DIM; ob += PROJ_OPT_OUT_PAR) {
    init_token_loop:
        for (int t = 0; t < PROJ_TOKENS; ++t) {
#pragma HLS PIPELINE II=1
        init_out_loop:
            for (int op = 0; op < PROJ_OPT_OUT_PAR; ++op) {
#pragma HLS UNROLL
                acc_buf[t][op] = static_cast<proj_opt_acc_t>(bias[ob + op]);
            }
        }

    k_block_loop:
        for (int kb = 0; kb < PROJ_IN_DIM; kb += PROJ_OPT_K_PAR) {
        load_weight_flat_loop:
            for (int idx = 0; idx < PROJ_OPT_K_PAR * PROJ_OPT_OUT_PAR; ++idx) {
#pragma HLS PIPELINE II=1
                const int kp = idx / PROJ_OPT_OUT_PAR;
                const int op = idx - kp * PROJ_OPT_OUT_PAR;
                weight_tile[kp][op] = weight[kb + kp][ob + op];
            }

        compute_token_loop:
            for (int t = 0; t < PROJ_TOKENS; ++t) {
#pragma HLS PIPELINE II=1
                proj_opt_t input_tile[PROJ_OPT_K_PAR];
#pragma HLS ARRAY_PARTITION variable=input_tile complete dim=1
            load_input_tile_loop:
                for (int kp = 0; kp < PROJ_OPT_K_PAR; ++kp) {
#pragma HLS UNROLL
                    input_tile[kp] = input_buf[t][kb + kp];
                }

            compute_out_loop:
                for (int op = 0; op < PROJ_OPT_OUT_PAR; ++op) {
#pragma HLS UNROLL
                    proj_opt_acc_t acc = acc_buf[t][op];
                compute_k_loop:
                    for (int kp = 0; kp < PROJ_OPT_K_PAR; ++kp) {
#pragma HLS UNROLL
                        acc += static_cast<proj_opt_acc_t>(input_tile[kp]) *
                               static_cast<proj_opt_acc_t>(weight_tile[kp][op]);
                    }
                    acc_buf[t][op] = acc;
                }
            }
        }

    store_token_loop:
        for (int t = 0; t < PROJ_TOKENS; ++t) {
#pragma HLS PIPELINE II=1
        store_out_loop:
            for (int op = 0; op < PROJ_OPT_OUT_PAR; ++op) {
#pragma HLS UNROLL
                output[t][ob + op] = static_cast<proj_opt_t>(acc_buf[t][op]);
            }
        }
    }
}
