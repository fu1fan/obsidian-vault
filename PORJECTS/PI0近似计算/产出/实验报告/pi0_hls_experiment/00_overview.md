# HLS 实验总览

## 实验目标

HLS 实验用于把 PyTorch 侧筛出的近似计算方向落到可综合 kernel 上，验证它们在 Vitis HLS 中的实现可行性、估算 latency、II、clock 和资源消耗。

这些结果是 tile-level evidence，不是完整 pi0/VLA FPGA 部署。

## 模块映射

| 模块 | HLS kernel | 对应 pi0/VLA 位置 |
| --- | --- | --- |
| GEMM/projector | `int8_gemm`、`int8_gemm_opt`、`fixed_projector_tile`、`fixed_projector_tile_opt_*` | projector、QKV/O linear、FFN、state/action projection |
| Softmax | `exact_softmax`、`lut_softmax` | attention score normalization |
| GELU | `exact_gelu`、`gelu_pwl` | FFN activation |
| RMSNorm | `exact_rmsnorm`、`rmsnorm_rsqrt` | Transformer normalization |

## 指标解释

- `latency_cycles`：Vitis HLS 估算周期数。
- `II`：initiation interval，越小越利于流水。
- `estimated_clock_ns`：综合报告中的估算时钟周期，不等同板级 timing closure。
- `estimated_time`：`latency_cycles * estimated_clock_ns`，用于比较周期和时钟共同作用。
- `LUT/FF/BRAM/DSP`：资源估算。
- `MSE/MAE/cosine/relative_l2/KL`：C-sim 输出对 golden reference 的数值误差。

## PPT 可讲的核心结论

- GEMM/projector 的主要瓶颈不是单纯 MAC 数量，而是权重访问、local buffer、accumulator partition 和输出通道并行方式。
- nonlinear kernel 中，LUT softmax 和 PWL GELU 都能显著降低 expensive float nonlinear 的资源压力。
- RMSNorm rsqrt 精度很好、estimated time 更优，但当前资源使用增加，是后续优化点。
- 所有 kernel 均通过当前记录中的 C-sim 和 C-synthesis，说明模块级实现是可综合的。

## PPT 中应避免的说法

- 不要把 HLS estimated clock 说成板级实测频率。
- 不要把 tile latency 说成完整 pi0 inference latency。
- 不要把当前 kernel 结果说成已完成 host/kernel scheduling、DDR bandwidth、inter-kernel streaming 或 full model deployment。

## 建议一句话总结

HLS 实验把 PyTorch 侧的近似计算候选推进到了可综合 tile-level kernel：INT8 GEMM/projector 展示了并行和本地缓存带来的大幅 cycle speedup，LUT softmax 与 PWL GELU 展示了 nonlinear 资源收益，RMSNorm rsqrt 展示了高精度但仍需资源优化的 tradeoff。
