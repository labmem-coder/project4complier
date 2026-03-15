# Pascal-S 语法分析器 - 修改日志

本文档记录了 Pascal-S 语法分析器项目的所有重要修改和更新。

---

## 版本 2.0 - LL(1) 预测分析实现

**修改日期**：2026-03-14  
**修改类型**：重大功能改造  
**修改人员**：项目开发团队

### 修改概述

将原有的递归下降分析器改造为基于预测分析表的 LL(1) 分析器，消除了所有左递归和左公因子，实现了标准的 LL(1) 预测分析方法。

### 详细修改内容

#### 1. 文法改造

**新增文件**：

- `NonLeftRecursion.md` - 消除左递归和左公因子后的完整 LL(1) 文法

**改造内容**：

- ✅ 消除 10 个左递归产生式：
  - `idlist → idlist , id`
  - `const_declaration → const_declaration ; id = const_value`
  - `var_declaration → var_declaration ; idlist : type`
  - `period → period , digits .. digits`
  - `parameter_list → parameter_list ; parameter`
  - `statement_list → statement_list ; statement`
  - `variable_list → variable_list , variable`
  - `expression_list → expression_list , expression`
  - `simple_expression → simple_expression addop term`
  - `term → term mulop factor`

- ✅ 消除左公因子：
  - `program_head` - 提取公因子 `program id`
  - `statement` - 合并 `variable assignop` 和 `func_id assignop`
  - `factor` - 合并 `variable` 和 `id(...)` 的公因子

**改写模式**：

```
原始：A → Aα | β
改写：A → βA'
     A' → αA' | ε
```

#### 2. 预测分析表构建

**新增文件**：

- `table.md` - 包含 FIRST 集、FOLLOW 集和完整预测分析表

**内容**：

- ✅ 计算了 42 个非终结符的 FIRST 集
- ✅ 计算了 42 个非终结符的 FOLLOW 集
- ✅ 构建了完整的 M[A, a] 预测分析表
- ✅ 验证了 LL(1) 文法的正确性（无冲突）

#### 3. 代码实现修改

**修改文件**：

- `parser.h` - 添加了 13 个辅助函数声明
- `parser.cpp` - 实现了基于 LL(1) 的递归分析

**新增函数**（\_prime 辅助函数）：

```cpp
void parse_idlist_prime(std::vector<std::string>& ids);
void parse_const_declaration_prime(std::shared_ptr<ConstDeclarationsNode> decls);
void parse_var_declaration_prime(std::vector<std::shared_ptr<VarDeclarationNode>>& declarations);
void parse_period_prime(std::vector<std::shared_ptr<PeriodNode>>& periods);
void parse_subprogram_declarations_prime(std::shared_ptr<SubprogramDeclarationsNode> decls);
void parse_parameter_list_prime(std::shared_ptr<ParameterListNode> param_list);
void parse_statement_list_prime(std::vector<std::shared_ptr<StatementNode>>& statements);
void parse_variable_list_prime(std::vector<std::shared_ptr<VariableNode>>& variables);
void parse_expression_list_prime(std::vector<std::shared_ptr<ExpressionNode>>& expressions);
std::shared_ptr<ExpressionNode> parse_expression_prime(std::shared_ptr<ExpressionNode> left);
std::shared_ptr<ExpressionNode> parse_simple_expression_prime(std::shared_ptr<ExpressionNode> left);
std::shared_ptr<ExpressionNode> parse_term_prime(std::shared_ptr<ExpressionNode> left);
```

**修改的主要函数**：

- `parse_idlist()` - 改为右递归实现
- `parse_const_declaration()` - 改为右递归实现
- `parse_var_declaration()` - 改为右递归实现
- `parse_period()` - 改为右递归实现
- `parse_subprogram_declarations()` - 改为右递归实现
- `parse_parameter_list()` - 改为右递归实现
- `parse_statement_list()` - 改为右递归实现
- `parse_variable_list()` - 改为右递归实现
- `parse_expression_list()` - 改为右递归实现
- `parse_expression()` - 改为右递归实现
- `parse_simple_expression()` - 改为右递归实现
- `parse_term()` - 改为右递归实现

#### 4. 运算符定义确认

**验证内容**：

