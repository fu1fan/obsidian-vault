---
title: Moloch Beamer 使用说明整理
source: "[[moloch.pdf]]"
created: 2026-05-10
tags:
  - latex
  - beamer
  - moloch
  - presentation
---

# Moloch Beamer 使用说明整理

> 基于本地文档 [[REFS/LaTeX/Beamer/moloch.pdf]] 整理。  
> Moloch 是一个极简、现代、可定制的 LaTeX Beamer 主题，继承自 Metropolis，目标是减少视觉噪音、把空间留给内容。

---

## 1. 快速结论

如果只是想马上开始，用这一套即可：

```tex
\documentclass[aspectratio=169]{beamer}

\usetheme[
  titlepage=moloch,
  sectionpage=progressbar,
  progressbar=frametitle
]{moloch}

\molochcolors{theme=paper, variant=light}

\title{你的标题}
\subtitle{副标题，可选}
\author{作者}
\institute{机构}
\date{\today}

\begin{document}

\maketitle

\section{第一部分}

\begin{frame}{第一页标题}
  \begin{itemize}
    \item 第一个要点
    \item 第二个要点
  \end{itemize}
\end{frame}

\begin{frame}[standout]
  Thank you!
\end{frame}

\end{document}
```

我个人推荐的默认风格：

```tex
\usetheme[
  titlepage=moloch,
  sectionpage=progressbar,
  progressbar=frametitle
]{moloch}
\molochcolors{theme=paper, variant=light}
```

如果是投影环境、教室、会议室：

```tex
\molochcolors{theme=highcontrast, variant=light}
```

如果是代码较多的技术分享：

```tex
\molochcolors{theme=tomorrow, variant=dark}
```

---

## 2. 安装

### 2.1 从 CTAN / TeX Live 安装

Moloch 已进入 CTAN。通常只要 TeX 发行版足够新，就可以直接使用：

```bash
tlmgr update --all
```

如果权限不足：

```bash
sudo tlmgr update --all
```

MacTeX 用户也可以用 **TeX Live Utility** 图形界面更新。

### 2.2 从源码安装

如果需要开发版：

```bash
git clone https://github.com/jolars/moloch.git
cd moloch
l3build install
```

### 2.3 依赖

Moloch 依赖：

- `beamer`
- `tikz`
- `pgfopts`
- `etoolbox`
- `calc`

一般现代 TeX Live / MacTeX 都已包含。

---

## 3. 最小示例

```tex
\documentclass{beamer}
\usetheme{moloch}

\title{A Minimal Example}
\date{\today}
\author{Johan Larsson}
\institute{Some University}

\begin{document}

\maketitle

\section{First Section}

\begin{frame}
  \frametitle{First Frame}

  Hello, world!

\end{frame}

\end{document}
```

也可以用 Beamer 简写：

```tex
\begin{frame}{First Frame}
  Hello, world!
\end{frame}
```

---

## 4. Pandoc 使用

如果用 Markdown / Pandoc 生成 Beamer：

```bash
pandoc -t beamer -V theme:moloch -o output.pdf input.md
```

常用增强参数示例：

```bash
pandoc input.md \
  -t beamer \
  -V theme:moloch \
  -V aspectratio:169 \
  -o output.pdf
```

Markdown frontmatter 示例：

```yaml
---
title: 我的演示
author: 作者
date: 2026-05-10
theme: moloch
aspectratio: 169
---
```

注意：较老版本 Pandoc 可能不能正确保留 `.standout` 等自定义 frame 属性。建议使用新版 Pandoc。

---

## 5. 选项设置方式

Moloch 使用 `key=value` 风格设置。

### 5.1 加载主题时设置

```tex
\usetheme[
  titlepage=moloch,
  sectionpage=progressbar,
  progressbar=frametitle
]{moloch}
```

### 5.2 演示过程中切换设置

```tex
\molochset{
  sectionpage=simple,
  progressbar=none
}
```

适合在不同章节临时调整风格。

---

## 6. Main Theme：标题格式

### `titleformat`

