# 真实 pi0 权重函数简化实验详细报告

## 1. 实验目的

该实验用于验证 softmax、FFN activation 和 RMSNorm 的近似替换在真实 pi0 权重分布下是否仍然可行。相比前置函数近似实验，本实验使用真实 `lerobot/pi0_base` 的 FFN 权重、Q/K projection 权重和 RMSNorm scale。

需要强调：输入仍然是随机 tensor，因此该实验仍然是模块级 benchmark，不是完整 pi0/VLA policy evaluation。

## 2. 实验内容

实验脚本：`source_scripts/exp_pi0_real_weight_simplify.py`

结果文件：

- `csv/pi0_real_weight_simplify.csv`
- `csv/pi0_real_weight_simplify_summary.md`

实验分为三类：

### FFN activation replacement

使用真实 FFN 权重，在完整 gated FFN 输出上比较：

- exact GELU
- tanh GELU
- PWL GELU
- clipped-linear GELU
- identity
- exact SiLU
- hard-swish

这里不是只比较 activation 函数本身，而是比较完整 FFN 输出。这一点对 PPT 很重要，因为 activation 误差会经过 up/down projection 放大或改变。

### Softmax approximation

使用真实 Q/K projection 权重生成 attention scores，再比较：

- exact softmax
- LUT softmax
- PWL softmax
- Taylor2 softmax
- Taylor3 softmax
- clipped-linear normalize
- base2 softmax

注意：`base2_softmax` 在当前 PyTorch 实现中使用 exact `torch.pow`，因此应视为数学重写 baseline，而不是低成本硬件近似。

### RMSNorm approximation

使用真实 RMSNorm scale，比较：

- exact RMSNorm
- INT8 input fake quant RMSNorm
- Newton-Raphson rsqrt 1 step
- Newton-Raphson rsqrt 2 steps
- PWL rsqrt

## 3. 实验过程

1. 加载 `results/pi0_module_weights/selected_modules.pt` 中的真实模块权重。
2. 对 FFN 生成随机输入，分别使用真实 VLM FFN 和 action expert FFN 权重。
3. 对 softmax，先用真实 Q/K projection 生成 attention score，再应用不同 softmax 近似。
4. 对 RMSNorm，使用真实 RMSNorm scale 和随机输入。
5. 使用 exact variant 作为 reference。
6. 记录 MSE、MAE、max error、cosine similarity、relative L2 error；softmax 额外记录 KL divergence。
7. 保存 CSV 和图片。

## 4. 关键结果

总结果行数：`38`

### FFN activation

- max FFN activation replacement relative L2：`1.019450`
- 最大误差主要来自 identity 等激进替换。
- tanh GELU 在该设置中接近 exact GELU。
- PWL GELU 在最终 FFN 输出上的 relative L2 约 `0.06-0.07`。
- clipped-linear GELU 约 `0.23`，误差偏大。

解释：activation 近似不能只看单函数输出，必须在完整 FFN 上评估。激进替换会导致输出显著偏移。

### Softmax

- 按 KL divergence 看，`base2_softmax` 最接近 exact，但它使用 exact `torch.pow`，不应作为低成本硬件近似宣传。
- 排除 `base2_softmax` 后，最好的 hardware-like softmax approximation 是 `lut_softmax`。
- `lut_softmax` 的 relative L2 约 `0.000043`，KL 约 `4.846268e-09`。

解释：LUT softmax 是当前最适合继续硬件化的 softmax 近似候选。

### RMSNorm

- max RMSNorm approximation relative L2：`0.011693`

解释：RMSNorm rsqrt 近似在真实权重 scale 下仍然保持较低误差，是较稳的硬件候选。

## 5. 结论

- LUT softmax 是真实权重实验中最好的 hardware-like softmax 近似方向。
- RMSNorm reciprocal-sqrt approximation 误差低，适合作为硬件优化候选。
- tanh GELU 基本可以视作高精度近似，PWL GELU 可作为硬件折中方案。
- identity、clipped-linear、hard-swish 等激进 activation replacement 不能支撑高精度结论。
- 函数近似应优先选择 softmax LUT 和 RMSNorm rsqrt；activation 近似需要更谨慎。

## 6. 限制

- 输入仍然是随机 tensor，不是真实 pi0 activation。
- 没有完整 pi0 forward，也没有 action trajectory 或任务成功率。
- `base2_softmax` 当前使用 exact `torch.pow`，不代表低成本硬件实现。
- 结论是模块级趋势，需要真实 activation trace 和下游验证补强。

## 7. PPT 可用讲法

可以说：

> 在真实 pi0 权重下，LUT softmax 和 RMSNorm rsqrt 仍然保持较低误差，是更适合进入 HLS/硬件实现的函数近似候选。

可以说：

> Activation replacement 不能只看函数本身，必须看完整 FFN 输出；实验里 identity 和 clipped-linear 误差较大，因此不能作为高精度替换方案。

不要说：

> 所有 nonlinear function 都可以安全替换。

## 8. 结果文件

CSV：

- `csv/pi0_real_weight_simplify.csv`
- `csv/pi0_real_weight_simplify_summary.md`

图片：

![Real Weight Simplify Latency](PORJECTS/PI0近似计算/产出/实验报告/pi0_pytorch_experiment/03_real_pi0_weight_simplify/figures/pi0_real_weight_simplify_latency.png)

![Real Weight Simplify Error](PORJECTS/PI0近似计算/产出/实验报告/pi0_pytorch_experiment/03_real_pi0_weight_simplify/figures/pi0_real_weight_simplify_error.png)
