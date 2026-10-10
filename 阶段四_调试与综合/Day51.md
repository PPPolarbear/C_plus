# Day 51 · 全部产物回归验证

**Block 17/19** · 阶段四（Day 46–57）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build
ls build/ | grep test
```

---

## 🎯 今天唯一的目标

**给 45 天写的所有代码做一次全面体检。**

这是收尾前的最后一道关。今天过了，你的 `mini-stl` 就是干净的。

---

## 📖 读（约 20 分钟）

- **不读新内容。**回看 GNU GDB Manual “Breakpoints”与“Examining Data”、Valgrind User Manual “Memcheck error messages”，只针对回归中失败的模块查对应章节

---

## ✍️ 写（约 120 分钟）

### 任务 1：跑一遍所有测试

```bash
for t in test_vector test_string test_smartptr test_optional test_diff test_cycle; do
    echo "===== $t ====="
    if [ -f "build/$t" ]; then
        valgrind --leak-check=full --error-exitcode=1 -q ./build/$t
        echo "退出码: $?"
    else
        echo "（未构建）"
    fi
done
```

**要求：所有退出码都是 0。**

### 任务 2：给每个组件补一个"边界测试"

```cpp
// 空容器
MyVector<int> empty_vec;
assert(empty_vec.size() == 0);
assert(empty_vec.empty());
assert(empty_vec.begin() == empty_vec.end());
empty_vec.pop_back();        // 空容器 pop 不应该崩

MyString empty_str;
assert(empty_str.size() == 0);
assert(std::strlen(empty_str.c_str()) == 0);
assert(empty_str == MyString(""));

// 自我赋值
MyVector<int> v{1,2,3};
v = v;
assert(v.size() == 3);

// 大量元素
MyVector<int> big;
for (int i = 0; i < 1000000; ++i) big.push_back(i);
assert(big.size() == 1000000);
assert(big[999999] == 999999);

// 移动后使用（有效但未指定）
MyVector<int> a{1,2,3};
MyVector<int> b = std::move(a);
assert(b.size() == 3);
a.clear();                    // 移动后的对象应该可以安全调用成员函数
assert(a.size() == 0);
```

### 任务 3：整理体检报告

填这张表（**这是你的项目质量档案**）：

| 组件 | 功能测试 | 对拍 | valgrind | ASan | 边界测试 |
|---|---|---|---|---|---|
| MyVector | ☐ | ☐ | ☐ | ☐ | ☐ |
| MyString | ☐ | ☐ | ☐ | ☐ | ☐ |
| MyUniquePtr | ☐ | — | ☐ | ☐ | ☐ |
| MySharedPtr | ☐ | — | ☐ | ☐ | ☐ |
| MyOptional | ☐ | — | ☐ | ☐ | ☐ |
| ScopeGuard | ☐ | — | ☐ | ☐ | ☐ |

**全部打勾才算过。**

### 任务 4：写一个"已知限制"清单

诚实记下哪些地方还没做到位：

```markdown
## 已知限制
- MySharedPtr 未实现 weak_ptr
- MyVector 未实现 insert / erase
- MyString 未实现 find / substr
- 未测试异常安全（Day 53 补）
- 未做性能对比（Day 55 补）
```

**写下限制不是示弱，是专业。** 面试时被问到"你的实现有什么不足"，你有答案。

---

## ✅ 验收（打勾才算过）

- [ ] 所有测试程序的 valgrind **退出码都是 0**
- [ ] 六项边界测试全部写出来并通过
- [ ] 体检报告表全部打勾
- [ ] "已知限制"清单写好
- [ ] 100 万元素的 `MyVector` 能创建并正确访问

---

## ✅ Block 17 完成检查

- [ ] 能写出并识别五种内存错误
- [ ] valgrind 和 ASan 都会用，知道各自适用场景
- [ ] **全部历史产物通过回归验证**

**三条都打勾 → 进 Day 52。**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 某个测试 valgrind 报错 | 先单独跑它定位，用 `--track-origins=yes` |
| 100 万元素很慢 | 正常，valgrind 下会慢 20-50 倍。跳过 valgrind 只跑普通模式 |
| 空容器 `pop_back` 崩溃 | 需要加 `if (empty()) return;` 或断言 |

---

## 🔜 明天（Day 52）

异常安全——从"能跑"到"能给别人用"。
