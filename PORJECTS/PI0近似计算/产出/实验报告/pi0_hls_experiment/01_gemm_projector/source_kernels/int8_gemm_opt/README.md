# Optimized INT8 GEMM HLS Benchmark

This kernel keeps the original INT8 GEMM benchmark shape but adds local buffering and output/K parallelism. It is a module-level action-projection tile benchmark, not a full pi0 deployment.

## Shape And Type

- Shape: `GEMM_OPT_M=50`, `GEMM_OPT_K=32`, `GEMM_OPT_N=1024`
- Input/weight: INT8
- Bias/accumulator: INT32
- Output: INT16 after the same requantization as `int8_gemm`
- Parallelism: `GEMM_OPT_OUT_PAR=16`, `GEMM_OPT_K_PAR=1`

## Optimization

- Preload the full weight tile and bias vector once into local partitioned buffers.
- Preload one input row into a local K buffer.
- Process output channels in compile-time tiles.
- Completely partition the output accumulator tile.
- Reuse local weights across all 50 rows instead of rereading them from AXI.
- Pipeline the K loop while unrolling 16 output-channel MAC lanes.

## Top Function

`int8_gemm_opt_kernel`

## Local C++ Smoke Test

```bash
g++ -std=c++17 kernel.cpp tb.cpp -o /tmp/int8_gemm_opt_tb
/tmp/int8_gemm_opt_tb
```

## Self-Managed Batch Flow

The repository does not track a Vitis Unified component wrapper. The runner
generates a temporary component under `hls_exp/.hls_work/int8_gemm_opt/`
with `syn.top=int8_gemm_opt_kernel` and the configured macro defines.

```bash
python scripts/run_all_hls.py --kernels int8_gemm_opt --skip-synth
python scripts/run_all_hls.py --kernels int8_gemm_opt
```
