#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "Parsing/parser.hpp"
#include <unordered_map>
#include <limits>

class Interpreter
{
public:
    Interpreter();
    double interpret(const shared_ptr<ASTNode> &root);

private:
    double visit(const shared_ptr<ASTNode> &node);
    double visitNumberNode(const shared_ptr<NumberNode> &node);
    double visitOperatorNode(const shared_ptr<OperatorNode> &node);
    double visitIdentifierNode(const shared_ptr<IdentifierNode> &node);

    // Bảng lưu giá trị các biến đã khai báo. string là kiểu, double là giá trị
    unordered_map<string, pair<string, double>> symbolTable;
};

#endif
