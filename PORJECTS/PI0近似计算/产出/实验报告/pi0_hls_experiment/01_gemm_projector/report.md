# GEMM / Projector HLS 实验报告

## 1. 实验目的

GEMM 和 projector 是 VLA/pi0 类模型中最核心的吞吐模块之一，覆盖 visual projector、QKV/O projection、FFN、state/action projection 等线性计算。该实验验证 INT8 GEMM tile 和 fixed-point projector tile 在 HLS 中的可综合性，并进一步评估 latency optimization 的效果。

## 2. Kernel 内容

本组包含四类 kernel：

- `int8_gemm`：基础 INT8 activation/weight GEMM tile。
- `int8_gemm_opt`：使用 local weight/bias buffer 和 16-lane output parallelism 的优化版本。
- `fixed_projector_tile`：fixed-point visual projector tile，shape 为 `64x1152x256`。
- `fixed_projector_tile_opt_*`：projector 的 OUT_PAR/K_PAR sweep，包括 p4k4、p8k4、p8k8、p16k8。

## 3. HLS 过程

1. 为每个 kernel 提供 C++ kernel、testbench 和 Python golden reference。
2. 运行 C simulation，检查输出与 golden reference 的 MSE、MAE、cosine、relative L2。
3. 运行 Vitis HLS C-synthesis，收集 latency cycles、II、estimated clock、LUT、FF、BRAM、DSP。
4. 对 GEMM/projector 优化版本比较 baseline 与 optimized variant 的 speedup 和资源代价。

## 4. 关键结果

### INT8 GEMM baseline

- kernel：`int8_gemm`
- shape：`50x32x1024`
- latency：`1,742,214` cycles
- estimated clock：`7.300 ns`
- DSP：`5`
- MSE：`0`
- cosine：`1.0`

解释：baseline 已经 C-sim exact match，但输出通道计算序列化较多，DSP 使用很低，latency 仍然较高。

### INT8 GEMM optimized

- kernel：`int8_gemm_opt`
- variant：p16k1
- latency：`220,287` cycles
- speedup vs baseline：`7.909x`
- estimated clock：`7.300 ns`
- DSP：`64`
- MSE：`0`
- cosine：`1.0`

解释：有效优化来自 full weight/bias tile preload、partitioned local buffers 和 16-lane output MAC loop。一个早期 p16k4 AXI-loaded weight tile 尝试虽然增加 DSP，但因为 AXI weight-tile load 导致 II=64，被保留为 discarded comparison。

### Fixed Projector baseline

- kernel：`fixed_projector_tile`
- shape：`64x1152x256`
- latency：`37,783,054` cycles
- estimated clock：`7.300 ns`
- DSP：`4`
- cosine：`0.999965`
- relative L2：`8.333756e-03`

解释：baseline 主要受权重 AXI 访问和低并行度限制。

### Fixed Projector optimized

- best latency variant：`fixed_projector_tile_opt_p16k8`
- OUT_PAR/K_PAR：`16/8`
- latency：`577,774` cycles
- speedup vs baseline：`65.39x`
- estimated time：`4.21775 ms`
- DSP：`512`
- BRAM：`67`
- cosine：`0.999965`
- relative L2：`8.333756e-03`

低 DSP 替代方案：

- `fixed_projector_tile_opt_p8k8`
- latency：`770,254` cycles
- speedup vs baseline：`49.05x`
- DSP：`256`

## 5. 结论

- GEMM/projector 的性能瓶颈主要来自 memory placement 和并行组织，而不是只增加乘法器数量。
- 本地 weight/bias buffer、accumulator partition 和 output-channel parallelism 是有效优化手段。
- `int8_gemm_opt` p16k1 是 action-projection-style INT8 tile 的推荐实现。
- fixed projector 若以 latency 为优先，可选 p16k8；若控制 DSP，可选 p8k8。

## 6. 限制

- 这是 tile-level kernel，不包含 full pi0 memory orchestration。
- 没有 host/kernel launch overhead、DDR bandwidth contention、inter-kernel scheduling 或 board-level timing closure。
- 资源和 latency 是 Vitis HLS estimate，不是板级实测。

## 7. PPT 可用讲法

可以说：

> GEMM/projector 的优化关键在于把权重和累加器搬到合适的本地结构，并展开输出通道并行；仅仅增加 MAC lanes，如果仍然从 AXI 内层加载权重，latency 不会明显下降。

不要说：

> 这个结果就是完整 pi0 在 FPGA 上的推理延迟。

## 8. 结果文件

- CSV：`csv/hls_latency_optimization.csv`
- 源码：`source_kernels/`
- 运行状态：`run_status/`
