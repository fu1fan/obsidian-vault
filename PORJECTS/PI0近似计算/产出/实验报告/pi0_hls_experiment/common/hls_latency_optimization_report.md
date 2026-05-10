# HLS Latency Optimization Report

This report covers tile-level Vitis HLS kernels only. It is not a full pi0 FPGA deployment and not an end-to-end VLA run. Latency, II, clock, and resource numbers are Vitis HLS estimates, not board measurements.

## Before/After Summary

| kernel | variant | OUT_PAR | K_PAR | latency_cycles | II | estimated_clock_ns | estimated_time_ms | LUT | FF | BRAM | DSP | MSE | cosine | speedup_vs_baseline |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| fixed_projector_tile | baseline | NA | NA | 37783054 | 2 | 7.300 | 275.816 | 5477 | 3510 | 19 | 4 | 3.178747272625e-07 | 0.999965279440 | 1 |
| fixed_projector_tile_opt_p4k4 | p4k4 | 4 | 4 | 1901710 | 1 | 7.300 | 13.8825 | 7240 | 6035 | 35 | 64 | 3.178747272625e-07 | 0.999965279440 | 19.87 |
| fixed_projector_tile_opt_p8k4 | p8k4 | 8 | 4 | 1143502 | 1 | 7.300 | 8.34756 | 9636 | 8514 | 35 | 128 | 3.178747272625e-07 | 0.999965279440 | 33.04 |
| fixed_projector_tile_opt_p8k8 | p8k8 | 8 | 8 | 770254 | 1 | 7.300 | 5.62285 | 13266 | 12882 | 67 | 256 | 3.178747272625e-07 | 0.999965279440 | 49.05 |
| fixed_projector_tile_opt_p16k8 | p16k8 | 16 | 8 | 577774 | 1 | 7.300 | 4.21775 | 21549 | 22148 | 67 | 512 | 3.178747272625e-07 | 0.999965279440 | 65.39 |
| int8_gemm | baseline | NA | NA | 1742214 | 1 | 7.300 | 12.7182 | 6953 | 4024 | 19 | 5 | 0.000000000000e+00 | 1.000000000000 | 1 |
| int8_gemm_opt | p16k1 | 16 | 1 | 220287 | 1 | 7.300 | 1.6081 | 18132 | 6659 | 35 | 64 | 0.000000000000e+00 | 1.000000000000 | 7.909 |
| int8_gemm_opt | p16k4_axi_weight_tile | 16 | 4 | 1642364 | 64 | 7.300 | 11.9893 | 17794 | 6778 | 3 | 36 | 0.000000000000e+00 | 1.000000000000 | 1.061 |

## Baseline Bottlenecks

- `fixed_projector_tile` was dominated by the token loop: the reported top latency was 37,783,054 cycles, while the inner `in_loop_out_loop` ran once per token with achieved II=2 and only 4 DSP. The design reread weights through AXI in the compute loop and did not keep a fully partitioned output accumulator tile.
- `int8_gemm` already had an inner loop at II=1, but it serialized 1024 output channels for each row and used only 5 DSP, so the total latency stayed at 1,742,214 cycles.

## Optimizations Applied

- `fixed_projector_tile_opt_*` reorders the projector computation into output-channel blocks, preloads input rows, uses local weight tiles, fully partitions the output accumulator tile, and unrolls output/K lanes.
- Four fixed projector settings were synthesized: OUT_PAR/K_PAR = 4/4, 8/4, 8/8, and 16/8.
- `int8_gemm_opt` first tried a 16x4 AXI-loaded weight tile. That raised DSP usage but achieved II=64, so it was discarded. The final version preloads full weight and bias tiles into partitioned local buffers, then runs a 16-lane output MAC loop at achieved II=1.

## Effective Results

- Best fixed projector result: `fixed_projector_tile_opt_p16k8` reached 577774 cycles, 65.39x faster than baseline, with DSP 512 and BRAM 67.
- INT8 GEMM result: `int8_gemm_opt` variant `p16k1` reached 220287 cycles, 7.909x faster than baseline, with exact C-sim agreement (MSE 0.000000000000e+00, cosine 1.000000000000).
- No optimized kernel in this run failed synthesis. The first INT8 p16k4 design was kept as a discarded comparison because its memory placement prevented the added MAC lanes from improving latency.

## Recommendation

- Use `fixed_projector_tile_opt_p16k8` when latency is the priority and 512 DSP is acceptable.
- Use `fixed_projector_tile_opt_p8k8` as a lower-DSP alternative: it still gives a large latency reduction with 256 DSP.
- Use `int8_gemm_opt` p16k1 for the action-projection-style INT8 tile; the useful optimization is local weight/bias reuse, not just K unrolling.

## Limits

- These are tile-level kernels. They do not model full pi0 memory orchestration, host/kernel launch overhead, inter-kernel scheduling, or board-level timing closure.
- Flow step reduction remains an algorithm-level approximation and is not represented as an HLS kernel.
