# Day 16 · new/delete 与手动动态数组

**Block 6/19** · 阶段二（Day 16–30）· 主题：资源管理

---

## ⚡ 现在就开始

```bash
mkdir -p cpp-camp3 && cd cpp-camp3
```

**开一个新项目。** 阶段二的所有产物都放这里——这就是你未来的 `mini-stl`。

---

## 🎯 今天唯一的目标

**用裸指针手工管理一块动态数组，并且不泄漏。**

这是 `MyVector` 的原料。今天用最原始的方式做一遍，明天开始用类封装。

---

## 📖 读（约 30 分钟）

- LearnCpp 19.1 “Dynamic memory allocation with new and delete”、19.2 “Dynamically allocating arrays”
- 重点对照单对象的 `new` / `delete` 与数组的 `new[]` / `delete[]`

---

## ✍️ 写（约 120 分钟）

建立 `cpp-camp3/` 的 CMake 骨架（复用 Day 01 的结构）。

```cpp
// 全部用裸指针实现，不许用 std::vector

// ---- 分配与释放 ----
int* alloc_array(size_t n);                 // new int[n]
void free_array(int* p);                    // delete[] p

// ---- 操作 ----
void fill_sequence(int* p, size_t n, int start);
void print_array(const int* p, size_t n);
int  sum_array(const int* p, size_t n);

// ---- 对比实验 ----
void demo_new_vs_new_array();   // new int  vs  new int[10]
void demo_delete_mismatch();    // 故意用 delete 释放 new[] 的数组，看 valgrind 报什么
```

### 关键实验：打印地址与字节差

```cpp
int* p = new int[5];
std::cout << "p     = " << p     << "\n";
std::cout << "p + 1 = " << p + 1 << "\n";
std::cout << "字节差 = " << (char*)(p+1) - (char*)p << "\n";   // 应该是 sizeof(int)
```

---

## ✅ 验收（打勾才算过）

- [ ] `alloc_array` / `free_array` 配对使用，valgrind 零泄漏
- [ ] 打印出 `p` 和 `p+1` 的字节差 = `sizeof(int)`
- [ ] `demo_delete_mismatch()` 中，valgrind **报出了错误**（mismatched new/delete）
- [ ] 能解释 `new[]` 为什么必须配 `delete[]`（**提示：编译器要记录元素个数才知道析构几次**）
- [ ] 能说出 `new int(5)` 和 `new int[5]` 的区别

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| valgrind 没报错 | 有些环境对 mismatched delete 不敏感，换 `delete` 释放 `new[]` 的指针再试 |
| `free_array` 里忘了判空 | `delete[] nullptr` 是安全的，但养成判空习惯 |

---

## 🔜 明天（Day 17）

**故意制造一个 double free 崩溃**，理解深浅拷贝。
