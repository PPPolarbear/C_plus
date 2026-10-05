# Day 49 · valgrind 与有毒程序（上）

**Block 17/19** · 阶段四（Day 46–57）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && mkdir -p tests/poison && touch tests/poison/p1_leak.cpp
```

---

## 🎯 今天唯一的目标

**系统性地写出前三种内存错误，并用 valgrind 逐个解剖。**

Day 18 你见过它们，今天要**写出来 + 读懂报告**。

---

## 📖 读（约 30 分钟）

- 搜「valgrind memcheck 参数 用法」
- 重点记：`--leak-check=full` / `--track-origins=yes` / `--show-leak-kinds=all`

---

## ✍️ 写（约 120 分钟）

### 有毒程序 1：内存泄漏

```cpp
// tests/poison/p1_leak.cpp
#include <iostream>

int* leak_a() {
    int* p = new int[100];       // ← 没 delete
    return nullptr;
}

void leak_b() {
    int* p = new int(5);
    if (*p > 0) return;          // ← 提前返回，泄漏
    delete p;
}

int main() {
    leak_a();
    leak_b();
    std::cout << "done\n";
    return 0;
}
```

```bash
valgrind --leak-check=full --show-leak-kinds=all ./build/p1_leak
```

**抄下关键行**：`definitely lost` / `indirectly lost` / `still reachable` 各是多少字节。

### 有毒程序 2：缓冲区越界

```cpp
// tests/poison/p2_overflow.cpp
int main() {
    int* p = new int[5];
    for (int i = 0; i <= 5; ++i) {     // ← i <= 5，越界一个
        p[i] = i;
    }
    delete[] p;
    return 0;
}
```

**观察**：valgrind 报 `Invalid write of size 4`，并告诉你越界在哪一行。

### 有毒程序 3：释放后使用

```cpp
// tests/poison/p3_uaf.cpp
#include <iostream>

int main() {
    int* p = new int(42);
    delete p;
    std::cout << *p << "\n";           // ← 读已释放内存
    *p = 100;                          // ← 写已释放内存
    return 0;
}
```

**观察**：`Invalid read` / `Invalid write` + `Address ... is 0 bytes inside a block of size 4 free'd`

### 记录表

| 程序 | valgrind 关键词 | 报告里指出的行号 | 修复方式 |
|---|---|---|---|
| p1 | | | |
| p2 | | | |
| p3 | | | |

**填完这张表。**

### 进阶参数

```bash
# 追踪未初始化值的来源
valgrind --track-origins=yes ./build/p4_uninit

# 输出到文件
valgrind --log-file=vg.log ./build/p1_leak

# 只看错误摘要
valgrind -q ./build/p1_leak
```

---

## ✅ 验收（打勾才算过）

- [ ] 三个有毒程序都写出来了
- [ ] valgrind **准确指出了每个错误的行号**
- [ ] 能读懂 `definitely lost` 和 `indirectly lost` 的区别
- [ ] 记录表填完
- [ ] 会用 `--track-origins=yes`
- [ ] 会 `--log-file` 把报告写文件

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| p2 没报错 | valgrind 有时对栈上越界不敏感，确认是 `new[]` 分配的堆内存 |
| 报告太长 | 加 `2>&1 \| head -40` |
| 看不懂 `Address 0x... is 0 bytes inside` | 意思是"这个地址在某块已释放内存的起始位置" |

---

## 🔜 明天（Day 50）

剩下两种错误 + ASan 对比。
