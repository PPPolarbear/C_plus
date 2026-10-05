# Day 11 · 构造函数与成员初始化列表

**Block 4/19** · 阶段一（Day 1–15）

---

## ⚡ 现在就开始

```bash
cd cpp-camp2 && cmake --build build
```

---

## 🎯 今天唯一的目标

**搞清"成员初始化列表"和"在函数体里赋值"的区别——这是面试常问、也是后面写 `MyVector` 必须用对的。**

---

## 📖 读（约 40 分钟）

- learncpp：构造函数、成员初始化列表
- 重点：**成员初始化列表 vs 构造函数体内赋值**

---

## ✍️ 写（约 110 分钟）

### 任务 1：给 BankAccount 加构造函数

```cpp
class BankAccount {
public:
    BankAccount();                                        // 默认构造
    BankAccount(const std::string& owner, double balance);
    explicit BankAccount(const std::string& owner);       // 单参加 explicit
    // ...
};
```

实现时**必须用成员初始化列表**：

```cpp
BankAccount::BankAccount(const std::string& owner, double balance)
    : owner_(owner)          // ← 初始化列表
    , balance_(balance)
{
    // 函数体留空
}
```

### 任务 2：对比实验（今天的重点）

写一个类，里面有个"会打印的成员"，分别用两种方式初始化，观察打印次数：

```cpp
class Noisy {
public:
    Noisy()          { std::cout << "默认构造\n"; }
    Noisy(int)       { std::cout << "int 构造\n"; }
    Noisy(const Noisy&) { std::cout << "拷贝构造\n"; }
    Noisy& operator=(const Noisy&) { 
        std::cout << "拷贝赋值\n"; return *this; 
    }
};

class HolderA {   // ❌ 在函数体里赋值
public:
    HolderA(int v) { n_ = Noisy(v); }   // 先默认构造，再拷贝赋值 → 打印 2 次
private:
    Noisy n_;
};

class HolderB {   // ✅ 用初始化列表
public:
    HolderB(int v) : n_(v) {}           // 直接构造 → 打印 1 次
private:
    Noisy n_;
};
```

**跑出来对比打印次数。**

### 任务 3：explicit 实验

```cpp
void take(BankAccount acc);
// take("张三");          // ❌ 有 explicit 时编译错误
// take(BankAccount("张三"));  // ✅ 显式构造才行
```

把 `explicit` 去掉再试一次，看是否能编译通过。

### 任务 4：成员初始化顺序

```cpp
class Order {
    int a_;   // 声明顺序
    int b_;
public:
    Order() : b_(1), a_(b_) {}   // 初始化列表顺序 ≠ 声明顺序
    void print() const;
};
// 观察 a_ 到底是什么值 —— 说明什么？
```

---

## ✅ 验收（打勾才算过）

- [ ] `HolderA` 打印 2 次，`HolderB` 打印 1 次
- [ ] 能解释为什么（**成员变量在进入函数体前就已经构造完了**）
- [ ] 能说出成员初始化的**真实顺序**（按**声明顺序**，不是初始化列表的书写顺序）
- [ ] `explicit` 的实验做通，能说出它挡掉了什么隐式转换
- [ ] BankAccount 的三个构造函数都实现并测试通过

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| `no matching constructor` | 默认构造没写，或者写了别的构造后编译器不再自动生成 |
| 初始化列表语法报错 | 冒号开头，逗号分隔，**末尾不能有逗号** |

---

## 🔜 明天（Day 12）

静态成员 + 完成 BankAccount。
