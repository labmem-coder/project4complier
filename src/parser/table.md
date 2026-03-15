# Pascal-S LL(1) 预测分析表

本文档包含消除左递归和左公因子后的 Pascal-S 文法的 FIRST 集、FOLLOW 集和预测分析表。

---

## 一、FIRST 集

### 非终结符 FIRST 集

| 非终结符                 | FIRST 集                                    |
| ------------------------ | ------------------------------------------- |
| programstruct            | {program}                                   |
| program_head             | {program}                                   |
| program_head'            | {(, ε}                                      |
| program_body             | {const, var, procedure, function, begin, ε} |
| idlist                   | {id}                                        |
| idlist'                  | {',', ε}                                    |
| const_declarations       | {const, ε}                                  |
| const_declaration        | {id}                                        |
| const_declaration'       | {;, ε}                                      |
| const_value              | {+, -, num, '}                              |
| var_declarations         | {var, ε}                                    |
| var_declaration          | {id}                                        |
| var_declaration'         | {;, ε}                                      |
| type                     | {integer, real, boolean, char, array}       |
| basic_type               | {integer, real, boolean, char}              |
| period                   | {num}                                       |
| period'                  | {',', ε}                                    |
| subprogram_declarations  | {procedure, function, ε}                    |
| subprogram_declarations' | {procedure, function, ε}                    |
| subprogram               | {procedure, function}                       |
| subprogram_head          | {procedure, function}                       |
| formal_parameter         | {(, ε}                                      |
| parameter_list           | {id, var}                                   |
| parameter_list'          | {;, ε}                                      |
| parameter                | {id, var}                                   |
| var_parameter            | {var}                                       |
| value_parameter          | {id}                                        |
| subprogram_body          | {const, var, begin}                         |
| compound_statement       | {begin}                                     |
| statement_list           | {id, begin, if, for, read, write, ε}        |
| statement_list'          | {;, ε}                                      |
| statement                | {id, begin, if, for, read, write, ε}        |
| statement_id_part        | {[, (, :=, ε}                               |
| variable_list            | {id}                                        |
| variable_list'           | {',', ε}                                    |
| variable                 | {id}                                        |
| id_varpart               | {[, ε}                                      |
| else_part                | {else, ε}                                   |
| expression_list          | {num, (, id, not, -}                        |
| expression_list'         | {',', ε}                                    |
| expression               | {num, (, id, not, -}                        |
| expression'              | {=, <>, <, <=, >, >=, ε}                    |
| simple_expression        | {num, (, id, not, -}                        |
| simple_expression'       | {+, -, or, ε}                               |
| term                     | {num, (, id, not, -}                        |
| term'                    | {\*, /, div, mod, and, ε}                   |
| factor                   | {num, (, id, not, -}                        |
| factor_id_part           | {[, (, ε}                                   |

---

## 二、FOLLOW 集

### 非终结符 FOLLOW 集

| 非终结符                 | FOLLOW 集                                                                                        |
| ------------------------ | ------------------------------------------------------------------------------------------------ |
| programstruct            | {$}                                                                                              |
| program_head             | {;}                                                                                              |
| program_head'            | {;}                                                                                              |
| program_body             | {.}                                                                                              |
| idlist                   | {), :}                                                                                           |
| idlist'                  | {), :}                                                                                           |
| const_declarations       | {var, procedure, function, begin}                                                                |
| const_declaration        | {;}                                                                                              |
| const_declaration'       | {var, procedure, function, begin}                                                                |
| const_value              | {;}                                                                                              |
| var_declarations         | {procedure, function, begin}                                                                     |
| var_declaration          | {;}                                                                                              |
| var_declaration'         | {procedure, function, begin}                                                                     |
| type                     | {;}                                                                                              |
| basic_type               | {;, )}                                                                                           |
| period                   | {]}                                                                                              |
| period'                  | {]}                                                                                              |
| subprogram_declarations  | {begin}                                                                                          |
| subprogram_declarations' | {begin}                                                                                          |
| subprogram               | {;}                                                                                              |
| subprogram_head          | {;}                                                                                              |
| formal_parameter         | {;, :}                                                                                           |
| parameter_list           | {)}                                                                                              |
| parameter_list'          | {)}                                                                                              |
| parameter                | {;, )}                                                                                           |
| var_parameter            | {;, )}                                                                                           |
| value_parameter          | {;, )}                                                                                           |
| subprogram_body          | {;}                                                                                              |
| compound_statement       | {;, end, else}                                                                                   |
| statement_list           | {end}                                                                                            |
| statement_list'          | {end}                                                                                            |
| statement                | {;, end, else}                                                                                   |
| statement_id_part        | {;, end, else}                                                                                   |
| variable_list            | {)}                                                                                              |
| variable_list'           | {)}                                                                                              |
| variable                 | {',', )}                                                                                         |
| id_varpart               | {:=, ',', ), ;, end, else, =, <>, <, <=, >, >=, +, -, or, \*, /, div, mod, and, ], to, do, then} |
| else_part                | {;, end}                                                                                         |
| expression_list          | {), ]}                                                                                           |
| expression_list'         | {), ]}                                                                                           |
| expression               | {), ], ',', ;, end, else, to, do, then}                                                          |
| expression'              | {), ], ',', ;, end, else, to, do, then}                                                          |
| simple_expression        | {), ], ',', ;, end, else, to, do, then, =, <>, <, <=, >, >=}                                     |
| simple_expression'       | {), ], ',', ;, end, else, to, do, then, =, <>, <, <=, >, >=}                                     |
| term                     | {), ], ',', ;, end, else, to, do, then, =, <>, <, <=, >, >=, +, -, or}                           |
| term'                    | {), ], ',', ;, end, else, to, do, then, =, <>, <, <=, >, >=, +, -, or}                           |
| factor                   | {), ], ',', ;, end, else, to, do, then, =, <>, <, <=, >, >=, +, -, or, \*, /, div, mod, and}     |
| factor_id_part           | {), ], ',', ;, end, else, to, do, then, =, <>, <, <=, >, >=, +, -, or, \*, /, div, mod, and}     |

