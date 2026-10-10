# Day 07 · 指针基础

**Block 3/19** · 阶段一（Day 1–15）

---

## ⚡ 现在就开始

```bash
cd cpp-camp2 && code src/main.cpp
```

---

## 🎯 今天唯一的目标

**能把"地址"和"值"在心里分开——这是指针的全部难点。**

不是背语法，是在脑子里建立起"变量有地址、指针存地址"的图景。

---

## 📖 读（约 40 分钟）

- LearnCpp 12.1 “Introduction to compound data types”、12.7 “Introduction to pointers”、12.8 “Null pointers”、12.10 “Pass by address”
- **这是本阶段最重要的一块，读慢一点**

---

## ✍️ 写（约 110 分钟）

```cpp
// ---- 基础 ----
void pointer_basics();       // 声明 int*、取地址 &x、解引用 *p、nullptr
void pointer_null_demo();    // 解引用 nullptr 会怎样（会崩，用注释写下来）

// ---- 指针与数组 ----
void array_pointer_demo();   // 验证 *(arr + i) == arr[i]
void iterate_by_pointer(int* arr, int n);   // 用指针遍历数组

// ---- 指针运算 ----
void pointer_arithmetic();   // p+1 到底加了几个字节？打印地址差验证

// ---- 指针的指针 ----
void pointer_to_pointer();   // int** pp

// ---- const 与指针（今天只碰一下，明天深入）----
void const_pointer_intro();  // const int* p  vs  int* const p
```

**核心实验**：打印每个变量的地址，观察指针里存的值就是那个地址。

```cpp
int  x = 42;
int* p = &x;
std::cout << "x 的地址: " << &x << "\n";
std::cout << "p 里存的值: " << p << "\n";   // 应该和上一行一样
std::cout << "解引用 *p: " << *p << "\n";   // 42
```

---

## ✅ 验收（打勾才算过）

- [ ] 能画出图：`x` 在内存里、`p` 指向它、`*p` 取值
- [ ] `pointer_arithmetic()` 中打印的地址差 = `sizeof(int)`（亲眼看到）
- [ ] 能解释 `*(arr + i)` 为什么等于 `arr[i]`
- [ ] 能说出 `int** pp` 是什么（指针的指针）
- [ ] 能说出：指针未初始化就解引用会发生什么（**野指针**）

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 程序段错误（Segmentation fault） | 大概率解引用了空指针/野指针，用 gdb 的 `bt` 看崩在哪 |
| 打印地址是一串乱码 | 正常，地址就是十六进制数。用 `(void*)p` 打印更规范 |
| 分不清 `&` 和 `*` | `&` 取地址（在变量前），`*` 解引用（在指针前）。类型声明里的 `*` 是第三层含义 |

---

## 🔜 明天（Day 08）

引用与 const 正确性——指针的"安全版"。
