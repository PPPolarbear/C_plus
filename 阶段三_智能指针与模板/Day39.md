# Day 39 · 多线程计数实验 + 验收

**Block 13/19** · 阶段三（Day 31–45）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build
```

---

## 🎯 今天唯一的目标

**用数据证明原子计数的必要性，并把这个结论变成能讲给别人听的东西。**

---

## 📖 读（约 20 分钟）

不读新内容。

---

## ✍️ 写（约 120 分钟）

### 任务 1：非原子 vs 原子 对照实验

写一个可在两种模式间切换的测试（用宏或两个类）：

```cpp
// test_thread.cpp
#include <thread>
#include <vector>
#include <iostream>
#include <atomic>

constexpr int THREADS = 8;
constexpr int LOOPS   = 200000;

// ---- 非原子版本 ----
struct NaiveCounter {
    long count = 1;
    void add() { ++count; }
    void sub() { --count; }
};

// ---- 原子版本 ----
struct AtomicCounter {
    std::atomic<long> count{1};
    void add() { ++count; }
    void sub() { --count; }
};

template <typename Counter>
long run_test() {
    Counter c;
    std::vector<std::thread> ts;
    for (int i = 0; i < THREADS; ++i) {
        ts.emplace_back([&c]{
            for (int j = 0; j < LOOPS; ++j) {
                c.add();
                c.sub();
            }
        });
    }
    for (auto& t : ts) t.join();
    return c.count;
}

int main() {
    std::cout << "非原子版本结果: " << run_test<NaiveCounter>()  << " (应为 1)\n";
    std::cout << "原子版本结果  : " << run_test<AtomicCounter>() << " (应为 1)\n";
}
```

**跑 5 次，把结果记录下来。** 非原子版本大概率出现 != 1 的结果。

### 任务 2：记录数据

| 次数 | 非原子结果 | 原子结果 |
|---|---|---|
| 1 | | |
| 2 | | |
| 3 | | |
| 4 | | |
| 5 | | |

**把这张表填进 `进度追踪.md`。**

### 任务 3：面试答案模拟（**今天的重点**）

**对着空气讲 2 分钟**，回答这个问题：

> **"shared_ptr 是线程安全的吗？"**

标准答案的结构：

1. **先分清两个层面**：是"操作同一个 shared_ptr 对象"还是"操作不同的 shared_ptr 对象"？
2. **引用计数是安全的**：多个不同的 shared_ptr 副本在各自线程里拷贝/析构时，计数增减是原子的，不会算错
3. **指向的对象不安全**：`atomic` 保护的是计数，**不是 T**。多线程同时读写 `*p` 仍需自己加锁
4. **同一个 shared_ptr 变量也不安全**：多线程同时读写同一个 `shared_ptr` 对象（比如一个线程赋值、一个线程读），`ptr_` 和 `ref_count_` 的更新不是作为一个整体原子的

**讲不流畅就回去看 Day 37 的注释。**

---

## ✅ 验收（打勾才算过）

- [ ] 非原子版本**至少有一次**结果 != 1（数据记录在案）
- [ ] 原子版本 5 次全部 = 1
- [ ] 数据表填进了 `进度追踪.md`
- [ ] **能脱稿讲清"shared_ptr 是线程安全的吗"**（四个层次）
- [ ] 能说出循环引用的成因和解法

---

## ✅ Block 13 完成检查

- [ ] 引用计数为什么在堆上
- [ ] 拷贝赋值"先加后减"
- [ ] 原子计数的作用和边界
- [ ] 循环引用与 `weak_ptr`
- [ ] 能用数据支撑"为什么要原子计数"

**五条都打勾 → 进 Day 40。**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 链接错误 `pthread_create` | CMake 里加 `find_package(Threads REQUIRED)` + `target_link_libraries(app Threads::Threads)` |
| 非原子版本总是 1 | 加大 `LOOPS` 到 1000000，或多跑几次 |
| 讲不出来 | 不要背，先自己在纸上画两个 shared_ptr 指向同一个计数的图 |

---

## 🔜 明天（Day 40）

迭代器——让你的容器能用标准库算法。
