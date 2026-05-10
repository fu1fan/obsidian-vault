#ifndef PI0_APPROX_VLA_FIXED_PROJECTOR_TILE_OPT_KERNEL_HPP
#define PI0_APPROX_VLA_FIXED_PROJECTOR_TILE_OPT_KERNEL_HPP

#ifndef PROJ_TOKENS
#define PROJ_TOKENS 64
#endif

#ifndef PROJ_IN_DIM
#define PROJ_IN_DIM 1152
#endif

#ifndef PROJ_OUT_DIM
#define PROJ_OUT_DIM 256
#endif

#ifndef PROJ_OPT_OUT_PAR
#define PROJ_OPT_OUT_PAR 8
#endif

#ifndef PROJ_OPT_K_PAR
#define PROJ_OPT_K_PAR 4
#endif

#if (PROJ_OUT_DIM % PROJ_OPT_OUT_PAR) != 0
#error "PROJ_OUT_DIM must be divisible by PROJ_OPT_OUT_PAR"
#endif

#if (PROJ_IN_DIM % PROJ_OPT_K_PAR) != 0
#error "PROJ_IN_DIM must be divisible by PROJ_OPT_K_PAR"
#endif

#ifdef HLS_NO_AP_FIXED
typedef float proj_opt_t;
typedef float proj_opt_acc_t;
#else
#include <ap_fixed.h>
typedef ap_fixed<16, 6> proj_opt_t;
typedef ap_fixed<40, 16> proj_opt_acc_t;
#endif

extern "C" void fixed_projector_tile_opt_kernel(
    const proj_opt_t input[PROJ_TOKENS][PROJ_IN_DIM],
    const proj_opt_t weight[PROJ_IN_DIM][PROJ_OUT_DIM],
    const proj_opt_t bias[PROJ_OUT_DIM],
    proj_opt_t output[PROJ_TOKENS][PROJ_OUT_DIM]);

#endif
