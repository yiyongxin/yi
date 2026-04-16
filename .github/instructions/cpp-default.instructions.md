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

## yi_core 仓库规则（核心库专用）

- 仓库与 CMake 项目名使用 `yi_core`，版本起始为 `0.0.1`，后续按语义化版本发布。
- 公开头文件应放在 `include/yi/...` 下，跨服务共享类型放在 `include/yi/core/common`。
- CMake 导出请命名 `YiCoreTargets`，并提供 `YiCoreConfig.cmake` 与 `YiCoreConfigVersion.cmake` 以支持 `find_package(yi_core)`。
- 对 `yi_core` 的 breaking change 必须由基础团队审批并在 CHANGELOG 中记录；重大改动需提前通知消费方并同时发布兼容迁移说明。
- 避免在同一次提交中进行大规模 API 重构；如需重构，请拆分为多个原子 PR 并在 PR 描述中列出兼容性影响。

这些规则旨在保证核心库的稳定性与可预测性，任何对约定的变更请先更新 `core/CORE_STRUCTURE.md` 并提交给基础团队审核。