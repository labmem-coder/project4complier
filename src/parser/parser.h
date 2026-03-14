#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include <string>
#include <vector>
#include <stdexcept>

enum class TokenType {
    PROGRAM, BEGIN, END, CONST, VAR, PROCEDURE, FUNCTION,
    IF, THEN, ELSE, FOR, TO, DO, READ, WRITE,
    INTEGER, REAL, BOOLEAN, CHAR, ARRAY, OF,
    ID, NUMBER, CHAR_LITERAL,
    PLUS, MINUS, MULTIPLY, DIVIDE, DIV, MOD,
    EQ, NE, LT, LE, GT, GE,
    ASSIGN, SEMICOLON, COLON, COMMA, DOT, DOTDOT,
    LPAREN, RPAREN, LBRACKET, RBRACKET,
    AND, OR, NOT,
    END_OF_FILE,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
    
    Token(TokenType t = TokenType::UNKNOWN, const std::string& v = "", int l = 0, int c = 0)
        : type(t), value(v), line(l), column(c) {}
};

class ParseError : public std::runtime_error {
public:
    Token token;
    
    ParseError(const std::string& msg, const Token& tok)
        : std::runtime_error(msg), token(tok) {}
};

class Parser {
private:
    std::vector<Token> tokens;
    size_t pos;
    Token current_token;
    
    void advance();
    Token peek(int offset = 1);
    Token expect(TokenType type);
    bool match(TokenType type);
    bool match(TokenType type1, TokenType type2);
    bool match(TokenType type1, TokenType type2, TokenType type3);
    bool match(TokenType type1, TokenType type2, TokenType type3, TokenType type4);
    
    std::shared_ptr<ProgramNode> parse_program();
    std::shared_ptr<ProgramHeadNode> parse_program_head();
    std::vector<std::string> parse_idlist();
    void parse_idlist_prime(std::vector<std::string>& ids);
    std::shared_ptr<ProgramBodyNode> parse_program_body();
    
    std::shared_ptr<ConstDeclarationsNode> parse_const_declarations();
    std::shared_ptr<ConstDeclarationNode> parse_const_declaration();
    void parse_const_declaration_prime(std::shared_ptr<ConstDeclarationsNode> decls);
    std::pair<std::string, std::string> parse_const_value();
    
    std::shared_ptr<VarDeclarationsNode> parse_var_declarations();
    std::vector<std::shared_ptr<VarDeclarationNode>> parse_var_declaration();
    void parse_var_declaration_prime(std::vector<std::shared_ptr<VarDeclarationNode>>& declarations);
    std::shared_ptr<TypeNode> parse_type();
    std::string parse_basic_type();
    std::vector<std::shared_ptr<PeriodNode>> parse_period();
    void parse_period_prime(std::vector<std::shared_ptr<PeriodNode>>& periods);
    
    std::shared_ptr<SubprogramDeclarationsNode> parse_subprogram_declarations();
    void parse_subprogram_declarations_prime(std::shared_ptr<SubprogramDeclarationsNode> decls);
    std::shared_ptr<SubprogramNode> parse_subprogram();
    std::shared_ptr<SubprogramHeadNode> parse_subprogram_head();
    std::shared_ptr<ParameterListNode> parse_formal_parameter();
    std::shared_ptr<ParameterListNode> parse_parameter_list();
    void parse_parameter_list_prime(std::shared_ptr<ParameterListNode> param_list);
    std::shared_ptr<ParameterNode> parse_parameter();
    std::shared_ptr<SubprogramBodyNode> parse_subprogram_body();
    
    std::shared_ptr<CompoundStatementNode> parse_compound_statement();
    std::vector<std::shared_ptr<StatementNode>> parse_statement_list();
    void parse_statement_list_prime(std::vector<std::shared_ptr<StatementNode>>& statements);
    std::shared_ptr<StatementNode> parse_statement();
    std::shared_ptr<StatementNode> parse_assignment_or_call();
    std::shared_ptr<IfNode> parse_if_statement();
    std::shared_ptr<ForNode> parse_for_statement();
    std::shared_ptr<ReadNode> parse_read_statement();
    std::shared_ptr<WriteNode> parse_write_statement();
    
    std::vector<std::shared_ptr<VariableNode>> parse_variable_list();
    void parse_variable_list_prime(std::vector<std::shared_ptr<VariableNode>>& variables);
    std::shared_ptr<VariableNode> parse_variable();
    std::vector<std::shared_ptr<ExpressionNode>> parse_expression_list();
    void parse_expression_list_prime(std::vector<std::shared_ptr<ExpressionNode>>& expressions);
    
    std::shared_ptr<ExpressionNode> parse_expression();
    std::shared_ptr<ExpressionNode> parse_expression_prime(std::shared_ptr<ExpressionNode> left);
    std::shared_ptr<ExpressionNode> parse_simple_expression();
    std::shared_ptr<ExpressionNode> parse_simple_expression_prime(std::shared_ptr<ExpressionNode> left);
    std::shared_ptr<ExpressionNode> parse_term();
    std::shared_ptr<ExpressionNode> parse_term_prime(std::shared_ptr<ExpressionNode> left);
    std::shared_ptr<ExpressionNode> parse_factor();
    
public:
    Parser(const std::vector<Token>& toks);
    std::shared_ptr<ProgramNode> parse();
};

#endif
