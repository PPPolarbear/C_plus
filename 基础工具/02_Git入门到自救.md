# 02 · Git 入门到自救

> **适用**：会 `add` / `commit` / `push`，一遇到报错就卡住，分支基本没碰过。
> **读 + 练约 2 小时。**（这份最长，因为你也最需要它）

---

## ⚡ 现在就开始

```bash
cd /d D:\AI\C_plus_TEST
git status
```

看到 `On branch main` 和一堆文件状态，说明你在一个 git 仓库里。往下。

> 如果报 `fatal: not a git repository` —— 说明你**不在**仓库目录里。这是你上次撞到的第一个错，第五节会讲。

---

## 🎯 这一份解决什么问题

你上次的经历很典型：**命令敲了，报错了，不知道该看哪一行。**

这份的目标不是让你背命令，是让你**看到报错能自己判断**。

---

## 一、心智模型：三个区域

**这是理解 Git 的第一道坎。** 记住这张图，后面全都通了：

```
   工作区              暂存区              本地仓库           远程仓库
 (你的文件夹)        (staging)          (.git/)          (GitHub)
      │                  │                  │                 │
      │   git add        │   git commit     │   git push      │
      ├─────────────────▶├─────────────────▶├────────────────▶│
      │                  │                  │                 │
      │◀─────────────────┤◀─────────────────┤◀────────────────┤
      │   （改文件）      │  git restore     │   git pull /    │
      │                  │  --staged        │   git fetch     │
```

| 区域 | 是什么 | 怎么看 |
|---|---|---|
| **工作区** | 你眼前能编辑的文件 | 直接打开文件夹 |
| **暂存区** | "下次提交要包含哪些改动"的清单 | `git status` |
| **本地仓库** | 已经提交的历史 | `git log` |
| **远程仓库** | GitHub 上的那份 | 网页上看 |

**为什么要有"暂存区"**：因为一次提交应该只做一件事。改了三处，你想分两次提交，就靠暂存区来挑选。

---

## 二、基本流程

```bash
git status              # ① 先看状态（永远先做这个）
git add file.cpp        # ② 把改动放入暂存区
git add .               #    或者加全部
git commit -m "说明"     # ③ 提交
git push                # ④ 推到远程
```

### `git status` 的三种状态

```
Untracked files:      ← 新文件，Git 还没管
        newfile.cpp

Changes to be committed:   ← 已经在暂存区，等着被提交
        modified:   a.cpp

Changes not staged for commit:   ← 改了但还没 add
        modified:   b.cpp
```

**养成习惯：每次操作前先 `git status`。** 90% 的"我搞不清楚现在什么状态"都能靠它解决。

### 提交信息怎么写

```bash
# ❌ 差
git commit -m "改了一下"

# ✅ 好
git commit -m "feat: MyVector 实现移动构造，copy 次数降低 60%"
```

常用前缀：`feat:` 新功能 / `fix:` 修 bug / `docs:` 文档 / `refactor:` 重构 / `test:` 测试

---

## 三、分支模型（**你最缺的部分**）

### 为什么需要分支

**分支 = 一条独立的开发线。** 你可以在分支上乱改，不影响 `main`。改好了再合并回去。

```
main:     A ── B ── C ──────────── F
                    \             /
feature:             D ── E ──────
```

### 基本操作

```bash
git branch                    # 列出所有分支（* 是当前所在）
git branch feature-x          # 创建分支
git switch feature-x          # 切换过去（新命令）
git checkout feature-x        # 切换（老命令，效果一样）
git switch -c feature-x       # 创建 + 切换，一步到位（最常用）

git switch main               # 切回主分支
git branch -d feature-x       # 删除分支（已合并）
git branch -D feature-x       # 强制删除（未合并）
```

> **`switch` vs `checkout`**：`checkout` 是老命令，功能太多太杂（既能切分支又能恢复文件）。Git 2.23 拆出了 `switch`（切分支）和 `restore`（恢复文件）。**新代码用 `switch`。**

### 合并

```bash
git switch main
git merge feature-x           # 把 feature-x 合并进 main
```

两种合并结果：

