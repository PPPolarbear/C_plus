# Day 37 · 原子计数与线程安全

**Block 13/19** · 阶段三（Day 31–45）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build
```

---

## 🎯 今天唯一的目标

**把 `long*` 换成 `std::atomic<long>*`，并理解它解决了什么、没解决什么。**

---

## 📖 读（约 40 分钟）

- 搜「C++ shared_ptr 线程安全」
- 搜「C++ atomic 原子操作」
- **重点：找到"shared_ptr 是不是线程安全的"这个问题的标准答案**

---

## ✍️ 写（约 110 分钟）

### 任务 1：改成原子计数

```cpp
#include <atomic>

template <typename T>
class MySharedPtr {
    // ...
private:
    T*                 ptr_;
    std::atomic<long>* ref_count_;    // ← 改成原子类型
};
```

所有 `++(*ref_count_)` / `--(*ref_count_)` 不需要改写法，`std::atomic` 重载了这些运算符。

构造处：

```cpp
explicit MySharedPtr(T* p)
    : ptr_(p)
    , ref_count_(p ? new std::atomic<long>(1) : nullptr)
{}
```

### 任务 2：写一个"出错的"多线程测试

先用**非原子**版本（把 `std::atomic<long>` 换回 `long`）跑这个测试：

```cpp
#include <thread>
#include <vector>

void stress_test() {
    MySharedPtr<int> base(new int(0));

    std::vector<std::thread> threads;
    for (int t = 0; t < 8; ++t) {
        threads.emplace_back([&base]{
            for (int i = 0; i < 100000; ++i) {
                MySharedPtr<int> copy = base;   // 计数 +1
            }                                    // copy 析构，计数 -1
        });
    }
    for (auto& t : threads) t.join();

    std::cout << "最终计数: " << base.use_count() << "\n";   // 应该是 1
}
```

**非原子版本**：多跑几次，观察计数是否偶尔 != 1，或者是否崩溃。

**原子版本**：稳定输出 1。

> 如果非原子版本也很稳定，说明你的机器缓存一致性太强——多跑几次，或加大线程数。

### 任务 3：写清"线程安全的边界"

在代码注释里写下这段（**这是面试标准答案**）：

```cpp
// ============================================
// MySharedPtr 的线程安全边界
// ============================================
// ✅ 安全：多个线程【拷贝/析构同一个 shared_ptr 对象】时，
//          引用计数的增减是原子的，不会算错。
//          （前提：不同的 shared_ptr 对象，各自操作自己）
//
// ❌ 不安全：多个线程【同时读写 shared_ptr 指向的对象本身】。
//           atomic 保护的是计数，不是 T。
//           要保护 T，需要额外加锁。
//
// ❌ 不安全：多个线程【同时读写同一个 shared_ptr 变量】。
//           ptr_ 和 ref_count_ 是两个独立的成员，
//           原子操作不能保证它们作为一个整体的更新是原子的。
// ============================================
```

---

## ✅ 验收（打勾才算过）

- [ ] 换成 `std::atomic<long>` 后编译通过、测试通过
- [ ] 多线程压力测试稳定输出 `最终计数: 1`
- [ ] 能说出 **"引用计数是原子的，但指向的对象不是"**
- [ ] 能说出：为什么"同时读写同一个 shared_ptr 变量"仍然不安全
- [ ] valgrind（`--tool=helgrind` 或普通模式）无严重错误

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| `std::atomic` 编译报错 | `#include <atomic>`；链接时可能需要 `-pthread` |
| 线程测试要加 `-pthread` | 在 `CMakeLists.txt` 加 `target_link_libraries(app pthread)` |
| 非原子版本也不崩 | 正常，数据竞争是"可能出错"而非"必然出错"。加大循环次数 |

---

## 🔜 明天（Day 38）

制造一个循环引用泄漏——理解为什么需要 weak_ptr。
