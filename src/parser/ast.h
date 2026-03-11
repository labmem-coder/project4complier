#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>

enum class ASTNodeType {
    PROGRAM,
    PROGRAM_HEAD,
    PROGRAM_BODY,
    CONST_DECLARATIONS,
    CONST_DECLARATION,
    VAR_DECLARATIONS,
    VAR_DECLARATION,
    TYPE_NODE,
    PERIOD,
    SUBPROGRAM_DECLARATIONS,
    SUBPROGRAM,
    SUBPROGRAM_HEAD,
    PARAMETER_LIST,
    PARAMETER,
    SUBPROGRAM_BODY,
    COMPOUND_STATEMENT,
    ASSIGNMENT,
    PROCEDURE_CALL,
    IF_STATEMENT,
    FOR_STATEMENT,
    READ_STATEMENT,
    WRITE_STATEMENT,
    EMPTY_STATEMENT,
    VARIABLE,
    BINARY_OP,
    UNARY_OP,
    NUMBER,
    FUNCTION_CALL
};

class ASTNode {
public:
    ASTNodeType type;
    virtual ~ASTNode() = default;
    
protected:
    ASTNode(ASTNodeType t) : type(t) {}
};

class ProgramHeadNode : public ASTNode {
public:
    std::string name;
    std::vector<std::string> parameters;
    
    ProgramHeadNode(const std::string& n) 
        : ASTNode(ASTNodeType::PROGRAM_HEAD), name(n) {}
};

class ConstDeclarationNode : public ASTNode {
public:
    std::string name;
    std::string value;
    std::string sign;
    
    ConstDeclarationNode(const std::string& n, const std::string& v, const std::string& s = "")
        : ASTNode(ASTNodeType::CONST_DECLARATION), name(n), value(v), sign(s) {}
};

class ConstDeclarationsNode : public ASTNode {
public:
    std::vector<std::shared_ptr<ConstDeclarationNode>> declarations;
    
    ConstDeclarationsNode() : ASTNode(ASTNodeType::CONST_DECLARATIONS) {}
};

class PeriodNode : public ASTNode {
public:
    int start;
    int end;
    
    PeriodNode(int s, int e) : ASTNode(ASTNodeType::PERIOD), start(s), end(e) {}
};

class TypeNode : public ASTNode {
public:
    std::string basic_type;
    bool is_array;
    std::vector<std::shared_ptr<PeriodNode>> periods;
    
    TypeNode(const std::string& bt, bool arr = false)
        : ASTNode(ASTNodeType::TYPE_NODE), basic_type(bt), is_array(arr) {}
};

class VarDeclarationNode : public ASTNode {
public:
    std::vector<std::string> identifiers;
    std::shared_ptr<TypeNode> var_type;
    
    VarDeclarationNode() : ASTNode(ASTNodeType::VAR_DECLARATION) {}
};

class VarDeclarationsNode : public ASTNode {
public:
    std::vector<std::shared_ptr<VarDeclarationNode>> declarations;
    
    VarDeclarationsNode() : ASTNode(ASTNodeType::VAR_DECLARATIONS) {}
};

class ExpressionNode : public ASTNode {
public:
    ExpressionNode(ASTNodeType t) : ASTNode(t) {}
};

class NumberNode : public ExpressionNode {
public:
    std::string value;
    
    NumberNode(const std::string& v) : ExpressionNode(ASTNodeType::NUMBER), value(v) {}
};

class VariableNode : public ExpressionNode {
public:
    std::string name;
    std::vector<std::shared_ptr<ExpressionNode>> indices;
    
    VariableNode(const std::string& n) : ExpressionNode(ASTNodeType::VARIABLE), name(n) {}
};

class BinaryOpNode : public ExpressionNode {
public:
    std::string op;
    std::shared_ptr<ExpressionNode> left;
    std::shared_ptr<ExpressionNode> right;
    
    BinaryOpNode(const std::string& o, std::shared_ptr<ExpressionNode> l, std::shared_ptr<ExpressionNode> r)
        : ExpressionNode(ASTNodeType::BINARY_OP), op(o), left(l), right(r) {}
};

class UnaryOpNode : public ExpressionNode {
public:
    std::string op;
    std::shared_ptr<ExpressionNode> operand;
    
    UnaryOpNode(const std::string& o, std::shared_ptr<ExpressionNode> opnd)
        : ExpressionNode(ASTNodeType::UNARY_OP), op(o), operand(opnd) {}
};

class FunctionCallNode : public ExpressionNode {
public:
    std::string name;
    std::vector<std::shared_ptr<ExpressionNode>> arguments;
    
    FunctionCallNode(const std::string& n) : ExpressionNode(ASTNodeType::FUNCTION_CALL), name(n) {}
};

