# cpp-self

手写系列正式工程：把每轮"造轮子"的成果从 `review/*-lab/` 收进来，变成能编译、能测、能讲的正式实现。

## 为什么单独开一个工程

`review/` 里的 lab 是**教学现场**——允许半成品、允许挂着注释、允许用单文件对着跑。
这里放的是**可交付版本**：header-only + CMake + ctest，能直接拿去做面试讲稿和 GitHub 展示。

## 已有模块

| 模块 | 头文件 | 测试 | 状态 |
|---|---|---|---|
| `String` | `include/cppself/string.hpp` | `tests/test_string.cpp`（12 例） | ✅ W2 D1 第 8 版 |

## 计划中的模块

按 8 周计划的顺序推进，做完一个收一个：

- [x] `String` —— 五法则、copy-swap、moved-from 契约
- [ ] `UniquePtr` —— 带状态 deleter、EBO、条件 noexcept
- [ ] `SharedPtr` —— 原子引用计数、控制块
- [ ] `BlockingQueue` —— 条件变量、优雅停机（W4）
- [ ] `SpinLock` —— 内存序（W5）
- [ ] `ThreadPool` —— 与 `cpp-auto` 的 thrpool 对照实现（W6）

## 构建与测试

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCPPSELF_SANITIZE=ON
cmake --build build -j$(nproc)
ctest --test-dir build --output-on-failure
```

`CPPSELF_SANITIZE=ON` 会打开 ASan + UBSan。**WSL 里的 GCC 11.4 支持，Cygwin 的 GCC 7.3 不支持。**

每个用例是独立进程（`test_string 1` … `test_string 12`），崩一个不会掩盖后面的。

## 约定

- 一个类一个头文件，header-only，不引第三方依赖
- 测试用例**先写**，实现后补；用例编号固定，不删不改语义
- 每个类的不变式写在头文件顶部，别只写在脑子里
