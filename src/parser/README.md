# Pascal-S 语法分析器 (Parser)

## 概述

这是一个完整的 Pascal-S 编译器语法分析模块，使用 C++ 实现 **LL(1) 预测分析法**（基于递归实现），接受词法分析器的 Token 序列，输出抽象语法树 (AST)。

### 核心特性

- ✅ **LL(1) 文法**：消除了所有左递归和左公因子
- ✅ **预测分析表驱动**：基于 FIRST 集和 FOLLOW 集构建
- ✅ **递归实现**：使用递归函数实现预测分析，代码清晰易维护
- ✅ **完整 AST 构建**：生成完整的抽象语法树供后续编译阶段使用
- ✅ **严格错误处理**：提供详细的语法错误定位信息

## 文件结构

### 核心代码

- `ast.h` - AST 节点类定义（所有语法树节点类型）
- `parser.h` - 语法分析器类声明（LL(1) 递归分析器接口）
- `parser.cpp` - 语法分析器实现（基于预测分析表的递归实现）
- `test_parser.cpp` - 测试程序（包含 Token 文件读取和解析测试）

### 文法文档

- `grammar.md` - **原始 Pascal-S 文法**（包含左递归）
- `NonLeftRecursion.md` - **消除左递归和左公因子后的文法**（LL(1) 文法）
- `table.md` - **预测分析表**（包含 FIRST 集、FOLLOW 集和完整预测分析表）

### 测试文件

- `test1.tokens`, `test2.tokens`, `test3.tokens` - 基础测试用例
- `test4.tokens` - 程序结构测试（常量声明、程序头等）
- `test5.tokens` - 变量与子程序声明测试
- `test6.tokens` - 复合语句与参数测试
- `temp.txt` - 测试源代码（Pascal-S 程序）

### 项目文档

- `README.md` - 本文档
- `log.md` - 项目修改日志

## 编译和运行

### 手动编译

```bash
cd src/parser
g++ -std=c++17 -Wall -Wextra -g parser.cpp test_parser.cpp -o test_parser
```

### 运行测试

```bash
# Windows
test_parser.exe test1.tokens

# Linux/Mac
./test_parser test1.tokens
```

### 清理

```bash
# Windows
del *.o test_parser.exe

# Linux/Mac
rm -f *.o test_parser
```

## Token 文件格式

测试程序从文件读取 Token，每行一个 Token，格式为：

```
TOKEN_TYPE VALUE LINE COLUMN
```

### 示例

```
PROGRAM program 1 1
ID TestProgram 1 9
SEMICOLON ; 1 35
VAR var 2 1
ID x 2 5
COLON : 2 9
INTEGER integer 2 11
SEMICOLON ; 2 18
BEGIN begin 3 1
END end 4 1
DOT . 4 4
END_OF_FILE EOF 5 1
```

### 注释

以 `#` 开头的行会被忽略，可用于添加注释。

## 使用方法

### 1. 准备 Token 文件

创建一个 `.tokens` 文件，每行包含一个 Token 的信息（类型、值、行号、列号）。

### 2. 运行解析器

```bash
test_parser your_tokens.tokens
```

### 3. 编程接口

```cpp
#include "parser.h"
#include <vector>

// 从文件读取 tokens
std::vector<Token> tokens = read_tokens_from_file("input.tokens");

// 创建解析器并解析
Parser parser(tokens);
try {
    auto ast = parser.parse();
    // 使用 AST 进行后续处理
} catch (const ParseError& e) {
    std::cerr << "Parse error: " << e.what() << std::endl;
}
```

## Token 类型

解析器支持以下 Token 类型：

### 关键字

- `PROGRAM`, `BEGIN`, `END`
- `CONST`, `VAR`
- `PROCEDURE`, `FUNCTION`
- `IF`, `THEN`, `ELSE`
- `FOR`, `TO`, `DO`
- `READ`, `WRITE`
- `INTEGER`, `REAL`, `BOOLEAN`, `CHAR`
- `ARRAY`, `OF`
- `AND`, `OR`, `NOT`
- `DIV`, `MOD`

### 标识符和字面量

- `ID` - 标识符
- `NUMBER` - 数字
- `CHAR_LITERAL` - 字符字面量

### 运算符

- `PLUS` (+), `MINUS` (-), `MULTIPLY` (\*), `DIVIDE` (/)
- `EQ` (=), `NE` (<>), `LT` (<), `LE` (<=), `GT` (>), `GE` (>=)
- `ASSIGN` (:=)

### 分隔符

- `SEMICOLON` (;), `COLON` (:), `COMMA` (,), `DOT` (.), `DOTDOT` (..)
- `LPAREN` ((), `RPAREN` ())
- `LBRACKET` ([), `RBRACKET` (])

## AST 节点类型

### 程序结构

- `ProgramNode` - 程序根节点
- `ProgramHeadNode` - 程序头
- `ProgramBodyNode` - 程序体

### 声明

- `ConstDeclarationsNode` - 常量声明列表
- `ConstDeclarationNode` - 单个常量声明
- `VarDeclarationsNode` - 变量声明列表
- `VarDeclarationNode` - 单个变量声明
- `TypeNode` - 类型节点
- `PeriodNode` - 数组范围

### 子程序

- `SubprogramDeclarationsNode` - 子程序声明列表
- `SubprogramNode` - 子程序
- `SubprogramHeadNode` - 子程序头
- `SubprogramBodyNode` - 子程序体
- `ParameterListNode` - 参数列表
- `ParameterNode` - 参数

