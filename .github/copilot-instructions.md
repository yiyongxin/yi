# C++ 项目通用规则

本仓库默认使用 C++26。

## 代码设计

- 优先小而清晰的函数，单个函数只做一件事。
- 优先使用标准库容器与算法，避免重复造轮子。
- 接口设计先考虑可测试性，再考虑复用性。
- 新增模块时，先定义头文件中的最小公开接口。

## 命名与结构

- 类型名使用 `PascalCase`，函数和变量使用 `snake_case`。
- 常量使用 `kPascalCase` 风格。
- 头文件与源文件成对出现，例如 `foo.hpp` + `foo.cpp`。
- 公共头文件避免包含不必要依赖，优先前置声明。

## yi_core (核心库) 结构与发布规则

- 仓库与项目名：`yi_core`，CMake 项目中使用 `project(yi_core VERSION 0.0.1)`。核心团队负责低频发布与版本管理。
- 目录约定：
	- `include/yi/`：对外共享类型与协议（例如 `include/yi/core/common`、`include/yi/core/serialize`）
	- `src/`：实现
	- `cmake/`：导出模板（例如 `YiCoreConfig.cmake.in`）
	- `tests/`：单元测试
	- `examples/`：使用示例
- CMake 导出与安装：库目标命名为 `yi_core`，并通过 `install(TARGETS yi_core EXPORT YiCoreTargets)` 导出 `YiCoreTargets`，同时提供 `YiCoreConfig.cmake` 模板供 `find_package(yi_core)` 使用。
- 协议/类型管理：跨服务共享的通用类型应放在 `include/yi/core/common` 并随 `yi_core` 低频发布；服务专属协议应放在相应服务仓库。


## 质量与安全

- 避免裸 `new/delete`，优先使用 RAII 与智能指针。
- 所有边界输入都要做参数校验。
- 对外可恢复错误优先返回错误信息，不要静默吞错。
- 不引入与任务无关的大规模重构或格式化改动。

## 提交变更

- 改动尽量最小化，并与当前任务直接相关。
- 若修改行为逻辑，补充或更新对应测试。
- 说明中明确: 改了什么、为什么改、如何验证。