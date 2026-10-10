# Day 46 · gdb 基础操作

**Block 16/19** · 阶段四（Day 46–57）· 主题：调试与综合

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build
gdb ./build/app
```

---

## 🎯 今天唯一的目标

**把 gdb 的十个常用命令练成肌肉记忆。**

不是"知道有这些命令"，是**不看小抄就能敲出来**。

---

## 📖 读（约 30 分钟）

- GNU GDB Manual：“Starting your Program”、“Breakpoints”、“Continuing and Stepping”、“Examining Data”、“Backtraces”
- 只整理本日要实际使用的断点、单步、打印变量和查看调用栈命令

---

## ✍️ 写（约 120 分钟）

### 十个必会命令

把这张表抄下来，然后**每个都亲手敲一遍**：

| 命令 | 简写 | 作用 |
|---|---|---|
| `break main` | `b main` | 在 main 下断点 |
| `break file.cpp:42` | `b` | 在某行下断点 |
| `run` | `r` | 启动程序 |
| `next` | `n` | 执行下一行（**不进入函数**） |
| `step` | `s` | 执行下一行（**进入函数**） |
| `continue` | `c` | 继续执行到下一个断点 |
| `print x` | `p x` | 打印变量 |
| `backtrace` | `bt` | 看调用栈 |
| `frame n` | `f n` | 切换到第 n 层栈帧 |
| `quit` | `q` | 退出 |

### 练习程序

```cpp
// debug_demo.cpp
#include <iostream>
#include <vector>

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int sum_vector(const std::vector<int>& v) {
    int s = 0;
    for (int x : v) s += x;
    return s;
}

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};
    int f = factorial(5);
    int s = sum_vector(v);
    std::cout << "f=" << f << " s=" << s << "\n";
    return 0;
}
```

### 实操清单

```bash
gdb ./build/debug_demo

# 1. 在 main 下断点
(gdb) b main
(gdb) r

# 2. 单步走，观察变量
(gdb) n
(gdb) p v
(gdb) n
(gdb) p v.size()

# 3. 进入函数（对比 next 和 step）
(gdb) b sum_vector
(gdb) c
(gdb) s              # 进入 sum_vector
(gdb) bt             # 看调用栈
(gdb) p v            # 打印 vector（试试看）
(gdb) p v[0]         # 打印单个元素
(gdb) f 0            # 切回 main
(gdb) c              # 跑完
```

### 特殊技巧

```bash
# 打印 vector 的所有元素
(gdb) p v
# 可能需要：/usr/share/gcc/python/libstdcxx 的 pretty printer
# 没有的话用：
(gdb) p v._M_impl._M_start[0]@5

# 条件断点
(gdb) b sum_vector if v.size() > 3

# 监视点（变量被改时停下）
(gdb) watch s

# 打印数组
(gdb) p *data_@10        # 打印 data_ 开始的 10 个元素
```

---

## ✅ 验收（打勾才算过）

- [ ] 十个命令**不看小抄**能敲出来
- [ ] 能说出 `next` 和 `step` 的区别
- [ ] 能在 gdb 里打印一个 `std::vector` 的内容
- [ ] 会用 `bt` 看调用栈，并用 `frame` 切换
- [ ] 会下条件断点
- [ ] 会用 `watch` 监视变量变化

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 看不到变量值 `<optimized out>` | 用 `-DCMAKE_BUILD_TYPE=Debug` 重新编译 |
| 打印 vector 报错 | 试试 `p v._M_impl._M_start[0]@5`，或装 pretty printer |
| gdb 找不到可执行文件 | 路径写完整：`gdb ./build/debug_demo` |

---

## 🔜 明天（Day 47）

三个真 bug 定位实战。
