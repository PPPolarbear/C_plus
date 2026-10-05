# Day 56 · godbolt 看汇编与分析

**Block 19/19** · 阶段四（Day 46–57）

---

## ⚡ 现在就开始

打开 https://godbolt.org/

---

## 🎯 今天唯一的目标

**把"为什么慢"从猜测变成证据——看到具体是哪几条指令。**

这是区分"我学过 C++"和"我理解 C++"的地方。面试时能讲出这个，档次立刻不同。

---

## 📖 读（约 20 分钟）

- 搜「Compiler Explorer 使用教程」
- 了解 `-O0` / `-O2` 的区别

---

## ✍️ 写（约 120 分钟）

### 任务 1：验证"零开销抽象"

在 godbolt 里贴入：

```cpp
struct PointCpp { int x; int y; };
extern "C" struct PointC { int x; int y; };

int sum_cpp(const PointCpp& p) { return p.x + p.y; }
int sum_c(const PointC& p)     { return p.x + p.y; }
```

**用 `-O2` 编译，对比两者的汇编。** 应该**完全一样**——这就是"零开销抽象"。

### 任务 2：看虚函数的代价

```cpp
struct Base { virtual int f() { return 1; } };
struct Derived : Base { int f() override { return 2; } };

int call_virtual(Base* b) { return b->f(); }     // 虚函数：间接调用
int call_direct(Base* b)  { return b->Base::f(); } // 直接调用
```

**对比汇编**：虚函数会多出"读 vptr → 读 vtable → 间接 call"三步。

### 任务 3：看你的 MyVector 扩容

贴入一个简化版的 `push_back`：

```cpp
#include <cstddef>

template <typename T>
struct Vec {
    T*     data = nullptr;
    size_t size = 0, cap = 0;

    void push_back(const T& val) {
        if (size >= cap) {
            size_t nc = cap == 0 ? 1 : cap * 2;
            T* nd = new T[nc];
            for (size_t i = 0; i < size; ++i) nd[i] = data[i];
            delete[] data;
            data = nd;
            cap  = nc;
        }
        data[size++] = val;
    }
};

template struct Vec<int>;
```

**对比 `std::vector<int>::push_back`**（可以贴一个 `std::vector` 版本）。

观察：标准库多了哪些指令？可能是：
- `__builtin_expect` 分支预测提示
- `memcpy` 而不是循环（对平凡类型）
- 更紧凑的寄存器使用

### 任务 4：记录三个发现

| # | 现象 | 在 godbolt 里看到的证据 | 解释 |
|---|---|---|---|
| 1 | 零开销抽象 | | |
| 2 | 虚函数的代价 | | |
| 3 | 我的 push_back vs std | | |

**填完这张表。**

### 任务 5：理解 `-O0` 到 `-O2` 的变化

同一个函数，切换优化级别，观察汇编长度变化。

**结论**：`-O0` 的汇编又长又慢，`-O2` 会内联、消除冗余。**这就是为什么 benchmark 必须用 Release。**

---

## ✅ 验收（打勾才算过）

- [ ] 亲眼确认 `PointCpp` 和 `PointC` 编译出**相同汇编**
- [ ] 能看到虚函数的间接调用多出的指令
- [ ] 对比过自己的 `push_back` 和 `std::vector` 的汇编
- [ ] 三个发现记录在案
- [ ] 感受过 `-O0` 和 `-O2` 的差异
- [ ] **能对别人讲清"什么是零开销抽象"**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 找不到自己的函数 | 用 `extern "C"` 避免名字修饰，或在右侧搜索框过滤 |
| 汇编看不懂 | **不需要全懂**，看结构：有没有 loop、有没有 call、指令条数 |
| std::vector 汇编太长 | 只贴 `push_back` 调用点，别贴整个 vector 实现 |

---

## 🔜 明天（Day 57）

最后一天：README、推送、简历条目。**里程碑 4。**
