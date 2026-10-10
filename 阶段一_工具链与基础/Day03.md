# Day 03 · 完整流程跑通 + gdb 初体验

**Block 1/19** · 阶段一（Day 1–15）

---

## ⚡ 现在就开始

```bash
cd cpp-camp && cmake --build build && ./build/app
```

能跑通就开始今天。**跑不通先回去修。**

---

## 🎯 今天唯一的目标

**从零重新走一遍完整流程，并第一次在 gdb 里让程序停下来。**

---

## 📖 读（约 20 分钟）

- LearnCpp 0.9 “Configuring your compiler: Build configurations”，只看 Debug 配置的作用
- GNU GDB Manual：“Starting your Program”、“Breakpoints”、“Continuing and Stepping”、“Examining Data”
- 本日只练 `break` / `run` / `next` / `print`，不扩展阅读其他 GDB 命令

---

## ✍️ 写（约 120 分钟）

### 任务 1：从零重建（不看昨天的文件）

新建 `cpp-camp2/`，**凭记忆**重建一套多文件项目。

卡住了才回去看 Day 01。**这一步检验你是真会还是抄会。**

### 任务 2：打开调试符号

确认 `CMakeLists.txt` 里有：

```cmake
set(CMAKE_BUILD_TYPE Debug)
```
或编译时带 `-g`。

### 任务 3：gdb 初体验

```bash
cd cpp-camp2
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
gdb ./build/app
```

在 gdb 里依次敲：

```
break main        # 在 main 开头下断点
run               # 跑起来，会停住
next              # 执行下一行
print a           # 打印变量 a
next              # 再下一行
print b           # 打印变量 b
continue          # 继续跑完
quit              # 退出
```

**目标不是学会 gdb，是确认它能用。**

### 任务 4：看一眼汇编（可选）

去 https://godbolt.org/，把你的 `add` 函数贴进去，看编译出来的汇编是什么样。

---

## ✅ 验收（打勾才算过）

- [ ] `cpp-camp2/` 在没有参考昨天文件的情况下建成了
- [ ] 能跑通完整的 `cmake -B build && cmake --build build && ./build/app`
- [ ] 在 gdb 里成功让程序停在 `main`，并打印出至少一个变量
- [ ] 能说出 `break` / `next` / `print` 各自干什么
- [ ] 打开过 godbolt 看过一次汇编（哪怕看不懂）

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| gdb 里看不到变量值 `<optimized out>` | 没开 Debug 模式，加 `-DCMAKE_BUILD_TYPE=Debug` 重新编译 |
| `No symbol table is loaded` | 同上，缺 `-g` |
| 完全重建失败 | 允许回看 Day 01，但**记下你忘了哪一步** |

---

## ✅ Block 1 完成检查

- [ ] 能手动建一个多文件 CMake 项目
- [ ] 分得清编译错误和链接错误
- [ ] 能在 gdb 里下断点、看变量

**三条都打勾 → 进 Day 04。**

---

## 🔜 明天（Day 04）

开始补基础语法：类型转换、控制流。
