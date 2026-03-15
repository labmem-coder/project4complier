#include "parser.h"
#include <sstream>
#include <iostream> 

Parser::Parser(const std::vector<Token>& toks) : tokens(toks), pos(0), error_recovery_mode(false) {
    if (!tokens.empty()) {
        current_token = tokens[0];
    }
}

void Parser::advance() {
    pos++;
    if (pos < tokens.size()) {
        current_token = tokens[pos];
    } else {
        current_token = Token(TokenType::END_OF_FILE, "", 0, 0);
    }
}

Token Parser::peek(int offset) {
    size_t peek_pos = pos + offset;
    if (peek_pos < tokens.size()) {
        return tokens[peek_pos];
    }
    return Token(TokenType::END_OF_FILE, "", 0, 0);
}

Token Parser::expect(TokenType type) {
    if (current_token.type != type) {
        std::string expected_str = token_type_to_string(type);
        std::string message = "Expected " + expected_str;
        report_error(message, expected_str, "Skipped current token");
        
        // Skip the unexpected token and continue
        advance();
        
        // Return a dummy token to allow parsing to continue
        return Token(type, "", current_token.line, current_token.column);
    }
    Token tok = current_token;
    advance();
    return tok;
}

bool Parser::match(TokenType type) {
    return current_token.type == type;
}

bool Parser::match(TokenType type1, TokenType type2) {
    return current_token.type == type1 || current_token.type == type2;
}

bool Parser::match(TokenType type1, TokenType type2, TokenType type3) {
    return current_token.type == type1 || current_token.type == type2 || current_token.type == type3;
}

bool Parser::match(TokenType type1, TokenType type2, TokenType type3, TokenType type4) {
    return current_token.type == type1 || current_token.type == type2 || 
           current_token.type == type3 || current_token.type == type4;
}

std::string Parser::token_type_to_string(TokenType type) {
    switch (type) {
        case TokenType::PROGRAM: return "program";
        case TokenType::BEGIN: return "begin";
        case TokenType::END: return "end";
        case TokenType::CONST: return "const";
        case TokenType::VAR: return "var";
        case TokenType::PROCEDURE: return "procedure";
        case TokenType::FUNCTION: return "function";
        case TokenType::IF: return "if";
        case TokenType::THEN: return "then";
        case TokenType::ELSE: return "else";
        case TokenType::FOR: return "for";
        case TokenType::TO: return "to";
        case TokenType::DO: return "do";
        case TokenType::READ: return "read";
        case TokenType::WRITE: return "write";
        case TokenType::INTEGER: return "integer";
        case TokenType::REAL: return "real";
        case TokenType::BOOLEAN: return "boolean";
        case TokenType::CHAR: return "char";
        case TokenType::ARRAY: return "array";
        case TokenType::OF: return "of";
        case TokenType::ID: return "identifier";
        case TokenType::NUMBER: return "number";
        case TokenType::CHAR_LITERAL: return "character literal";
        case TokenType::PLUS: return "+";
        case TokenType::MINUS: return "-";
        case TokenType::MULTIPLY: return "*";
        case TokenType::DIVIDE: return "/";
        case TokenType::DIV: return "div";
        case TokenType::MOD: return "mod";
        case TokenType::EQ: return "=";
        case TokenType::NE: return "<>";
        case TokenType::LT: return "<";
        case TokenType::LE: return "<=";
        case TokenType::GT: return ">";
        case TokenType::GE: return ">=";
        case TokenType::ASSIGN: return ":=";
        case TokenType::SEMICOLON: return ";";
        case TokenType::COLON: return ":";
        case TokenType::COMMA: return ",";
        case TokenType::DOT: return ".";
        case TokenType::DOTDOT: return "..";
        case TokenType::LPAREN: return "(";
        case TokenType::RPAREN: return ")";
        case TokenType::LBRACKET: return "[";
        case TokenType::RBRACKET: return "]";
        case TokenType::AND: return "and";
        case TokenType::OR: return "or";
        case TokenType::NOT: return "not";
        case TokenType::END_OF_FILE: return "end of file";
        default: return "unknown";
    }
}

