---
id: black02026
type: document
title: "$π_0$: A Vision-Language-Action Flow Model for General Robot Control"
issued:
  date-parts:
    - - 2026
      - 1
      - 8
title-short: $π_0$
URL: http://arxiv.org/abs/2410.24164
DOI: 10.48550/arXiv.2410.24164
publisher: arXiv
abstract: Robot learning holds tremendous promise to unlock the full potential of flexible, general, and dexterous robot systems, as well as to address some of the deepest questions in artificial intelligence. However, bringing robot learning to the level of generality required for effective real-world systems faces major obstacles in terms of data, generalization, and robustness. In this paper, we discuss how generalist robot policies (i.e., robot foundation models) can address these challenges, and how we can design effective generalist robot policies for complex and highly dexterous tasks. We propose a novel flow matching architecture built on top of a pre-trained vision-language model (VLM) to inherit Internet-scale semantic knowledge. We then discuss how this model can be trained on a large and diverse dataset from multiple dexterous robot platforms, including single-arm robots, dual-arm robots, and mobile manipulators. We evaluate our model in terms of its ability to perform tasks in zero shot after pre-training, follow language instructions from people and from a high-level VLM policy, and its ability to acquire new skills via fine-tuning. Our results cover a wide variety of tasks, such as laundry folding, table cleaning, and assembling boxes.
tags:
  - literature_note
author:
  - family: Black
    given: Kevin
  - family: Brown
    given: Noah
  - family: Driess
    given: Danny
  - family: Esmail
    given: Adnan
  - family: Equi
    given: Michael
  - family: Finn
    given: Chelsea
  - family: Fusai
    given: Niccolo
  - family: Groom
    given: Lachy
  - family: Hausman
    given: Karol
  - family: Ichter
    given: Brian
  - family: Jakubczak
    given: Szymon
  - family: Jones
    given: Tim
  - family: Ke
    given: Liyiming
  - family: Levine
    given: Sergey
  - family: Li-Bell
    given: Adrian
  - family: Mothukuri
    given: Mohith
  - family: Nair
    given: Suraj
  - family: Pertsch
    given: Karl
  - family: Shi
    given: Lucy Xiaoyang
  - family: Tanner
    given: James
  - family: Vuong
    given: Quan
  - family: Walling
    given: Anna
  - family: Wang
    given: Haohuan
  - family: Zhilinsky
    given: Ury
keyword: Computer Science - Machine Learning, Computer Science - Robotics
accessed:
  date-parts:
    - - 2026
      - 5
      - 4
source: arXiv.org
year: "2026"
dateCreated: 2026-05-04
reading-status: to-read
aliases:
  - "$π_0$: A Vision-Language-Action Flow Model for General Robot Control"
author-links:
  - "[[Author/Kevin Black]]"
  - "[[Author/Noah Brown]]"
  - "[[Author/Danny Driess]]"
  - "[[Author/Adnan Esmail]]"
  - "[[Author/Michael Equi]]"
  - "[[Author/Chelsea Finn]]"
  - "[[Author/Niccolo Fusai]]"
  - "[[Author/Lachy Groom]]"
  - "[[Author/Karol Hausman]]"
  - "[[Author/Brian Ichter]]"
  - "[[Author/Szymon Jakubczak]]"
  - "[[Author/Tim Jones]]"
  - "[[Author/Liyiming Ke]]"
  - "[[Author/Sergey Levine]]"
  - "[[Author/Adrian Li-Bell]]"
  - "[[Author/Mohith Mothukuri]]"
  - "[[Author/Suraj Nair]]"
  - "[[Author/Karl Pertsch]]"
  - "[[Author/Lucy Xiaoyang Shi]]"
  - "[[Author/James Tanner]]"
  - "[[Author/Quan Vuong]]"
  - "[[Author/Anna Walling]]"
  - "[[Author/Haohuan Wang]]"
  - "[[Author/Ury Zhilinsky]]"
attachment:
  - "[[libs/biblib/black02026/black02026.pdf|PDF]]"
related:
  - "[[@beyerpaligemma2024 - PaliGemma_ A versatile 3B VLM for transfer|PaliGemma: A versatile 3B VLM for transfer]]"
  - "[[@vaswaniattention2023 - Attention Is All You Need|Attention Is All You Need]]"
month: "1"
day: "8"
---

# $π_0$: A Vision-Language-Action Flow Model for General Robot Control 
*约定*
	黄色为帮助理解的重要内容
	红色为概念
	蓝色为逻辑

 *笔记*

