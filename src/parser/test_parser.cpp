#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

void print_indent(int indent);

std::map<std::string, TokenType> token_type_map = {
    {"PROGRAM", TokenType::PROGRAM}, {"BEGIN", TokenType::BEGIN}, {"END", TokenType::END},
    {"CONST", TokenType::CONST}, {"VAR", TokenType::VAR},
    {"PROCEDURE", TokenType::PROCEDURE}, {"FUNCTION", TokenType::FUNCTION},
    {"IF", TokenType::IF}, {"THEN", TokenType::THEN}, {"ELSE", TokenType::ELSE},
    {"FOR", TokenType::FOR}, {"TO", TokenType::TO}, {"DO", TokenType::DO},
    {"READ", TokenType::READ}, {"WRITE", TokenType::WRITE},
    {"INTEGER", TokenType::INTEGER}, {"REAL", TokenType::REAL},
    {"BOOLEAN", TokenType::BOOLEAN}, {"CHAR", TokenType::CHAR},
    {"ARRAY", TokenType::ARRAY}, {"OF", TokenType::OF},
    {"ID", TokenType::ID}, {"NUMBER", TokenType::NUMBER}, {"CHAR_LITERAL", TokenType::CHAR_LITERAL},
    {"PLUS", TokenType::PLUS}, {"MINUS", TokenType::MINUS},
    {"MULTIPLY", TokenType::MULTIPLY}, {"DIVIDE", TokenType::DIVIDE},
    {"DIV", TokenType::DIV}, {"MOD", TokenType::MOD},
    {"EQ", TokenType::EQ}, {"NE", TokenType::NE},
    {"LT", TokenType::LT}, {"LE", TokenType::LE},
    {"GT", TokenType::GT}, {"GE", TokenType::GE},
    {"ASSIGN", TokenType::ASSIGN}, {"SEMICOLON", TokenType::SEMICOLON},
    {"COLON", TokenType::COLON}, {"COMMA", TokenType::COMMA},
    {"DOT", TokenType::DOT}, {"DOTDOT", TokenType::DOTDOT},
    {"LPAREN", TokenType::LPAREN}, {"RPAREN", TokenType::RPAREN},
    {"LBRACKET", TokenType::LBRACKET}, {"RBRACKET", TokenType::RBRACKET},
    {"AND", TokenType::AND}, {"OR", TokenType::OR}, {"NOT", TokenType::NOT},
    {"END_OF_FILE", TokenType::END_OF_FILE}
};

std::vector<Token> read_tokens_from_file(const std::string& filename) {
    std::vector<Token> tokens;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return tokens;
    }
    
    std::string line;
    int line_num = 0;
    
    while (std::getline(file, line)) {
        line_num++;
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        std::istringstream iss(line);
        std::string token_type_str, token_value;
        int line_pos = 0, col_pos = 0;
        
        if (iss >> token_type_str >> token_value >> line_pos >> col_pos) {
            auto it = token_type_map.find(token_type_str);
            if (it != token_type_map.end()) {
                tokens.push_back(Token(it->second, token_value, line_pos, col_pos));
            } else {
                std::cerr << "Warning: Unknown token type " << token_type_str << " at line " << line_num << std::endl;
            }
        }
    }
    
    file.close();
    
    if (tokens.empty() || tokens.back().type != TokenType::END_OF_FILE) {
        tokens.push_back(Token(TokenType::END_OF_FILE, "", line_num, 0));
    }
    
    return tokens;
}

void print_indent(int indent) {
    for (int i = 0; i < indent; i++) {
        std::cout << "  ";
    }
}

void print_expression(std::shared_ptr<ExpressionNode> expr, int indent);
void print_statement(std::shared_ptr<StatementNode> stmt, int indent);