void Parser::report_error(const std::string& message, const std::string& expected, const std::string& recovery) {
    std::string found = current_token.value.empty() ? 
        token_type_to_string(current_token.type) : 
        "'" + current_token.value + "'";
    
    errors.emplace_back(
        current_token.line,
        current_token.column,
        message,
        expected,
        found,
        recovery
    );
    
    error_recovery_mode = true;
}

bool Parser::is_synch_token(const std::set<TokenType>& synch_set) {
    return synch_set.find(current_token.type) != synch_set.end();
}

void Parser::skip_to_synch(const std::set<TokenType>& synch_set) {
    while (current_token.type != TokenType::END_OF_FILE && 
           !is_synch_token(synch_set)) {
        advance();
    }
    error_recovery_mode = false;
}

void Parser::print_errors() const {
    if (errors.empty()) {
        return;
    }
    
    std::cerr << "\n=== Syntax Errors Found ===\n\n";
    for (size_t i = 0; i < errors.size(); i++) {
        const auto& err = errors[i];
        std::cerr << "Error " << (i + 1) << " at line " << err.line 
                  << ", column " << err.column << ":\n";
        std::cerr << "  Message: " << err.message << "\n";
        std::cerr << "  Expected: " << err.expected << "\n";
        std::cerr << "  Found: " << err.found << "\n";
        std::cerr << "  Recovery: " << err.recovery << "\n\n";
    }
    std::cerr << "Total errors: " << errors.size() << "\n";
}

std::shared_ptr<ProgramNode> Parser::parse() {
    return parse_program();
}

std::shared_ptr<ProgramNode> Parser::parse_program() {
    auto program = std::make_shared<ProgramNode>();
    expect(TokenType::PROGRAM);
    program->program_head = parse_program_head();
    expect(TokenType::SEMICOLON);
    program->program_body = parse_program_body();
    expect(TokenType::DOT);
    return program;
}

std::shared_ptr<ProgramHeadNode> Parser::parse_program_head() {
    std::string name = expect(TokenType::ID).value;
    auto head = std::make_shared<ProgramHeadNode>(name);
    
    if (match(TokenType::LPAREN)) {
        advance();
        head->parameters = parse_idlist();
        expect(TokenType::RPAREN);
    }
    
    return head;
}

std::vector<std::string> Parser::parse_idlist() {
    std::vector<std::string> ids;
    ids.push_back(expect(TokenType::ID).value);
    parse_idlist_prime(ids);
    return ids;
}

void Parser::parse_idlist_prime(std::vector<std::string>& ids) {
    if (match(TokenType::COMMA)) {
        advance();
        ids.push_back(expect(TokenType::ID).value);
        parse_idlist_prime(ids);
    }
}

std::shared_ptr<ProgramBodyNode> Parser::parse_program_body() {
    auto body = std::make_shared<ProgramBodyNode>();
    body->const_declarations = parse_const_declarations();
    body->var_declarations = parse_var_declarations();
    body->subprogram_declarations = parse_subprogram_declarations();
    body->compound_statement = parse_compound_statement();
    return body;
}

std::shared_ptr<ConstDeclarationsNode> Parser::parse_const_declarations() {
    if (!match(TokenType::CONST)) {
        return nullptr;
    }
    advance();
    
    auto decls = std::make_shared<ConstDeclarationsNode>();
    decls->declarations.push_back(parse_const_declaration());
    parse_const_declaration_prime(decls);
    expect(TokenType::SEMICOLON);
    
    return decls;
}

std::shared_ptr<ConstDeclarationNode> Parser::parse_const_declaration() {
    std::string name = expect(TokenType::ID).value;
    expect(TokenType::EQ);
    std::pair<std::string, std::string> const_val = parse_const_value();
    auto decl = std::make_shared<ConstDeclarationNode>(name, const_val.first, const_val.second);
    return decl;
}

