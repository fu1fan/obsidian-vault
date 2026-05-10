---
title: Git Submodule 子仓库使用教程
created: 2026-05-10
tags:
  - git
  - submodule
  - obsidian
  - latex
---

# Git Submodule 子仓库使用教程

适用场景：整个 Obsidian Vault 由 Git 同步，但 Vault 内某些项目，例如 TeX / LaTeX 项目，需要有自己的独立 GitHub 仓库和提交历史。

例如本 Vault 中的案例：

```text
PORJECTS/PI0近似计算/产出/汇报
```

已被改成子仓库：

```text
https://github.com/fu1fan/pi0-report.git
```

---

## 1. 不要直接在 Vault 内随便 `git init`

如果在 Vault 的某个内部文件夹直接执行：

```bash
cd PORJECTS/my-paper
git init
```

外层 Vault 仓库会把它识别成一个嵌套 Git 仓库。

如果之后在外层仓库执行：

```bash
git add PORJECTS/my-paper
```

Git 可能会警告：

```text
warning: adding embedded git repository
```

这表示你添加了一个“嵌入式 Git 仓库”，但它还不是一个标准 submodule。别人 clone 外层 Vault 时，通常不会自动拿到里面的完整内容。

正确做法是：要么完全忽略这个目录，要么正式添加为 submodule。

---

## 2. 什么时候该用 submodule？

适合使用 submodule 的情况：

- Vault 内有独立项目，例如论文、LaTeX 汇报、代码实验；
- 这个项目需要独立 GitHub 仓库；
- 希望外层 Vault 只记录该项目对应的 commit 指针；
- 希望 GitHub 上点击 Vault 内的目录时能跳转到子仓库。

不适合使用 submodule 的情况：

- 只是普通笔记文件；
- 不想处理两层 Git 提交；
- 希望 Obsidian Git 插件无脑自动同步所有内容；
- 子项目里包含不该公开的文件，但你又打算创建 public 子仓库。

---

## 3. 新建一个 GitHub public 子仓库

可以用 GitHub CLI：

```bash
gh repo create fu1fan/pi0-report --public --description "PI0 report TeX project"
```

也可以在 GitHub 网页上手动创建。

创建后得到类似 URL：

```text
https://github.com/fu1fan/pi0-report.git
```

> 注意：如果是 public 仓库，务必确认里面没有隐私信息、API key、个人数据或不该公开的材料。

---

## 4. 将已有 Vault 内文件夹改成 submodule

以下以路径为例：

```bash
TARGET="PORJECTS/PI0近似计算/产出/汇报"
REMOTE_URL="https://github.com/fu1fan/pi0-report.git"
```

### 4.1 先在目标文件夹内初始化独立仓库

```bash
cd "$TARGET"
git init -b main
```

建议给 TeX 项目单独写一个 `.gitignore`：

```gitignore
# macOS
.DS_Store

# LaTeX build/intermediate files
*.aux
*.auxlock
*.bbl
*.bcf
*.blg
*.brf
*.dvi
*.fdb_latexmk
*.fls
*.glg
*.glo
*.gls
*.glsdefs
*.idx
*.ilg
*.ind
*.ist
*.lof
*.log
*.lot
*.nav
*.out
*.pdfsync
*.ps
*.run.xml
*.snm
*.synctex
*.synctex.gz
*.synctex.gz(busy)
*.toc
*.vrb
*.xdv
*.xmpi
*.pyg
*.listing
*.loa
*.lol
*.maf
*.mtc*
*.spl
*.thm
_minted*/
```

然后提交项目源文件：

```bash
git add .gitignore assets *.tex *.bib *.pdf
git commit -m "Initial project"
git remote add origin "$REMOTE_URL"
git push -u origin main
```

如果 `*.bib` 在 `assets/` 里，确认它已被包含；也可以显式指定文件：

```bash
git add .gitignore assets demo.tex demo.pdf
```

---

### 4.2 回到 Vault 外层仓库，移除原本直接追踪的文件

回到 Vault 根目录：

```bash
cd /path/to/your/vault
```

如果这个目录之前已经被外层 Vault 直接追踪过，需要先从外层索引移除，但保留本地文件：

```bash
git rm -r --cached -- "$TARGET"
```

这个命令的含义：

- 从外层 Vault 的 Git 索引中移除该目录下的普通文件；
- 不删除本地实际文件；
- 为下一步添加 submodule 做准备。

---

### 4.3 正式添加为 submodule

```bash
git submodule add "$REMOTE_URL" "$TARGET"
```

如果目标目录里已经是一个 Git 仓库，Git 会提示类似：

```text
Adding existing repo at '...' to the index
```

这是正常的。

然后建议执行：

```bash
git submodule absorbgitdirs -- "$TARGET"
```

这会把子仓库的 Git 元数据移动到外层仓库的：

```text
.git/modules/...
```

而子目录里只保留一个 `.git` 文件，指向真正的 gitdir。这是标准 submodule 布局。

---

### 4.4 检查 `.gitmodules`

外层 Vault 根目录会生成 `.gitmodules`：

