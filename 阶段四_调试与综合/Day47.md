# Day 47 · 三个 bug 定位实战

**Block 16/19** · 阶段四（Day 46–57）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && touch tests/buggy.cpp
```

---

## 🎯 今天唯一的目标

**只用 gdb，把三个 bug 一个一个找出来。**

不是"看一眼代码就知道哪错了"——是**用调试器定位**。

---

## 📖 读（约 20 分钟）

- **不读新内容。**对照 GNU GDB Manual “Examining Data”、“Examining the Stack”与“Continuing and Stepping”，只回看定位今天三个 bug 所需的命令

---

## ✍️ 写（约 120 分钟）

### 三个 bug 藏在下面这段代码里

```cpp
// tests/buggy.cpp
#include <iostream>

// ---------- Bug 1：返回局部数组 ----------
int* make_array(int n) {
    int arr[10];
    for (int i = 0; i < n && i < 10; ++i) arr[i] = i * i;
    return arr;                       // ← 有问题
}

// ---------- Bug 2：除零 ----------
int safe_divide(int a, int b) {
    return a / b;                     // ← 有问题
}

// ---------- Bug 3：释放后使用 ----------
void use_after_free() {
    int* p = new int(5);
    delete p;
    *p = 10;                          // ← 有问题
    std::cout << *p << "\n";
}

int main() {
    int* a = make_array(5);
    std::cout << "a[2] = " << a[2] << "\n";

    std::cout << "10 / 0 = " << safe_divide(10, 0) << "\n";

    use_after_free();

    return 0;
}
```

### 用 gdb 逐个定位

**Bug 1**：
```bash
gdb ./build/buggy
(gdb) b make_array
(gdb) r
(gdb) n                # 单步走
(gdb) p arr            # 看 arr 的地址
(gdb) p &arr[0]
(gdb) finish           # 执行完这个函数
(gdb) p a              # 看返回的指针
# 观察：返回的地址是不是已经失效了（栈帧已销毁）
```

**Bug 2**：
```bash
(gdb) b safe_divide
(gdb) c
(gdb) p a
(gdb) p b              # ← 看 b 是 0
(gdb) n                # 单步执行 a/b
# 观察：程序收到 SIGFPE 信号
(gdb) bt               # 看崩在哪
```

**Bug 3**：
```bash
(gdb) b use_after_free
(gdb) c
(gdb) n
(gdb) p p              # 记下地址
(gdb) n
(gdb) n                # 执行 delete
(gdb) p p              # 地址还是一样，但内存已经不属于你了
(gdb) n                # 执行 *p = 10
# 观察：可能不崩，但已经是 UB
```

### 记录表

| Bug | gdb 里的表现 | 根本原因 | 怎么修 |
|---|---|---|---|
| 1 | | | |
| 2 | | | |
| 3 | | | |

**填完这张表。**

---

## ✅ 验收（打勾才算过）

- [ ] 三个 bug 都用 gdb **定位到了具体行**
- [ ] Bug 2 亲眼看到 `SIGFPE` 信号
- [ ] 能说出 Bug 1 的返回地址为什么失效（**栈帧已销毁**）
- [ ] 能说出 Bug 3 为什么"可能不崩但已经错了"（**未定义行为**）
- [ ] 记录表填完
- [ ] 三个 bug 都修好了（Bug 1 用 `new[]`，Bug 2 加判断，Bug 3 去掉那行）

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| Bug 3 没崩 | 正常，UB 不一定崩。**重点是用 valgrind 能抓到** |
| gdb 里 `p arr` 显示一堆数字 | `arr` 是数组，用 `p arr[0]@10` 打印前 10 个 |
| 不知道 SIGFPE 是什么 | 浮点异常（这里是整数除零），信号名 `SIGFPE` |

---

## 🔜 明天（Day 48）

watch 监视点 + 栈帧切换，Block 16 收尾。
