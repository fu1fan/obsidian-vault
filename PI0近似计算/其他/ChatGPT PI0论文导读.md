下面按 **12 个知识点** 整理，可直接复制到 Markdown 笔记里。

---

# π0 论文笔记：本轮对话知识点交叉整理

## 1. π0 不是普通 VLM，而是 VLA

**PDF 位置**：p.1 Fig.1；p.2–3 Introduction  
**原文定位**：

> “pre-trained vision-language model (VLM) backbone”  
> “turning it into a vision-language-action (VLA) model”  
> **中文笔记**：π0 不是“图像+文本→文本”，而是“图像+语言+机器人状态→连续动作”。它以预训练 VLM 为基础，再加入机器人动作建模。

---

## 2. 训练分为 VLM 初始化、机器人 pre-training、post-training

**PDF 位置**：p.3 Introduction；p.4 Section III；p.5 Section V  
**原文定位**：

> “pre-training/post-training separation”  
> “pre-trained on over 10,000 hours of robot data”  
> **中文笔记**：PaliGemma 阶段是 VLM 预训练；π0 的机器人阶段是完整 VLA 训练。海量数据用于 pre-training，高质量数据用于 post-training。

---

## 3. π0 基于 PaliGemma，但不是直接让 PaliGemma 输出动作

**PDF 位置**：p.4 Section III / IV  
**原文定位**：

> “based on the PaliGemma vision-language model”  
> “we add action outputs that use flow matching”  
> **中文笔记**：PaliGemma 提供视觉语言理解能力，π0 在其上加入动作输出结构，用 flow matching 生成连续动作。

---

## 4. π0 的核心结构：大 VLM backbone + 小 action expert

**PDF 位置**：p.4 Fig.3  
**原文定位**：

> “larger VLM backbone and a smaller action expert”  
> **中文笔记**：VLM backbone 处理图像和语言；action expert 处理机器人状态和动作 token。Fig.3 是画架构图时最重要的参考图。

---

## 5. 输入不是只有图像和语言，还包括机器人状态

**PDF 位置**：p.5 Section IV  
**原文定位**：

> “multiple RGB images, a language command, and the robot’s proprioceptive state”  
> **中文笔记**：π0 的观测为 `o_t = [I_t^1, ..., I_t^n, l_t, q_t]`。`q_t` 是机器人本体状态，例如关节角。

---

## 6. 输出不是单步动作，而是 action chunk

**PDF 位置**：p.5 Section IV  
**原文定位**：

> “action chunk of future actions”  
> “we use H = 50”  
> **中文笔记**：π0 一次预测未来 `H=50` 个动作，而不是每次只预测一个动作。这是它能支持高频控制的关键。

---

## 7. flow matching 训练：模型学的是“去噪方向”

**PDF 位置**：p.5 Section IV  
**原文定位**：

> “noisy actions”  
> “match the denoising vector field”  
> **中文笔记**：训练时构造  
> `A_t^τ = τA_t + (1-τ)ε`，  
> 模型预测 `vθ(A_t^τ, o_t)`，目标是逼近 `A_t - ε`。它不是直接预测动作，而是预测从噪声走向真实动作的方向。

---

## 8. flow matching 推理：从随机噪声积分成动作

**PDF 位置**：p.5 Section IV  
**原文定位**：

> “starting with random noise”  
> “forward Euler integration”  
> “10 integration steps”  
> **中文笔记**：推理时从随机动作 `A_t^0` 开始，用 Euler 公式迭代：  
> `A_t^{τ+δ} = A_t^τ + δvθ(A_t^τ, o_t)`。  
> 论文实验中 `δ=0.1`，共 10 步。

---

## 9. 10 steps 和 H=50 不是一回事

**PDF 位置**：p.5 Section IV；p.16 Appendix D  
**原文定位**：

> “10 integration steps”  
> “entire H-step action chunk”  
> **中文笔记**：  
> `10 steps` 是对同一个动作 chunk 反复去噪 10 次；  
> `H=50` 是最终输出的动作序列长度。  
> 所以不是“预测 10 个动作”，而是“用 10 次迭代生成 50 个动作”。

---

## 10. PaliGemma 的信息通过 self-attention/KV 被 action expert 读取

**PDF 位置**：p.15 Appendix B, “Action expert”  
**原文定位**：

> “weights interact only through the transformer’s self-attention layers”  
> **中文笔记**：不是 PaliGemma 先输出文字再传给 action expert。更准确是：image/language tokens 产生 VLM hidden states / K,V；action tokens 在 self-attention 中读取这些 K,V。

---

## 11. VLM tokens 通常不读 action tokens

**PDF 位置**：p.15 Appendix B, “Attention mask”  
**原文定位**：

> “blockwise causal attention mask with 3 blocks”  
> “tokens in each block cannot attend to future blocks”  
> **中文笔记**：三个 block 是：  
> `[images, language]`、`[q_t]`、`[noisy actions]`。  
> 前面的 block 不能看后面的 block，所以 VLM 不读 action 的 K/V；action tokens 可以读完整输入。

---

## 12. 50Hz 指动作执行频率，不是完整 VLM 50Hz 推理

**PDF 位置**：p.3 Introduction；p.16 Appendix D / Table I  
**原文定位**：

> “control robots at frequencies of up to 50 Hz”  
> “run inference every 0.5 seconds”  
> **中文笔记**：π0 一次生成一个 H-step action chunk，然后底层控制器按 50Hz 执行。论文还说明 50Hz 机器人每 0.5s 推理一次，执行 25 个动作后再更新。完整 VLM 并不是每 20ms 跑一次。

---

## 13. 推理耗时表是本项目优化依据

**PDF 位置**：p.16 Appendix D / Table I  
**原文定位**：

> “image encoders 14 ms”  
> “observation forward pass 32 ms”  
> “x10 action forward pass (flow) 27 ms”  
> **中文笔记**：Table I 给出 RTX 4090 上 3 相机输入时：图像编码 14ms，observation forward 32ms，10 次 action forward 27ms，总 on-board 73ms。说明优化目标应放在 GEMM/Linear、attention、softmax、action expert 和 flow steps 上。

---

# 一句话总括

π0 的计算链路是：

```text
多相机图像 + 语言指令 + 机器人状态
        ↓
PaliGemma / VLM backbone 形成视觉语言上下文
        ↓
action expert 读取上下文，并结合 noisy action + timestep
        ↓
flow matching 预测去噪方向
        ↓
10 次 Euler 积分
        ↓
输出 H=50 的连续 action chunk
        ↓
底层控制器按 50Hz 执行
```

最适合你项目标注的页码：**p.1 Fig.1、p.4 Fig.3、p.5 Section IV、p.15 Appendix B、p.16 Appendix D / Table I**。