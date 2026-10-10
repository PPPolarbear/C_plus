# Day 12 · 静态成员 + BankAccount 完成

**Block 4/19** · 阶段一（Day 1–15）

---

## ⚡ 现在就开始

```bash
cd cpp-camp2 && cmake --build build && ./build/app
```

---

## 🎯 今天唯一的目标

**理解 `static` 成员属于"类"而不是"对象"，并把 BankAccount 收尾。**

---

## 📖 读（约 30 分钟）

- LearnCpp 15.6 “Static member variables”、15.7 “Static member functions”
- 重点：为什么 `static` 成员变量要在 `.cpp` 里**再定义一次**

---

## ✍️ 写（约 120 分钟）

### 任务 1：给 BankAccount 加静态计数器

```cpp
class BankAccount {
public:
    BankAccount();
    BankAccount(const std::string& owner, double balance);
    explicit BankAccount(const std::string& owner);
    // ↓ 静态成员函数
    static int account_count();       // 统计一共创建了多少个账户

private:
    std::string owner_;
    double      balance_;
    static int  count_;               // ← 声明
};

// 在 .cpp 里：
int BankAccount::count_ = 0;          // ← 定义（这一行不能少）
```

在每个构造函数里 `++count_;`

### 任务 2：观察 static 的行为

```cpp
BankAccount a("A", 100);
BankAccount b("B", 200);
std::cout << BankAccount::account_count();   // 应该打印 2

// 注意：是通过【类名】调用，不是对象名
// 试着写 a.account_count() —— 也能编译，但不推荐，会误导读者
```

### 任务 3：加转账功能

```cpp
bool BankAccount::transfer_to(BankAccount& other, double amount) {
    // 先 withdraw，成功再 other.deposit
    // 注意：如果 deposit 失败要回滚
}
```

**这个函数有坑**：如果 `withdraw` 成功但 `deposit` 失败，钱就凭空消失了。想想怎么处理。

### 任务 4：完善 print

```cpp
void BankAccount::print() const {
    std::cout << "[" << owner_ << "] " << balance_ << "\n";
}
```

---

## ✅ 验收（打勾才算过）

- [ ] `account_count()` 返回正确的账户数
- [ ] 能解释：**为什么 `static int count_;` 要在 `.cpp` 里再写一遍**
- [ ] 能解释：`static` 成员函数为什么**不能访问非静态成员**（试着写，看报错）
- [ ] `transfer_to` 处理了"转账失败"的情况（要么都成功，要么都不变）
- [ ] valgrind 零错误

---

## ✅ Block 4 完成检查

- [ ] 能写出一个封装良好的类（private 数据 + public 接口）
- [ ] 构造函数三种形式都会写，且**用成员初始化列表**
- [ ] 能说出成员初始化的真实顺序
- [ ] `static` 成员会声明、会定义、会解释

**四条都打勾 → 进 Day 13。**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| `undefined reference to BankAccount::count_` | 忘了在 `.cpp` 里写 `int BankAccount::count_ = 0;` |
| `transfer_to` 回滚怎么写 | 先记下原余额，失败时再改回去 |

---

## 🔜 明天（Day 13）

析构函数——对象是怎么"死"的。RAII 的入口。
