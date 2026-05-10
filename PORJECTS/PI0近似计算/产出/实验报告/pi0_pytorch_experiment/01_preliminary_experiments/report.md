# 前置基础实验详细报告

## 1. 实验目的

前置基础实验用于建立模块级 benchmark 流程，并筛选后续真实 pi0 权重实验中值得重点关注的近似方向。它不使用真实 pi0 权重，主要用随机输入和随机权重验证不同近似方法的误差、延迟和存储变化。

这些实验对应 VLA/pi0 中常见的计算模块：

- Transformer Linear / FFN GEMM
- VLM visual projector
- Attention softmax
- FFN activation，例如 GELU
- RMSNorm

## 2. 实验内容

### Linear / GEMM 量化

实验脚本：`source_scripts/exp_linear_quant.py`

对随机 Linear 层比较：

- FP32 baseline
- FP16
- INT8 fake quant
- INT4 weight-only fake quant

输出指标包括 MSE、MAE、max error、cosine similarity、latency 和估算权重大小。

### Vision Projector 量化

实验脚本：`source_scripts/exp_projector_quant.py`

使用 projector-like shape：`batch=1,tokens=256,in=1152,out=2048`，模拟视觉 token 映射到语言 embedding 空间。比较 FP32、FP16、INT8 fake quant、INT4 weight-only fake quant。

### Softmax 近似

实验脚本：`source_scripts/exp_softmax_approx.py`

对 attention score 做 max-subtraction 稳定化后，比较：

- exact `torch.softmax`
- LUT exp softmax
- PWL exp softmax
- Taylor2 exp softmax
- Taylor3 exp softmax

除常规误差外，softmax 额外记录 KL divergence。

### GELU / RMSNorm 近似

实验脚本：`source_scripts/exp_gelu_rmsnorm_approx.py`

GELU 比较 exact GELU、tanh GELU、PWL GELU、LUT GELU。RMSNorm 比较 FP32、FP16、INT8 input fake quant 和 approximate rsqrt。

### Scale Sweep

实验脚本：`source_scripts/exp_scale_sweep.py`

将 Linear 扩展到更大 shape，最大权重规模达到 `8192 x 8192`；将 softmax 扩展到 `heads=32,seq=1024`。目的是观察规模变大后存储节省和近似误差是否仍然稳定。

## 3. 实验过程

1. 使用 FP32 输出作为 reference。
2. 对每个 variant 计算输出。
3. 检查输出是否存在 NaN 或 Inf。
4. 计算误差指标：MSE、MAE、max error、cosine similarity，softmax 额外计算 KL divergence。
5. 使用 warmup + repeat 记录 PyTorch 平均延迟。
6. 对量化实验按位宽估算权重存储大小。
7. 保存 CSV 和图片。

## 4. 关键结果

### Linear / GEMM

- 当前结果中 cosine similarity 最低约 `0.987824`。
- INT8 权重存储约为 FP32 的 `1/4`。
- INT4 weight-only 权重存储约为 FP32 的 `1/8`。
- PyTorch fake quant 未表现出真实硬件加速，因为量化和反量化开销包含在计时中。

### Vision Projector

- 当前结果中 cosine similarity 最低约 `0.989768`。
- FP32 projector 权重约 `9 MB`。
- INT8 估算约 `2.25 MB`，INT4 估算约 `1.125 MB`。

### Softmax

- 当前基础 softmax 近似实验中 cosine similarity 最低约 `0.994338`。
- KL divergence 最大约 `0.010631`。
- LUT/Taylor 风格近似整体更稳定，PWL 或粗糙近似误差更容易变大。

### GELU / RMSNorm

- 当前结果中 cosine similarity 最低约 `0.999926`。
- tanh GELU 与 exact GELU 非常接近。
- RMSNorm approximate rsqrt 在单模块实验中误差较小。

### Scale Sweep

- 最大 Linear shape 下，FP32 权重估算 `256 MB`，INT8 `64 MB`，INT4 `32 MB`。
- FP16 在部分大 shape 上有明显 PyTorch 延迟收益，最高约 `2.49x`。
- fake INT8/INT4 的 PyTorch 延迟不能用于声称真实硬件加速。

## 5. 结论

- Linear/GEMM/projector 是最自然的量化对象，INT8 是后续真实权重实验中最值得关注的候选。
- INT4 有显著压缩收益，但基础实验已显示误差更大，应谨慎推进。
- Softmax、GELU、RMSNorm 的近似需要分别评估：LUT softmax、tanh/PWL GELU、approx rsqrt RMSNorm 都有继续研究价值。
- 前置实验主要用于筛选方向，不应用来声称真实 pi0 行为。

## 6. 限制

- 使用随机权重和随机输入，不代表真实 pi0 参数分布。
- PyTorch fake quant 计时包含额外量化开销，不等同于 fused INT8/INT4 kernel。
- 没有真实 activation trace 或任务级指标。

## 7. PPT 可用讲法

可以说：

> 前置实验先在标准 Linear/projector/softmax/GELU/RMSNorm 模块上验证 benchmark 流程，并筛选候选近似方向。结果显示 INT8 对 GEMM/projector 类模块最稳，LUT softmax 和 RMSNorm rsqrt 是值得硬件化的函数近似候选。

不要说：

> 前置实验证明完整 pi0 可以安全量化。

## 8. 结果文件

CSV：

- `csv/linear_quant.csv`
- `csv/projector_quant.csv`
- `csv/softmax_approx.csv`
- `csv/gelu_rmsnorm_approx.csv`
- `csv/scale_sweep_linear.csv`
- `csv/scale_sweep_softmax.csv`

图片：

![Latency Compare](PORJECTS/PI0近似计算/产出/实验报告/pi0_pytorch_experiment/01_preliminary_experiments/figures/latency_compare.png)

![Error Compare](PORJECTS/PI0近似计算/产出/实验报告/pi0_pytorch_experiment/01_preliminary_experiments/figures/error_compare.png)

![Cosine Compare](PORJECTS/PI0近似计算/产出/实验报告/pi0_pytorch_experiment/01_preliminary_experiments/figures/cosine_compare.png)

![Model Size Compare](PORJECTS/PI0近似计算/产出/实验报告/pi0_pytorch_experiment/01_preliminary_experiments/figures/model_size_compare.png)

![Scale Sweep Latency](PORJECTS/PI0近似计算/产出/实验报告/pi0_pytorch_experiment/01_preliminary_experiments/figures/scale_sweep_latency.png)

![Scale Sweep Memory](PORJECTS/PI0近似计算/产出/实验报告/pi0_pytorch_experiment/01_preliminary_experiments/figures/scale_sweep_memory.png)

![Scale Sweep Error](PORJECTS/PI0近似计算/产出/实验报告/pi0_pytorch_experiment/01_preliminary_experiments/figures/scale_sweep_error.png)