### 语句

- `CompoundStatementNode` - 复合语句
- `AssignmentNode` - 赋值语句
- `ProcedureCallNode` - 过程调用
- `IfNode` - 条件语句
- `ForNode` - 循环语句
- `ReadNode` - 读语句
- `WriteNode` - 写语句
- `EmptyStatementNode` - 空语句

### 表达式

- `BinaryOpNode` - 二元运算
- `UnaryOpNode` - 一元运算
- `NumberNode` - 数字
- `VariableNode` - 变量
- `FunctionCallNode` - 函数调用

## 测试用例

测试程序包含三个测试用例：

1. **基本程序测试** - 包含常量、变量、过程声明和基本语句
2. **For 循环测试** - 测试循环结构
3. **数组测试** - 测试数组声明和访问

## 语法规则

### 文法改造

解析器实现基于 **LL(1) 预测分析方法**，对原始文法进行了以下改造：

1. **消除左递归**：将所有形如 `A → Aα | β` 的产生式改写为 `A → βA'` 和 `A' → αA' | ε`
2. **消除左公因子**：提取公共前缀，确保每个非终结符的产生式可以通过向前看一个符号唯一确定
3. **构建预测分析表**：基于 FIRST 集和 FOLLOW 集构建 M[A, a] 预测分析表

详细文法定义请参考：

- **原始文法**：`grammar.md`
- **LL(1) 文法**：`NonLeftRecursion.md`
- **预测分析表**：`table.md`

### 主要文法改写

| 原始产生式（左递归）                               | 改写后（右递归）                                                                                           |
| -------------------------------------------------- | ---------------------------------------------------------------------------------------------------------- |
| `idlist → idlist , id`                             | `idlist → id idlist'`<br>`idlist' → , id idlist' \| ε`                                                     |
| `expression_list → expression_list , expression`   | `expression_list → expression expression_list'`<br>`expression_list' → , expression expression_list' \| ε` |
| `simple_expression → simple_expression addop term` | `simple_expression → term simple_expression'`<br>`simple_expression' → addop term simple_expression' \| ε` |
| `term → term mulop factor`                         | `term → factor term'`<br>`term' → mulop factor term' \| ε`                                                 |

### 运算符优先级

按照 Pascal-S 标准，运算符优先级从高到低：

1. **一元运算符**：`not`, `-` (uminus)
2. **乘法运算符** (mulop)：`*`, `/`, `div`, `mod`, `and`
3. **加法运算符** (addop)：`+`, `-`, `or`
4. **关系运算符** (relop)：`=`, `<>`, `<`, `<=`, `>`, `>=`

## 错误处理

解析器在遇到语法错误时会抛出 `ParseError` 异常，包含：

- 错误消息
- 错误位置（行号、列号）
- 当前 Token 信息

## 接口说明

### Parser 类

```cpp
class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::shared_ptr<ProgramNode> parse();
};
```

### Token 结构

```cpp
struct Token {
    TokenType type;      // Token 类型
    std::string value;   // Token 值
    int line;           // 行号
    int column;         // 列号
};
```

## LL(1) 分析实现细节

### 预测分析表使用

解析器通过以下方式实现预测分析：

1. **主解析函数**：对应每个非终结符 A，实现 `parse_A()` 函数
2. **辅助函数**：对应每个 A' 非终结符，实现 `parse_A_prime()` 函数
3. **向前看符号**：使用 `match()` 函数检查当前 token 类型，决定使用哪个产生式
4. **递归调用**：按照产生式右部递归调用相应的解析函数

### 示例：表达式解析

```cpp
// expression → simple_expression expression'
std::shared_ptr<ExpressionNode> Parser::parse_expression() {
    auto left = parse_simple_expression();
    return parse_expression_prime(left);
}

// expression' → relop simple_expression | ε
std::shared_ptr<ExpressionNode> Parser::parse_expression_prime(
    std::shared_ptr<ExpressionNode> left) {
    if (match(TokenType::EQ, TokenType::NE, TokenType::LT,
              TokenType::LE, TokenType::GT, TokenType::GE)) {
        std::string op = current_token.value;
        advance();
        auto right = parse_simple_expression();
        return std::make_shared<BinaryOpNode>(op, left, right);
    }
    return left;  // ε 产生式
}
```

## 注意事项

1. Token 序列必须以 `TokenType::END_OF_FILE` 结束
2. 所有关键字应为小写（如 "program", "begin" 等）
3. 赋值运算符使用 `:=` 而非 `=`（等号 `=` 用于关系运算和常量定义）
4. 数组下标使用方括号 `[]`
5. 参数列表使用分号 `;` 分隔参数组
6. 常量声明使用 `=` 而非 `:=`

## 相关文档

- **文法定义**：查看 `grammar.md` 了解原始 Pascal-S 文法
- **LL(1) 文法**：查看 `NonLeftRecursion.md` 了解消除左递归后的文法
- **预测分析表**：查看 `table.md` 了解 FIRST/FOLLOW 集和预测分析表
- **修改日志**：查看 `log.md` 了解项目历史修改记录

## 版本信息

- **当前版本**：2.0 (LL(1) 预测分析版本)
- **上一版本**：1.0 (递归下降分析版本)
- **最后更新**：2026-03-14
