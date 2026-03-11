# Pascal-S 语法分析器 (Parser)

## 概述

这是一个完整的 Pascal-S 编译器语法分析模块，使用 C++ 实现递归下降分析法，接受词法分析器的 Token 序列，输出抽象语法树 (AST)。

## 文件结构

- `ast.h` - AST 节点类定义
- `parser.h` - 语法分析器类声明
- `parser.cpp` - 语法分析器实现
- `test_parser.cpp` - 测试程序（包含模拟词法分析器输出）
- `Makefile` - 编译配置文件

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
- `PLUS` (+), `MINUS` (-), `MULTIPLY` (*), `DIVIDE` (/)
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

解析器严格遵循 `grammar.md` 中定义的 Pascal-S 文法产生式，实现了完整的递归下降分析。

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

## 注意事项

1. Token 序列必须以 `TokenType::END_OF_FILE` 结束
2. 所有关键字应为小写（如 "program", "begin" 等）
3. 赋值运算符使用 `:=` 而非 `=`
4. 数组下标使用方括号 `[]`
5. 参数列表使用分号 `;` 分隔参数组