控制标题、子标题、section 标题、frame 标题，以及 standout 页文字格式。

可选值：

| 值 | 效果 |
|---|---|
| `regular` | 普通格式，推荐默认 |
| `smallcaps` | 小型大写 |
| `allsmallcaps` | 全部小型大写 |
| `allcaps` | 全部大写 |

示例：

```tex
\usetheme[titleformat=smallcaps]{moloch}
```

### 更细粒度的标题格式

可分别设置：

```tex
\usetheme[
  titleformat title=regular,
  titleformat subtitle=regular,
  titleformat section=smallcaps,
  titleformat frametitle=regular
]{moloch}
```

### 注意

不建议在有数学符号、数字、复杂标点的标题中使用 `allcaps` 或 `allsmallcaps`，因为数字、标点、数学符号不会像字母一样被转换，视觉上容易不一致。

---

## 7. Inner Theme：标题页、章节页、块环境

Inner theme 主要控制：

- 标题页
- section / subsection 分隔页
- standout 页
- block 样式

### 7.1 `titlepage`

控制标题页布局。

| 值 | 适合场景 |
|---|---|
| `moloch` | 默认，Moloch 自定义布局，有标题分隔线，推荐 |
| `plain` | 接近传统 Beamer 标题页 |
| `split` | 左右双栏标题页，适合作者、机构、日期信息较多的场合 |

示例：

```tex
\usetheme[titlepage=moloch]{moloch}
```

或：

```tex
\usetheme[titlepage=split]{moloch}
```

标题页元数据：

```tex
\title{Moloch Theme}
\subtitle{A Minimalist Beamer Theme}
\author{John Doe}
\institute{University of Example}
\date{\today}
\titlegraphic{\includegraphics[width=2cm]{assets/logo.pdf}}
```

### 7.2 `sectionpage`

控制每个 section 开头是否自动插入章节页。

| 值 | 效果 |
|---|---|
| `none` | 不显示章节页 |
| `simple` | 只显示章节标题 |
| `progressbar` | 显示章节标题，并加一条进度条；默认值 |

示例：

```tex
\usetheme[sectionpage=progressbar]{moloch}
```

也可以中途切换：

```tex
\molochset{sectionpage=simple}
\section{第二部分}

\molochset{sectionpage=none}
\section{第三部分}
```

建议：

- 长演示：`sectionpage=progressbar`
- 短演示：`sectionpage=simple` 或 `none`
- 如果章节很多，慎用自动章节页，避免节奏被打断。

### 7.3 `subsectionpage`

控制 subsection 开头是否自动插入小节页。

| 值 | 效果 |
|---|---|
| `none` | 默认，不显示 subsection 页 |
| `simple` | 显示小节标题 |
| `progressbar` | 显示小节标题和进度条 |

示例：

```tex
\usetheme[subsectionpage=simple]{moloch}
```

注意：如果同时启用 `sectionpage` 和 `subsectionpage`，可能出现连续两页都是分隔页。结构复杂时可以考虑：

```tex
\usetheme[
  sectionpage=none,
  subsectionpage=progressbar
]{moloch}
```

### 7.4 `titleseparator linewidth`

控制默认标题页中标题与作者信息之间分隔线的粗细。

```tex
\usetheme[titleseparator linewidth=2pt]{moloch}
```

隐藏分隔线：

```tex
\usetheme[titleseparator linewidth=0pt]{moloch}
```

### 7.5 `standoutnumbering`

控制 standout 页是否参与页码计数。

| 值 | 含义 |
|---|---|
| `none` | 默认；standout 页不计入 frame number |
| `hide` | 计入页码，但不显示页码 |
| `show` | 计入页码，并显示页码 |

示例：

```tex
\usetheme[standoutnumbering=show]{moloch}
```

---

## 8. Outer Theme：进度条、页码、frametitle 边距

Outer theme 控制演示外框层面的元素：

- 顶部 / 底部 / 标题下方进度条
- 页码
- frame title 样式和边距

### 8.1 `progressbar`

控制每页是否显示进度条。

