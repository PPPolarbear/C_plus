# Day 15 · ⭐ 里程碑 1：RAII 验收

**Block 5/19** · 阶段一（Day 1–15）· **阶段收尾**

---

## ⚡ 现在就开始

```bash
cd cpp-camp2 && cmake --build build && ./build/app
```

---

## 🎯 今天唯一的目标

**证明你真的理解 RAII——不是"能写出来"，是"能讲清楚为什么"**

今天不写新东西，只做三件事：**测试、讲清、复盘**。

---

## 📖 读（约 20 分钟）

不读新内容。

---

## ✍️ 写（约 120 分钟）

### 任务 1：ScopeGuard 三路径测试

写 `test_scope_guard.cpp`，覆盖三条路径：

```cpp
// 路径 A：正常返回
void test_normal() {
    ScopeGuard g([]{ std::cout << "[OK] 正常路径清理\n"; });
}

// 路径 B：提前 return
void test_early_return() {
    ScopeGuard g([]{ std::cout << "[OK] 提前 return 清理\n"; });
    if (true) return;
    std::cout << "这行不会执行\n";
}

// 路径 C：抛异常
void test_exception() {
    ScopeGuard g([]{ std::cout << "[OK] 异常路径清理\n"; });
    throw std::runtime_error("boom");
}

// 路径 D：手动 dismiss
void test_dismiss() {
    ScopeGuard g([]{ std::cout << "[不该出现] 已 dismiss\n"; });
    g.dismiss();
}
```

**四条路径全部输出正确才算过。**

### 任务 2：做一个"资源管理器"

把 ScopeGuard 用在一个真实的资源上：

```cpp
class ManagedBuffer {
public:
    explicit ManagedBuffer(size_t n)
        : data_(new int[n]), size_(n)
        , guard_([this]{ delete[] data_; std::cout << "缓冲区已释放\n"; })
    {}
private:
    int*       data_;
    size_t     size_;
    ScopeGuard guard_;    // ← 把清理交给 guard
};
```

**注意**：此时不再需要手写析构函数——guard 会代劳。这就是 RAII 的威力。

### 任务 3：口头讲清（最重要的验收方式）

**对着空气讲 3 分钟**，录音或写下来，内容：

1. RAII 是什么（一句话：**把资源的生命周期绑定到对象的生命周期上**）
2. 为什么 C++ 不需要 GC
3. 析构函数的调用时机为什么是确定的
4. `ScopeGuard` 为什么必须禁止拷贝

**讲不流畅 = 没真懂。** 卡住的地方回去看对应 Day。

---

## ✅ 验收（打勾才算过）

- [ ] 四条路径全部输出正确
- [ ] `ManagedBuffer` 不写析构函数也能正确释放内存
- [ ] valgrind 零泄漏
- [ ] **能用一句话说出 RAII 是什么**
- [ ] **能解释为什么 `ScopeGuard` 必须禁止拷贝**
- [ ] 能说出栈对象析构的四个触发时机（正常结束、return、异常、块结束）

---

## 🎯 阶段一总验收

| 检查项 | 打勾 |
|---|---|
| 能手动建多文件 CMake 项目 | ☐ |
| 分得清编译错误和链接错误 | ☐ |
| 能在 gdb 里下断点看变量 | ☐ |
| 传值/传引用/const 引用讲得清 | ☐ |
| 指针三件套（声明、取地址、解引用）熟练 | ☐ |
| `const int*` / `int* const` / `const int* const` 分清 | ☐ |
| 类封装、构造函数、成员初始化列表会写 | ☐ |
| `static` 成员会声明会定义会解释 | ☐ |
| **RAII 能讲清、ScopeGuard 能跑通** | ☐ |

**九条全打勾 → 阶段一完成，进阶段二。**

---

## 📝 阶段一复盘（填进 `进度追踪.md`）

- 最卡的地方：____________
- 最有收获的地方：____________
- 哪个 Day 想重做一遍：____________

---

## 🔜 明天（Day 16）

进入阶段二：动态内存与深浅拷贝。**从这天开始，你不再"学 C++"，而是"造东西"。**