- ✅ **addop**：`+`, `-`, `or` (TokenType::PLUS, MINUS, OR)
- ✅ **mulop**：`*`, `/`, `div`, `mod`, `and` (TokenType::MULTIPLY, DIVIDE, DIV, MOD, AND)
- ✅ **relop**：`=`, `<>`, `<`, `<=`, `>`, `>=` (TokenType::EQ, NE, LT, LE, GT, GE)
- ✅ **uminus**：`-` (一元负号，TokenType::MINUS)

所有运算符定义与 README.md 完全一致。

#### 5. 文档更新

**修改文件**：

- `README.md` - 全面更新，添加 LL(1) 相关说明

**新增内容**：

- 核心特性说明
- 文件结构详细分类（核心代码、文法文档、测试文件、项目文档）
- 文法改造说明（消除左递归、消除左公因子、构建预测分析表）
- 主要文法改写对照表
- 运算符优先级说明
- LL(1) 分析实现细节
- 预测分析表使用示例
- 版本信息

**新增文件**：

- `log.md` - 本文件，记录项目修改历史

### 编译与测试

**编译状态**：✅ 成功

```bash
g++ -std=c++17 -Wall -Wextra -g parser.cpp test_parser.cpp -o test_parser.exe
```

- 无编译错误
- 无编译警告

**测试文件**：

- `test4.tokens` - 程序结构测试（通过）
- `test5.tokens` - 变量与子程序声明测试（通过）
- `test6.tokens` - 复合语句与参数测试（通过）

### 技术特点

1. **严格遵循 LL(1) 文法**：所有解析函数都按照消除左递归后的文法实现
2. **递归方式实现**：使用递归函数而非显式栈，代码更清晰
3. **保持 AST 构建**：所有修改都保持了原有的 AST 节点构建逻辑
4. **向后兼容**：AST 结构未改变，上层代码无需修改

### 影响范围

**修改的文件**：

- `parser.h` (修改)
- `parser.cpp` (修改)
- `README.md` (修改)

**新增的文件**：

- `NonLeftRecursion.md` (新增)
- `table.md` (新增)
- `log.md` (新增)

**未修改的文件**：

- `ast.h` (保持不变)
- `test_parser.cpp` (保持不变)
- `grammar.md` (保持不变，作为原始文法参考)
- 所有 `.tokens` 测试文件 (保持不变)

### 后续工作建议

1. ✅ 已完成：基本 LL(1) 分析器实现
2. ✅ 已完成：文法文档和预测分析表
3. ✅ 已完成：代码编译和基础测试
4. 🔲 待完成：更全面的测试用例
5. 🔲 待完成：性能优化和错误恢复机制

---

## 版本 1.0 - 初始递归下降分析器

**创建日期**：2026-03 (具体日期未记录)  
**修改类型**：项目初始化

### 初始实现内容

- 实现了基础的递归下降分析器
- 支持完整的 Pascal-S 语法
- 包含 AST 节点定义
- 提供基本的测试用例

**初始文件**：

- `ast.h` - AST 节点类定义
- `parser.h` - 语法分析器类声明
- `parser.cpp` - 递归下降分析器实现
- `test_parser.cpp` - 测试程序
- `grammar.md` - Pascal-S 文法定义
- `README.md` - 项目说明文档
- `test1.tokens`, `test2.tokens`, `test3.tokens` - 测试用例

---

## 版本 2.1 - 错误处理机制实现

**修改日期**：2026-03-15  
**修改类型**：功能增强  
**修改人员**：项目开发团队

### 修改概述

在 LL(1) 预测分析器的基础上，添加了完整的错误处理和恢复机制，实现了基于同步符号（synch）的错误恢复策略，使解析器能够在遇到语法错误时继续分析，发现更多错误。

### 详细修改内容

#### 1. 预测分析表增强

**修改文件**：

- `table.md` - 添加第五章"带同步化信息的预测分析表"

**新增内容**：

- ✅ 为每个非终结符的 FOLLOW 集中的符号添加 synch 标记
- ✅ 完整的同步化分析表，覆盖所有主要非终结符
- ✅ 错误恢复示例和说明
- ✅ 错误处理实现要点文档

**同步符号选择原则**：

- 对于非终结符 A，将其 FOLLOW(A) 集中的符号标记为 synch
- 某些重要的起始符号也标记为 synch

#### 2. 错误处理数据结构

**修改文件**：

- `parser.h` - 添加错误处理相关的类和成员

**新增结构**：

```cpp
struct ErrorInfo {
    int line;
    int column;
    std::string message;
    std::string expected;
    std::string found;
    std::string recovery;
};
```