| 值 | 效果 |
|---|---|
| `none` | 默认，不显示每页进度条 |
| `head` | 页面顶部显示进度条 |
| `foot` | 页面底部显示进度条 |
| `frametitle` | frame title 下方显示进度条 |

示例：

```tex
\usetheme[progressbar=frametitle]{moloch}
```

或中途切换：

```tex
\molochset{progressbar=head}
```

建议：

- 正式汇报：`progressbar=frametitle`
- 极简展示：`progressbar=none`
- 不建议同时让 section 页和每页都过度强调进度条，否则视觉会偏重。

### 8.2 `progressbar linewidth`

控制进度条粗细：

```tex
\usetheme[
  progressbar=frametitle,
  progressbar linewidth=1pt
]{moloch}
```

### 8.3 页码设置

Moloch 旧的 `numbering` 选项已废弃。现在推荐使用 Beamer 原生模板。

只显示当前页：

```tex
\setbeamertemplate{page number in head/foot}[framenumber]
```

显示当前页 / 总页数：

```tex
\setbeamertemplate{page number in head/foot}[totalframenumber]
```

### 8.4 `frametitle margin top/bottom/left/right`

控制 frame title 周围边距。

默认大致为：

- 上下：`1.4ex`
- 左右：`1.6ex`

示例：

```tex
\molochset{frametitle margin top=1cm}
\molochset{frametitle margin left=0pt}
```

常见用途：

- 标题太贴边：增大 left / top
- 想要更紧凑：减小 top / bottom
- 与机构模板对齐：调整 left / right

---

## 9. Color Theme：颜色主题

Moloch 的颜色系统有两层：

1. 选择预设主题
2. 用 `\molochcolors{...}` 微调局部颜色

### 9.1 推荐使用 `\molochcolors`

推荐写法：

```tex
\molochcolors{theme=paper, variant=light}
```

旧写法也存在：

```tex
\usetheme[colortheme=paper]{moloch}
```

但新写法更灵活。

### 9.2 预设颜色主题

| 主题 | 特点 | 推荐场景 |
|---|---|---|
| `default` | 接近 Metropolis / Moloch 默认风格 | 通用 |
| `highcontrast` | 高对比度，参考 Okabe-Ito 色板 | 投影、无障碍、低光环境 |
| `tomorrow` | 基于 Tomorrow 配色 | 代码多、技术分享 |
| `paper` | 白底黑字，无 frametitle 背景，极简 | 学术、论文汇报、打印风格 |
| `catppuccin` | 柔和粉彩，低对比 | 在线分享、审美型展示 |

示例：

```tex
\molochcolors{theme=default, variant=light}
\molochcolors{theme=highcontrast, variant=dark}
\molochcolors{theme=tomorrow, variant=dark}
\molochcolors{theme=paper, variant=light}
\molochcolors{theme=catppuccin, variant=light}
```

### 9.3 `variant`

控制亮色 / 暗色版本：

```tex
\molochcolors{variant=light}
\molochcolors{variant=dark}
```

注意：旧的 `background=dark` 已废弃，应改用：

```tex
\molochcolors{variant=dark}
```

### 9.4 `block`

控制 Beamer block 环境背景。

| 值 | 效果 |
|---|---|
| `transparent` | 默认，透明背景 |
| `fill` | 给 block 添加浅色背景 |

示例：

```tex
\usetheme[block=fill]{moloch}
```

或：

```tex
\molochset{block=fill}
```

建议：

- 如果经常使用 theorem / example / alertblock：`block=fill`
- 如果页面已经有图、表、公式：保持 `transparent` 更干净

---

## 10. 细粒度颜色自定义

### 10.1 基本语法

```tex
\molochcolors{
  option1=color1,
  option2=color2
}
```

### 10.2 可自定义颜色键

| 键 | 控制对象 |
|---|---|
| `alerted text` | `\alert{}` 和 alert block 标题 |
| `example text` | example block 标题 / 示例文本 |
| `normal text fg` | 正文前景色 |
| `normal text bg` | 正文背景色 |
| `frametitle fg` | frame title 文字色 |
| `frametitle bg` | frame title 背景色 |
| `progressbar fg` | 进度条前景色 |
| `progressbar bg` | 进度条背景色 |
| `title separator` | 标题页分隔线颜色 |
| `standout fg` | standout 页文字色 |
| `standout bg` | standout 页背景色 |

