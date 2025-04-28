#include "Interpreter/interpreter.hpp"
#include <iostream>

// Constructor
Interpreter::Interpreter() {}

// Interpret the AST and return the result
double Interpreter::interpret(const shared_ptr<ASTNode> &root)
{
    return visit(root);
}

// General visit method
double Interpreter::visit(const shared_ptr<ASTNode> &node)
{
    if (auto numberNode = dynamic_pointer_cast<NumberNode>(node))
    {
        return visitNumberNode(numberNode);
    }
    else if (auto operatorNode = dynamic_pointer_cast<OperatorNode>(node))
    {
        return visitOperatorNode(operatorNode);
    }
    else if (auto identifierNode = dynamic_pointer_cast<IdentifierNode>(node))
    {
        return visitIdentifierNode(identifierNode);
    }
    else
    {
        throw runtime_error("Unknown node type");
    }
}

// Visit NumberNode and return its value
double Interpreter::visitNumberNode(const shared_ptr<NumberNode> &node)
{
    return stod(node->getValue()); // Giá trị đã chuyển sang thập phân trong Parser
}

// Visit IdentifierNode and return its value
double Interpreter::visitIdentifierNode(const shared_ptr<IdentifierNode> &node)
{
    const string &name = node->getName();

    // Kiểm tra biến đã khai báo
    if (symbolTable.find(name) == symbolTable.end())
    {
        throw runtime_error("Variable '" + name + "' is not declared");
    }

    // Trả về giá trị của biến
    return symbolTable[name].second;
}

// Visit OperatorNode and evaluate the operation
double Interpreter::visitOperatorNode(const shared_ptr<OperatorNode> &node)
{
    const string &op = node->getOp();

    if (op == "declare")
    {
        auto typeNode = dynamic_pointer_cast<TypeDeclNode>(node->getLeft());
        auto assignNode = dynamic_pointer_cast<OperatorNode>(node->getRight());

        if (!typeNode)
        {
            throw runtime_error("Invalid declaration syntax");
        }

        string type = typeNode->getType();
        string variableName;
        double initialValue = 0;

        if (assignNode && assignNode->getOp() == "=")
        {
            auto identifierNode = dynamic_pointer_cast<IdentifierNode>(assignNode->getLeft());
            if (!identifierNode)
            {
                throw runtime_error("Left-hand side of '=' must be a variable");
            }

            variableName = identifierNode->getName();
            initialValue = visit(assignNode->getRight());
        }
        else
        {
            auto identifierNode = dynamic_pointer_cast<IdentifierNode>(node->getRight());
            if (!identifierNode)
            {
                throw runtime_error("Invalid declaration syntax");
            }

            variableName = identifierNode->getName();
        }

        if (symbolTable.find(variableName) != symbolTable.end())
        {
            throw runtime_error("Variable '" + variableName + "' is already declared");
        }

        // Lưu vào symbolTable
        if (type == "8b")
        {
            symbolTable[variableName] = {"8b", static_cast<unsigned char>(initialValue)};
        }
        else if (type == "16b")
        {
            symbolTable[variableName] = {"16b", static_cast<unsigned short>(initialValue)};
        }
        else if (type == "32b")
        {
            symbolTable[variableName] = {"32b", static_cast<unsigned int>(initialValue)};
        }
        else
        {
            throw runtime_error("Unknown type: " + type);
        }

        cout << "Variable '" << variableName << "' declared as " << type << " with value " << initialValue << endl;
        return initialValue;
    }

    if (op == "=")
    {
        auto identifierNode = dynamic_pointer_cast<IdentifierNode>(node->getLeft());
        if (!identifierNode)
        {
            throw runtime_error("Left-hand side of '=' must be a variable");
        }

        const string &variableName = identifierNode->getName();
        double value = visit(node->getRight());

        // Ensure the variable is declared before assignment
        if (symbolTable.find(variableName) == symbolTable.end())
        {
            throw runtime_error("Variable '" + variableName + "' is not declared");
        }

        // Ensure the value fits the variable's type
        const string &type = symbolTable[variableName].first; // Lấy kiểu dữ liệu từ symbolTable
        if (type == "8b" && value <= numeric_limits<unsigned char>::max())
        {
            symbolTable[variableName].second = static_cast<unsigned char>(value); // Gán giá trị
        }
        else if (type == "16b" && value <= numeric_limits<unsigned short>::max())
        {
            symbolTable[variableName].second = static_cast<unsigned short>(value); // Gán giá trị
        }
        else if (type == "32b" && value <= numeric_limits<unsigned int>::max())
        {
            symbolTable[variableName].second = static_cast<unsigned int>(value); // Gán giá trị
        }
        else
        {
            throw runtime_error("Value out of range for the declared type");
        }

        return value;
    }

    // Other operators: +, -, *, /
    double left = visit(node->getLeft());
    double right = visit(node->getRight());

    if (op == "+")
    {
        return left + right;
    }
    else if (op == "-")
    {
        return left - right;
    }
    else if (op == "*")
    {
        return left * right;
    }
    else if (op == "/")
    {
        if (right == 0)
        {
            throw runtime_error("Division by zero");
        }
        return left / right;
    }
    else
    {
        throw runtime_error("Unknown operator: " + op);
    }
}