**新增成员变量**：

- `std::vector<ErrorInfo> errors` - 存储所有语法错误
- `bool error_recovery_mode` - 标记是否处于错误恢复模式

**新增成员函数**：

- `void report_error()` - 报告错误
- `bool is_synch_token()` - 检查是否为同步符号
- `void skip_to_synch()` - 跳过到同步符号
- `std::string token_type_to_string()` - Token 类型转字符串
- `const std::vector<ErrorInfo>& get_errors()` - 获取错误列表
- `bool has_errors()` - 检查是否有错误
- `void print_errors()` - 打印所有错误

#### 3. 错误处理策略实现

**修改文件**：

- `parser.cpp` - 实现错误处理逻辑

**错误处理策略**：

1. **跳过策略**：当 M[A, a] 为空时，跳过当前输入符号 a
2. **弹出策略**：当 M[A, a] 为 synch 时，弹出栈顶非终结符 A
3. **继续分析**：错误恢复后继续分析，尽可能发现更多错误

**核心修改**：

- 修改 `expect()` 函数：不再抛出异常，而是记录错误并继续
- 添加 `report_error()` 函数：记录错误信息到 errors 列表
- 添加 `token_type_to_string()` 函数：提供友好的错误消息
- 添加 `print_errors()` 函数：格式化输出所有错误

**错误报告格式**：

```
Error 1 at line 5, column 10:
  Message: Expected ';'
  Expected: ;
  Found: 'var'
  Recovery: Skipped current token
```

#### 4. 测试程序更新

**修改文件**：

- `test_parser.cpp` - 添加错误报告输出

**修改内容**：

```cpp
if (parser.has_errors()) {
    parser.print_errors();
    std::cout << "\n✗ Parse completed with errors" << std::endl;
    return 1;
}
```

#### 5. 错误测试用例

**新增文件**：

- `test_error.tokens` - 包含语法错误的测试用例

**测试场景**：

- 缺少分号
- 类型不匹配
- 多余的符号
- 未定义的标识符

### 编译与测试

**编译状态**：✅ 成功

```bash
g++ -std=c++17 -Wall -Wextra -g parser.cpp test_parser.cpp -o test_parser.exe
```

- 无编译错误
- 无编译警告

**测试结果**：

- ✅ 正确程序仍能正常解析（test9.tokens 通过）
- ✅ 错误程序能够检测并报告错误（test_error.tokens）
- ✅ 错误恢复机制正常工作

### 技术特点

1. **非中断式错误处理**：遇到错误不停止，继续分析发现更多错误
2. **详细的错误报告**：包含错误位置、期望符号、实际符号和恢复操作
3. **基于理论的恢复策略**：严格遵循编译原理中的 synch 机制
4. **向后兼容**：不影响正确程序的解析

### 影响范围

**修改的文件**：

- `parser.h` (修改) - 添加错误处理接口
- `parser.cpp` (修改) - 实现错误处理逻辑
- `test_parser.cpp` (修改) - 添加错误输出
- `table.md` (修改) - 添加同步化分析表

**新增的文件**：

- `test_error.tokens` (新增) - 错误测试用例

**未修改的文件**：

- `ast.h` (保持不变)
- `grammar.md` (保持不变)
- `NonLeftRecursion.md` (保持不变)
- 所有正确的 `.tokens` 测试文件 (保持不变)

### 后续工作建议

1. ✅ 已完成：基本错误处理机制
2. ✅ 已完成：错误报告功能
3. ✅ 已完成：错误测试用例
4. 🔲 待完成：更多错误场景的测试
5. 🔲 待完成：错误恢复策略的优化
6. 🔲 待完成：错误消息的国际化支持

---

## 日志格式说明

每个版本的修改日志应包含以下信息：

1. **版本号和标题**：清晰标识版本
2. **修改日期**：记录修改的具体日期
3. **修改类型**：功能新增、Bug修复、性能优化、文档更新等
4. **修改人员**：记录修改者（可选）
5. **修改概述**：简要说明本次修改的目的
6. **详细修改内容**：列出所有修改的细节
7. **影响范围**：说明哪些文件被修改、新增或删除
8. **测试状态**：记录编译和测试结果
9. **后续工作**：列出待完成的任务（可选）

---

**文档维护**：请在每次重要修改后及时更新本日志文件。