### 10.3 当前 variant 快速调整

```tex
\molochcolors{
  alerted text=purple,
  progressbar fg=teal
}
```

### 10.4 分别设置 light / dark

```tex
\molochcolors{
  light/alerted text=red!80!black,
  dark/alerted text=orange,
  light/progressbar fg=blue,
  dark/progressbar fg=cyan
}
```

然后可以自由切换：

```tex
\molochcolors{variant=light}
% ...
\molochcolors{variant=dark}
```

### 10.5 使用 Beamer 原生命令低层调整

如果 `\molochcolors` 没暴露某个颜色，可以用：

```tex
\setbeamercolor{progress bar}{fg=orange, bg=orange!50!black!30}
\setbeamercolor{title separator}{fg=teal}
\setbeamercolor{progress bar in head/foot}{parent=progress bar}
\setbeamercolor{progress bar in section page}{parent=progress bar}
```

---

## 11. Standout Frames：强调页

Moloch 提供一种特殊 frame：大字、居中、反色背景。适合：

- 结尾页
- 章节关键结论
- 一句话观点
- 休息 / Q&A / Thank you 页

基本写法：

```tex
\begin{frame}[standout]
  Thank you!
\end{frame}
```

带标题：

```tex
\begin{frame}[standout]{Conclusion}
  One idea per slide.
\end{frame}
```

如果需要 label，注意顺序：

```tex
\begin{frame}[label=conclusion, standout]{Conclusion}
  Awesome slide
\end{frame}
```

不要写成：

```tex
\begin{frame}[standout, label=conclusion]{Conclusion}
  Awesome slide
\end{frame}
```

因为 `standout` 内部会创建局部 group，label 可能失效。

---

## 12. 常见 Beamer 内容写法

### 12.1 列表

```tex
\begin{frame}{核心观点}
  \begin{itemize}
    \item 第一条
    \item 第二条
    \item 第三条
  \end{itemize}
\end{frame}
```

逐步显示：

```tex
\begin{frame}{逐步显示}
  \begin{itemize}[<+->]
    \item 第一条
    \item 第二条
    \item 第三条
  \end{itemize}
\end{frame}
```

### 12.2 两栏

```tex
\begin{frame}{左右布局}
  \begin{columns}[T]
    \begin{column}{0.48\textwidth}
      \begin{itemize}
        \item 左侧要点
        \item 支撑解释
      \end{itemize}
    \end{column}
    \begin{column}{0.48\textwidth}
      \includegraphics[width=\linewidth]{figure.pdf}
    \end{column}
  \end{columns}
\end{frame}
```

### 12.3 Block

```tex
\begin{frame}{Block 示例}
  \begin{block}{普通 block}
    这是普通说明。
  \end{block}

  \begin{alertblock}{注意}
    这是重点提醒。
  \end{alertblock}

  \begin{exampleblock}{例子}
    这是一个例子。
  \end{exampleblock}
\end{frame}
```

### 12.4 公式

```tex
\begin{frame}{一个公式}
  \[
    E = mc^2
  \]

  \begin{itemize}
    \item 用一句话解释公式含义
    \item 不要在同一页塞太多公式
  \end{itemize}
\end{frame}
```

### 12.5 图片

```tex
\begin{frame}{图片页}
  \centering
  \includegraphics[width=0.85\textwidth]{figure.pdf}
\end{frame}
```

### 12.6 代码

含 verbatim / minted / listings 的 frame 要加 `fragile`：

```tex
\begin{frame}[fragile]{代码示例}
\begin{verbatim}
print("hello")
\end{verbatim}
\end{frame}
```

---

## 13. 推荐预设方案

### 13.1 学术论文汇报

