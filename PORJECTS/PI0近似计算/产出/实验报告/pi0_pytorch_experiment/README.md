# pi0 PyTorch PPT 实验材料包

这个包用于快速制作 PPT，内容聚焦 PyTorch 侧的模块级近似计算实验。分类只保留三部分：

1. 前置基础实验
2. 真实 pi0 权重量化实验
3. 真实 pi0 权重函数简化实验

随机 pi0-aligned 实验只作为早期 sanity check，不在主报告中展开。正式叙事建议聚焦真实 `lerobot/pi0_base` 权重下的模块级 benchmark。

## 推荐 PPT 叙事顺序

1. 背景：pi0/VLA 中可优化的模块包括 GEMM/projector、attention softmax、FFN activation、RMSNorm。
2. 前置基础实验：先用通用模块确认量化和函数近似的误差/延迟记录流程。
3. 真实权重量化：用真实 pi0 权重验证 INT8/INT4/W4A8 在模块输出上的误差趋势。
4. 真实权重函数简化：用真实 FFN 权重、真实 Q/K score、真实 RMSNorm scale 验证 LUT softmax、GELU、RMSNorm 近似。
5. 总结：INT8 GEMM/projector 是最稳方向；LUT softmax 和 RMSNorm rsqrt 是较好的硬件候选；INT4/W4A8 和激进 activation replacement 需要谨慎。

## 核心结论

- INT8 是当前最有证据支持的量化目标：真实权重实验中 minimum cosine 为 `0.994843`。
- INT4/W4A8 压缩更强，但误差显著更大：真实权重实验中 INT4 minimum cosine 为 `0.921080`。
- LUT softmax 是最好的 hardware-like softmax 近似候选；`base2_softmax` 在 PyTorch 中使用 exact `torch.pow`，更适合作为数学重写 baseline。
- RMSNorm rsqrt 近似误差较低：真实权重函数简化中 max relative L2 为 `0.011693`。
- 激进 activation replacement 风险高：真实权重函数简化中 FFN activation replacement max relative L2 为 `1.019450`。

## 不能过度声称的内容

- 这些实验不是完整 pi0/VLA 复现。
- 真实权重实验仍然使用随机输入，不是真实 robot observation 或 activation trace。
- 没有真实 action trajectory、机器人任务成功率或端到端 policy evaluation。
- 因此结论应表述为“模块级近似计算趋势”，不要表述为“真实 pi0 任务质量提升或保持”。

## 目录说明

- `00_overview.md`：一页式总览，适合作为 PPT 讲稿开头。
- `01_preliminary_experiments/report.md`：前置基础实验详细报告。
- `02_real_pi0_weight_quant/report.md`：真实 pi0 权重量化实验详细报告。
- `03_real_pi0_weight_simplify/report.md`：真实 pi0 权重函数简化实验详细报告。
- 每个实验目录下的 `csv/`、`figures/`、`source_scripts/` 分别保存结果表、图片和源码脚本。