class StatementNode : public ASTNode {
public:
    StatementNode(ASTNodeType t) : ASTNode(t) {}
};

class EmptyStatementNode : public StatementNode {
public:
    EmptyStatementNode() : StatementNode(ASTNodeType::EMPTY_STATEMENT) {}
};

class CompoundStatementNode : public StatementNode {
public:
    std::vector<std::shared_ptr<StatementNode>> statements;
    
    CompoundStatementNode() : StatementNode(ASTNodeType::COMPOUND_STATEMENT) {}
};

class AssignmentNode : public StatementNode {
public:
    std::shared_ptr<ASTNode> target;
    std::shared_ptr<ExpressionNode> expression;
    
    AssignmentNode() : StatementNode(ASTNodeType::ASSIGNMENT) {}
};

class ProcedureCallNode : public StatementNode {
public:
    std::string name;
    std::vector<std::shared_ptr<ExpressionNode>> arguments;
    
    ProcedureCallNode(const std::string& n) : StatementNode(ASTNodeType::PROCEDURE_CALL), name(n) {}
};

class IfNode : public StatementNode {
public:
    std::shared_ptr<ExpressionNode> condition;
    std::shared_ptr<StatementNode> then_statement;
    std::shared_ptr<StatementNode> else_statement;
    
    IfNode() : StatementNode(ASTNodeType::IF_STATEMENT) {}
};

class ForNode : public StatementNode {
public:
    std::string variable;
    std::shared_ptr<ExpressionNode> start_expr;
    std::shared_ptr<ExpressionNode> end_expr;
    std::shared_ptr<StatementNode> body;
    
    ForNode() : StatementNode(ASTNodeType::FOR_STATEMENT) {}
};

class ReadNode : public StatementNode {
public:
    std::vector<std::shared_ptr<VariableNode>> variables;
    
    ReadNode() : StatementNode(ASTNodeType::READ_STATEMENT) {}
};

class WriteNode : public StatementNode {
public:
    std::vector<std::shared_ptr<ExpressionNode>> expressions;
    
    WriteNode() : StatementNode(ASTNodeType::WRITE_STATEMENT) {}
};

class ParameterNode : public ASTNode {
public:
    std::vector<std::string> identifiers;
    std::string param_type;
    bool is_var;
    
    ParameterNode() : ASTNode(ASTNodeType::PARAMETER), is_var(false) {}
};

class ParameterListNode : public ASTNode {
public:
    std::vector<std::shared_ptr<ParameterNode>> parameters;
    
    ParameterListNode() : ASTNode(ASTNodeType::PARAMETER_LIST) {}
};

class SubprogramHeadNode : public ASTNode {
public:
    std::string name;
    bool is_function;
    std::shared_ptr<ParameterListNode> parameters;
    std::string return_type;
    
    SubprogramHeadNode(const std::string& n, bool is_func)
        : ASTNode(ASTNodeType::SUBPROGRAM_HEAD), name(n), is_function(is_func) {}
};

class SubprogramBodyNode : public ASTNode {
public:
    std::shared_ptr<ConstDeclarationsNode> const_declarations;
    std::shared_ptr<VarDeclarationsNode> var_declarations;
    std::shared_ptr<CompoundStatementNode> compound_statement;
    
    SubprogramBodyNode() : ASTNode(ASTNodeType::SUBPROGRAM_BODY) {}
};

class SubprogramNode : public ASTNode {
public:
    std::shared_ptr<SubprogramHeadNode> head;
    std::shared_ptr<SubprogramBodyNode> body;
    
    SubprogramNode() : ASTNode(ASTNodeType::SUBPROGRAM) {}
};

class SubprogramDeclarationsNode : public ASTNode {
public:
    std::vector<std::shared_ptr<SubprogramNode>> subprograms;
    
    SubprogramDeclarationsNode() : ASTNode(ASTNodeType::SUBPROGRAM_DECLARATIONS) {}
};

class ProgramBodyNode : public ASTNode {
public:
    std::shared_ptr<ConstDeclarationsNode> const_declarations;
    std::shared_ptr<VarDeclarationsNode> var_declarations;
    std::shared_ptr<SubprogramDeclarationsNode> subprogram_declarations;
    std::shared_ptr<CompoundStatementNode> compound_statement;
    
    ProgramBodyNode() : ASTNode(ASTNodeType::PROGRAM_BODY) {}
};

class ProgramNode : public ASTNode {
public:
    std::shared_ptr<ProgramHeadNode> program_head;
    std::shared_ptr<ProgramBodyNode> program_body;
    
    ProgramNode() : ASTNode(ASTNodeType::PROGRAM) {}
};

#endif
