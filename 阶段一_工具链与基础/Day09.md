# Day 09 · 综合 + 面试题自测

**Block 3/19** · 阶段一（Day 1–15）

---

## ⚡ 现在就开始

```bash
cd cpp-camp2 && cmake --build build && ./build/app
```

---

## 🎯 今天唯一的目标

**把前 8 天的知识串成一个完整的小程序，并自测 8 道面试题。**

---

## 📖 读（约 20 分钟）

- 不读新内容。回看你前两天写的注释和抄下来的编译错误

---

## ✍️ 写（约 120 分钟）

### 综合练习：手写一个动态 int 数组（不用 vector）

这是 `MyVector` 的**前传**——今天用裸指针做一遍，Day 19 用类重做一遍。

```cpp
// 用裸指针 + new/delete 实现
int*   create_array(int n);                       // new int[n]
void   destroy_array(int* p);                     // delete[] p
void   fill_sequence(int* p, int n, int start);
int    sum_array(const int* p, int n);            // 注意 const
int    find_max(const int* p, int n);
void   reverse_in_place(int* p, int n);           // 原地翻转
void   print_array(const int* p, int n);
int*   copy_array(const int* src, int n);         // 返回新数组
```

**全部用裸指针实现，不许用 `std::vector`。**

在 `main` 里串起来跑一遍。

---

## ✅ 自测 8 题（能口头答上来才算过）

1. `const int* p` 和 `int* const p` 有什么区别？
2. 引用和指针的三个区别是什么？
3. 为什么函数参数优先用 `const T&`？
4. 数组名和指针是同一个东西吗？
5. `delete` 和 `delete[]` 有什么区别？用错了会怎样？
6. 野指针是什么？怎么避免？
7. 函数的形参是实参的什么？（值传递的本质）
8. `static` 局部变量的生命周期和作用域分别是什么？

**卡住的记下来，回去看对应的 Day。**

---

## ✅ 验收（打勾才算过）

- [ ] 综合练习跑通
- [ ] `sum_array` 等只读函数全部用了 `const int*`
- [ ] valgrind 零错误零泄漏
- [ ] 8 道面试题**至少答上 6 道**

---

## ✅ Block 3 完成检查

- [ ] 指针：能画图、能算地址、能遍历数组
- [ ] 引用：能说清和指针的区别
- [ ] const：三种指针写法全分清
- [ ] 能用裸指针手写动态数组

**四条都打勾 → 进 Day 10。**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| `reverse_in_place` 写不出来 | 用两个下标 `i` 和 `j` 从两端向中间走，交换 `p[i]` 和 `p[j]` |
| valgrind 报错 | 检查 `new[]` 有没有配 `delete[]`（不是 `delete`） |

---

## 🔜 明天（Day 10）

进入类：封装、成员函数。从此开始造对象。
