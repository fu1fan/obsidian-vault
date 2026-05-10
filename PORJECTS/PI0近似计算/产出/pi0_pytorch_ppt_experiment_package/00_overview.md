# PyTorch 实验总览

## 实验目标

本项目的 PyTorch 实验用于评估 pi0/VLA 风格模型中的模块级近似计算方案，重点回答三个问题：

1. Linear/GEMM/projector 是否适合低精度量化？
2. attention softmax、GELU、RMSNorm 是否存在可接受的近似实现？
3. 这些趋势在真实 pi0 权重分布下是否仍然成立？

前期曾使用 pi0-aligned random tensor 做 sanity check 和候选方案筛选；由于趋势和真实权重实验一致，PPT 主报告建议只聚焦基础实验和真实权重实验。

## 实验分组

| 分组 | 作用 | 主要材料 |
| --- | --- | --- |
| 前置基础实验 | 验证 benchmark 流程，筛选候选近似方向 | Linear/projector quant、softmax approx、GELU/RMSNorm approx、scale sweep |
| 真实 pi0 权重量化实验 | 用真实 `lerobot/pi0_base` 权重验证 INT8/INT4/W4A8 误差趋势 | visual projector、attention projection、FFN、state/action projections |
| 真实 pi0 权重函数简化实验 | 用真实 FFN/QK/RMSNorm 权重验证 softmax、activation、RMSNorm 近似 | LUT softmax、PWL/tanh GELU、rsqrt RMSNorm |

## 指标解释

- `cosine_similarity`：输出方向相似度，越接近 1 越好。
- `relative_l2_error`：相对 L2 误差，越小越好。
- `MSE/MAE/max_error`：逐元素误差，用于定位误差幅度。
- `KL divergence`：用于 softmax 概率分布近似，越小越好。
- `latency_mean_ms`：PyTorch 计时结果；fake quant 包含 quant/dequant 开销，不能直接等同硬件延迟。
- `estimated_weight_size_MB`：按数据位宽估算权重存储大小。

## PPT 可讲的核心结论

- INT8 是目前最稳妥的量化目标，尤其适合 Linear/projector/GEMM 类模块。
- INT4/W4A8 可以显著压缩存储，但真实权重下误差明显上升，需要校准和下游验证。
- LUT softmax 和 RMSNorm rsqrt 是最值得转向硬件实现的函数近似候选。
- Activation replacement 必须在完整 FFN 输出上评估；identity/clipped-linear 这类激进替换不适合高精度主张。

## PPT 中应避免的说法

- 不要说“已经验证完整 pi0”。
- 不要说“证明真实机器人任务成功率不下降”。
- 不要把 PyTorch fake INT8/INT4 延迟说成真实硬件加速。
- 不要把 `base2_softmax` 说成低成本硬件近似；当前 PyTorch 实现使用 exact `torch.pow`。

## 建议一句话总结

这些实验提供了模块级证据链：基础实验用于筛选方向，真实 pi0 权重实验用于提高可信度；当前最稳的路线是 INT8 GEMM/projector、LUT softmax 和 RMSNorm rsqrt，而 INT4/W4A8 与激进 activation replacement 需要进一步校准和真实下游验证。