| 情况 | 结果 |
|---|---|
| main 没动过 | **快进合并**（Fast-forward），`main` 直接指到 feature 的位置，干净 |
| main 也动过 | **三方合并**，产生一个 merge commit |

### ⚠️ 冲突（Conflict）

两个分支改了**同一文件的同一行**，Git 不知道怎么合，就会冲突：

```
<<<<<<< HEAD
int x = 1;          ← 当前分支（main）的内容
=======
int x = 2;          ← 要合并进来的分支的内容
>>>>>>> feature-x
```

**处理步骤**：

1. 打开冲突文件，手动决定要哪个（**把 `<<<<<<<` `=======` `>>>>>>>` 三行标记都删掉**）
2. `git add 冲突文件`
3. `git commit`

```bash
git status                # 冲突时会告诉你哪些文件冲突了
git merge --abort         # 反悔：放弃这次合并，回到合并前
```

**冲突不可怕，它只是"Git 不知道该选哪个，让你来定"。**

---

## 四、远程仓库

```bash
git remote -v                              # 看远程地址
git remote add origin <url>                # 添加远程
git remote set-url origin <url>            # 改地址（注意 origin 不能少！）
git push -u origin main                    # 推送，-u 记住这个对应关系
git push                                   # 之后直接 push 就行
git pull                                   # 拉取并合并远程改动
git fetch                                  # 只拉取不合并
```

### HTTPS vs SSH

| | HTTPS | SSH |
|---|---|---|
| 地址 | `https://github.com/用户/仓库.git` | `git@github.com:用户/仓库.git` |
| 认证 | Token / 浏览器 OAuth | SSH 密钥 |
| 换机器 | 要重新配 | 要重新配密钥 |
| **推荐** | | ✅ **更稳，不用反复登录** |

### 配置 SSH（一次性）

```bash
ssh-keygen -t ed25519 -C "你的邮箱"        # 一路回车
cat ~/.ssh/id_ed25519.pub                   # 复制输出
```

然后加到 GitHub：**Settings → SSH and GPG keys → New SSH key**

```bash
ssh -T git@github.com                       # 测试
# 输出 "Hi 用户名! You've successfully authenticated" 就对了
```

> **`ssh -T` 的这行输出会告诉你密钥挂在哪个账号下** —— 这是排查账号问题最快的方法。

---

## 五、⭐ 报错自救手册（**收藏这一节**）

### A. `fatal: not a git repository`

**原因**：你当前**不在** git 仓库里。（你上次在 `~` 目录敲 `git remote set-url` 就是这个）

**解法**：
```bash
cd /d D:\AI\C_plus_TEST      # 先进仓库
git status                    # 确认看到 "On branch main"
```
**判断标准**：目录里有没有 `.git` 文件夹。

---

### B. `usage: git remote set-url <name> <newurl>`

**原因**：命令**参数没给全**。`set-url` 需要先告诉它改哪个远程（名字）。

**解法**：
```bash
git remote set-url origin <新地址>
#                   ↑↑↑↑↑↑ 不能少
git remote -v                 # 改完必须确认
```

**所有 `git remote` 子命令都要求先给远程名。** 默认叫 `origin`。

---

### C. `Permission to 某人/仓库.git denied to 另一个人` + `403`

**原因**：**两个账号不一致**。前半是仓库所有者，后半是你实际登录的账号。

**解法**（三选一）：
1. 去 GitHub 网页**退出当前账号，登录仓库所有者那个**
2. 换 SSH（见第四节）——**最省事**
3. 把另一个账号加为协作者：仓库 Settings → Collaborators

**验证方式**：
```bash
ssh -T git@github.com         # 会直接告诉你当前是哪个账号
```

---

### D. `! [rejected] main -> main (non-fast-forward)` / `fetch first`

**原因**：**远程有你本地没有的提交**（通常是你在网页上改了文件，比如加了 README）。

**解法**：
```bash
git pull --rebase origin main     # 先拉下来
git push                          # 再推
```

**不要**用 `git push -f`（强推会**永久删掉**远程的提交）。

---

### E. `fatal: refusing to merge unrelated histories`

**原因**：本地和远程是**两条互不相干的历史**（常见于：先在网页建了仓库带 README，又在本地 `git init`）。

