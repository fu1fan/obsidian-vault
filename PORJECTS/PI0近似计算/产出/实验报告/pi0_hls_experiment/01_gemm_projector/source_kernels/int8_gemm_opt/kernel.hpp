#ifndef PI0_APPROX_VLA_INT8_GEMM_OPT_KERNEL_HPP
#define PI0_APPROX_VLA_INT8_GEMM_OPT_KERNEL_HPP

#include <stdint.h>

#ifndef GEMM_OPT_M
#define GEMM_OPT_M 50
#endif

#ifndef GEMM_OPT_K
#define GEMM_OPT_K 32
#endif

#ifndef GEMM_OPT_N
#define GEMM_OPT_N 1024
#endif

#ifndef GEMM_OPT_OUT_PAR
#define GEMM_OPT_OUT_PAR 16
#endif

#ifndef GEMM_OPT_K_PAR
#define GEMM_OPT_K_PAR 1
#endif

#if (GEMM_OPT_N % GEMM_OPT_OUT_PAR) != 0
#error "GEMM_OPT_N must be divisible by GEMM_OPT_OUT_PAR"
#endif

#if (GEMM_OPT_K % GEMM_OPT_K_PAR) != 0
#error "GEMM_OPT_K must be divisible by GEMM_OPT_K_PAR"
#endif

typedef int8_t gemm_opt_i8_t;
typedef int16_t gemm_opt_o16_t;
typedef int32_t gemm_opt_acc_t;

extern "C" void int8_gemm_opt_kernel(
    const gemm_opt_i8_t input[GEMM_OPT_M][GEMM_OPT_K],
    const gemm_opt_i8_t weight[GEMM_OPT_K][GEMM_OPT_N],
    const gemm_opt_acc_t bias[GEMM_OPT_N],
    gemm_opt_o16_t output[GEMM_OPT_M][GEMM_OPT_N],
    int32_t scale_q15,
    int32_t shift);

#endif
