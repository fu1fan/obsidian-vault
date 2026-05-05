---
id: beyerpaligemma2024
type: document
title: "PaliGemma: A versatile 3B VLM for transfer"
issued:
  date-parts:
    - - 2024
      - 10
      - 10
title-short: PaliGemma
URL: http://arxiv.org/abs/2407.07726
DOI: 10.48550/arXiv.2407.07726
publisher: arXiv
abstract: PaliGemma is an open Vision-Language Model (VLM) that is based on the SigLIP-So400m vision encoder and the Gemma-2B language model. It is trained to be a versatile and broadly knowledgeable base model that is effective to transfer. It achieves strong performance on a wide variety of open-world tasks. We evaluate PaliGemma on almost 40 diverse tasks including standard VLM benchmarks, but also more specialized tasks such as remote-sensing and segmentation.
tags:
  - literature_note
author:
  - family: Beyer
    given: Lucas
  - family: Steiner
    given: Andreas
  - family: Pinto
    given: André Susano
  - family: Kolesnikov
    given: Alexander
  - family: Wang
    given: Xiao
  - family: Salz
    given: Daniel
  - family: Neumann
    given: Maxim
  - family: Alabdulmohsin
    given: Ibrahim
  - family: Tschannen
    given: Michael
  - family: Bugliarello
    given: Emanuele
  - family: Unterthiner
    given: Thomas
  - family: Keysers
    given: Daniel
  - family: Koppula
    given: Skanda
  - family: Liu
    given: Fangyu
  - family: Grycner
    given: Adam
  - family: Gritsenko
    given: Alexey
  - family: Houlsby
    given: Neil
  - family: Kumar
    given: Manoj
  - family: Rong
    given: Keran
  - family: Eisenschlos
    given: Julian
  - family: Kabra
    given: Rishabh
  - family: Bauer
    given: Matthias
  - family: Bošnjak
    given: Matko
  - family: Chen
    given: Xi
  - family: Minderer
    given: Matthias
  - family: Voigtlaender
    given: Paul
  - family: Bica
    given: Ioana
  - family: Balazevic
    given: Ivana
  - family: Puigcerver
    given: Joan
  - family: Papalampidi
    given: Pinelopi
  - family: Henaff
    given: Olivier
  - family: Xiong
    given: Xi
  - family: Soricut
    given: Radu
  - family: Harmsen
    given: Jeremiah
  - family: Zhai
    given: Xiaohua
keyword: Computer Science - Computer Vision and Pattern Recognition, Computer Science - Artificial Intelligence, Computer Science - Computation and Language, Computer Science - Machine Learning
accessed:
  date-parts:
    - - 2026
      - 5
      - 4
source: arXiv.org
year: "2024"
dateCreated: 2026-05-04
reading-status: to-read
aliases:
  - "PaliGemma: A versatile 3B VLM for transfer"
author-links:
  - "[[Author/Lucas Beyer]]"
  - "[[Author/Andreas Steiner]]"
  - "[[Author/André Susano Pinto]]"
  - "[[Author/Alexander Kolesnikov]]"
  - "[[Author/Xiao Wang]]"
  - "[[Author/Daniel Salz]]"
  - "[[Author/Maxim Neumann]]"
  - "[[Author/Ibrahim Alabdulmohsin]]"
  - "[[Author/Michael Tschannen]]"
  - "[[Author/Emanuele Bugliarello]]"
  - "[[Author/Thomas Unterthiner]]"
  - "[[Author/Daniel Keysers]]"
  - "[[Author/Skanda Koppula]]"
  - "[[Author/Fangyu Liu]]"
  - "[[Author/Adam Grycner]]"
  - "[[Author/Alexey Gritsenko]]"
  - "[[Author/Neil Houlsby]]"
  - "[[Author/Manoj Kumar]]"
  - "[[Author/Keran Rong]]"
  - "[[Author/Julian Eisenschlos]]"
  - "[[Author/Rishabh Kabra]]"
  - "[[Author/Matthias Bauer]]"
  - "[[Author/Matko Bošnjak]]"
  - "[[Author/Xi Chen]]"
  - "[[Author/Matthias Minderer]]"
  - "[[Author/Paul Voigtlaender]]"
  - "[[Author/Ioana Bica]]"
  - "[[Author/Ivana Balazevic]]"
  - "[[Author/Joan Puigcerver]]"
  - "[[Author/Pinelopi Papalampidi]]"
  - "[[Author/Olivier Henaff]]"
  - "[[Author/Xi Xiong]]"
  - "[[Author/Radu Soricut]]"
  - "[[Author/Jeremiah Harmsen]]"
  - "[[Author/Xiaohua Zhai]]"
attachment:
  - "[[libs/biblib/beyerpaligemma2024/beyerpaligemma2024.pdf|PDF]]"
month: "10"
day: "10"
related:
  - "[[@vaswaniattention2023 - Attention Is All You Need|Attention Is All You Need]]"
---

# PaliGemma: A versatile 3B VLM for transfer 

 _Notes_

这篇论文 [[libs/biblib/beyerpaligemma2024/beyerpaligemma2024.pdf]] 介绍的是 **PaliGemma**：一个 Google DeepMind 开源的、小型但强迁移能力的视觉语言模型（VLM）。

## 一句话总结

**PaliGemma 是一个约 3B 参数的开放视觉语言基础模型，结合 SigLIP 图像编码器和 Gemma-2B 语言模型，目标不是直接做聊天助手，而是作为可微调、可迁移到多种视觉语言任务的基础模型。**

---

## 1. 这篇论文想解决什么问题？

