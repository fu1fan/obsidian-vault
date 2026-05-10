# RMSNorm HLS 实验报告

## 1. 实验目的

RMSNorm 是 Transformer 中常见 normalization。exact RMSNorm 使用 sqrt/rsqrt 相关计算，硬件中可能较贵。该实验比较 float sqrt RMSNorm baseline 与 LUT-initialized Newton-Raphson rsqrt RMSNorm，评估近似 rsqrt 的精度、estimated time 和资源 tradeoff。

## 2. Kernel 内容

- `exact_rmsnorm`：float sqrt RMSNorm baseline。
- `rmsnorm_rsqrt`：fixed-point RMSNorm，使用 LUT initial guess 和 Newton-Raphson rsqrt refinement。
- `rmsnorm_rsqrt` 包含 `nr1` 和 `nr2` 两个 branch。

测试 shape：`hidden1024`。

## 3. HLS 过程

1. C-sim 比较 exact/golden 输出与 rsqrt approximation 输出。
2. C-synthesis 收集 latency、II、estimated clock 和资源。
3. 对 `nr1`、`nr2` 分别比较误差与资源。

## 4. 关键结果

### exact_rmsnorm baseline

- latency：`2,094` cycles
- II：`2095`
- estimated clock：`15.145 ns`
- estimated time：`3.171363e+04 ns`
- LUT：`3657`
- DSP：`10`
- cosine：`1.0`

### rmsnorm_rsqrt nr1

- latency：`2,080` cycles
- II：`2081`
- estimated clock：`7.300 ns`
- estimated time：`1.518400e+04 ns`
- time speedup：`2.089x`
- LUT：`5649`
- DSP：`64`
- MSE：`2.950255e-07`
- cosine：`0.999999855`
- relative L2：`5.446508e-04`

### rmsnorm_rsqrt nr2

- latency：`2,080` cycles
- II：`2081`
- estimated clock：`7.300 ns`
- time speedup：`2.089x`
- MSE：`3.014300e-07`
- cosine：`0.999999849`
- relative L2：`5.505308e-04`

资源 tradeoff：

- LUT：`3657 -> 5649`
- DSP：`10 -> 64`
- BRAM：保持 `2`

## 5. 结论

- RMSNorm rsqrt 的数值精度非常好，cosine 大于 `0.9999998`。
- estimated clock 从 `15.145 ns` 改善到 `7.300 ns`，estimated-time speedup 约 `2.089x`。
- 当前实现用更多 LUT 和 DSP 换取更好的 estimated time；如果 DSP 是主要约束，需要继续优化 rsqrt 结构。
- `nr1` 和 `nr2` 在当前测试下精度非常接近，后续可优先选择资源/时序更简单的分支。

## 6. 限制

- 这是 hidden1024 tile-level RMSNorm，不是完整 Transformer block。
- estimated-time improvement 不等同板级实测吞吐。
- 当前资源增加较明显，不能只宣传 speedup，也要说明 tradeoff。

## 7. PPT 可用讲法

可以说：

> RMSNorm rsqrt 近似精度非常高，estimated time 也更好，但当前实现增加 LUT 和 DSP，所以它是一个需要继续做资源优化的候选。

不要说：

> RMSNorm 近似同时无条件降低所有资源。

## 8. 结果文件

- CSV：`csv/hls_kernel_summary.csv`、`csv/hls_optimization_comparison.csv`
- 源码：`source_kernels/`
- 运行状态：`run_status/`
