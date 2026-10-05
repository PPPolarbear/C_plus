# Day 14 · ScopeGuard 实现

**Block 5/19** · 阶段一（Day 1–15）

---

## ⚡ 现在就开始

```bash
cd cpp-camp2 && touch include/scope_guard.h
```

---

## 🎯 今天唯一的目标

**写出 ScopeGuard——把"析构时自动执行清理"变成一个可用工具。**

这个 20 行的类，就是 RAII 的全部精髓。写出来，你后面写 `MyVector` 的析构函数就是同一套思路。

---

## 📖 读（约 30 分钟）

- 搜「C++ ScopeGuard 实现」「C++ RAII 守卫」
- 只读思路，**不要抄代码**，看完就关掉自己写

---

## ✍️ 写（约 120 分钟）

```cpp
// include/scope_guard.h
#pragma once
#include <functional>
#include <utility>

class ScopeGuard {
public:
    explicit ScopeGuard(std::function<void()> cleanup)
        : cleanup_(std::move(cleanup)), active_(true) {}

    ~ScopeGuard() {
        if (active_) cleanup_();      // ← 核心就这一行
    }

    // ---- 禁止拷贝（关键！）----
    ScopeGuard(const ScopeGuard&)            = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

    // ---- 允许移动（可选，先跳过也行）----
    ScopeGuard(ScopeGuard&& other) noexcept
        : cleanup_(std::move(other.cleanup_)), active_(other.active_) {
        other.active_ = false;
    }

    void dismiss() noexcept { active_ = false; }   // 手动取消

private:
    std::function<void()> cleanup_;
    bool active_;
};
```

### 为什么必须禁止拷贝？

**先自己想一想再往下。**

提示：如果允许拷贝，两个 `ScopeGuard` 会各持有一份 `cleanup_`，析构时**执行两次**。
这和你 Day 17 要接触的"浅拷贝 double free"是**同一个病**。

### 使用示例

```cpp
void demo() {
    std::cout << "进入\n";

    ScopeGuard g([]{ std::cout << "清理！\n"; });

    std::cout << "工作中\n";
    // 无论怎么离开这个函数，都会打印"清理！"
    return;
    // 提前 return ✅
    // 抛异常 ✅
}
```

### 实战场景

```cpp
// 场景 1：资源清理
void file_demo() {
    FILE* f = std::fopen("test.txt", "w");
    ScopeGuard guard([f]{ std::fclose(f); std::cout << "文件已关\n"; });
    // 就算后面抛异常，文件也会关
}

// 场景 2：状态回滚
void state_demo(bool& flag) {
    flag = true;
    ScopeGuard guard([&flag]{ flag = false; std::cout << "状态已回滚\n"; });
}
```

---

## ✅ 验收（打勾才算过）

- [ ] `ScopeGuard` 在正常退出时触发清理
- [ ] **提前 `return` 时也触发**
- [ ] **抛异常时也触发**
- [ ] 试着拷贝一个 `ScopeGuard`，**编译失败**（这是正确行为）
- [ ] 能解释：**为什么必须禁止拷贝**
- [ ] 两个实战场景（文件、状态回滚）都能跑通

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| `std::function` 报错 | 加 `#include <functional>` |
| lambda 捕获局部变量报错 | 用 `[&flag]` 引用捕获，注意变量生命周期要长于 guard |
| 不知道怎么验证异常路径 | `throw std::runtime_error("x")` 在 guard 之后，用 `try-catch` 包住调用处 |

---

## 🔜 明天（Day 15）

ScopeGuard 全面测试 + 阶段一验收。**这是你第一个里程碑。**
