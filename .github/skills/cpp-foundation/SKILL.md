---
name: cpp-foundation
description: "Use when: starting a new C++ module, adding a new class pair (.hpp/.cpp), or implementing a small feature with tests in this repository."
---

# C++ Foundation Skill

## 目标

快速生成符合仓库约定的 C++ 最小实现骨架，尤其针对 `yi_core`（核心框架）模块的创建与维护。

## 输入

- 模块名 (例如 `user_service`) 或核心模块 `yi_core`
- 是否需要对外公开头文件
- 是否需要单元测试

## 工作流（针对 `yi_core`）

1. 在 `core/` 仓库中按下列结构创建文件（参考 `core/CORE_STRUCTURE.md`）：
	- `include/yi/`：对外公共头
	- `src/`：实现
	- `cmake/`：导出与安装模板（如 `YiCoreConfig.cmake.in`）
	- `tests/`：单元测试
	- `examples/`：使用示例
2. 在 `CMakeLists.txt` 中设置项目名 `yi_core` 与版本（当前约定 `0.0.1`）。
3. 提供最小化的导出：`install(TARGETS yi_core EXPORT YiCoreTargets)` 与 `install(EXPORT ...)`。
4. 为公共类型放在 `include/yi/core/common`，协议/序列化放在 `include/yi/core/serialize`。
5. 若要本地联调，服务仓库可通过 `FetchContent` 的 `SOURCE_DIR` 指向本地同级路径覆盖远程依赖。
6. 输出改动摘要与验证命令（见下方验证步骤）。

## 产出标准

- 项目名与库名统一使用 `yi_core` / `yi_modules` / `user-service`。
- `yi_core` 初始版本为 `0.0.1`，后续遵循语义化版本发布策略。
- 头文件路径以 `include/yi/...` 为根，避免直接使用私有路径。
- 导出与安装支持 CMake `find_package(yi_core)`。

## 验证命令（示例）

在 `core` 目录：

```bash
mkdir -p build && cd build
cmake ..
cmake --build .
```

若启用 `FetchContent` 测试集成，在 `user-service` 中运行：

```bash
mkdir -p build && cd build
cmake ..
cmake --build .
```

## 可复用模板

- `templates/module.hpp.tmpl`
- `templates/module.cpp.tmpl`
- `templates/module_test.cpp.tmpl`

## yi_core 结构约定（补充）

当创建或变更 `yi_core`（核心框架）模块时，请遵循下列约定：

- 仓库名与 CMake 项目名：`yi_core`，初始版本 `0.0.1`。
- 目录布局：
	- `include/yi/`：公开头文件（例如 `include/yi/core/...`、`include/yi/serialize/...`）
	- `src/`：实现文件（与 `include` 镜像）
	- `cmake/`：导出配置模板（例如 `YiCoreConfig.cmake.in`）
	- `tests/`：单元测试
	- `examples/`：使用示例

- CMake 目标命名与导出：库目标请命名为 `yi_core` 并导出 `YiCoreTargets`，为其它仓库使用 `find_package(yi_core REQUIRED)` 提供支持。

工作流建议：先在 `include/` 定义最小公共类型，再实现 `src/`；为跨服务共享类型放在 `include/yi/core/common`。修改核心库版本需由基础团队审核并打 tag（低频发布）。