# Pascal-S 消除左递归和左公因子后的文法

本文法基于 `grammar.md` 中的原始文法，消除了所有直接左递归和左公因子，适用于 LL(1) 预测分析。

## 终结符说明
- **关键字**: program, const, var, procedure, function, begin, end, if, then, else, for, to, do, read, write, array, of, integer, real, boolean, char
- **运算符**: 
  - addop: +, -, or
  - mulop: *, /, div, mod, and
  - relop: =, <>, <, <=, >, >=
  - uminus: - (一元负号)
  - assignop: :=
- **分隔符**: ;, :, ,, ., .., (, ), [, ]
- **标识符和字面量**: id, num, letter

---

## 1. 程序结构相关

### 1.1 程序结构
```
programstruct → program_head ; program_body .
```

### 1.2 程序头（消除左公因子）
```
program_head → program id program_head'
program_head' → ( idlist ) | ε
```

### 1.3 程序体
```
program_body → const_declarations var_declarations subprogram_declarations compound_statement
```

### 1.4 标识符列表（消除左递归）
```
idlist → id idlist'
idlist' → , id idlist' | ε
```

### 1.5 常量声明（消除左递归）
```
const_declarations → const const_declaration ; | ε
const_declaration → id = const_value const_declaration'
const_declaration' → ; id = const_value const_declaration' | ε
```

### 1.6 常量值
```
const_value → + num | - num | num | ' letter '
```

---

## 2. 变量与子程序声明相关

### 2.1 变量声明（消除左递归）
```
var_declarations → var var_declaration ; | ε
var_declaration → idlist : type var_declaration'
var_declaration' → ; idlist : type var_declaration' | ε
```

### 2.2 类型
```
type → basic_type | array [ period ] of basic_type
basic_type → integer | real | boolean | char
```

### 2.3 数组范围（消除左递归）
```
period → num .. num period'
period' → , num .. num period' | ε
```

### 2.4 子程序声明（消除左递归）
```
subprogram_declarations → subprogram_declarations' 
subprogram_declarations' → subprogram ; subprogram_declarations' | ε
```

### 2.5 子程序
```
subprogram → subprogram_head ; subprogram_body
```

### 2.6 子程序头
```
subprogram_head → procedure id formal_parameter 
                | function id formal_parameter : basic_type
```

### 2.7 形式参数
```
formal_parameter → ( parameter_list ) | ε
```

### 2.8 参数列表（消除左递归）
```
parameter_list → parameter parameter_list'
parameter_list' → ; parameter parameter_list' | ε
```

---

## 3. 子程序参数与复合语句相关

### 3.1 参数
```
parameter → var_parameter | value_parameter
var_parameter → var value_parameter
value_parameter → idlist : basic_type
```

### 3.2 子程序体
```
subprogram_body → const_declarations var_declarations compound_statement
```

### 3.3 复合语句
```
compound_statement → begin statement_list end
```

### 3.4 语句列表（消除左递归）
```
statement_list → statement statement_list'
statement_list' → ; statement statement_list' | ε
```

---

## 4. 语句与变量相关

### 4.1 语句（消除左公因子，合并 variable 和 func_id 的赋值）
```
statement → id statement_id_part
          | begin statement_list end
          | if expression then statement else_part
          | for id := expression to expression do statement
          | read ( variable_list )
          | write ( expression_list )
          | ε

statement_id_part → id_varpart := expression    (赋值语句，包括变量和函数返回值)
                  | ( expression_list )          (过程调用)
                  | ε                            (过程调用，无参数)
```

### 4.2 变量列表（消除左递归）
```
variable_list → variable variable_list'
variable_list' → , variable variable_list' | ε
```

### 4.3 变量
```
variable → id id_varpart
id_varpart → [ expression_list ] | ε
```

### 4.4 其他部分
```
else_part → else statement | ε
```

---

## 5. 过程调用与表达式相关

### 5.1 表达式列表（消除左递归）
```
expression_list → expression expression_list'
expression_list' → , expression expression_list' | ε
```

### 5.2 表达式
```
expression → simple_expression expression'
expression' → relop simple_expression | ε
```

### 5.3 简单表达式（消除左递归）
```
simple_expression → term simple_expression'
simple_expression' → addop term simple_expression' | ε
```

### 5.4 项（消除左递归）
```
term → factor term'
term' → mulop factor term' | ε
```

### 5.5 因子（消除左公因子）
```
factor → num
       | ( expression )
       | id factor_id_part
       | not factor
       | - factor

factor_id_part → [ expression_list ]      (数组变量)
               | ( expression_list )      (函数调用)
               | ε                        (简单变量)
```

---

## 文法改写说明

### 消除左递归的模式
对于形如 `A → Aα | β` 的左递归，改写为：
```
A → βA'
A' → αA' | ε
```

### 消除左公因子的模式
对于形如 `A → αβ | αγ` 的左公因子，改写为：
```
A → αA'
A' → β | γ
```

### 主要改动
1. **idlist, const_declaration, var_declaration, period, parameter_list, statement_list, variable_list, expression_list** - 消除直接左递归
2. **simple_expression, term** - 消除左递归（表达式优先级）
3. **program_head** - 消除左公因子 `program id`
4. **statement** - 合并 `variable assignop` 和 `func_id assignop`，统一为 `id statement_id_part`
5. **factor** - 消除 `variable` 和 `id(...)` 的左公因子
6. **subprogram_declarations** - 改写为右递归形式以消除左递归