void print_expression(std::shared_ptr<ExpressionNode> expr, int indent) {
    if (!expr) return;
    
    if (expr->type == ASTNodeType::NUMBER) {
        auto num = std::static_pointer_cast<NumberNode>(expr);
        print_indent(indent);
        std::cout << "Number: " << num->value << std::endl;
    } else if (expr->type == ASTNodeType::VARIABLE) {
        auto var = std::static_pointer_cast<VariableNode>(expr);
        print_indent(indent);
        std::cout << "Variable: " << var->name;
        if (!var->indices.empty()) {
            std::cout << " [" << var->indices.size() << " indices]";
        }
        std::cout << std::endl;
        for (auto& idx : var->indices) {
            print_expression(idx, indent + 1);
        }
    } else if (expr->type == ASTNodeType::BINARY_OP) {
        auto binop = std::static_pointer_cast<BinaryOpNode>(expr);
        print_indent(indent);
        std::cout << "BinaryOp: " << binop->op << std::endl;
        print_indent(indent + 1);
        std::cout << "Left:" << std::endl;
        print_expression(binop->left, indent + 2);
        print_indent(indent + 1);
        std::cout << "Right:" << std::endl;
        print_expression(binop->right, indent + 2);
    } else if (expr->type == ASTNodeType::UNARY_OP) {
        auto unop = std::static_pointer_cast<UnaryOpNode>(expr);
        print_indent(indent);
        std::cout << "UnaryOp: " << unop->op << std::endl;
        print_expression(unop->operand, indent + 1);
    } else if (expr->type == ASTNodeType::FUNCTION_CALL) {
        auto func = std::static_pointer_cast<FunctionCallNode>(expr);
        print_indent(indent);
        std::cout << "FunctionCall: " << func->name << " (" << func->arguments.size() << " args)" << std::endl;
        for (auto& arg : func->arguments) {
            print_expression(arg, indent + 1);
        }
    }
}

void print_statement(std::shared_ptr<StatementNode> stmt, int indent) {
    if (!stmt) return;
    
    if (stmt->type == ASTNodeType::ASSIGNMENT) {
        auto assign = std::static_pointer_cast<AssignmentNode>(stmt);
        print_indent(indent);
        std::cout << "Assignment:" << std::endl;
        print_indent(indent + 1);
        std::cout << "Target:" << std::endl;
        if (assign->target->type == ASTNodeType::VARIABLE) {
            print_expression(std::static_pointer_cast<ExpressionNode>(assign->target), indent + 2);
        } else {
            print_indent(indent + 2);
            std::cout << "(identifier)" << std::endl;
        }
        print_indent(indent + 1);
        std::cout << "Value:" << std::endl;
        print_expression(assign->expression, indent + 2);
    } else if (stmt->type == ASTNodeType::PROCEDURE_CALL) {
        auto call = std::static_pointer_cast<ProcedureCallNode>(stmt);
        print_indent(indent);
        std::cout << "ProcedureCall: " << call->name << " (" << call->arguments.size() << " args)" << std::endl;
        for (auto& arg : call->arguments) {
            print_expression(arg, indent + 1);
        }
    } else if (stmt->type == ASTNodeType::IF_STATEMENT) {
        auto if_stmt = std::static_pointer_cast<IfNode>(stmt);
        print_indent(indent);
        std::cout << "If:" << std::endl;
        print_indent(indent + 1);
        std::cout << "Condition:" << std::endl;
        print_expression(if_stmt->condition, indent + 2);
        print_indent(indent + 1);
        std::cout << "Then:" << std::endl;
        print_statement(if_stmt->then_statement, indent + 2);
        if (if_stmt->else_statement) {
            print_indent(indent + 1);
            std::cout << "Else:" << std::endl;
            print_statement(if_stmt->else_statement, indent + 2);
        }
    } else if (stmt->type == ASTNodeType::FOR_STATEMENT) {
        auto for_stmt = std::static_pointer_cast<ForNode>(stmt);
        print_indent(indent);
        std::cout << "For: " << for_stmt->variable << std::endl;
        print_indent(indent + 1);
        std::cout << "Start:" << std::endl;
        print_expression(for_stmt->start_expr, indent + 2);
        print_indent(indent + 1);
        std::cout << "End:" << std::endl;
        print_expression(for_stmt->end_expr, indent + 2);
        print_indent(indent + 1);
        std::cout << "Body:" << std::endl;
        print_statement(for_stmt->body, indent + 2);
    } else if (stmt->type == ASTNodeType::READ_STATEMENT) {
        auto read = std::static_pointer_cast<ReadNode>(stmt);
        print_indent(indent);
        std::cout << "Read: (" << read->variables.size() << " vars)" << std::endl;
        for (auto& var : read->variables) {
            print_expression(var, indent + 1);
        }
    } else if (stmt->type == ASTNodeType::WRITE_STATEMENT) {
        auto write = std::static_pointer_cast<WriteNode>(stmt);
        print_indent(indent);
        std::cout << "Write: (" << write->expressions.size() << " exprs)" << std::endl;
        for (auto& expr : write->expressions) {
            print_expression(expr, indent + 1);
        }
    } else if (stmt->type == ASTNodeType::COMPOUND_STATEMENT) {
        auto compound = std::static_pointer_cast<CompoundStatementNode>(stmt);
        print_indent(indent);
        std::cout << "CompoundStatement: (" << compound->statements.size() << " stmts)" << std::endl;
        for (auto& s : compound->statements) {
            print_statement(s, indent + 1);
        }
    } else if (stmt->type == ASTNodeType::EMPTY_STATEMENT) {
        print_indent(indent);
        std::cout << "EmptyStatement" << std::endl;
    }
}