void Parser::parse_const_declaration_prime(std::shared_ptr<ConstDeclarationsNode> decls) {
    if (match(TokenType::SEMICOLON)) {
        Token next = peek();
        if (next.type == TokenType::ID) {
            advance();
            decls->declarations.push_back(parse_const_declaration());
            parse_const_declaration_prime(decls);
        }
    }
}

std::pair<std::string, std::string> Parser::parse_const_value() {
    std::string sign = "";
    
    if (match(TokenType::PLUS, TokenType::MINUS)) {
        sign = current_token.value;
        advance();
    }
    
    if (match(TokenType::NUMBER)) {
        std::string value = current_token.value;
        advance();
        return {value, sign};
    } else if (match(TokenType::CHAR_LITERAL)) {
        std::string value = current_token.value;
        advance();
        return {value, sign};
    } else {
        throw ParseError("Expected number or character constant", current_token);
    }
}

std::shared_ptr<VarDeclarationsNode> Parser::parse_var_declarations() {
    if (!match(TokenType::VAR)) {
        return nullptr;
    }
    advance();
    
    auto decls = std::make_shared<VarDeclarationsNode>();
    auto var_decls = parse_var_declaration();
    decls->declarations.insert(decls->declarations.end(), var_decls.begin(), var_decls.end());
    expect(TokenType::SEMICOLON);
    
    return decls;
}

std::vector<std::shared_ptr<VarDeclarationNode>> Parser::parse_var_declaration() {
    std::vector<std::shared_ptr<VarDeclarationNode>> declarations;
    
    auto ids = parse_idlist();
    expect(TokenType::COLON);
    auto type = parse_type();
    
    auto decl = std::make_shared<VarDeclarationNode>();
    decl->identifiers = ids;
    decl->var_type = type;
    declarations.push_back(decl);
    
    parse_var_declaration_prime(declarations);
    return declarations;
}

void Parser::parse_var_declaration_prime(std::vector<std::shared_ptr<VarDeclarationNode>>& declarations) {
    if (match(TokenType::SEMICOLON)) {
        Token next = peek();
        if (next.type == TokenType::ID) {
            advance();
            auto ids = parse_idlist();
            expect(TokenType::COLON);
            auto type = parse_type();
            
            auto decl = std::make_shared<VarDeclarationNode>();
            decl->identifiers = ids;
            decl->var_type = type;
            declarations.push_back(decl);
            
            parse_var_declaration_prime(declarations);
        }
    }
}

std::shared_ptr<TypeNode> Parser::parse_type() {
    if (match(TokenType::ARRAY)) {
        advance();
        expect(TokenType::LBRACKET);
        auto periods = parse_period();
        expect(TokenType::RBRACKET);
        expect(TokenType::OF);
        std::string basic_type = parse_basic_type();
        
        auto type = std::make_shared<TypeNode>(basic_type, true);
        type->periods = periods;
        return type;
    } else {
        std::string basic_type = parse_basic_type();
        return std::make_shared<TypeNode>(basic_type, false);
    }
}

std::string Parser::parse_basic_type() {
    if (match(TokenType::INTEGER, TokenType::REAL, TokenType::BOOLEAN, TokenType::CHAR)) {
        std::string type = current_token.value;
        advance();
        return type;
    } else {
        throw ParseError("Expected basic type (integer, real, boolean, char)", current_token);
    }
}

std::vector<std::shared_ptr<PeriodNode>> Parser::parse_period() {
    std::vector<std::shared_ptr<PeriodNode>> periods;
    
    int start = std::stoi(expect(TokenType::NUMBER).value);
    expect(TokenType::DOTDOT);
    int end = std::stoi(expect(TokenType::NUMBER).value);
    periods.push_back(std::make_shared<PeriodNode>(start, end));
    
    parse_period_prime(periods);
    return periods;
}

void Parser::parse_period_prime(std::vector<std::shared_ptr<PeriodNode>>& periods) {
    if (match(TokenType::COMMA)) {
        advance();
        int start = std::stoi(expect(TokenType::NUMBER).value);
        expect(TokenType::DOTDOT);
        int end = std::stoi(expect(TokenType::NUMBER).value);
        periods.push_back(std::make_shared<PeriodNode>(start, end));
        parse_period_prime(periods);
    }
}