> [!PDF|yellow] [[black02026.pdf#page=2&selection=161,9,166,65&color=yellow|black02026, p.2]]
> > we begin by utilizing a pre-trained vision-language model (VLM) to import Internet-scale experience. By basing our model on a VLM, we inherit the general knowledge, semantic reasoning, and problem-solving abilities of languageand vision-language models. We then further train our model to incorporate robot actions, turning it into a vision-language-
>   [[black02026.pdf#page=3&selection=0,0,0,24&color=yellow|black02026, p.3]]
> > action (VLA) model [7]. 
> 
> 
>π0 不是“图像+文本→文本”，而是“图像+语言+机器人状态→连续动作”。它以预训练 VLM 为基础，再加入机器人动作建模。
>

> [!PDF|yellow] [[black02026.pdf#page=4&selection=170,29,176,44&color=yellow|black02026, p.4]]
> > To turn the base PaliGemma VLM into π0, we add action outputs that use flow matching [32, 28] to generate continuous action distributions.
> 
> PaliGemma 提供视觉语言理解能力，π0 在其上加入动作输出结构，用 flow matching 生成连续动作

> [!PDF|red] [[black02026.pdf#page=4&selection=80,3,83,1&color=red|black02026, p.4]]
> > a larger VLM backbone and a smaller action expert 
> 
> π0 的核心结构：大 VLM backbone + 小 action expert。VLM backbone 处理图像和语言；action expert 处理机器人状态和动作 token。Fig.3 是画架构图时最重要的参考图。

> [!PDF|yellow] [[black02026.pdf#page=5&selection=55,20,104,29&color=yellow|black02026, p.5]]
> > he observation consists of multiple RGB images, a language command, and the robot’s proprioceptive state, such that ot = [I1 t , ..., In t , ℓt, qt], where Ii t is ith image (with 2 or 3 images per robot), ℓt is a sequence of language tokens, and qt is a vector of joint angles. 
> 
> 输入不是只有图像和语言，还包括机器人状态。π0 的观测为 `o_t = [I_t^1, ..., I_t^n, l_t, q_t]`。`q_t` 是机器人本体状态，例如关节角。

> [!PDF|] [[black02026.pdf#page=5&selection=253,2,327,3|black02026, p.5]]
> > In practice, the network is trained by sampling random noise ϵ ∼ N (0, I), computing the “noisy actions” Aτ t = τ At + (1 − τ )ϵ, and then training the network outputs vθ (Aτ t , ot) to match the denoising vector field u(Aτ t |At) = At − ϵ. T
> 
> 训练时构造  `A_t^τ = τA_t + (1-τ)ε`， 模型预测 `vθ(A_t^τ, o_t)`，目标是逼近 `A_t - ε`。
> 不直接预测动作，而是预测从噪声走向真实动作的方向。

> [!PDF|note] [[black02026.pdf#page=5&selection=348,2,402,29&color=note|black02026, p.5]]
> > starting with random noise A0 t ∼ N (0, I). We use the forward Euler integration rule: Aτ +δ t = Aτ t + δvθ (Aτ t , ot), where δ is the integration step size.
> 
> π0 的训练和推理其实是同一个 flow matching 机制的两面：训练时，模型已经能看到真实示范动作 `A_t`，于是论文人为采样一个高斯噪声 `ε` 和一个 flow 时间 `τ`，把真实动作和噪声混合成带噪动作 `A_t^τ = τA_t + (1-τ)ε`，再让模型在当前观测 `o_t` 条件下预测从噪声走向真实动作的“去噪方向” `vθ(A_t^τ, o_t)`，监督目标就是 `A_t - ε`；换句话说，训练阶段是在教 action expert：任意给你一个乱的动作 chunk，你应该往哪个方向改，才能变成合理的真实动作。推理时没有真实动作，模型先随机生成一个初始动作 chunk `A_t^0 ~ N(0,I)`，然后把当前图像、语言、机器人状态形成的观测 `o_t` 作为条件，反复调用 action expert 预测去噪方向，并用 Euler 公式 `A_t^{τ+δ}=A_t^τ+δvθ(A_t^τ,o_t)` 更新整个动作 chunk；论文实验中用 10 次 integration steps，即每次都在修正同一个 `H=50` 的动作序列，最后输出完整的连续 action chunk。也就是说，**训练是学“动作空间里的方向场”，推理是从随机噪声出发沿着这个方向场走 10 步，得到 50 步未来动作**。
> 
> 注意：`10 steps` 是对同一个动作 chunk 反复去噪 10 次；  
> `H=50` 是最终输出的动作序列长度。  
> 所以不是“预测 10 个动作”，而是“用 10 次迭代生成 50 个动作”，这十次迭代在1s内完成，所以论文说能达到50Hz的控制输出

> [!PDF|yellow] [[black02026.pdf#page=15&selection=528,0,529,7&color=yellow|black02026, p.15]]
> > weights interact only through the transformer’s self-attention layers.
> 
> 不是 PaliGemma 先输出文字再传给 action expert。更准确是：image/language tokens 产生 VLM hidden states / K,V；action tokens 在 self-attention 中读取这些 K,V。

> [!PDF|yellow] [[black02026.pdf#page=16&selection=292,40,296,9&color=yellow|black02026, p.16]]
> >  For the 20Hz UR5e and Franka robots, we run inference every 0.8 seconds (after executing 16 actions), and for all other robots, which run at 50Hz, we run inference every 0.5 seconds (after executing 25 actions).
> 
> π0 一次生成一个 H-step action chunk，然后底层控制器按 50Hz 执行。50Hz 机器人每 0.5s 推理一次，执行 25 个动作后再更新。完整 VLM 并不是每 20ms 跑一次。

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
