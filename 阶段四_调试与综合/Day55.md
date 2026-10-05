# Day 55 · benchmark 编写与运行

**Block 19/19** · 阶段四（Day 46–57）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && mkdir -p bench && touch bench/bench_vector.cpp
```

---

## 🎯 今天唯一的目标

**跑出你的第一组性能数字。**

**没有数字的项目等于没有项目。** 今天的产出会直接变成简历上的一行字。

---

## 📖 读（约 20 分钟）

- 搜「C++ chrono 计时 高精度」
- 搜「google benchmark 使用」（可选，今天用 `chrono` 就够）

---

## ✍️ 写（约 120 分钟）

### 计时框架

```cpp
// bench/bench_vector.cpp
#include "myvector.h"
#include <vector>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>

template <typename Fn>
double time_it(Fn&& fn, int repeat = 1) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < repeat; ++i) fn();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count() / repeat;
}

void row(const std::string& name, double mine, double ref) {
    std::cout << std::left  << std::setw(28) << name
              << std::right << std::setw(12) << std::fixed << std::setprecision(2) << mine << " ms"
              << std::setw(12) << ref << " ms"
              << std::setw(10) << (mine / ref) << "x\n";
}

int main() {
    const int N = 1'000'000;

    std::cout << std::left << std::setw(28) << "测试项"
              << std::right << std::setw(12) << "MyVector"
              << std::setw(12) << "std::vector"
              << std::setw(10) << "比值" << "\n";
    std::cout << std::string(62, '-') << "\n";

    // ---- 1. push_back N 次 ----
    double a = time_it([&]{
        MyVector<int> v;
        for (int i = 0; i < N; ++i) v.push_back(i);
    }, 3);

    double b = time_it([&]{
        std::vector<int> v;
        for (int i = 0; i < N; ++i) v.push_back(i);
    }, 3);
    row("push_back 1e6", a, b);

    // ---- 2. 预留容量后 push_back ----
    a = time_it([&]{
        MyVector<int> v;
        v.reserve(N);
        for (int i = 0; i < N; ++i) v.push_back(i);
    }, 3);

    b = time_it([&]{
        std::vector<int> v;
        v.reserve(N);
        for (int i = 0; i < N; ++i) v.push_back(i);
    }, 3);
    row("reserve + push_back 1e6", a, b);

    // ---- 3. 拷贝构造 ----
    MyVector<int> src;
    std::vector<int> ref_src;
    for (int i = 0; i < N; ++i) { src.push_back(i); ref_src.push_back(i); }

    a = time_it([&]{ MyVector<int> c(src); }, 5);
    b = time_it([&]{ std::vector<int> c(ref_src); }, 5);
    row("拷贝构造 1e6", a, b);

    // ---- 4. 移动构造（应该接近 0）----
    a = time_it([&]{ MyVector<int> c(std::move(src)); src = MyVector<int>(ref_src); }, 5);
    // 简化：直接测空移动
    {
        MyVector<int> tmp;
        for (int i = 0; i < N; ++i) tmp.push_back(i);
        a = time_it([&]{ MyVector<int> c(std::move(tmp)); tmp = MyVector<int>(); }, 1);
    }
    row("移动构造 1e6", a, 0.01);   // std::vector 的移动是 O(1)

    // ---- 5. 随机访问 ----
    a = time_it([&]{
        volatile long long sum = 0;
        for (int i = 0; i < N; ++i) sum += src[i];
    }, 5);
    b = time_it([&]{
        volatile long long sum = 0;
        for (int i = 0; i < N; ++i) sum += ref_src[i];
    }, 5);
    row("随机访问 1e6", a, b);

    return 0;
}
```

### 编译（Release 模式）

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/bench_vector
```

**⚠️ 一定要用 Release！** Debug 模式下数字没有意义。

### 填表

跑完把结果填进来（**这是你的项目档案**）：

| 测试项 | MyVector | std::vector | 比值 |
|---|---|---|---|
| push_back 1e6 | | | |
| reserve + push_back | | | |
| 拷贝构造 1e6 | | | |
| 移动构造 1e6 | | | |
| 随机访问 1e6 | | | |

### 多跑几次取稳定值

```bash
for i in 1 2 3; do ./build/bench_vector; echo "---"; done
```

**第一次跑通常偏慢（冷启动），取后面几次的。**

---

## ✅ 验收（打勾才算过）

- [ ] benchmark 能在 Release 模式下跑出结果
- [ ] **移动构造明显快于拷贝构造**（最能说明问题的一项）
- [ ] 表格填完
- [ ] **能解释每个数字背后的原因**
      （比如：push_back 慢 1.2 倍 → 因为少了 XX 优化）
- [ ] 知道为什么要用 Release 模式

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 移动构造那一行写崩了 | 简化：单独构造一个 `MyVector`，测 `time_it([&]{ MyVector c(std::move(tmp)); })`，测完不管 |
| 数字波动很大 | 多跑几次取中位数；关掉其他占 CPU 的程序 |
| 和 std::vector 差距很大 | 正常，标准库有大量优化。**重点是能解释差距来自哪里** |

---

## 🔜 明天（Day 56）

用 godbolt 看汇编，把"为什么慢"落实到指令级。
