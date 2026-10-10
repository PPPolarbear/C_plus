# Day 50 · ASan 对比 + 有毒程序（下）

**Block 17/19** · 阶段四（Day 46–57）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && touch tests/poison/p4_uninit.cpp tests/poison/p5_mismatch.cpp
```

---

## 🎯 今天唯一的目标

**补完剩下两种错误，并理解 valgrind 和 ASan 各自该在什么时候用。**

---

## 📖 读（约 30 分钟）

- GCC Manual：“Instrumentation Options”中的 `-fsanitize=address` 选项
- Clang documentation：“AddressSanitizer”中的使用与检测范围
- Valgrind User Manual：“Memcheck: a memory error detector”，对比两种工具能发现的问题与运行方式

---

## ✍️ 写（约 120 分钟）

### 有毒程序 4：使用未初始化内存

```cpp
// tests/poison/p4_uninit.cpp
#include <iostream>

int main() {
    int* p = new int[5];              // ← 未初始化
    int sum = 0;
    for (int i = 0; i < 5; ++i) {
        sum += p[i];                  // ← 读未初始化的值
    }
    std::cout << "sum = " << sum << "\n";
    delete[] p;
    return 0;
}
```

**用 `--track-origins=yes` 跑**，看它能不能指出"这个未初始化的值是从哪来的"。

### 有毒程序 5：new[] / delete 错配

```cpp
// tests/poison/p5_mismatch.cpp
#include <iostream>

int main() {
    int* p = new int[10];
    delete p;              // ← 应该用 delete[]
    return 0;
}
```

**观察**：`Mismatched free() / delete / delete[]`

### ASan 对比实验

用 ASan 重新编译同一个程序，对比两者的输出：

```bash
# 方法 1：临时用 g++ 直接编译
g++ -std=c++17 -g -fsanitize=address -o p2_asan tests/poison/p2_overflow.cpp
./p2_asan

# 方法 2：在 CMake 里加选项
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
```

**在 CMake 里加一个开关**：

```cmake
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()
```

```bash
cmake -B build-asan -DENABLE_ASAN=ON
cmake --build build-asan
./build-asan/p2_overflow
```

### 对比表（**今天的核心产出**）

| 维度 | valgrind | ASan |
|---|---|---|
| 运行速度 | | |
| 报错详细程度 | | |
| 需要重新编译吗 | | |
| 能检测未初始化读吗 | | |
| 能检测越界吗 | | |
| 适合什么时候用 | | |

**自己跑出来填，不要查答案。**

---

## ✅ 验收（打勾才算过）

- [ ] p4 的未初始化读被检测到
- [ ] `--track-origins=yes` 能指出值的来源
- [ ] p5 的错配被检测到
- [ ] **ASan 编译并运行成功**，看到了报告
- [ ] 对比表填完
- [ ] 能说出：**什么时候用 valgrind、什么时候用 ASan**
      （参考：ASan 快、适合跑测试；valgrind 不需要重编译、报告更细）

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| ASan 报错说找不到库 | 装 `libasan`，或换 clang 试 |
| p4 没报未初始化 | `new int[5]` 可能被操作系统清零了，改用 `malloc` 更明显 |
| ASan 和 valgrind 输出格式完全不同 | 正常，两者实现机制不同。重点看"指向哪一行" |

---

## 🔜 明天（Day 51）

全部产物回归验证，Block 17 收尾。
