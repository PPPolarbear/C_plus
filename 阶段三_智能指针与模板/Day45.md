# Day 45 · ⭐ 里程碑 3：智能指针与模板验收

**Block 15/19** · 阶段三（Day 31–45）· **阶段收尾**

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build
```

---

## 🎯 今天唯一的目标

**给 MyOptional 补测试，并把阶段三三个组件全部回归验证。**

今天不写新功能，只做**测试、验证、讲清**。

---

## 📖 读（约 20 分钟）

- **不读新内容。**回看 LearnCpp 11.6 “Function templates”、13.13 “Class templates”、22.3 “Move constructors and move assignment”、22.5 “std::unique_ptr”、22.6 “std::shared_ptr”、22.7 “Circular dependency issues with std::shared_ptr, and std::weak_ptr”及12.15 “std::optional”；只查测试失败对应的章节
不读新内容。

---

## ✍️ 写（约 120 分钟）

### 任务 1：MyOptional 完整测试

```cpp
struct NoDefault {
    int v;
    explicit NoDefault(int x) : v(x) {}
    NoDefault(const NoDefault&)            = default;
    NoDefault(NoDefault&&) noexcept        = default;
    NoDefault& operator=(const NoDefault&) = default;
    NoDefault& operator=(NoDefault&&) noexcept = default;
    // 注意：没有默认构造函数
};

void test_optional() {
    // 1. 空状态
    MyOptional<NoDefault> a;
    assert(!a.has_value());

    // 2. 有值（不可默认构造的类型也能存）
    MyOptional<NoDefault> b(NoDefault(5));
    assert(b.has_value());
    assert(b->v == 5);

    // 3. 无值时 value() 抛异常
    bool threw = false;
    try { a.value(); } catch (const std::exception&) { threw = true; }
    assert(threw);

    // 4. value_or
    assert(a.value_or(NoDefault(9)).v == 9);

    // 5. 拷贝
    MyOptional<NoDefault> c = b;
    assert(c->v == 5);

    // 6. 移动
    MyOptional<NoDefault> d = std::move(c);
    assert(d->v == 5);

    // 7. reset
    d.reset();
    assert(!d.has_value());

    // 8. 赋值三种情况
    MyOptional<NoDefault> e, f(NoDefault(1)), g(NoDefault(2));
    f = g;      // 有→有
    f = e;      // 有→无
    e = g;      // 无→有
    assert(!f.has_value() && e.has_value() && e->v == 2);

    std::cout << "✅ MyOptional 全部测试通过\n";
}

// 9. 尺寸检查
static_assert(sizeof(MyOptional<NoDefault>) <= sizeof(NoDefault) + alignof(NoDefault),
              "MyOptional 不应该有明显的内存开销");
```

### 任务 2：三组件回归验证

```bash
valgrind --leak-check=full ./build/test_vector
valgrind --leak-check=full ./build/test_string
valgrind --leak-check=full ./build/test_smartptr
valgrind --leak-check=full ./build/test_diff
valgrind --leak-check=full ./build/test_optional
```

**全部必须零泄漏、零错误。**

### 任务 3：口头讲清（**今天最重要的验收**）

**对着空气讲 3 分钟**，回答：

1. **"shared_ptr 是线程安全的吗？"**（四个层次——见 Day 37）
2. **"unique_ptr 为什么不能拷贝？"**（独占所有权，拷贝会导致 double free）
3. **"shared_ptr 的引用计数为什么必须在堆上？"**（多个副本要共享同一个计数）
4. **"循环引用是什么？怎么解决？"**（weak_ptr）
5. **"std::optional 为什么不能用 `T data_`？"**（会强制 T 可默认构造）

**讲不流畅 → 回去看对应 Day。**

---

## ✅ 验收（打勾才算过）

- [ ] MyOptional 九项测试全部通过
- [ ] `sizeof` 断言通过（无额外开销）
- [ ] 五个测试程序 valgrind **全部干净**
- [ ] 五个问题**能脱稿讲清**
- [ ] 能画出 shared_ptr 共享引用的内存图

---

## 🎯 阶段三总验收

| 检查项 | 打勾 |
|---|---|
| 函数模板、类模板、成员模板、特化 | ☐ |
| `MyUniquePtr`：独占、禁拷贝、移动 | ☐ |
| `MySharedPtr`：引用计数、控制块、先加后减 | ☐ |
| 原子计数与线程安全边界 | ☐ |
| 循环引用与 `weak_ptr` | ☐ |
| 迭代器：裸指针版 + 自定义类 + traits | ☐ |
| `std::sort` 能跑通 | ☐ |
| placement new 与 `alignas` | ☐ |
| `MyOptional` 完整实现 | ☐ |
| **五个面试问题能脱稿讲清** | ☐ |

**十条全打勾 → 阶段三完成。**

---

## 📝 阶段三复盘（填进 `进度追踪.md`）

- 最卡的地方：____________
- `shared_ptr` 线程安全能讲清了吗：____________
- 哪个组件写得最顺：____________

---

## 💼 简历可以再加一行了

> 手写 `unique_ptr` / `shared_ptr`（引用计数 + 原子操作）/ `optional`（placement new + `alignas`），并为 `MyVector` 实现随机访问迭代器，通过 `std::sort` 等标准库算法验证

---

## 🔜 明天（Day 46）

进入阶段四：gdb 调试。