---

## 三、预测分析表

### 说明

- 表格行：非终结符
- 表格列：终结符（输入符号）
- 单元格内容：产生式编号或产生式右部
- 空白单元格表示语法错误

### 3.1 程序结构相关

| 非终结符      | program                       | id  | ;   | .   | (          | )   | const                                                                          | var                                                                            | begin                                                                          | end | ... |
| ------------- | ----------------------------- | --- | --- | --- | ---------- | --- | ------------------------------------------------------------------------------ | ------------------------------------------------------------------------------ | ------------------------------------------------------------------------------ | --- | --- |
| programstruct | program_head ; program_body . |     |     |     |            |     |                                                                                |                                                                                |                                                                                |     |     |
| program_head  | program id program_head'      |     |     |     |            |     |                                                                                |                                                                                |                                                                                |     |     |
| program_head' |                               |     |     |     | ( idlist ) | ε   |                                                                                |                                                                                |                                                                                |     |     |
| program_body  |                               |     |     |     |            |     | const_declarations var_declarations subprogram_declarations compound_statement | const_declarations var_declarations subprogram_declarations compound_statement | const_declarations var_declarations subprogram_declarations compound_statement |     |     |

### 3.2 完整预测分析表（主要产生式）

由于表格过大，以下按非终结符分组列出关键预测分析表项：

#### idlist 和 idlist'

