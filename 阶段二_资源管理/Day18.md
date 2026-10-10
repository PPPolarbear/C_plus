# Day 18 · valgrind 解剖崩溃

**Block 6/19** · 阶段二（Day 16–30）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build
valgrind --leak-check=full --show-leak-kinds=all ./build/app 2>&1 | head -60
```

---

## 🎯 今天唯一的目标

**学会读 valgrind 报告——这是你后面 39 天里最常用的工具。**

从今天起，每个 Block 的验收都会写"valgrind 零错误"。今天先学会怎么看它。

---

## 📖 读（约 30 分钟）

- Valgrind User Manual 的 “Memcheck: a memory error detector”与“Memcheck error messages”两节
- 对照报告中的 `Invalid free`、`definitely lost`、`indirectly lost`、`Invalid write` 四种诊断

---

## ✍️ 写（约 120 分钟）

### 任务 1：读懂昨天那份报告

回到昨天的 `ShallowCopy`，用 valgrind 跑，逐行解读：

| valgrind 输出片段 | 它的意思 |
|---|---|
| `Invalid free() / delete / delete[]` | |
| `Address 0x... is 0 bytes inside a block of size 20 free'd` | |
| `at 0x...: operator delete[]` | |
| `definitely lost: 20 bytes in 1 blocks` | |

**填完这张表。**

### 任务 2：五种内存错误的对照实验

写 5 个小程序，每个只犯一种错，用 valgrind 观察报告差异：

```cpp
// A. 泄漏：new 了没 delete
// B. 越界写：new int[5] 但写 p[10]
// C. 释放后使用：delete p; *p = 1;
// D. 未初始化读：int* p = new int[5]; 读 p[0]
// E. 错配：new[] 配 delete
```

**每个都跑 valgrind，把关键报错行抄下来。**

### 任务 3：修复 ShallowCopy（只是为了验证）

**临时**加上拷贝构造函数，确认 valgrind 干净了：

```cpp
ShallowCopy::ShallowCopy(const ShallowCopy& other)
    : data_(new int[other.size_]), size_(other.size_)
{
    std::copy(other.data_, other.data_ + size_, data_);
}
```

跑 valgrind → 零错误。

**然后再把这段删掉**——Day 22 要在 `MyVector` 上正式写。

---

## ✅ 验收（打勾才算过）

- [ ] 昨天那张解读表填完了
- [ ] 五种内存错误的 valgrind 报告都见过，能说出各自关键词
- [ ] 临时加上拷贝构造后 valgrind 变干净（**证明诊断是对的**）
- [ ] 能说出 `definitely lost` 和 `indirectly lost` 的区别
- [ ] 知道 `--leak-check=full` 和 `--show-leak-kinds=all` 是干什么的

---

## ✅ Block 6 完成检查

- [ ] `new`/`delete`/`new[]`/`delete[]` 配对使用熟练
- [ ] 亲手复现并解释过浅拷贝 double free
- [ ] 会读 valgrind 报告，能定位五类内存错误

**三条都打勾 → 进 Day 19。**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| valgrind 输出太长 | 加 `2>&1 \| head -60`，或 `--log-file=vg.log` 写文件 |
| `--show-leak-kinds=all` 报错 | 老版本 valgrind 不支持，去掉这个参数 |
| 分不清 C 和 E | C 是"指针本身已失效"，E 是"释放方式不匹配" |

---

## 🔜 明天（Day 19）

**开始写 MyVector。这是整个计划的分水岭。**
