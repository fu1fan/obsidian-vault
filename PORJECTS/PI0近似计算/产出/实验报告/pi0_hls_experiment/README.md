# pi0 HLS PPT 实验材料包

这个包用于快速制作 HLS / FPGA 相关 PPT，内容聚焦 Vitis HLS tile-level kernel 实验。它不包含完整 Vitis raw reports，也不包含 Vitis workspace 或构建缓存；包内保留 Markdown 报告、CSV 汇总、kernel 源码和每个 kernel 的 `run_status.json`，便于答辩时追溯。

## 推荐 PPT 叙事顺序

1. 先说明边界：HLS 结果是 tile-level C-sim / C-synthesis，不是完整 pi0 FPGA 部署，也不是板级实测。
2. 从 GEMM/projector 开始：这类模块是 VLA/pi0 中的主要吞吐瓶颈，优化重点是 memory placement、local buffer、accumulator partition 和输出通道并行。
3. 再讲 nonlinear kernel：softmax、GELU、RMSNorm，对应 PyTorch 实验中筛出的硬件候选。
4. 用 before/after 对比说明资源和延迟 tradeoff，而不是只讲误差。
5. 总结可落地路线：优先 INT8 GEMM/projector、LUT softmax、PWL GELU 和 RMSNorm rsqrt，但完整系统部署还需要内存编排和板级验证。

## 核心结论

- `int8_gemm_opt` p16k1 从 `1,742,214` cycles 降到 `220,287` cycles，约 `7.909x` cycle speedup，C-sim exact match。
- `fixed_projector_tile_opt_p16k8` 从 `37,783,054` cycles 降到 `577,774` cycles，约 `65.39x` speedup，但 DSP 增加到 `512`。
- `lut_softmax` 相比 float exp softmax cycle 略多，但 estimated clock 更好，estimated-time speedup `1.919x`，DSP 从 `14` 降到 `2`。
- `gelu_pwl` 相比 float tanh GELU 大幅减少资源：DSP `60 -> 2`，LUT `7854 -> 2301`，cosine `0.999995`。
- `rmsnorm_rsqrt` 精度很好，cosine `>0.9999998`，estimated-time speedup `2.089x`，但当前实现 LUT/DSP 增加，需要继续优化资源。

## 目录说明

- `00_overview.md`：HLS 总览和 PPT 讲法边界。
- `01_gemm_projector/report.md`：INT8 GEMM 与 fixed projector latency optimization。
- `02_softmax/report.md`：exact softmax 与 LUT softmax 对比。
- `03_gelu/report.md`：exact GELU 与 PWL GELU 对比。
- `04_rmsnorm/report.md`：exact RMSNorm 与 rsqrt RMSNorm 对比。
- `common/`：HLS 汇总 Markdown 和 CSV。
- 每个模块目录下的 `csv/`、`source_kernels/`、`run_status/` 分别保存结果表、源码和运行状态。

## 不能过度声称的内容

- 不要说这是完整 pi0 FPGA 部署。
- 不要说这是板级实测 latency 或 power。
- 不要说这些 kernel 已经完成系统级 memory orchestration。
- 正确表述是：这些 HLS 实验验证了模块级 kernel 的 C-sim 数值正确性、C-synthesis 可综合性、估算 latency/II/clock 和资源 tradeoff。
