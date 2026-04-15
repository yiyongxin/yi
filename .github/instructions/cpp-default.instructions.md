---
applyTo: "**/*.{h,hpp,hxx,c,cc,cpp,cxx}"
description: "Use when: writing, reviewing, or refactoring C/C++ files in this repo. Enforces baseline C++ style, safety checks, and test expectations."
---

# C++ 文件级规则

## Includes

- 先放对应头文件，再放标准库头，再放第三方头。
- 只包含当前文件真正需要的头文件。

## 函数与类

- 非成员且仅在当前编译单元使用的函数，优先放匿名命名空间。
- 构造函数初始化用初始化列表。
- 能 `const` 的成员函数要加 `const`。

## 错误处理

- 参数非法时尽早返回，减少嵌套层级。
- 错误信息要可定位问题，至少包含关键上下文。

## 测试要求

- 新增公共接口时，至少补一个正常路径测试。
- 涉及边界条件时，至少补一个边界测试。