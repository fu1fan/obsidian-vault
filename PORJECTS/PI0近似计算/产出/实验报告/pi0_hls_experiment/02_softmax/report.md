# Softmax HLS 实验报告

## 1. 实验目的

Softmax 是 attention score normalization 的核心 nonlinear 模块。该实验比较 float exp exact softmax 与 LUT exp softmax，验证 LUT 近似是否能在保持较低误差的同时降低资源并改善 estimated time。

## 2. Kernel 内容

- `exact_softmax`：float exp row-wise softmax baseline。
- `lut_softmax`：使用 LUT 近似 exp，输入先做 max subtraction，并 clamp 到近似区间。

测试 shape：`rows4_len128`。

## 3. HLS 过程

1. 使用 C++ testbench 与 Python golden reference 检查数值。
2. 对 exact 和 LUT softmax 分别运行 C-sim 和 C-synthesis。
3. 比较 latency cycles、estimated clock、estimated time、DSP/LUT/FF/BRAM 和 KL/cosine/relative L2。

## 4. 关键结果

### exact_softmax baseline

- latency：`1,727` cycles
- estimated clock：`15.145 ns`
- estimated time：`2.615542e+04 ns`
- LUT：`4133`
- DSP：`14`
- cosine：`0.999999999993`

### lut_softmax

- latency：`1,867` cycles
- estimated clock：`7.300 ns`
- estimated time：`1.362910e+04 ns`
- time speedup：`1.919x`
- LUT：`3692`
- DSP：`2`
- DSP 变化：`14 -> 2`
- KL：`1.746e-03`
- cosine：`0.999311`
- relative L2：`3.7714e-02`

## 5. 结论

- LUT softmax 的 cycle count 比 exact softmax 略高，但 estimated clock 明显更好，因此 estimated time 约 `1.919x` 更快。
- LUT softmax 显著减少 DSP 使用，从 `14` 降到 `2`。
- 数值误差在模块级 benchmark 中可控，cosine 为 `0.999311`。
- 对硬件实现而言，LUT softmax 是合理的 attention softmax 近似候选。

## 6. 限制

- 结果来自 row-wise tile，不代表完整 attention kernel 或 full model latency。
- KL 和 cosine 只反映当前测试输入/golden reference 下的误差。
- estimated clock 不是板级 timing closure。

## 7. PPT 可用讲法

可以说：

> LUT softmax 并不是 cycle 更少，而是通过简化 exp 逻辑改善 estimated clock 并大幅降低 DSP，所以综合 estimated time 和资源更有优势。

不要说：

> LUT softmax 在所有真实 attention 输入上都一定无损。

## 8. 结果文件

- CSV：`csv/hls_kernel_summary.csv`、`csv/hls_optimization_comparison.csv`
- 源码：`source_kernels/`
- 运行状态：`run_status/`
