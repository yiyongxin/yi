---
name: cpp-foundation
description: "Use when: starting a new C++ module, adding a new class pair (.hpp/.cpp), or implementing a small feature with tests in this repository."
---

# C++ Foundation Skill

## 目标

快速生成符合仓库约定的 C++ 最小实现骨架，减少首个模块开发时的反复沟通。

## 输入

- 模块名 (例如 `user_service`)
- 是否需要对外公开头文件
- 是否需要单元测试

## 工作流

1. 在 `include/` 或 `src/` 下确定头文件路径。
2. 创建同名 `.hpp` 与 `.cpp`，先写最小接口。
3. 为输入参数添加边界校验。
4. 若启用测试，补正常路径 + 边界路径。
5. 输出改动摘要与验证命令。

## 产出标准

- 命名遵循 `PascalCase`(类型) 与 `snake_case`(函数/变量)。
- 无裸 `new/delete`。
- 公共接口有基础注释(做什么、输入限制、返回含义)。

## 可复用模板

- `templates/module.hpp.tmpl`
- `templates/module.cpp.tmpl`
- `templates/module_test.cpp.tmpl`