- M[idlist, id] = id idlist'
- M[idlist', ,] = , id idlist'
- M[idlist', )] = ε
- M[idlist', :] = ε

#### const_declarations 和 const_declaration

- M[const_declarations, const] = const const_declaration ;
- M[const_declarations, var] = ε
- M[const_declarations, procedure] = ε
- M[const_declarations, function] = ε
- M[const_declarations, begin] = ε
- M[const_declaration, id] = id = const_value const_declaration'
- M[const_declaration', ;] = ; id = const_value const_declaration'
- M[const_declaration', var/procedure/function/begin] = ε

#### const_value

- M[const_value, +] = + num
- M[const_value, -] = - num
- M[const_value, num] = num
- M[const_value, '] = ' letter '

#### var_declarations 和 var_declaration

- M[var_declarations, var] = var var_declaration ;
- M[var_declarations, procedure] = ε
- M[var_declarations, function] = ε
- M[var_declarations, begin] = ε
- M[var_declaration, id] = idlist : type var_declaration'
- M[var_declaration', ;] = ; idlist : type var_declaration'
- M[var_declaration', procedure/function/begin] = ε

#### type 和 basic_type

- M[type, integer] = basic_type
- M[type, real] = basic_type
- M[type, boolean] = basic_type
- M[type, char] = basic_type
- M[type, array] = array [ period ] of basic_type
- M[basic_type, integer] = integer
- M[basic_type, real] = real
- M[basic_type, boolean] = boolean
- M[basic_type, char] = char

#### period 和 period'

- M[period, num] = num .. num period'
- M[period', ,] = , num .. num period'
- M[period', ]] = ε

#### subprogram_declarations 和 subprogram_declarations'

- M[subprogram_declarations, procedure] = subprogram_declarations'
- M[subprogram_declarations, function] = subprogram_declarations'
- M[subprogram_declarations, begin] = subprogram_declarations'
- M[subprogram_declarations', procedure] = subprogram ; subprogram_declarations'
- M[subprogram_declarations', function] = subprogram ; subprogram_declarations'
- M[subprogram_declarations', begin] = ε

#### subprogram 和 subprogram_head

- M[subprogram, procedure] = subprogram_head ; subprogram_body
- M[subprogram, function] = subprogram_head ; subprogram_body
- M[subprogram_head, procedure] = procedure id formal_parameter
- M[subprogram_head, function] = function id formal_parameter : basic_type

#### formal_parameter

- M[formal_parameter, (] = ( parameter_list )
- M[formal_parameter, ;] = ε
- M[formal_parameter, :] = ε

#### parameter_list 和 parameter_list'

- M[parameter_list, id] = parameter parameter_list'
- M[parameter_list, var] = parameter parameter_list'
- M[parameter_list', ;] = ; parameter parameter_list'
- M[parameter_list', )] = ε

#### parameter, var_parameter, value_parameter

- M[parameter, var] = var_parameter
- M[parameter, id] = value_parameter
- M[var_parameter, var] = var value_parameter
- M[value_parameter, id] = idlist : basic_type

#### subprogram_body

- M[subprogram_body, const] = const_declarations var_declarations compound_statement
- M[subprogram_body, var] = const_declarations var_declarations compound_statement
- M[subprogram_body, begin] = const_declarations var_declarations compound_statement

#### compound_statement

- M[compound_statement, begin] = begin statement_list end

#### statement_list 和 statement_list'

- M[statement_list, id] = statement statement_list'
- M[statement_list, begin] = statement statement_list'
- M[statement_list, if] = statement statement_list'
- M[statement_list, for] = statement statement_list'
- M[statement_list, read] = statement statement_list'
- M[statement_list, write] = statement statement_list'
- M[statement_list, end] = statement statement_list'
- M[statement_list', ;] = ; statement statement_list'
- M[statement_list', end] = ε

#### statement 和 statement_id_part

- M[statement, id] = id statement_id_part
- M[statement, begin] = begin statement_list end
- M[statement, if] = if expression then statement else_part
- M[statement, for] = for id := expression to expression do statement
- M[statement, read] = read ( variable_list )
- M[statement, write] = write ( expression_list )
- M[statement, end] = ε
- M[statement, ;] = ε
- M[statement_id_part, [] = id_varpart := expression
- M[statement_id_part, :=] = id_varpart := expression
- M[statement_id_part, (] = ( expression_list )
- M[statement_id_part, ;] = ε
- M[statement_id_part, end] = ε
- M[statement_id_part, else] = ε

#### variable_list 和 variable_list'

- M[variable_list, id] = variable variable_list'
- M[variable_list', ,] = , variable variable_list'
- M[variable_list', )] = ε

#### variable 和 id_varpart

- M[variable, id] = id id_varpart
- M[id_varpart, [] = [ expression_list ]
- M[id_varpart, 其他] = ε (FOLLOW集中的符号)

#### else_part

- M[else_part, else] = else statement
- M[else_part, ;] = ε
- M[else_part, end] = ε

#### expression_list 和 expression_list'

- M[expression_list, num] = expression expression_list'
- M[expression_list, (] = expression expression_list'
- M[expression_list, id] = expression expression_list'
- M[expression_list, not] = expression expression_list'
- M[expression_list, -] = expression expression_list'
- M[expression_list', ,] = , expression expression_list'
- M[expression_list', )] = ε
- M[expression_list', ]] = ε

#### expression 和 expression'

- M[expression, num/(/id/not/-] = simple_expression expression'
- M[expression', =] = relop simple_expression
- M[expression', <>] = relop simple_expression
- M[expression', <] = relop simple_expression
- M[expression', <=] = relop simple_expression
- M[expression', >] = relop simple_expression
- M[expression', >=] = relop simple_expression
- M[expression', 其他] = ε (FOLLOW集中的符号)

#### simple_expression 和 simple_expression'

- M[simple_expression, num/(/id/not/-] = term simple_expression'
- M[simple_expression', +] = addop term simple_expression'
- M[simple_expression', -] = addop term simple_expression'
- M[simple_expression', or] = addop term simple_expression'
- M[simple_expression', 其他] = ε (FOLLOW集中的符号)

#### term 和 term'

- M[term, num/(/id/not/-] = factor term'
- M[term', *] = mulop factor term'
- M[term', /] = mulop factor term'
- M[term', div] = mulop factor term'
- M[term', mod] = mulop factor term'
- M[term', and] = mulop factor term'
- M[term', 其他] = ε (FOLLOW集中的符号)

#### factor 和 factor_id_part

- M[factor, num] = num
- M[factor, (] = ( expression )
- M[factor, id] = id factor_id_part
- M[factor, not] = not factor
- M[factor, -] = - factor
- M[factor_id_part, [] = [ expression_list ]
- M[factor_id_part, (] = ( expression_list )
- M[factor_id_part, 其他] = ε (FOLLOW集中的符号)

---

## 四、LL(1) 文法验证

### 验证条件

对于每个非终结符 A 的每对不同产生式 A → α | β，需满足：

1. FIRST(α) ∩ FIRST(β) = ∅
2. 若 ε ∈ FIRST(α)，则 FIRST(β) ∩ FOLLOW(A) = ∅

### 验证结果

经过消除左递归和左公因子后，该文法满足 LL(1) 条件：

- ✓ 所有产生式的 FIRST 集互不相交
- ✓ 含 ε 产生式的非终结符，其 FIRST 集与 FOLLOW 集不相交
- ✓ 预测分析表中每个单元格最多只有一个产生式

**结论：该文法是 LL(1) 文法，可以使用预测分析方法进行语法分析。**

---

## 五、带同步化信息的预测分析表

### 5.1 错误处理说明

**错误处理策略：**

1. 若 `M[A, a]` 为**空**，则跳过输入符号 `a`，继续读取下一个符号
2. 若 `M[A, a]` 为 **synch**，则弹出栈顶非终结符 `A`，继续分析

**同步符号选择原则：**

- 对于非终结符 `A`，将其 `FOLLOW(A)` 集中的符号标记为 `synch`
- 某些重要的起始符号也标记为 `synch`，以便快速恢复

### 5.2 主要非终结符的同步化分析表

#### 5.2.1 程序结构相关

**programstruct**

- FOLLOW(programstruct) = {$}

| 输入符号 | M[programstruct, a]           |
| -------- | ----------------------------- |
| program  | program_head ; program_body . |
| $        | synch                         |

**program_head**

- FOLLOW(program_head) = {;}

| 输入符号 | M[program_head, a]       |
| -------- | ------------------------ |
| program  | program id program_head' |
| ;        | synch                    |

**program_body**

- FOLLOW(program_body) = {.}

| 输入符号  | M[program_body, a]                                                             |
| --------- | ------------------------------------------------------------------------------ |
| const     | const_declarations var_declarations subprogram_declarations compound_statement |
| var       | const_declarations var_declarations subprogram_declarations compound_statement |
| procedure | const_declarations var_declarations subprogram_declarations compound_statement |
| function  | const_declarations var_declarations subprogram_declarations compound_statement |
| begin     | const_declarations var_declarations subprogram_declarations compound_statement |
| .         | synch                                                                          |

**idlist 和 idlist'**

- FOLLOW(idlist) = {), :}
- FOLLOW(idlist') = {), :}

| 输入符号 | M[idlist, a] | M[idlist', a] |
| -------- | ------------ | ------------- |
| id       | id idlist'   |               |
| ,        |              | , id idlist'  |
| )        | synch        | ε, synch      |
| :        | synch        | ε, synch      |

#### 5.2.2 声明相关

**const_declarations**

- FOLLOW(const_declarations) = {var, procedure, function, begin}

| 输入符号  | M[const_declarations, a]  |
| --------- | ------------------------- |
| const     | const const_declaration ; |
| var       | ε, synch                  |
| procedure | ε, synch                  |
| function  | ε, synch                  |
| begin     | ε, synch                  |

**const_declaration 和 const_declaration'**

- FOLLOW(const_declaration) = {;}
- FOLLOW(const_declaration') = {var, procedure, function, begin}

| 输入符号  | M[const_declaration, a]             | M[const_declaration', a]              |
| --------- | ----------------------------------- | ------------------------------------- |
| id        | id = const_value const_declaration' |                                       |
| ;         | synch                               | ; id = const_value const_declaration' |
| var       |                                     | ε, synch                              |
| procedure |                                     | ε, synch                              |
| function  |                                     | ε, synch                              |
| begin     |                                     | ε, synch                              |

**var_declarations**

- FOLLOW(var_declarations) = {procedure, function, begin}

| 输入符号  | M[var_declarations, a] |
| --------- | ---------------------- |
| var       | var var_declaration ;  |
| procedure | ε, synch               |
| function  | ε, synch               |
| begin     | ε, synch               |

**var_declaration 和 var_declaration'**

- FOLLOW(var_declaration) = {;}
- FOLLOW(var_declaration') = {procedure, function, begin}

| 输入符号  | M[var_declaration, a]          | M[var_declaration', a]           |
| --------- | ------------------------------ | -------------------------------- |
| id        | idlist : type var_declaration' |                                  |
| ;         | synch                          | ; idlist : type var_declaration' |
| procedure |                                | ε, synch                         |
| function  |                                | ε, synch                         |
| begin     |                                | ε, synch                         |

**type 和 basic_type**

- FOLLOW(type) = {;}
- FOLLOW(basic_type) = {;, )}

| 输入符号 | M[type, a]                     | M[basic_type, a] |
| -------- | ------------------------------ | ---------------- |
| integer  | basic_type                     | integer          |
| real     | basic_type                     | real             |
| boolean  | basic_type                     | boolean          |
| char     | basic_type                     | char             |
| array    | array [ period ] of basic_type |                  |
| ;        | synch                          | synch            |
| )        |                                | synch            |

#### 5.2.3 子程序相关

**subprogram_declarations 和 subprogram_declarations'**

- FOLLOW(subprogram_declarations) = {begin}
- FOLLOW(subprogram_declarations') = {begin}

| 输入符号  | M[subprogram_declarations, a]   | M[subprogram_declarations', a]        |
| --------- | ------------------------------- | ------------------------------------- |
| procedure | subprogram_declarations'        | subprogram ; subprogram_declarations' |
| function  | subprogram_declarations'        | subprogram ; subprogram_declarations' |
| begin     | subprogram_declarations', synch | ε, synch                              |

**parameter_list 和 parameter_list'**

- FOLLOW(parameter_list) = {)}
- FOLLOW(parameter_list') = {)}

| 输入符号 | M[parameter_list, a]      | M[parameter_list', a]       |
| -------- | ------------------------- | --------------------------- |
| id       | parameter parameter_list' |                             |
| var      | parameter parameter_list' |                             |
| ;        |                           | ; parameter parameter_list' |
| )        | synch                     | ε, synch                    |

#### 5.2.4 语句相关

**compound_statement**

- FOLLOW(compound_statement) = {;, end, else}

| 输入符号 | M[compound_statement, a] |
| -------- | ------------------------ |
| begin    | begin statement_list end |
| ;        | synch                    |
| end      | synch                    |
| else     | synch                    |

**statement_list 和 statement_list'**

- FOLLOW(statement_list) = {end}
- FOLLOW(statement_list') = {end}

| 输入符号 | M[statement_list, a]             | M[statement_list', a]       |
| -------- | -------------------------------- | --------------------------- |
| id       | statement statement_list'        |                             |
| begin    | statement statement_list'        |                             |
| if       | statement statement_list'        |                             |
| for      | statement statement_list'        |                             |
| read     | statement statement_list'        |                             |
| write    | statement statement_list'        |                             |
| ;        |                                  | ; statement statement_list' |
| end      | statement statement_list', synch | ε, synch                    |

**statement**

- FOLLOW(statement) = {;, end, else}

| 输入符号 | M[statement, a]                                 |
| -------- | ----------------------------------------------- |
| id       | id statement_id_part                            |
| begin    | compound_statement                              |
| if       | if expression then statement else_part          |
| for      | for id := expression to expression do statement |
| read     | read ( variable_list )                          |
| write    | write ( expression_list )                       |
| ;        | ε, synch                                        |
| end      | ε, synch                                        |
| else     | ε, synch                                        |

**variable_list 和 variable_list'**

- FOLLOW(variable_list) = {)}
- FOLLOW(variable_list') = {)}

| 输入符号 | M[variable_list, a]     | M[variable_list', a]      |
| -------- | ----------------------- | ------------------------- |
| id       | variable variable_list' |                           |
| ,        |                         | , variable variable_list' |
| )        | synch                   | ε, synch                  |

#### 5.2.5 表达式相关

**expression_list 和 expression_list'**

- FOLLOW(expression_list) = {), ]}
- FOLLOW(expression_list') = {), ]}

| 输入符号 | M[expression_list, a]       | M[expression_list', a]        |
| -------- | --------------------------- | ----------------------------- |
| num      | expression expression_list' |                               |
| (        | expression expression_list' |                               |
| id       | expression expression_list' |                               |
| not      | expression expression_list' |                               |
| -        | expression expression_list' |                               |
| ,        |                             | , expression expression_list' |
| )        | synch                       | ε, synch                      |
| ]        | synch                       | ε, synch                      |

**expression 和 expression'**

- FOLLOW(expression) = {), ], ',', ;, end, else, to, do, then}
- FOLLOW(expression') = {), ], ',', ;, end, else, to, do, then}

| 输入符号 | M[expression, a]              | M[expression', a]       |
| -------- | ----------------------------- | ----------------------- |
| num      | simple_expression expression' |                         |
| (        | simple_expression expression' |                         |
| id       | simple_expression expression' |                         |
| not      | simple_expression expression' |                         |
| -        | simple_expression expression' |                         |
| =        |                               | relop simple_expression |
| <>       |                               | relop simple_expression |
| <        |                               | relop simple_expression |
| <=       |                               | relop simple_expression |
| >        |                               | relop simple_expression |
| >=       |                               | relop simple_expression |
| )        | synch                         | ε, synch                |
| ]        | synch                         | ε, synch                |
| ,        | synch                         | ε, synch                |
| ;        | synch                         | ε, synch                |
| end      | synch                         | ε, synch                |
| else     | synch                         | ε, synch                |
| to       | synch                         | ε, synch                |
| do       | synch                         | ε, synch                |
| then     | synch                         | ε, synch                |

**simple_expression 和 simple_expression'**

- FOLLOW(simple_expression) = {), ], ',', ;, end, else, to, do, then, =, <>, <, <=, >, >=}
- FOLLOW(simple_expression') = {), ], ',', ;, end, else, to, do, then, =, <>, <, <=, >, >=}

| 输入符号 | M[simple_expression, a] | M[simple_expression', a]      |
| -------- | ----------------------- | ----------------------------- |
| num      | term simple_expression' |                               |
| (        | term simple_expression' |                               |
| id       | term simple_expression' |                               |
| not      | term simple_expression' |                               |
| -        | term simple_expression' |                               |
| +        |                         | addop term simple_expression' |
| -        |                         | addop term simple_expression' |
| or       |                         | addop term simple_expression' |
| =        | synch                   | ε, synch                      |
| <>       | synch                   | ε, synch                      |
| <        | synch                   | ε, synch                      |
| <=       | synch                   | ε, synch                      |
| >        | synch                   | ε, synch                      |
| >=       | synch                   | ε, synch                      |
| )        | synch                   | ε, synch                      |
| ]        | synch                   | ε, synch                      |
| ,        | synch                   | ε, synch                      |
| ;        | synch                   | ε, synch                      |
| end      | synch                   | ε, synch                      |
| else     | synch                   | ε, synch                      |
| to       | synch                   | ε, synch                      |
| do       | synch                   | ε, synch                      |
| then     | synch                   | ε, synch                      |

**term 和 term'**

- FOLLOW(term) = {), ], ',', ;, end, else, to, do, then, =, <>, <, <=, >, >=, +, -, or}
- FOLLOW(term') = {), ], ',', ;, end, else, to, do, then, =, <>, <, <=, >, >=, +, -, or}

| 输入符号 | M[term, a]   | M[term', a]        |
| -------- | ------------ | ------------------ |
| num      | factor term' |                    |
| (        | factor term' |                    |
| id       | factor term' |                    |
| not      | factor term' |                    |
| -        | factor term' |                    |
| \*       |              | mulop factor term' |
| /        |              | mulop factor term' |
| div      |              | mulop factor term' |
| mod      |              | mulop factor term' |
| and      |              | mulop factor term' |
| +        | synch        | ε, synch           |
| -        | synch        | ε, synch           |
| or       | synch        | ε, synch           |
| =        | synch        | ε, synch           |
| <>       | synch        | ε, synch           |
| <        | synch        | ε, synch           |
| <=       | synch        | ε, synch           |
| >        | synch        | ε, synch           |
| >=       | synch        | ε, synch           |
| )        | synch        | ε, synch           |
| ]        | synch        | ε, synch           |
| ,        | synch        | ε, synch           |
| ;        | synch        | ε, synch           |
| end      | synch        | ε, synch           |
| else     | synch        | ε, synch           |
| to       | synch        | ε, synch           |
| do       | synch        | ε, synch           |
| then     | synch        | ε, synch           |

**factor**

- FOLLOW(factor) = {), ], ',', ;, end, else, to, do, then, =, <>, <, <=, >, >=, +, -, or, \*, /, div, mod, and}

| 输入符号 | M[factor, a]      |
| -------- | ----------------- |
| num      | num               |
| (        | ( expression )    |
| id       | id factor_id_part |
| not      | not factor        |
| -        | - factor          |
| \*       | synch             |
| /        | synch             |
| div      | synch             |
| mod      | synch             |
| and      | synch             |
| +        | synch             |
| -        | synch             |
| or       | synch             |
| =        | synch             |
| <>       | synch             |
| <        | synch             |
| <=       | synch             |
| >        | synch             |
| >=       | synch             |
| )        | synch             |
| ]        | synch             |
| ,        | synch             |
| ;        | synch             |
| end      | synch             |
| else     | synch             |
| to       | synch             |
| do       | synch             |
| then     | synch             |

### 5.3 错误恢复示例

**示例 1：缺少分号**

```
输入：program Test var x: integer begin end.
栈：  [program_head, ;, program_body, .]
当前输入：var
```

- M[program_head, var] 为空 → 报告错误"期望 ';'"
- 跳过 var，继续分析

**示例 2：多余的符号**

```
输入：begin x := 1 + + 2 end
栈：  [term']
当前输入：+（第二个）
```

- M[term', +] = synch → 弹出 term'
- 继续分析

**示例 3：类型错误**

```
输入：var x: unknown;
栈：  [type]
当前输入：unknown
```

- M[type, unknown] 为空 → 报告错误"期望类型名"
- 跳过 unknown，继续分析

---

## 六、错误处理实现要点

### 6.1 错误检测

1. 在每个 `expect()` 调用时检测 token 类型不匹配
2. 在每个 `match()` 调用时检测是否命中 synch 标记
3. 记录错误位置（行号、列号）和上下文信息

### 6.2 错误恢复

1. **跳过策略**：遇到空表项时，跳过当前输入符号
2. **弹出策略**：遇到 synch 时，弹出栈顶非终结符
3. **继续分析**：恢复后继续分析，尽可能发现更多错误

### 6.3 错误报告

错误报告应包含：

- 错误类型（缺少符号、多余符号、类型不匹配等）
- 错误位置（行号、列号）
- 期望的符号
- 实际的符号
- 恢复操作（跳过/弹出）

**示例错误报告格式：**

```
Error at line 5, column 10:
  Expected: ';'
  Found: 'var'
  Recovery: Skipped 'var'
  Context: After program declaration
```