```tex
\documentclass[aspectratio=169]{beamer}
\usetheme[
  titlepage=moloch,
  sectionpage=progressbar,
  progressbar=frametitle,
  block=transparent
]{moloch}
\molochcolors{theme=paper, variant=light}
\setbeamertemplate{page number in head/foot}[totalframenumber]
```

特点：白底黑字，清爽，适合公式、图表、论文汇报。

### 13.2 技术分享 / 代码演示

```tex
\documentclass[aspectratio=169]{beamer}
\usetheme[
  titlepage=moloch,
  sectionpage=progressbar,
  progressbar=frametitle,
  block=fill
]{moloch}
\molochcolors{theme=tomorrow, variant=dark}
```

特点：暗色，更适合代码和投屏。

### 13.3 会议室投影 / 高可读性

```tex
\documentclass[aspectratio=169]{beamer}
\usetheme[
  titlepage=moloch,
  sectionpage=simple,
  progressbar=foot
]{moloch}
\molochcolors{theme=highcontrast, variant=light}
```

特点：对比强，适合投影仪质量不稳定的场景。

### 13.4 轻量极简

```tex
\documentclass[aspectratio=169]{beamer}
\usetheme[
  titlepage=plain,
  sectionpage=none,
  progressbar=none
]{moloch}
\molochcolors{theme=paper, variant=light}
```

特点：最不打扰内容。

---

## 14. 已知问题与规避

### 14.1 小型大写 / 全大写字体问题

不是所有字体都有 small caps。尤其使用 pdfLaTeX 默认 Computer Modern Sans 时，小型大写可能不可用。

建议：

```tex
\usetheme[titleformat=regular]{moloch}
```

不要为了风格强行使用：

```tex
\usetheme[titleformat=allsmallcaps]{moloch}
```

### 14.2 与其他 Beamer color theme 混用

不推荐这样：

```tex
\usetheme{moloch}
\usecolortheme{seahorse}
```

更稳妥的写法：

```tex
\useoutertheme{moloch}
\useinnertheme{moloch}
\usefonttheme{moloch}
\usecolortheme{seahorse}
```

这样不会先加载 Moloch 的 color theme，能减少冲突。

### 14.3 XeLaTeX + second screen notes 文本变白

如果使用 Beamer 的 second screen notes，并且用 XeLaTeX 编译，section 页之后文字可能变白。可在导言区加：

```tex
\makeatletter
\def\beamer@framenotesbegin{%
  \usebeamercolor[fg]{normal text}
  \gdef\beamer@noteitems{}%
  \gdef\beamer@notes{}%
}
\makeatother
```

### 14.4 Standout frame 的 label 顺序

正确：

```tex
\begin{frame}[label=conclusion, standout]{Conclusion}
  Awesome slide
\end{frame}
```

不推荐：

```tex
\begin{frame}[standout, label=conclusion]{Conclusion}
  Awesome slide
\end{frame}
```

### 14.5 旧 Pandoc 不支持 standout

Pandoc 1.17.2 之前可能忽略 `.standout` 这类自定义 frame 属性。解决方法：升级 Pandoc。

---

## 15. 设计建议

### 15.1 一页只讲一个核心点

Moloch 的优势是极简。如果一页里塞太多内容，会破坏主题的简洁感。

推荐：

- 一页一个主张
- 每页 3–5 个 bullet
- 公式页只放一组核心公式
- 图表页只放一张主图 + 少量解释

### 15.2 进度条只保留一个主要位置

不要让所有地方都有进度提示。推荐二选一：

```tex
sectionpage=progressbar
```

或：

```tex
progressbar=frametitle
```

如果二者都用，也要确保不显得过于“横线很多”。

### 15.3 颜色主题选择逻辑

- **paper**：正式、学术、打印友好
- **highcontrast**：投影、无障碍、低光环境
- **tomorrow**：代码、技术、暗色演示
- **catppuccin**：柔和、线上展示、审美优先
- **default**：接近 Metropolis 的通用选择

### 15.4 Block 不要滥用 fill

`block=fill` 会让 block 更显眼，但如果满页都是填充块，页面会显得拥挤。适合：

