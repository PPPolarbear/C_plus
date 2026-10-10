# Day 48 · watch / 栈帧 + 自测

**Block 16/19** · 阶段四（Day 46–57）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build
```

---

## 🎯 今天唯一的目标

**掌握"变量在哪被改了"这个高频调试场景，Block 16 收尾。**

`watch` 是 gdb 里最实用的命令之一——**变量一被改就停下来**。

---

## 📖 读（约 20 分钟）

- GNU GDB Manual：“Setting Watchpoints”、“Backtraces”、“Selecting a Frame”
- 重点练 `watch`、`bt` 与 `frame`，分别对应变量改写、调用栈和栈帧切换

---

## ✍️ 写（约 120 分钟）

### 任务 1：制造一个"变量被意外修改"的场景

```cpp
// tests/watch_demo.cpp
#include <iostream>
#include <vector>

struct Account {
    int balance = 1000;
    int id = 0;
};

void suspicious_function(Account& acc, int idx) {
    // 这个函数"看起来"只该改 id
    acc.id = idx;

    // 但这里有个越界写，偷偷改了 balance
    int* p = reinterpret_cast<int*>(&acc);
    p[-1] = 9999;              // ← 越界，改了别的内存
}

int main() {
    Account acc;
    std::cout << "初始 balance = " << acc.balance << "\n";

    suspicious_function(acc, 42);

    std::cout << "之后 balance = " << acc.balance << "\n";   // 变了！
    return 0;
}
```

### 任务 2：用 watch 抓凶手

```bash
gdb ./build/watch_demo
(gdb) b main
(gdb) r
(gdb) watch acc.balance       # ← 监视点
(gdb) c
# 程序会在 balance 被修改时停下
(gdb) bt                      # ← 看是谁改的
```

**`bt` 会直接告诉你改它的那个函数。** 这就是 watch 的价值。

### 任务 3：栈帧切换练习

```cpp
// tests/frames.cpp
void level3(int x) { std::cout << x / 0 << "\n"; }   // 故意崩
void level2(int x) { level3(x); }
void level1(int x) { level2(x); }
int  main()        { level1(42); }
```

```bash
gdb ./build/frames
(gdb) r
# 崩了
(gdb) bt              # 看调用栈，应该是 level3 ← level2 ← level1 ← main
(gdb) f 0             # 切到 level3
(gdb) p x             # 42
(gdb) f 1             # 切到 level2
(gdb) p x             # 42
(gdb) f 3             # 切到 main
(gdb) info locals     # 看局部变量
```

### 任务 4：其他实用命令

```bash
(gdb) info breakpoints     # 列出所有断点
(gdb) delete 1             # 删除 1 号断点
(gdb) disable 2            # 禁用
(gdb) enable 2             # 启用
(gdb) ptype acc            # 看类型定义
(gdb) x/10x p              # 以十六进制查看内存
(gdb) display x            # 每次停下来都自动打印 x
```

---

## ✅ 验收（打勾才算过）

- [ ] `watch` 成功抓住了改 `balance` 的那一行
- [ ] 能用 `bt` 从崩溃点一路追溯回 `main`
- [ ] 会用 `info locals` 看局部变量
- [ ] 会用 `display` 设置自动打印
- [ ] 能回答面试题：**"线上程序崩溃了怎么排查？"**
      （答：看 core dump / 日志 → gdb 加载 → `bt` 看调用栈 → 定位到函数和行号 → 检查入参）

---

## ✅ Block 16 完成检查

- [ ] 十个常用命令成为肌肉记忆
- [ ] 能用 gdb 定位段错误、除零、越界
- [ ] 会用 `watch` 找"变量被谁改了"
- [ ] 能用 `bt` + `frame` 在调用栈里穿梭

**四条都打勾 → 进 Day 49。**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| `watch` 说无法监视 | 变量可能被优化掉了，用 Debug 模式编译 |
| `bt` 只显示一行 | 检查编译时有没有 `-g` |
| 越界写没生效 | 结构体布局因编译器而异，可以换个偏移量试 |

---

## 🔜 明天（Day 49）

valgrind 系统性练习。
