# Day 31 · 模板基础

**Block 11/19** · 阶段三（Day 31–45）· 主题：智能指针与模板

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build
```

---

## 🎯 今天唯一的目标

**把模板从"会用"变成"会写"——重点是理解实例化机制。**

前 30 天你已经在用模板（`MyVector<T>`），但今天是第一次系统地理解它。

---

## 📖 读（约 40 分钟）

- learncpp：函数模板、类模板、模板特化
- 重点：**模板是在编译期展开的**

---

## ✍️ 写（约 110 分钟）

### 任务 1：函数模板

```cpp
template <typename T>
T my_max(const T& a, const T& b) { return a > b ? a : b; }

// 多个模板参数
template <typename K, typename V>
void print_pair(const K& k, const V& v);

// 非类型模板参数
template <typename T, size_t N>
T array_sum(const T (&arr)[N]);        // 编译期就知道 N

// 特化（针对特定类型做特殊处理）
template <>
const char* my_max<const char*>(const char* const& a, const char* const& b);
// 注意：const char* 的 > 比较的是指针地址，不是字符串内容 —— 所以需要特化
```

### 任务 2：类模板 + 成员函数模板

```cpp
template <typename T>
class Box {
public:
    explicit Box(T v) : v_(v) {}
    T get() const { return v_; }

    template <typename U>
    Box<U> transform(U (*f)(const T&)) const;   // 成员函数模板
private:
    T v_;
};
```

### 任务 3：验证"编译期展开"

```cpp
template <typename T>
void which_type() {
    std::cout << "T 的大小 = " << sizeof(T) << "\n";
}

which_type<int>();        // 4
which_type<double>();     // 8
which_type<char>();       // 1
```

**关键认知**：`MyVector<int>` 和 `MyVector<double>` 是**两个完全不同的类**，编译器为每个类型生成一份代码。

### 任务 4：看看模板报错有多长

故意写一个错误：

```cpp
template <typename T>
T add(T a, T b) { return a + b; }

struct NoAdd {};
add(NoAdd{}, NoAdd{});    // ← 编译错误
```

**把报错信息贴出来，观察它有多长。** 这就是为什么模板报错难读——以后遇到不要慌。

---

## ✅ 验收（打勾才算过）

- [ ] 能写函数模板、类模板、成员函数模板
- [ ] 能说出 `my_max<const char*>` 为什么需要特化
- [ ] 能解释：**`MyVector<int>` 和 `MyVector<double>` 是两个不同的类**
- [ ] 见过一次模板报错，知道它为什么长
- [ ] 能解释：为什么模板的实现必须放在头文件里（**编译期展开，链接器看不到**）

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 模板函数链接错误 | 实现放头文件，或显式实例化 |
| `array_sum` 推导不出 N | 参数类型要写 `const T (&arr)[N]`（引用数组） |
| 特化语法报错 | 特化要写 `template <>`，且放在主模板之后 |

---

## 🔜 明天（Day 32）

MyUniquePtr——最简单的智能指针。
