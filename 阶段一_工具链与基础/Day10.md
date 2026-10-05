# Day 10 · 类基础与封装

**Block 4/19** · 阶段一（Day 1–15）

---

## ⚡ 现在就开始

```bash
cd cpp-camp2 && touch include/bank_account.h src/bank_account.cpp
```

记得把新 `.cpp` 加进 `CMakeLists.txt` 的 `add_executable`。

---

## 🎯 今天唯一的目标

**理解"封装"到底在保护什么**——不是把成员变 private 走个形式，是控制谁能改内部状态。

---

## 📖 读（约 40 分钟）

- learncpp：类的介绍、成员函数、访问说明符
- 重点：`public` / `private` 的**意义**，不是语法

---

## ✍️ 写（约 110 分钟）

```cpp
// include/bank_account.h
#pragma once
#include <string>

class BankAccount {
public:
    // ---- 访问器（getter）----
    const std::string& owner() const;
    double             balance() const;

    // ---- 修改器（setter / 业务方法）----
    bool deposit(double amount);      // 金额 <= 0 返回 false
    bool withdraw(double amount);     // 余额不足返回 false

    // ---- 信息 ----
    void print() const;

private:
    std::string owner_;
    double      balance_;
};

// 自由函数（对比：为什么这些不该做成成员）
void transfer(BankAccount& from, BankAccount& to, double amount);
```

**先不写构造函数**（明天写），今天用 `public` 临时手动赋值来测试逻辑。

在 `main` 里测试：
- 存 100，取 30，查余额
- 取 10000（余额不足）
- 存 -50（非法）

---

## ✅ 验收（打勾才算过）

- [ ] `deposit(-50)` 返回 `false` 且余额不变
- [ ] `withdraw(10000)` 返回 `false` 且余额不变
- [ ] 能从外部读 `balance()`，但**不能直接改 `balance_`**（试着写 `acc.balance_ = 999;` 看编译报错）
- [ ] `print()` 标了 `const`，能被 const 对象调用
- [ ] 能说出：**为什么 `balance_` 要 private**（答：防止外部绕过 `withdraw` 的余额检查直接改成负数）

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 忘了头文件怎么写 | 类定义写 `.h`，成员函数实现写 `.cpp`，`.cpp` 里用 `BankAccount::` 前缀 |
| `balance_` 报未初始化 | 正常，明天写构造函数解决。今天先手动赋值 |

---

## 🔜 明天（Day 11）

构造函数与成员初始化列表——对象是怎么"出生"的。
