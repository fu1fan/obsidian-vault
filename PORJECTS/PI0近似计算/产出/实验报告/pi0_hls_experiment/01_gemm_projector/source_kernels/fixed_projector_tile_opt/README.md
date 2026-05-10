# Optimized Fixed-Point Visual Projector Tile HLS Benchmark

This kernel is an optimized variant of `fixed_projector_tile`. It keeps the same
tile-level visual projector shape and numeric type, but reorganizes the loop
nest to improve HLS parallelism.

## Tile Shape

- Tokens: `64`
- Input dimension: `1152`
- Output tile dimension: `256`
- Mapping: visual projector tile `[64,1152] -> [64,256]`
- Input/weight/bias/output type: `ap_fixed<16,6>`
- Accumulator: `ap_fixed<40,16>`

## Top Function

`fixed_projector_tile_opt_kernel`

## Tunable Macros

- `PROJ_OPT_OUT_PAR`: number of output channels computed in parallel.
- `PROJ_OPT_K_PAR`: number of K elements reduced per compute step.

Requested experiment variants:

- `OUT_PAR=4`, `K_PAR=4`
- `OUT_PAR=8`, `K_PAR=4`
- `OUT_PAR=8`, `K_PAR=8`
- `OUT_PAR=16`, `K_PAR=8`

## Optimization Strategy

- Preload the full input tile into local BRAM once.
- Process one output-channel tile at a time.
- Load a local `K_PAR x OUT_PAR` weight tile before compute.
- Keep a `tokens x OUT_PAR` accumulator tile with output dimension completely
  partitioned.
- Unroll output and K inner loops so DSP use can scale with `OUT_PAR * K_PAR`.
- Pipeline load/compute/store loops and avoid global AXI reads directly inside
  the MAC body.

This remains a module-level benchmark, not a full pi0 FPGA deployment.

## Self-Managed Batch Flow

The runner materializes the requested macro variants in ignored temporary work
directories under `hls_exp/.hls_work/`.

```bash
python scripts/run_all_hls.py --kernels fixed_projector_tile_opt_p4k4 fixed_projector_tile_opt_p8k4 fixed_projector_tile_opt_p8k8 fixed_projector_tile_opt_p16k8 --skip-synth
python scripts/run_all_hls.py --kernels fixed_projector_tile_opt_p16k8
```
