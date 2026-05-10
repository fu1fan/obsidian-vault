# INT8 Tiled GEMM HLS Benchmark

This kernel is a module-level benchmark for pi0-style projection and Transformer linear layers. It does not deploy pi0 end to end.

## Default Tile

- Mapping: action projection tile, `[1,50,32] -> [1,50,1024]`
- Compile-time shape: `GEMM_M=50`, `GEMM_K=32`, `GEMM_N=1024`
- Input: INT8 activation
- Weight: INT8 weight
- Bias / accumulator: INT32
- Output: INT16 after affine requantization

The same source can be rebuilt with macros for larger pi0-like tiles:

- Visual projector tile: `GEMM_M=64`, `GEMM_K=1152`, `GEMM_N=256`
- FFN tile: `GEMM_M=128`, `GEMM_K=2048`, `GEMM_N=512`

## Top Function

`int8_gemm_kernel`

## Local C++ Smoke Test

```bash
g++ -std=c++17 kernel.cpp tb.cpp -o /tmp/int8_gemm_tb
/tmp/int8_gemm_tb
```

## Self-Managed Batch Flow

The repository does not track a Vitis Unified component wrapper. The runner
generates a temporary component under `hls_exp/.hls_work/int8_gemm/`
with `syn.top=int8_gemm_kernel`.

```bash
python scripts/run_all_hls.py --kernels int8_gemm --skip-synth
python scripts/run_all_hls.py --kernels int8_gemm
```

Expected C-sim metric line:

```text
HLS_METRIC kernel=int8_gemm dtype=int8_acc32_out16 shape=50x32x1024 ...
```