std::shared_ptr<SubprogramDeclarationsNode> Parser::parse_subprogram_declarations() {
    auto decls = std::make_shared<SubprogramDeclarationsNode>();
    parse_subprogram_declarations_prime(decls);
    
    if (decls->subprograms.empty()) {
        return nullptr;
    }
    return decls;
}

void Parser::parse_subprogram_declarations_prime(std::shared_ptr<SubprogramDeclarationsNode> decls) {
    if (match(TokenType::PROCEDURE, TokenType::FUNCTION)) {
        decls->subprograms.push_back(parse_subprogram());
        expect(TokenType::SEMICOLON);
        parse_subprogram_declarations_prime(decls);
    }
}

std::shared_ptr<SubprogramNode> Parser::parse_subprogram() {
    auto subprog = std::make_shared<SubprogramNode>();
    subprog->head = parse_subprogram_head();
    expect(TokenType::SEMICOLON);
    subprog->body = parse_subprogram_body();
    return subprog;
}

std::shared_ptr<SubprogramHeadNode> Parser::parse_subprogram_head() {
    bool is_function = false;
    
    if (match(TokenType::PROCEDURE)) {
        advance();
    } else if (match(TokenType::FUNCTION)) {
        advance();
        is_function = true;
    } else {
        throw ParseError("Expected PROCEDURE or FUNCTION", current_token);
    }
    
    std::string name = expect(TokenType::ID).value;
    auto head = std::make_shared<SubprogramHeadNode>(name, is_function);
    head->parameters = parse_formal_parameter();
    
    if (is_function) {
        expect(TokenType::COLON);
        head->return_type = parse_basic_type();
    }
    
    return head;
}

std::shared_ptr<ParameterListNode> Parser::parse_formal_parameter() {
    if (!match(TokenType::LPAREN)) {
        return nullptr;
    }
    advance();
    
    auto param_list = parse_parameter_list();
    expect(TokenType::RPAREN);
    return param_list;
}

std::shared_ptr<ParameterListNode> Parser::parse_parameter_list() {
    auto param_list = std::make_shared<ParameterListNode>();
    param_list->parameters.push_back(parse_parameter());
    parse_parameter_list_prime(param_list);
    return param_list;
}

void Parser::parse_parameter_list_prime(std::shared_ptr<ParameterListNode> param_list) {
    if (match(TokenType::SEMICOLON)) {
        advance();
        param_list->parameters.push_back(parse_parameter());
        parse_parameter_list_prime(param_list);
    }
}

std::shared_ptr<ParameterNode> Parser::parse_parameter() {
    auto param = std::make_shared<ParameterNode>();
    
    if (match(TokenType::VAR)) {
        param->is_var = true;
        advance();
    }
    
    param->identifiers = parse_idlist();
    expect(TokenType::COLON);
    param->param_type = parse_basic_type();
    
    return param;
}

std::shared_ptr<SubprogramBodyNode> Parser::parse_subprogram_body() {
    auto body = std::make_shared<SubprogramBodyNode>();
    body->const_declarations = parse_const_declarations();
    body->var_declarations = parse_var_declarations();
    body->compound_statement = parse_compound_statement();
    return body;
}

std::shared_ptr<CompoundStatementNode> Parser::parse_compound_statement() {
    auto compound = std::make_shared<CompoundStatementNode>();
    expect(TokenType::BEGIN);
    compound->statements = parse_statement_list();
    expect(TokenType::END);
    return compound;
}

std::vector<std::shared_ptr<StatementNode>> Parser::parse_statement_list() {
    std::vector<std::shared_ptr<StatementNode>> statements;
    statements.push_back(parse_statement());
    parse_statement_list_prime(statements);
    return statements;
}

void Parser::parse_statement_list_prime(std::vector<std::shared_ptr<StatementNode>>& statements) {
    if (match(TokenType::SEMICOLON)) {
        advance();
        if (!match(TokenType::END)) {
            statements.push_back(parse_statement());
            parse_statement_list_prime(statements);
        }
    }
}

