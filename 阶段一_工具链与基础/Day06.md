# Day 06 · 基础语法综合验收

**Block 2/19** · 阶段一（Day 1–15）

---

## ⚡ 现在就开始

```bash
cd cpp-camp2 && cmake --build build && ./build/app
```

---

## 🎯 今天唯一的目标

**用一个小综合练习，检验前 5 天的语法是否真的到位。**

---

## 📖 读（约 20 分钟）

- **不读新内容。**只回看做练习时卡住的对应章节：LearnCpp 4.1 “Introduction to fundamental data types”、4.12 “Introduction to type conversion and static_cast”、8.2 “If statements and blocks”、8.5 “Switch statement basics”、8.8 “Introduction to loops and while statements”、8.10 “For statements”、2.4 “Introduction to function parameters and arguments”、12.5 “Pass by lvalue reference”或12.6 “Pass by const lvalue reference”

---

## ✍️ 写（约 120 分钟）

### 综合练习：命令行版学生成绩统计

写一个完整的小程序，要求用到前 5 天所有知识点：

```cpp
// 数据结构
struct Student {
    std::string name;
    int         score;
};

// 功能函数
void   input_students(std::vector<Student>& out);      // 从 cin 读入若干学生
double average_score(const std::vector<Student>& s);   // 平均分（const 引用）
int    max_score(const std::vector<Student>& s);
int    min_score(const std::vector<Student>& s);
int    count_above(const std::vector<Student>& s, int threshold);
void   sort_by_score(std::vector<Student>& s);         // 按分数排序
void   print_report(const std::vector<Student>& s);    // 打印报表

// 工具函数
std::string grade_of(int score);                       // 90+ A, 80+ B, ...
```

**在 `main` 里串起来，跑一次完整流程。**

---

## ✅ 验收（打勾才算过）

- [ ] 程序能读入学生数据、算出统计、打印报表
- [ ] **所有只读参数的函数都用了 `const&`**（不许用传值）
- [ ] 能说出：为什么 `average_score` 用 `const&` 而不是 `vector<Student>`
- [ ] `grade_of` 用了 switch 或 if-else 链，边界正确（90 是 A，89 是 B）
- [ ] valgrind 跑一遍无错误

---

## ✅ Block 2 完成检查

- [ ] 类型转换、溢出、浮点精度：能解释现象
- [ ] 传值 / 传引用 / const 引用：能说清区别
- [ ] 函数重载、默认参数、static 局部变量：能正确使用

**三条都打勾 → 进 Day 07。**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 不知道怎么从 cin 读一行 | `std::getline(std::cin, str)` |
| 排序报一堆模板错误 | 今天用简单冒泡即可，`std::sort` 留到 Day 42 |
| 忘了 struct 怎么用 | 它和 class 几乎一样，只是默认 public |

---

## 🔜 明天（Day 07）

进入 C++ 与 Java/Python 最大的分野：指针。
