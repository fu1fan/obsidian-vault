# Vitis HLS Kernel Benchmark Summary

This report is module-level only. It does not deploy full pi0 and does not run end-to-end VLA inference.

PyTorch/Python experiments provide module error trends with random or real pi0 weights. HLS kernels provide tile-level implementability evidence: C-sim numerical checks, synthesis status, and report-derived latency/resource fields when synthesis succeeds.

## Kernel Mapping

- `int8_gemm`: projector / QKV / FFN / state-action projection GEMM tile.
- `int8_gemm_opt`: output/K-parallel INT8 GEMM latency optimization variant.
- `exact_softmax`: float exp softmax baseline for before/after comparison.
- `lut_softmax`: attention score normalization.
- `exact_gelu`: float tanh GELU baseline for before/after comparison.
- `gelu_pwl`: FFN activation approximation.
- `exact_rmsnorm`: float sqrt RMSNorm baseline for before/after comparison.
- `rmsnorm_rsqrt`: Transformer RMSNorm reciprocal-square-root approximation.
- `fixed_projector_tile`: optional fixed-point visual projector tile.
- `fixed_projector_tile_opt_*`: output/K-parallel projector latency optimization variants.

Flow step reduction is an algorithm-level approximation and is intentionally not represented as an HLS kernel.

## Results

| kernel | variant | comparison_group | role | data_type | shape | OUT_PAR | K_PAR | latency_cycles | ii | estimated_clock_ns | LUT | FF | BRAM | DSP | mse | mae | kl | cosine | relative_l2 | hls_synth_status | parse_status |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| exact_gelu | tanh | gelu | baseline | float32 | len4096 | NA | NA | 4159 | 4096 | 7.300 | 7854 | 8067 | 6 | 60 | 5.657816161014e-15 | 4.558468581405e-08 | NA | 1.000000000000 | 3.727853147564e-08 | passed | ok |
| exact_rmsnorm | sqrt | rmsnorm | baseline | float32 | hidden1024 | NA | NA | 2094 | 2095 | 15.145 | 3657 | 3334 | 2 | 10 | 8.495700724505e-15 | 7.512825617250e-08 | NA | 1.000000000000 | 9.238011305043e-08 | passed | ok |
| exact_softmax | exp | softmax | baseline | float32 | rows4_len128 | NA | NA | 1727 | 1728 | 15.145 | 4133 | 2892 | 9 | 14 | 6.270411025943e-18 | 1.072921340824e-09 | -5.427590331548e-08 | 0.999999999993 | 1.547688274752e-07 | passed | ok |
| fixed_projector_tile | default | projector | optimized | fixed16x6_acc40x16 | 64x1152x256 | NA | NA | 37783054 | 37783055 | 7.300 | 5477 | 3510 | 19 | 4 | 3.178747272625e-07 | 4.883789806627e-04 | NA | 0.999965279440 | 8.333756462524e-03 | passed | ok |
| fixed_projector_tile_opt_p16k8 | p16k8 | projector_latency | optimized | fixed16x6_acc40x16 | 64x1152x256 | 16 | 8 | 577774 | 577775 | 7.300 | 21549 | 22148 | 67 | 512 | 3.178747272625e-07 | 4.883789806627e-04 | NA | 0.999965279440 | 8.333756462524e-03 | passed | ok |
| fixed_projector_tile_opt_p4k4 | p4k4 | projector_latency | optimized | fixed16x6_acc40x16 | 64x1152x256 | 4 | 4 | 1901710 | 1901711 | 7.300 | 7240 | 6035 | 35 | 64 | 3.178747272625e-07 | 4.883789806627e-04 | NA | 0.999965279440 | 8.333756462524e-03 | passed | ok |
| fixed_projector_tile_opt_p8k4 | p8k4 | projector_latency | optimized | fixed16x6_acc40x16 | 64x1152x256 | 8 | 4 | 1143502 | 1143503 | 7.300 | 9636 | 8514 | 35 | 128 | 3.178747272625e-07 | 4.883789806627e-04 | NA | 0.999965279440 | 8.333756462524e-03 | passed | ok |
| fixed_projector_tile_opt_p8k8 | p8k8 | projector_latency | optimized | fixed16x6_acc40x16 | 64x1152x256 | 8 | 8 | 770254 | 770255 | 7.300 | 13266 | 12882 | 67 | 256 | 3.178747272625e-07 | 4.883789806627e-04 | NA | 0.999965279440 | 8.333756462524e-03 | passed | ok |
| gelu_pwl | default | gelu | optimized | fixed16x6 | len4096 | NA | NA | 4114 | 4096 | 7.300 | 2301 | 1806 | 1 | 2 | 3.836425107314e-05 | 3.405709433815e-03 | NA | 0.999995307692 | 3.070151235093e-03 | passed | ok |
| int8_gemm | default | gemm | optimized | int8_acc32_out16 | 50x32x1024 | NA | NA | 1742214 | 1742215 | 7.300 | 6953 | 4024 | 19 | 5 | 0.000000000000e+00 | 0.000000000000e+00 | NA | 1.000000000000 | 0.000000000000e+00 | passed | ok |
| int8_gemm_opt | p16k1 | gemm_latency | optimized | int8_acc32_out16 | 50x32x1024 | 16 | 1 | 220287 | 220288 | 7.300 | 18132 | 6659 | 35 | 64 | 0.000000000000e+00 | 0.000000000000e+00 | NA | 1.000000000000 | 0.000000000000e+00 | passed | ok |
| lut_softmax | default | softmax | optimized | fixed16x6_prob18x2 | rows4_len128 | NA | NA | 1867 | 1868 | 7.300 | 3692 | 2727 | 9 | 2 | 3.723352544601e-07 | 2.567741628471e-04 | 1.746296664206e-03 | 0.999311199515 | 3.771399742744e-02 | passed | ok |
| rmsnorm_rsqrt | nr1 | rmsnorm | optimized | fixed16x6_acc40x16 | hidden1024 | NA | NA | 2080 | 2081 | 7.300 | 5649 | 3857 | 2 | 64 | 2.950255249681e-07 | 4.613765880047e-04 | NA | 0.999999855028 | 5.446508053096e-04 | passed | ok |
| rmsnorm_rsqrt | nr2 | rmsnorm | optimized | fixed16x6_acc40x16 | hidden1024 | NA | NA | 2080 | 2081 | 7.300 | 5649 | 3857 | 2 | 64 | 3.014300453104e-07 | 4.730451477809e-04 | NA | 0.999999848934 | 5.505308032742e-04 | passed | ok |

## Failures Or Downgrades

- No HLS synthesis failures recorded.

## Artifacts

- CSV: `results/csv/hls_kernel_summary.csv`
- Per-kernel status and report copies: `results/hls_reports/`
- HLS source: `hls_exp/`
- Temporary self-managed HLS work dirs: `hls_exp/.hls_work/` (ignored, regenerated by `scripts/run_all_hls.py`)
