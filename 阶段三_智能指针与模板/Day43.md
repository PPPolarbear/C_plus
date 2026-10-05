# Day 43 · placement new 与 alignas

**Block 15/19** · 阶段三（Day 31–45）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && touch tests/test_placement.cpp
```

---

## 🎯 今天唯一的目标

**理解"内存"和"对象"是两件事——可以分开管理。**

这是 `std::optional` / `std::variant` 的实现基础，也是面试的进阶考点。

---

## 📖 读（约 40 分钟）

- 搜「C++ placement new 详解」
- 搜「C++ alignas 内存对齐」
- 搜「C++ 对象生命周期 内存分配」

---

## ✍️ 写（约 110 分钟）

### 任务 1：认识 placement new

普通的 `new` 做两件事：**分配内存 + 构造对象**。
`placement new` 只做**第二件**——在**已分配好的内存**上构造对象。

```cpp
#include <new>

void placement_demo() {
    // 1. 分配原始内存（不构造对象）
    alignas(std::string) unsigned char buffer[sizeof(std::string)];

    // 2. 在这块内存上构造对象
    std::string* p = new (buffer) std::string("hello");

    std::cout << *p << "\n";        // hello
    std::cout << p->size() << "\n"; // 5

    // 3. 显式调用析构（不会释放内存）
    p->~basic_string();
}
```

**关键**：`buffer` 是栈上的数组，**从头到尾没有发生内存分配**。对象直接在栈上构造出来了。

### 任务 2：为什么需要 `alignas`

```cpp
struct Unaligned {
    char buf[sizeof(double)];      // ❌ 对齐是 1，不是 8
};

struct Aligned {
    alignas(double) char buf[sizeof(double)];   // ✅ 对齐到 8 字节
};

std::cout << alignof(Unaligned) << "\n";   // 1
std::cout << alignof(Aligned)   << "\n";   // 8
```

**为什么重要**：`double` 要求 8 字节对齐，如果在一块只对齐到 1 的内存上构造 `double`，**在 x86 上慢、在某些架构（ARM）上直接崩溃**。

### 任务 3：写一个"手动管理生命周期"的容器

```cpp
class ManualBox {
public:
    ManualBox() : has_value_(false) {}

    void set(const std::string& v) {
        if (has_value_) {
            ptr()->~basic_string();          // 先析构旧的
        }
        new (storage_) std::string(v);       // 原地构造新的
        has_value_ = true;
    }

    const std::string& get() const { return *ptr(); }
    bool has_value() const { return has_value_; }

    ~ManualBox() {
        if (has_value_) ptr()->~basic_string();   // 必须手动析构
    }

private:
    std::string* ptr() { return reinterpret_cast<std::string*>(storage_); }
    const std::string* ptr() const { return reinterpret_cast<const std::string*>(storage_); }

    alignas(std::string) unsigned char storage_[sizeof(std::string)];
    bool has_value_;
};
```

### 任务 4：验证"真的没有额外内存分配"

```cpp
ManualBox box;
box.set("hello");
std::cout << "sizeof(ManualBox) = " << sizeof(ManualBox) << "\n";
std::cout << "sizeof(std::string) = " << sizeof(std::string) << "\n";
// 两者应该差不多（只多了一个 bool + 对齐填充）
```

**对比**：如果用 `std::string* ptr_` 存指针，就要额外 `new` 一次——**多一次内存分配，多一次间接寻址**。

---

## ✅ 验收（打勾才算过）

- [ ] `placement_demo()` 能跑通
- [ ] 能说出 `new` 做的两件事，以及 `placement new` 只做哪一件
- [ ] 能说出 `alignas` 解决什么问题
- [ ] `ManualBox` 能正确 set/get，valgrind 零泄漏
- [ ] 能解释：为什么 `ManualBox` 比"存指针"更好（**省一次分配**）
- [ ] 能说出为什么析构函数里必须**手动**调用 `~basic_string()`

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| placement new 报错 | `#include <new>` |
| valgrind 报泄漏 | `set()` 里替换旧值时忘了先析构 |
| `reinterpret_cast` 报错 | 需要 `#include <cstring>` 或检查类型拼写 |
| 析构函数名写错 | `std::string` 的析构是 `~basic_string()`（`string` 是 `basic_string<char>` 的别名） |

---

## 🔜 明天（Day 44）

用今天的技术实现 MyOptional。
