# Day 21 · 调试 + 观察扩容策略

**Block 7/19** · 阶段二（Day 16–30）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build && ./build/app
```

---

## 🎯 今天唯一的目标

**给 `MyVector` 加一个"能被观察"的能力，并为性能埋一个伏笔。**

今天做的事，会在 Day 55 变成你简历上的数字。

---

## 📖 读（约 30 分钟）

- LearnCpp 16.10 “std::vector resizing and capacity”
- cppreference：`std::vector` 页面中的 “Complexity” 与 “Capacity” 两节；只读扩容复杂度和容量变化

---

## ✍️ 写（约 120 分钟）

### 任务 1：给 MyVector 加调试输出

```cpp
template <typename T>
class MyVector {
public:
    // ...
    void dump() const {
        std::cout << "[size=" << size_ << " cap=" << capacity_ << "] ";
        for (size_t i = 0; i < size_; ++i) std::cout << data_[i] << " ";
        std::cout << "\n";
    }

    // 统计扩容次数（供实验用）
    static size_t realloc_count() { return realloc_count_; }
    static void   reset_realloc_count() { realloc_count_ = 0; }
private:
    static size_t realloc_count_;
};
template <typename T> size_t MyVector<T>::realloc_count_ = 0;
```

在 `reallocate` 里 `++realloc_count_;`

### 任务 2：增长策略对比实验

```cpp
void compare_growth() {
    const int N = 10000;

    // 策略 A：2 倍
    MyVector<int>::reset_realloc_count();
    MyVector<int> a;
    for (int i = 0; i < N; ++i) a.push_back(i);
    std::cout << "2倍策略 realloc 次数: " << MyVector<int>::realloc_count() << "\n";

    // 策略 B：+1（临时改成 +1 再编译一次）
    // ...
}
```

**把两种策略的次数都记下来。**

### 任务 3：给 T 加"构造/析构计数器"

```cpp
struct Counted {
    static int ctor, dtor, copy, move;
    int v = 0;
    Counted()          { ++ctor; }
    Counted(int x) : v(x) { ++ctor; }
    Counted(const Counted& o) : v(o.v) { ++copy; }
    Counted(Counted&& o) noexcept : v(o.v) { ++move; }
    ~Counted() { ++dtor; }
};

MyVector<Counted> v;
for (int i = 0; i < 10; ++i) v.push_back(Counted(i));
std::cout << "ctor=" << Counted::ctor << " dtor=" << Counted::dtor
          << " copy=" << Counted::copy << " move=" << Counted::move << "\n";
```

**观察：`ctor` 和 `dtor` 数量是否相等？** 不等就是有泄漏。

**观察：`copy` 有多少次？** 记住这个数字——Day 26 加上移动语义后它会大幅下降。

---

## ✅ 验收（打勾才算过）

- [ ] `dump()` 能正确打印容量和内容
- [ ] 拿到了 2 倍 vs +1 策略的 `realloc` 次数对比
- [ ] `Counted` 的 `ctor == dtor`（无泄漏）
- [ ] **记下了当前的 `copy` 次数**（这是 Day 26 的对比基准）
- [ ] valgrind 零泄漏

---

## ✅ Block 7 完成检查

- [ ] `MyVector` 能创建、push_back、自动扩容、正确析构
- [ ] 能解释 2 倍扩容为什么是均摊 O(1)
- [ ] 有可观察的调试手段（dump + 计数器）

**三条都打勾 → 进 Day 22。**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 模板里的 static 成员不会定义 | 写法：`template <typename T> size_t MyVector<T>::realloc_count_ = 0;` |
| `Counted` 计数器不对 | 记得 `Counted` 的移动构造要标 `noexcept` |

---

## 🔜 明天（Day 22）

写拷贝构造函数——正式解决 Day 17 那个 double free。