- 定理
- 例子
- 警示
- 小结

不适合：

- 每个 bullet 都包成 block
- 图表页再叠多个 block

### 15.5 Standout 页只放一句话

适合：

```tex
\begin{frame}[standout]
  One idea per slide.
\end{frame}
```

不适合：

- 大段文字
- 复杂公式
- 多层列表
- 多张图片

---

## 16. 我的常用模板

### 16.1 `main.tex`

```tex
\documentclass[aspectratio=169]{beamer}

\usetheme[
  titlepage=moloch,
  sectionpage=progressbar,
  progressbar=frametitle,
  block=transparent
]{moloch}

\molochcolors{theme=paper, variant=light}
\setbeamertemplate{page number in head/foot}[totalframenumber]

\usepackage{graphicx}
\usepackage{booktabs}
\usepackage{amsmath, amssymb}

\title{演示标题}
\subtitle{副标题}
\author{作者}
\institute{机构}
\date{\today}

\begin{document}

\maketitle

\section{背景}

\begin{frame}{问题背景}
  \begin{itemize}
    \item 背景一
    \item 背景二
    \item 背景三
  \end{itemize}
\end{frame}

\section{方法}

\begin{frame}{方法概览}
  \begin{columns}[T]
    \begin{column}{0.48\textwidth}
      \begin{itemize}
        \item 方法要点一
        \item 方法要点二
      \end{itemize}
    \end{column}
    \begin{column}{0.48\textwidth}
      \centering
      \includegraphics[width=\linewidth]{figures/method.pdf}
    \end{column}
  \end{columns}
\end{frame}

\section{结论}

\begin{frame}[standout]
  Thank you!
\end{frame}

\end{document}
```

### 16.2 编译命令

```bash
latexmk -xelatex main.tex
```

或：

```bash
latexmk -pdf main.tex
```

如果中文较多，通常建议 XeLaTeX / LuaLaTeX，并配置中文字体包，例如 `ctex`。

---

## 17. 速查表

### 17.1 主题加载

```tex
\usetheme{moloch}
```

```tex
\usetheme[
  titlepage=moloch,
  sectionpage=progressbar,
  progressbar=frametitle,
  block=fill
]{moloch}
```

### 17.2 运行中修改

```tex
\molochset{progressbar=none}
\molochset{sectionpage=simple}
\molochset{block=fill}
```

### 17.3 配色

```tex
\molochcolors{theme=paper, variant=light}
\molochcolors{theme=tomorrow, variant=dark}
\molochcolors{theme=highcontrast, variant=light}
```

### 17.4 页码

```tex
\setbeamertemplate{page number in head/foot}[framenumber]
\setbeamertemplate{page number in head/foot}[totalframenumber]
```

### 17.5 Standout

```tex
\begin{frame}[standout]
  Thank you!
\end{frame}
```

带 label：

```tex
\begin{frame}[label=conclusion, standout]{Conclusion}
  Thank you!
\end{frame}
```

---

## 18. 推荐默认组合

我会优先用这一套：

```tex
\documentclass[aspectratio=169]{beamer}
\usetheme[
  titlepage=moloch,
  sectionpage=progressbar,
  progressbar=frametitle,
  block=transparent
]{moloch}
\molochcolors{theme=paper, variant=light}
\setbeamertemplate{page number in head/foot}[totalframenumber]
```

原因：

- `aspectratio=169`：适配现代屏幕和投影
- `titlepage=moloch`：保留 Moloch 特色标题页
- `sectionpage=progressbar`：章节切换有节奏
- `progressbar=frametitle`：每页进度提示清楚但不夸张
- `block=transparent`：默认保持页面干净
- `paper`：学术 / 工作汇报更稳
- `totalframenumber`：听众能知道总进度

---

## 19. 相关链接

- 本地手册：[[REFS/LaTeX/Beamer/moloch.pdf]]
- GitHub：<https://github.com/jolars/moloch>
- CTAN：<https://ctan.org/pkg/beamertheme-moloch>
- Beamer 文档：<https://ctan.org/pkg/beamer>