**解法**：
```bash
git pull origin main --allow-unrelated-histories
```
手动解决冲突后 `git add` + `git commit`，再 push。

**预防**：在 GitHub 建仓库时**不要勾** "Add a README" / ".gitignore" / "license"。

---

### F. `error: Your local changes would be overwritten by merge`

**原因**：你有**未提交的改动**，`pull` 会覆盖它。

**解法**：
```bash
git stash              # 把改动先存起来
git pull
git stash pop          # 再取回来
```

---

### G. 提交错了 / 想撤销

| 想干什么 | 命令 | 危险度 |
|---|---|---|
| 撤销**未暂存**的改动 | `git restore file.cpp` | 低（改动会丢） |
| 撤销**已暂存**的改动 | `git restore --staged file.cpp` | 低 |
| 改**最后一次**提交的信息 | `git commit --amend -m "新信息"` | 低 |
| 忘了加文件 | `git add 漏掉的文件` → `git commit --amend --no-edit` | 低 |
| 撤销最后一次提交（保留改动） | `git reset --soft HEAD~1` | 中 |
| 撤销最后一次提交（丢弃改动） | `git reset --hard HEAD~1` | **高** |
| 撤销**已推送**的提交 | `git revert <hash>` | 中（安全，产生反向提交） |

> ⚠️ `--hard` 会**真的删掉你的改动**。用之前先确认。

---

### H. 🆘 终极救命：`git reflog`

**搞砸了任何东西，先别慌，敲这一条：**

```bash
git reflog
```

它记录了你**所有**的操作历史（包括被 reset 掉的提交）：

```
8e38b8f HEAD@{0}: reset: moving to HEAD~1
a1b2c3d HEAD@{1}: commit: 我误删的那个提交
```

找到你想回去的那个 hash，然后：

```bash
git reset --hard a1b2c3d       # 回到那个状态
# 或者
git switch -c rescue a1b2c3d   # 用那个提交建个新分支，更安全
```

**只要提交过，就几乎不会真的丢。** `reflog` 是 Git 的后悔药。

---

## 六、.gitignore

告诉 Git "这些文件不要管"：

```gitignore
# 构建产物
build/
build-*/
*.o
*.exe

# 编辑器
.vscode/
.idea/

# 日志
*.log
vg.log

# 系统
.DS_Store
Thumbs.db
```

**⚠️ 重要**：如果文件**已经被提交过**，加进 `.gitignore` 不会让它消失。要先：

```bash
git rm --cached 文件
```

---

## 七、练习（**做完再开始 Day 01**）

在 `C_plus_TEST` 里做：

- [ ] `git status` —— 能读懂三种状态分别是什么
- [ ] 建一个分支 `practice`，改一个文件，提交，切回 `main`
- [ ] 把 `practice` 合并进 `main`
- [ ] **故意制造一个冲突**：在 `main` 和另一个分支改同一行，合并，手动解决
- [ ] 用 `git log --oneline --graph --all` 看分支图
- [ ] `git reflog` 看看你刚才的操作记录
- [ ] 写一个 `.gitignore` 把 `build/` 排除掉

---

## ✅ 自测（不看小抄能答上来才算过）

- [ ] 说出 Git 的三个区域，以及 `add` / `commit` / `push` 各自把改动从哪送到哪
- [ ] `git remote set-url` 为什么必须带 `origin`
- [ ] `Permission denied ... 403` 说明什么问题，怎么快速验证
- [ ] `non-fast-forward` 怎么修（不能用强推）
- [ ] 冲突标记长什么样，处理的三步是什么
- [ ] `git reflog` 能救回什么
- [ ] `git stash` 什么时候用
- [ ] `--soft` 和 `--hard` 的区别

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 不知道现在什么状态 | **`git status`**，永远先敲这个 |
| 提交信息写错了 | `git commit --amend -m "新信息"` |
| 忘了加文件 | `git add 文件` → `git commit --amend --no-edit` |
| 想放弃所有本地改动 | `git restore .`（危险） |
| 分支删错了 | `git reflog` 找回 hash，再建分支 |
| 仓库整个乱了 | `git reflog`，或者重新 clone 一份 |

---

## 🔜 下一份

`03_Makefile.md` —— 理解 CMake 背后生成的到底是什么。