很多视觉语言模型，例如 GPT-4V、Gemini、Flamingo、BLIP-2、LLaVA 等，要么模型很大，要么主要面向 instruction/chat 使用。

这篇论文的核心问题是：

> 能不能做一个相对小、开放、训练良好的视觉语言基础模型，使它在经过少量或常规微调后，能迁移到很多不同任务上，并达到很强性能？

作者强调 PaliGemma 不是一个开箱即用的聊天式 VLM，而是一个 **base VLM for transfer**，也就是“适合迁移学习的视觉语言基础模型”。

---

## 2. 模型结构是什么？

PaliGemma 由三部分组成：

1. **图像编码器：[[SigLIP ViT-So400m]]
   - 约 400M 参数。
   - 负责把图像转换成视觉 token。
   - SigLIP 是 CLIP/ALIGN 一类对比学习视觉模型的改进版本。

2. **语言模型：Gemma-2B**
   - 使用 Google 的 Gemma 2B 预训练语言模型。
   - 是 decoder-only Transformer。
   - 负责根据图像 token 和文本 prompt 生成答案。

3. **线性投影层**
   - 把 SigLIP 输出的视觉 token 投影到 Gemma 的 embedding 空间。
   - 作者尝试过更复杂 connector，但发现简单线性层已经足够。

输入形式大致是：

```text
[image tokens] + [BOS] + prompt + [SEP] + answer
```

输出则是自回归生成的文本。

所以它可以统一处理：

- 图像描述
- VQA
- OCR
- 文档理解
- 图表问答
- 目标检测
- 实例分割
- 视频问答
- 多图推理

---

## 3. 它怎么训练？

论文把训练分成几个阶段：

### Stage 0：单模态预训练

不重新训练图像模型和语言模型，而是直接使用已有 checkpoint：

- SigLIP 图像编码器
- Gemma-2B 语言模型

### Stage 1：多模态预训练

这是核心阶段。

模型在大规模多模态数据上训练，包括：

- captioning
- OCR
- VQA
- question generation
- detection
- segmentation
- grounded captioning

重要的是，作者 **不冻结图像编码器**，而是端到端训练整个模型。

这和很多 VLM 做法不同。很多模型会冻结视觉 encoder，只训练连接层和语言模型。但作者认为，如果想让模型学到空间关系、细粒度视觉理解、定位能力，视觉 encoder 也应该被继续训练。

### Stage 2：提高分辨率

PaliGemma 有三个分辨率版本：

- 224 × 224
- 448 × 448
- 896 × 896

高分辨率对 OCR、文档、图表、检测、分割等任务尤其重要。

### Stage 3：迁移 / 微调

PaliGemma 的 base checkpoint 不一定直接好用，需要针对具体任务 fine-tune。

论文在大量 benchmark 上做了迁移实验，包括：

- COCO caption
- VQAv2
- OKVQA
- GQA
- ScienceQA
- ChartQA
- DocVQA
- TextVQA
- RefCOCO 分割
- ActivityNet / MSRVTT 视频任务
- Remote Sensing VQA 等

---

## 4. 主要实验结论

论文最重要的结论有几个：

### 1. 小模型也能很强

PaliGemma 总参数不到 3B，但在很多视觉语言任务上可以接近甚至超过更大的模型。

作者的观点是：

> 视觉语言任务不一定必须依赖超大 LLM，训练方式、数据混合、图像编码器和迁移能力同样关键。

### 2. 长时间多模态预训练很重要

作者做了消融实验，发现 Stage 1 多模态预训练越充分，迁移效果整体越好。

即使一些常见 benchmark 在短训练下已经不错，但更长训练会提升更多长尾、复杂、多样化任务。

### 3. 高分辨率非常关键

特别是这些任务明显受益于 448 或 896：

- OCR-VQA
- TextVQA
- DocVQA
- InfoVQA
- ChartQA
- ST-VQA
- segmentation

原因很直观：低分辨率下文字、表格、小目标会丢失细节。

### 4. 简单 connector 足够

作者发现 SigLIP 到 Gemma 之间的连接层用简单线性投影即可，复杂 MLP 没有明显收益。

### 5. 迁移不需要特别复杂的超参数

论文发现一个简单统一的 fine-tuning recipe 在多数任务上已经接近最佳结果。

这说明 PaliGemma 的 base model 本身迁移性较好。

---

## 5. 这篇论文的定位

PaliGemma 不是为了和 GPT-4V/Gemini 这种通用多模态助手直接竞争。

它更像是：

> 一个开放、轻量、可 fine-tune、适合研究和下游任务适配的视觉语言基础模型。

适合的应用包括：

- 自定义图像问答
- 领域 OCR / 文档理解
- 医学、遥感、工业图像理解
- 视觉 grounding
- 图像分割 / 检测的文本化建模
- 小规模数据上的任务迁移

---

## 6. 我的理解：这篇论文最有价值的地方

我觉得它的价值主要有三点：

1. **证明小型 VLM 仍然很有竞争力**  
   不是所有视觉语言任务都需要 7B、13B、70B 级别 LLM。

2. **强调 base model 和 instruction-tuned model 的区分**  
   PaliGemma 不是直接面向用户聊天，而是为研究者和工程师提供可迁移基础模型。

3. **系统研究了 VLM 训练细节**  
   包括分辨率、是否冻结视觉 encoder、预训练时长、connector 设计、few-shot fine-tuning 等，对做多模态模型的人很有参考价值。

---

简而言之，这篇论文讲的是：**如何用 SigLIP + Gemma 构建一个小而强、开放、适合迁移的视觉语言模型，并证明它在大量视觉语言任务上具有很好的微调性能。**