```ini
[submodule "PORJECTS/PI0近似计算/产出/汇报"]
	path = PORJECTS/PI0近似计算/产出/汇报
	url = https://github.com/fu1fan/pi0-report.git
```

检查子模块状态：

```bash
git submodule status -- "$TARGET"
```

检查外层仓库是否只记录 submodule 指针：

```bash
git ls-files -s -- "$TARGET"
```

正常情况下会看到 mode 是：

```text
160000
```

这表示它是 submodule 指针，而不是普通文件。

---

### 4.5 提交外层 Vault 的指针更新

```bash
git add .gitmodules "$TARGET"
git commit -m "Convert project to submodule"
git push
```

外层仓库记录的是子仓库的某个具体 commit，而不是复制子仓库的全部文件内容。

---

## 5. 日常使用流程

### 5.1 修改子项目后，需要提交两次

假设修改了：

```text
PORJECTS/PI0近似计算/产出/汇报/demo.tex
```

先提交子仓库：

```bash
cd "PORJECTS/PI0近似计算/产出/汇报"
git status
git add demo.tex
git commit -m "Update report slides"
git push
```

然后回到 Vault 根目录，提交 submodule 指针更新：

```bash
cd -
git status
git add "PORJECTS/PI0近似计算/产出/汇报"
git commit -m "Update pi0-report submodule pointer"
git push
```

原因：

- 子仓库 commit 保存实际内容；
- 外层 Vault commit 保存“这个目录现在指向子仓库的哪个 commit”。

---

## 6. 在新设备 clone Vault

推荐直接递归 clone：

```bash
git clone --recurse-submodules https://github.com/fu1fan/obsidian-vault.git
```

如果已经 clone 了 Vault，但子模块目录是空的或没拉下来：

```bash
git submodule update --init --recursive
```

如果想把子模块更新到远程最新分支：

```bash
git submodule update --remote --merge
```

然后仍然需要在外层 Vault 提交新的指针：

```bash
git add "PORJECTS/PI0近似计算/产出/汇报"
git commit -m "Update pi0-report submodule pointer"
git push
```

---

## 7. 常见问题与修复

### 7.1 误把内部 Git 仓库当普通目录 add 了

如果看到：

```text
warning: adding embedded git repository
```

可以先取消外层索引：

```bash
git rm --cached path/to/project
```

如果已经提交了错误状态，则需要再提交一次修正 commit。

---

### 7.2 外层和内层同时追踪同一批文件

表现：

- 在子仓库里 `git status` 有修改；
- 在 Vault 根目录 `git status` 也显示同一批文件修改。

处理方式：

```bash
git rm -r --cached -- path/to/project
git submodule add <repo-url> path/to/project
git submodule absorbgitdirs -- path/to/project
git commit -m "Convert project to submodule"
```

---

### 7.3 子仓库有新 commit，但外层 Vault 没更新

外层会显示类似：

```text
modified: path/to/project (new commits)
```

这不是错误。进入外层仓库提交指针即可：

```bash
git add path/to/project
git commit -m "Update submodule pointer"
git push
```

---

### 7.4 Obsidian Git 插件可能不会完整处理 submodule

有些自动同步工具只会处理当前仓库，不一定会递归处理 submodule。

因此建议：

- 子仓库项目用命令行明确 `git commit && git push`；
- 外层 Vault 再提交一次指针；
- 不要完全依赖自动同步插件处理两层仓库。

---

## 8. 推荐检查命令

在 Vault 根目录：

```bash
# 查看外层状态
git status -sb

# 查看子模块状态
git submodule status

# 查看某个路径是否是 submodule 指针
git ls-files -s -- "PORJECTS/PI0近似计算/产出/汇报"

# 查看 .gitmodules
cat .gitmodules
```

在子仓库目录：

```bash
# 查看子仓库状态
git status -sb

# 查看子仓库远程
git remote -v

# 推送子仓库
git push
```

---

## 9. 最小命令备忘

已有目录改成 submodule 的核心流程：

```bash
TARGET="PORJECTS/PI0近似计算/产出/汇报"
REMOTE_URL="https://github.com/fu1fan/pi0-report.git"

# 1. 初始化并推送子仓库
cd "$TARGET"
git init -b main
git add .
git commit -m "Initial project"
git remote add origin "$REMOTE_URL"
git push -u origin main

# 2. 回到 Vault 根目录
cd /path/to/vault

# 3. 外层仓库移除普通文件追踪，但保留本地文件
git rm -r --cached -- "$TARGET"

# 4. 添加为 submodule
git submodule add "$REMOTE_URL" "$TARGET"
git submodule absorbgitdirs -- "$TARGET"

# 5. 提交外层指针
git add .gitmodules "$TARGET"
git commit -m "Convert project to submodule"
git push
```

---

## 10. 本 Vault 当前案例

当前案例对应：

```text
path: PORJECTS/PI0近似计算/产出/汇报
url:  https://github.com/fu1fan/pi0-report.git
```

外层 Vault 只记录该路径的 submodule 指针。实际 TeX 文件由 `pi0-report` 子仓库管理。
