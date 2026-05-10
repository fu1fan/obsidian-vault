# GELU HLS 实验报告

## 1. 实验目的

GELU 是 Transformer FFN 中常见 activation。float tanh GELU 在硬件中包含较重 nonlinear 计算，因此该实验比较 exact tanh GELU baseline 与 fixed-point PWL GELU，评估 PWL 近似的资源收益和数值误差。

## 2. Kernel 内容

- `exact_gelu`：float tanh GELU baseline。
- `gelu_pwl`：fixed-point piecewise-linear GELU approximation。

测试 shape：`len4096`。

## 3. HLS 过程

1. 使用 Python golden reference 生成对照。
2. C++ testbench 检查 PWL GELU 输出误差。
3. Vitis HLS C-synthesis 收集 latency、II、clock 和资源。
4. 与 exact GELU baseline 做 before/after 对比。

## 4. 关键结果

### exact_gelu baseline

- latency：`4,159` cycles
- II：`4096`
- estimated clock：`7.300 ns`
- LUT：`7854`
- FF：`8067`
- BRAM：`6`
- DSP：`60`
- cosine：`1.0`

### gelu_pwl

- latency：`4,114` cycles
- II：`4096`
- estimated clock：`7.300 ns`
- LUT：`2301`
- FF：`1806`
- BRAM：`1`
- DSP：`2`
- MSE：`3.836425e-05`
- cosine：`0.999995`
- relative L2：`3.070151e-03`

资源变化：

- LUT 减少约 `70.7%`
- FF 减少约 `77.6%`
- BRAM 减少约 `83.3%`
- DSP 从 `60` 降到 `2`，减少约 `96.7%`

## 5. 结论

- PWL GELU 的 latency 与 exact GELU 接近，但资源收益非常明显。
- PWL GELU 保持较高 cosine，模块级误差较低。
- 对 FPGA activation kernel 来说，PWL GELU 是比 float tanh GELU 更合适的硬件候选。

## 6. 限制

- 该实验只验证 activation kernel 本身，不等同于完整 FFN 输出质量。
- PyTorch 真实权重实验显示 activation replacement 应在完整 FFN 上评估；HLS PWL GELU 后续应结合 FFN tile 做系统验证。
- HLS 数据是 C-synthesis estimate，不是板级实测。

## 7. PPT 可用讲法

可以说：

> PWL GELU 的价值主要是资源下降，尤其 DSP 从 60 降到 2，同时 cosine 仍接近 1。

不要说：

> 任何 activation 都可以直接替换成线性函数。

## 8. 结果文件

- CSV：`csv/hls_kernel_summary.csv`、`csv/hls_optimization_comparison.csv`
- 源码：`source_kernels/`
- 运行状态：`run_status/`
