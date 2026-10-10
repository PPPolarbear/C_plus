# Day 30 · ⭐ 里程碑 2：对拍通过

**Block 10/19** · 阶段二（Day 16–30）· **阶段收尾**

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && touch tests/test_diff.cpp
```

---

## 🎯 今天唯一的目标

**让 `MyString` 和 `std::string` 在 10000 次随机操作下行为完全一致。**

**这是你第一次拥有客观判分器。** 从今天起，"我觉得我会了"变成"对拍通过了"。

---

## 📖 读（约 20 分钟）

- **不读新内容。**只回看 LearnCpp 21.1 “Introduction to operator overloading”、21.3 “Overloading operators using normal functions”，并检查 MyString / MyVector 与对应标准类型的接口差异
不读新内容。今天只写测试。

---

## ✍️ 写（约 120 分钟）

### 对拍框架

```cpp
// tests/test_diff.cpp
#include "mystring.h"
#include <string>
#include <random>
#include <cassert>
#include <iostream>

int main() {
    std::mt19937 rng(42);              // 固定种子，结果可复现
    MyString    mine;
    std::string ref;

    const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    const int  OPS = 10000;
    int failures = 0;

    for (int i = 0; i < OPS; ++i) {
        int op = rng() % 5;
        char c = charset[rng() % 26];

        switch (op) {
            case 0:                                   // push_back
                mine.push_back(c);
                ref.push_back(c);
                break;
            case 1: {                                 // append
                std::string piece(1 + rng() % 5, c);
                mine.append(piece.c_str());
                ref.append(piece);
                break;
            }
            case 2:                                   // clear
                mine.clear();
                ref.clear();
                break;
            case 3:                                   // reserve
                {
                    size_t n = rng() % 100;
                    mine.reserve(n);
                    ref.reserve(n);
                }
                break;
            case 4:                                   // 随机访问
                if (ref.size() > 0) {
                    size_t idx = rng() % ref.size();
                    if (mine[idx] != ref[idx]) {
                        std::cout << "❌ [" << i << "] 下标 " << idx << " 不一致\n";
                        ++failures;
                    }
                }
                break;
        }

        // ---- 核心断言：每一轮都检查 ----
        if (mine.size() != ref.size()) {
            std::cout << "❌ [" << i << "] size 不一致: "
                      << mine.size() << " vs " << ref.size() << "\n";
            ++failures;
            break;
        }
        if (std::strcmp(mine.c_str(), ref.c_str()) != 0) {
            std::cout << "❌ [" << i << "] 内容不一致\n"
                      << "   mine: " << mine.c_str() << "\n"
                      << "   ref : " << ref.c_str() << "\n";
            ++failures;
            break;
        }
    }

    if (failures == 0) {
        std::cout << "✅ 对拍通过： " << OPS << " 次操作完全一致\n";
    }
    return failures == 0 ? 0 : 1;
}
```

### 也要对拍 MyVector

```cpp
// tests/test_vector.cpp —— 同样的思路
MyVector<int> mine;
std::vector<int> ref;
// 随机 push_back / pop_back / resize / 随机访问
// 每轮比较 size() 和所有元素
```

### 跑起来

```bash
cmake --build build
./build/test_diff
valgrind --leak-check=full ./build/test_diff
```

---

## ✅ 验收（打勾才算过）

- [ ] **`MyString` 对拍 10000 次操作，零差异**
- [ ] **`MyVector<int>` 对拍 10000 次操作，零差异**
- [ ] valgrind 零泄漏
- [ ] 故意改坏一处（比如 `append` 忘了写 `'\0'`），**对拍能抓出来**
- [ ] 能说出对拍相比手写测试用例的优势

> **第 4 条特别重要**：证明你的测试**真的有效**，不是摆设。

---

## 🎯 阶段二总验收

| 检查项 | 打勾 |
|---|---|
| `new`/`delete` 配对，理解 `new[]`/`delete[]` | ☐ |
| 复现并解释过浅拷贝 double free | ☐ |
| 会读 valgrind 报告 | ☐ |
| `MyVector` 拷贝语义 + copy-and-swap | ☐ |
| `MyVector` 移动语义 + `noexcept` | ☐ |
| `emplace_back` + placement new | ☐ |
| `MyString` 完整实现 | ☐ |
| 运算符重载 + 友元 | ☐ |
| **对拍测试通过** | ☐ |

**九条全打勾 → 阶段二完成。**

---

## 📝 阶段二复盘（填进 `进度追踪.md`）

- 对拍抓出来过几个 bug：____________
- copy 次数从 Day 21 到 Day 26 下降了多少：____________
- 最卡的地方：____________

---

## 💼 简历可以先写一行了

> 从零实现 `MyVector` / `MyString` 容器，覆盖 RAII、拷贝/移动语义、placement new；编写对拍测试（10⁴ 次随机操作与 `std` 标准库比对）通过率 100%，`valgrind` 零泄漏

---

## 🔜 明天（Day 31）

进入阶段三：智能指针与模板。