void print_ast(std::shared_ptr<ProgramNode> node, int indent = 0) {
    print_indent(indent);
    std::cout << "+--- Program: " << node->program_head->name << std::endl;
    
    if (!node->program_head->parameters.empty()) {
        print_indent(indent);
        std::cout << "| Parameters: ";
        for (size_t i = 0; i < node->program_head->parameters.size(); i++) {
            std::cout << node->program_head->parameters[i];
            if (i < node->program_head->parameters.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
    
    if (node->program_body->const_declarations) {
        print_indent(indent);
        std::cout << "|" << std::endl;
        print_indent(indent);
        std::cout << "+--- Constants: (" << node->program_body->const_declarations->declarations.size() << ")" << std::endl;
        for (auto& decl : node->program_body->const_declarations->declarations) {
            print_indent(indent);
            std::cout << "|   " << decl->name << " = ";
            if (!decl->sign.empty()) std::cout << decl->sign;
            std::cout << decl->value << std::endl;
        }
    }
    
    if (node->program_body->var_declarations) {
        print_indent(indent);
        std::cout << "|" << std::endl;
        print_indent(indent);
        std::cout << "+--- Variables: (" << node->program_body->var_declarations->declarations.size() << ")" << std::endl;
        for (auto& decl : node->program_body->var_declarations->declarations) {
            print_indent(indent);
            std::cout << "|   ";
            for (size_t i = 0; i < decl->identifiers.size(); i++) {
                std::cout << decl->identifiers[i];
                if (i < decl->identifiers.size() - 1) std::cout << ", ";
            }
            std::cout << " : " << decl->var_type->basic_type;
            if (decl->var_type->is_array) {
                std::cout << " [array]";
            }
            std::cout << std::endl;
        }
    }
    
    if (node->program_body->subprogram_declarations) {
        print_indent(indent);
        std::cout << "|" << std::endl;
        print_indent(indent);
        std::cout << "+--- Subprograms: (" << node->program_body->subprogram_declarations->subprograms.size() << ")" << std::endl;
        for (auto& subprog : node->program_body->subprogram_declarations->subprograms) {
            print_indent(indent);
            std::cout << "|   " << (subprog->head->is_function ? "Function" : "Procedure") << ": " << subprog->head->name;
            if (subprog->head->is_function) {
                std::cout << " : " << subprog->head->return_type;
            }
            std::cout << std::endl;
            if (subprog->head->parameters) {
                print_indent(indent);
                std::cout << "|     Parameters: " << subprog->head->parameters->parameters.size() << std::endl;
            }
        }
    }
    
    print_indent(indent);
    std::cout << "|" << std::endl;
    print_indent(indent);
    std::cout << "+--- Main Body:" << std::endl;
    print_indent(indent);
    std::cout << "|" << std::endl;
    
    for (auto& stmt : node->program_body->compound_statement->statements) {
        print_statement(stmt, indent + 1);
    }
    
    print_indent(indent);
    std::cout << "+--- End Program" << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "=== Pascal-S Parser Test ===" << std::endl << std::endl;
    
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <token_file>" << std::endl;
        std::cout << "Token file format: Each line contains: TOKEN_TYPE VALUE LINE COLUMN" << std::endl;
        std::cout << "Example: ID x 1 5" << std::endl;
        return 1;
    }
    
    std::string filename = argv[1];
    std::cout << "Reading tokens from: " << filename << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    
    try {
        auto tokens = read_tokens_from_file(filename);
        
        if (tokens.empty()) {
            std::cout << "✗ No tokens read from file" << std::endl;
            return 1;
        }
        
        std::cout << "Read " << tokens.size() << " tokens" << std::endl << std::endl;
        
        Parser parser(tokens);
        auto ast = parser.parse();
        
        std::cout << std::endl << "=== Abstract Syntax Tree ===" << std::endl << std::endl;
        print_ast(ast, 0);
        std::cout << std::endl << "✓ Parse successful!" << std::endl;
        
    } catch (const ParseError& e) {
        std::cout << "✗ Parse error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cout << "✗ Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
