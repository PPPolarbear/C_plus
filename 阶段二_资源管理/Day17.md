# Day 17 · 复现浅拷贝 double free

**Block 6/19** · 阶段二（Day 16–30）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && touch include/shallow_copy.h src/shallow_copy.cpp
```

---

## 🎯 今天唯一的目标

**亲手制造一个崩溃，并看懂它为什么崩。**

**看懂 bug 比看懂正确代码更有价值。** 今天这一步会是你整个阶段二的地基。

---

## 📖 读（约 30 分钟）

- LearnCpp 14.14 “Introduction to the copy constructor”、14.15 “Class initialization and copy elision”
- 用今天的崩溃实验观察：两个对象复制后是否仍指向同一块动态内存；只读概念，不看修复代码

---

## ✍️ 写（约 120 分钟）

### 任务 1：写一个"有病"的类

```cpp
// include/shallow_copy.h
#pragma once
#include <cstddef>

class ShallowCopy {
public:
    explicit ShallowCopy(size_t n);   // new int[n]，填 0..n-1
    ~ShallowCopy();                   // delete[] data_

    // ⚠️ 故意不写拷贝构造和拷贝赋值！
    
    void  print() const;
    int   get(size_t i) const;
    void  set(size_t i, int v);
    size_t size() const;

private:
    int*   data_;
    size_t size_;
};
```

### 任务 2：观察崩溃

```cpp
int main() {
    ShallowCopy a(5);
    a.print();               // 0 1 2 3 4

    ShallowCopy b = a;       // ← 浅拷贝！编译器生成的默认拷贝构造
    b.set(0, 999);

    a.print();               // ← 看！a 也被改了！
    b.print();

    std::cout << "即将离开 main\n";
    return 0;                // ← 两个对象析构，delete 同一块内存 → 崩溃
}
```

### 任务 3：把现象记录下来

跑之前先**预测**会发生什么，跑完对比：

| 问题 | 我的预测 | 实际结果 |
|---|---|---|
| `a.print()` 会打印什么？ | | |
| 程序会崩溃吗？在哪一行？ | | |
| 崩溃的原因是什么？ | | |

### 任务 4：用 valgrind 看它的诊断

```bash
cmake --build build
valgrind --leak-check=full ./build/app
```

**把 valgrind 的关键输出抄下来。** 它会同时报出：
- `Invalid free()` 或 `double free`
- `definitely lost`（内存泄漏）

---

## ✅ 验收（打勾才算过）

- [ ] **亲眼看到了 `a` 被 `b.set(0, 999)` 改掉**（浅拷贝的证据）
- [ ] **亲眼看到程序崩溃**
- [ ] valgrind 报出了 double free
- [ ] 能解释：为什么两个对象析构时会 delete 同一块内存
- [ ] 能说出修复思路：**"写拷贝构造函数，让它做深拷贝"**（明天不写，Day 22 写）
- [ ] 能一句话说出浅拷贝 vs 深拷贝：**"浅拷贝复制指针，深拷贝复制指针指向的内容"**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 程序没崩 | 换个编译器/优化级别，或者连续跑几次。也可直接看 valgrind 报告 |
| 崩溃信息看不懂 | 用 `gdb ./build/app`，崩了之后敲 `bt` 看调用栈 |
| 想直接修好它 | **忍住。** 今天的目的就是看懂病，不是治病 |

---

## 🔜 明天（Day 18）

用 valgrind 系统性地解剖这个崩溃，Block 6 收尾。
