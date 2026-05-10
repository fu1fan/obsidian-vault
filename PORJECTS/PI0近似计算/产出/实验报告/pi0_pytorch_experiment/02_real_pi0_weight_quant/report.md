# 真实 pi0 权重量化实验详细报告

## 1. 实验目的

该实验用于验证基础量化结论在真实 pi0 权重分布下是否仍然成立。相比前置基础实验，本实验不再使用随机权重，而是使用从 `lerobot/pi0_base` checkpoint 中提取的真实模块权重。

需要强调：输入仍然是随机 tensor，因此该实验反映的是真实参数分布下的模块级输出误差，不是完整 pi0 推理，也不是机器人任务成功率评估。

## 2. 实验内容

实验脚本：`source_scripts/exp_pi0_real_weight_quant.py`

结果文件：

- `csv/pi0_real_weight_quant.csv`
- `csv/pi0_real_weight_quant_summary.md`

测试模块包括：

- visual projector
- VLM layer0/layer9 attention Q/K/V/O projections
- VLM layer0 gated FFN
- action expert layer0 gated FFN
- state projection
- action input projection
- action output projection

比较的 variant 包括：

- FP32 baseline
- FP16
- INT8 fake quant
- INT4 weight-only fake quant
- W4A8 fake quant

## 3. 实验过程

1. 从已下载的 `lerobot/pi0_base` checkpoint 中提取 selected module tensors。
2. 加载 `results/pi0_module_weights/selected_modules.pt` 中的真实权重。
3. 对每个模块生成符合输入维度的随机 tensor。
4. 使用 FP32 输出作为 reference。
5. 对 FP16、INT8、INT4、W4A8 计算输出并与 FP32 对比。
6. 记录 MSE、MAE、max error、cosine similarity、relative L2 error、latency 和估算权重大小。
7. 保存 CSV 和图片。

所有选中权重已经是 `[out_dim, in_dim]` 布局，可直接用于 `torch.nn.functional.linear`，没有进行 transpose。

## 4. 关键结果

总结果行数：`70`

### INT8

- minimum cosine similarity：`0.994843`
- max relative L2 error：`0.101908`
- 存储压缩约为 FP32 的 `4x`

解释：INT8 在真实权重分布下仍然保持较高输出相似度，是当前最稳妥的量化目标。

### INT4 weight-only

- minimum cosine similarity：`0.921080`
- max relative L2 error：`0.416103`
- 存储压缩约为 FP32 的 `8x`

解释：INT4 有更强存储压缩，但误差明显大于 INT8，风险更高。

### W4A8

- minimum cosine similarity：约 `0.921040`
- max relative L2 error：约 `0.416216`

解释：当前 W4A8 fake quant 结果与 INT4 weight-only 风险接近，需要 calibration、grouped quantization 或真实 activation trace 才能继续判断。

## 5. 结论

- 真实 pi0 权重实验支持前置实验的主趋势：INT8 是最可靠的量化目标。
- INT4/W4A8 具有明显压缩收益，但输出误差显著变大，不能直接作为安全量化方案。
- 对实际硬件方向而言，优先级应放在 INT8 GEMM/projector/QKV/FFN 类模块上。
- 如果要继续推进 INT4/W4A8，应补充 per-channel/grouped calibration、真实 activation 输入和下游任务级验证。

## 6. 限制

- 输入是随机 tensor，不是真实 robot observation 或模型中间 activation。
- 没有运行完整 pi0 policy。
- PyTorch fake quant latency 包含 quant/dequant 开销，不能作为真实 INT8/INT4 硬件延迟。
- 结论只适用于模块级趋势，不适用于直接声明机器人任务质量。

## 7. PPT 可用讲法

可以说：

> 在真实 `lerobot/pi0_base` 权重下，INT8 仍然保持 high-cosine 输出，相比 INT4/W4A8 更稳。因此，面向硬件实现时，我会优先推进 INT8 GEMM/projector/QKV/FFN 这类模块。

可以说：

> INT4/W4A8 的压缩收益很明显，但真实权重下误差也明显放大，所以它们更适合作为后续 calibration 和真实 activation 验证方向。

不要说：

> INT8 已经证明完整 pi0 任务成功率不下降。

## 8. 结果文件

CSV：

- `csv/pi0_real_weight_quant.csv`
- `csv/pi0_real_weight_quant_summary.md`

图片：

![Real Weight Quant Latency](PORJECTS/PI0近似计算/产出/实验报告/pi0_pytorch_experiment/02_real_pi0_weight_quant/figures/pi0_real_weight_quant_latency.png)

![Real Weight Quant Error](PORJECTS/PI0近似计算/产出/实验报告/pi0_pytorch_experiment/02_real_pi0_weight_quant/figures/pi0_real_weight_quant_error.png)

![Real Weight Quant Size](PORJECTS/PI0近似计算/产出/实验报告/pi0_pytorch_experiment/02_real_pi0_weight_quant/figures/pi0_real_weight_quant_size.png)
