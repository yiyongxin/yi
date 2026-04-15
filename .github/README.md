# Agent Rules & Skills Scaffold

这个目录提供项目级的 Copilot 自定义基础架构。

## 目录结构

- `copilot-instructions.md`: 仓库级通用规则 (rules)。
- `instructions/cpp-default.instructions.md`: C/C++ 文件级规则。
- `skills/cpp-foundation/SKILL.md`: 可按需触发的 C++ 基础技能。
- `skills/cpp-foundation/templates/`: 代码模板。

## 使用建议

1. 先让 agent 按仓库规则工作，再按任务触发 skill。
2. 新增 skill 时，复制 `skills/cpp-foundation` 并改 `name` 与 `description`。
3. `description` 中写清触发关键词，便于被自动命中。