std::shared_ptr<StatementNode> Parser::parse_statement() {
    if (match(TokenType::BEGIN)) {
        return parse_compound_statement();
    } else if (match(TokenType::IF)) {
        return parse_if_statement();
    } else if (match(TokenType::FOR)) {
        return parse_for_statement();
    } else if (match(TokenType::READ)) {
        return parse_read_statement();
    } else if (match(TokenType::WRITE)) {
        return parse_write_statement();
    } else if (match(TokenType::ID)) {
        return parse_assignment_or_call();
    } else {
        return std::make_shared<EmptyStatementNode>();
    }
}

std::shared_ptr<StatementNode> Parser::parse_assignment_or_call() {
    std::string id = expect(TokenType::ID).value;
    
    if (match(TokenType::ASSIGN)) {
        advance();
        auto assignment = std::make_shared<AssignmentNode>();
        assignment->target = std::make_shared<VariableNode>(id);
        assignment->expression = parse_expression();
        return assignment;
    } else if (match(TokenType::LBRACKET)) {
        auto var = std::make_shared<VariableNode>(id);
        advance();
        var->indices = parse_expression_list();
        expect(TokenType::RBRACKET);
        expect(TokenType::ASSIGN);
        auto assignment = std::make_shared<AssignmentNode>();
        assignment->target = var;
        assignment->expression = parse_expression();
        return assignment;
    } else if (match(TokenType::LPAREN)) {
        advance();
        auto call = std::make_shared<ProcedureCallNode>(id);
        if (!match(TokenType::RPAREN)) {
            call->arguments = parse_expression_list();
        }
        expect(TokenType::RPAREN);
        return call;
    } else {
        return std::make_shared<ProcedureCallNode>(id);
    }
}

std::shared_ptr<IfNode> Parser::parse_if_statement() {
    auto if_node = std::make_shared<IfNode>();
    expect(TokenType::IF);
    if_node->condition = parse_expression();
    expect(TokenType::THEN);
    if_node->then_statement = parse_statement();
    
    if (match(TokenType::ELSE)) {
        advance();
        if_node->else_statement = parse_statement();
    }
    
    return if_node;
}

std::shared_ptr<ForNode> Parser::parse_for_statement() {
    auto for_node = std::make_shared<ForNode>();
    expect(TokenType::FOR);
    for_node->variable = expect(TokenType::ID).value;
    expect(TokenType::ASSIGN);
    for_node->start_expr = parse_expression();
    expect(TokenType::TO);
    for_node->end_expr = parse_expression();
    expect(TokenType::DO);
    for_node->body = parse_statement();
    return for_node;
}

std::shared_ptr<ReadNode> Parser::parse_read_statement() {
    auto read_node = std::make_shared<ReadNode>();
    expect(TokenType::READ);
    expect(TokenType::LPAREN);
    read_node->variables = parse_variable_list();
    expect(TokenType::RPAREN);
    return read_node;
}

std::shared_ptr<WriteNode> Parser::parse_write_statement() {
    auto write_node = std::make_shared<WriteNode>();
    expect(TokenType::WRITE);
    expect(TokenType::LPAREN);
    write_node->expressions = parse_expression_list();
    expect(TokenType::RPAREN);
    return write_node;
}

std::vector<std::shared_ptr<VariableNode>> Parser::parse_variable_list() {
    std::vector<std::shared_ptr<VariableNode>> variables;
    variables.push_back(parse_variable());
    parse_variable_list_prime(variables);
    return variables;
}

void Parser::parse_variable_list_prime(std::vector<std::shared_ptr<VariableNode>>& variables) {
    if (match(TokenType::COMMA)) {
        advance();
        variables.push_back(parse_variable());
        parse_variable_list_prime(variables);
    }
}

