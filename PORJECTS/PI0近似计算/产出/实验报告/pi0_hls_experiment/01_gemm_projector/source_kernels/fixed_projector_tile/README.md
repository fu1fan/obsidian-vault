# Fixed-Point Visual Projector Tile HLS Benchmark

This optional kernel binds the HLS experiment directly to pi0-style visual projection. It is still a tile-level benchmark, not a full pi0/VLA deployment.

## Tile Shape

- Tokens: `64`
- Input dimension: `1152`
- Output tile dimension: `256`
- Mapping: visual projector tile `[64,1152] -> [64,256]`
- Input/weight/bias/output type: `ap_fixed<16,6>`
- Accumulator: `ap_fixed<40,16>`

## Top Function

`fixed_projector_tile_kernel`

## Local C++ Smoke Test

```bash
g++ -std=c++17 -DHLS_NO_AP_FIXED kernel.cpp tb.cpp -o /tmp/fixed_projector_tile_tb
/tmp/fixed_projector_tile_tb
```

## Self-Managed Batch Flow

```bash
python scripts/run_all_hls.py --kernels fixed_projector_tile --skip-synth
python scripts/run_all_hls.py --kernels fixed_projector_tile
```
