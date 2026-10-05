# Day 42 · std::sort 验证

**Block 14/19** · 阶段三（Day 31–45）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build
```

---

## 🎯 今天唯一的目标

**让 `std::sort(myvec.begin(), myvec.end())` 成功排序。**

**这是你的迭代器实现正确的强证明**——标准库算法能跑在你的容器上，说明协议全部实现对了。

---

## 📖 读（约 20 分钟）

- 查 [cppreference 中文](https://zh.cppreference.com/) 的 `<algorithm>` 页
- 看看有哪些算法可以试

---

## ✍️ 写（约 120 分钟）

### 任务 1：std::sort 测试

```cpp
MyVector<int> v{5, 2, 8, 1, 9, 3};
std::sort(v.begin(), v.end());
v.dump();                        // 1 2 3 5 8 9

// 自定义比较
std::sort(v.begin(), v.end(), std::greater<int>());
v.dump();                        // 9 8 5 3 2 1
```

**跑通了？恭喜——你的迭代器是合格的随机访问迭代器。**

### 任务 2：跑一遍标准库算法全家桶

```cpp
MyVector<int> v{5, 2, 8, 1, 9, 3, 8};

// 查找
auto it = std::find(v.begin(), v.end(), 8);
if (it != v.end()) std::cout << "找到了: " << *it << "\n";

// 计数
auto n = std::count(v.begin(), v.end(), 8);
std::cout << "8 出现 " << n << " 次\n";

// 累加
int sum = std::accumulate(v.begin(), v.end(), 0);
std::cout << "总和 " << sum << "\n";

// 最值
std::cout << "最大 " << *std::max_element(v.begin(), v.end()) << "\n";
std::cout << "最小 " << *std::min_element(v.begin(), v.end()) << "\n";

// 变换
MyVector<int> doubled(v.size());
std::transform(v.begin(), v.end(), doubled.begin(), [](int x){ return x * 2; });
doubled.dump();

// 去重（需先排序）
MyVector<int> u{3, 1, 3, 2, 1};
std::sort(u.begin(), u.end());
auto last = std::unique(u.begin(), u.end());
std::cout << "去重后元素数: " << (last - u.begin()) << "\n";

// 反转
std::reverse(v.begin(), v.end());
v.dump();
```

### 任务 3：对拍验证

用 `std::vector` 做同样的操作，**结果应该完全一致**：

```cpp
void diff_algorithms() {
    MyVector<int> mine{5, 2, 8, 1, 9, 3};
    std::vector<int> ref{5, 2, 8, 1, 9, 3};

    std::sort(mine.begin(), mine.end());
    std::sort(ref.begin(), ref.end());

    assert(mine.size() == ref.size());
    for (size_t i = 0; i < ref.size(); ++i) {
        assert(mine[i] == ref[i]);          // 每个元素都要一致
    }
    std::cout << "✅ 算法对拍通过\n";
}
```

### 任务 4：验证迭代器能"被比较和解引用"

```cpp
// 这些都应该能编译
auto it1 = v.begin();
auto it2 = v.end();
bool b1 = (it1 != it2);
bool b2 = (it1 <  it2);
auto d  = it2 - it1;               // 距离
auto x  = it1[2];                  // 随机访问
```

---

## ✅ 验收（打勾才算过）

- [ ] **`std::sort` 能排序你的 `MyVector`**
- [ ] `find` / `count` / `accumulate` / `max_element` / `transform` / `reverse` 都能用
- [ ] 算法对拍与 `std::vector` 结果一致
- [ ] `it2 - it1` 能算出正确的距离
- [ ] `it1[2]` 随机访问正确
- [ ] valgrind 零错误

---

## ✅ Block 14 完成检查

- [ ] 裸指针版迭代器 + range-for
- [ ] 自定义 `Iterator` 类 + 五个 traits
- [ ] **`std::sort` 能跑通**（最强证明）

**三条都打勾 → 进 Day 43。**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| `std::sort` 报一堆模板错误 | 大概率是 `iterator_category` 写错了，必须是 `random_access_iterator_tag` |
| `it2 - it1` 类型不匹配 | `difference_type` 用 `std::ptrdiff_t` |
| `transform` 写入越界 | `doubled` 要先 `resize` 或构造够大 |

---

## 🔜 明天（Day 43）

placement new 与 alignas——为 MyOptional 做准备。