std::shared_ptr<VariableNode> Parser::parse_variable() {
    std::string name = expect(TokenType::ID).value;
    auto var = std::make_shared<VariableNode>(name);
    
    if (match(TokenType::LBRACKET)) {
        advance();
        var->indices = parse_expression_list();
        expect(TokenType::RBRACKET);
    }
    
    return var;
}

std::vector<std::shared_ptr<ExpressionNode>> Parser::parse_expression_list() {
    std::vector<std::shared_ptr<ExpressionNode>> expressions;
    expressions.push_back(parse_expression());
    parse_expression_list_prime(expressions);
    return expressions;
}

void Parser::parse_expression_list_prime(std::vector<std::shared_ptr<ExpressionNode>>& expressions) {
    if (match(TokenType::COMMA)) {
        advance();
        expressions.push_back(parse_expression());
        parse_expression_list_prime(expressions);
    }
}

std::shared_ptr<ExpressionNode> Parser::parse_expression() {
    auto left = parse_simple_expression();
    return parse_expression_prime(left);
}

std::shared_ptr<ExpressionNode> Parser::parse_expression_prime(std::shared_ptr<ExpressionNode> left) {
    if (match(TokenType::EQ, TokenType::NE, TokenType::LT, TokenType::LE) ||
        match(TokenType::GT, TokenType::GE)) {
        std::string op = current_token.value;
        advance();
        auto right = parse_simple_expression();
        return std::make_shared<BinaryOpNode>(op, left, right);
    }
    return left;
}

std::shared_ptr<ExpressionNode> Parser::parse_simple_expression() {
    auto left = parse_term();
    return parse_simple_expression_prime(left);
}

std::shared_ptr<ExpressionNode> Parser::parse_simple_expression_prime(std::shared_ptr<ExpressionNode> left) {
    if (match(TokenType::PLUS, TokenType::MINUS, TokenType::OR)) {
        std::string op = current_token.value;
        advance();
        auto right = parse_term();
        auto new_left = std::make_shared<BinaryOpNode>(op, left, right);
        return parse_simple_expression_prime(new_left);
    }
    return left;
}

std::shared_ptr<ExpressionNode> Parser::parse_term() {
    auto left = parse_factor();
    return parse_term_prime(left);
}

std::shared_ptr<ExpressionNode> Parser::parse_term_prime(std::shared_ptr<ExpressionNode> left) {
    if (match(TokenType::MULTIPLY, TokenType::DIVIDE, TokenType::DIV, TokenType::MOD) ||
        match(TokenType::AND)) {
        std::string op = current_token.value;
        advance();
        auto right = parse_factor();
        auto new_left = std::make_shared<BinaryOpNode>(op, left, right);
        return parse_term_prime(new_left);
    }
    return left;
}

std::shared_ptr<ExpressionNode> Parser::parse_factor() {
    if (match(TokenType::NUMBER)) {
        std::string value = current_token.value;
        advance();
        return std::make_shared<NumberNode>(value);
    } else if (match(TokenType::ID)) {
        std::string name = current_token.value;
        advance();
        
        if (match(TokenType::LPAREN)) {
            advance();
            auto func_call = std::make_shared<FunctionCallNode>(name);
            if (!match(TokenType::RPAREN)) {
                func_call->arguments = parse_expression_list();
            }
            expect(TokenType::RPAREN);
            return func_call;
        } else if (match(TokenType::LBRACKET)) {
            advance();
            auto var = std::make_shared<VariableNode>(name);
            var->indices = parse_expression_list();
            expect(TokenType::RBRACKET);
            return var;
        } else {
            return std::make_shared<VariableNode>(name);
        }
    } else if (match(TokenType::LPAREN)) {
        advance();
        auto expr = parse_expression();
        expect(TokenType::RPAREN);
        return expr;
    } else if (match(TokenType::NOT)) {
        advance();
        auto operand = parse_factor();
        return std::make_shared<UnaryOpNode>("not", operand);
    } else if (match(TokenType::MINUS)) {
        advance();
        auto operand = parse_factor();
        return std::make_shared<UnaryOpNode>("-", operand);
    } else {
        throw ParseError("Unexpected token in factor", current_token);
    }
}
