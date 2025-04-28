#include "Parsing/parser.hpp"

// NumberNode implementation
NumberNode::NumberNode(const string &value) : value(value) {}

void NumberNode::print() const
{
    cout << value;
}

string NumberNode::getValue() const
{
    return value;
}

// OperatorNode implementation
OperatorNode::OperatorNode(const string &op, shared_ptr<ASTNode> left, shared_ptr<ASTNode> right)
    : op(op), left(left), right(right) {}

void OperatorNode::print() const
{
    cout << "(";
    left->print();
    cout << " " << op << " ";
    right->print();
    cout << ")";
}

const string &OperatorNode::getOp() const
{
    return op;
}

const shared_ptr<ASTNode> &OperatorNode::getLeft() const
{
    return left;
}

const shared_ptr<ASTNode> &OperatorNode::getRight() const
{
    return right;
}

IdentifierNode::IdentifierNode(const string &name) : name(name) {}

void IdentifierNode::print() const
{
    cout << name;
}

string IdentifierNode::getName() const
{
    return name;
}

TypeDeclNode::TypeDeclNode(const string &type) : type(type) {}

void TypeDeclNode::print() const
{
    cout << "TypeDecl(" << type << ")";
}

string TypeDeclNode::getType() const
{
    return type;
}

// Parser implementation
Parser::Parser(const vector<Token *> &tokens) : tokens(tokens), currentIndex(0) {}

Token *Parser::currentToken()
{
    if (currentIndex < tokens.size())
    {
        return tokens[currentIndex];
    }
    return nullptr;
}

void Parser::advance()
{
    if (currentIndex < tokens.size())
    {
        currentIndex++;
    }
}

shared_ptr<ASTNode> Parser::parse()
{
    if (currentToken() && currentToken()->getType() == "Id")
    {
        return parseAssignment(); // Có thể là gán hoặc chỉ truy cập biến
    }

    return parseExpression();
}

shared_ptr<ASTNode> Parser::parseExpression()
{
    auto node = parseTerm();

    while (currentToken() && (currentToken()->getValue() == "+" || currentToken()->getValue() == "-"))
    {
        string op = currentToken()->getValue();
        advance();
        node = make_shared<OperatorNode>(op, node, parseTerm());
    }

    return node;
}

shared_ptr<ASTNode> Parser::parseTerm()
{
    auto node = parseFactor();

    while (currentToken() && (currentToken()->getValue() == "*" || currentToken()->getValue() == "/"))
    {
        string op = currentToken()->getValue();
        advance();
        node = make_shared<OperatorNode>(op, node, parseFactor());
    }

    return node;
}

shared_ptr<ASTNode> Parser::parseFactor()
{
    if (!currentToken())
    {
        throw runtime_error("Unexpected end of input");
    }

    if (currentToken()->getType() == "Int")
    {
        string value = currentToken()->getValue();

        // Xử lý hệ nhị phân
        if (value[0] == 'b')
        {
            value = value.substr(1);                    // Bỏ tiền tố 'b'
            int decimalValue = stoi(value, nullptr, 2); // Chuyển đổi sang thập phân
            advance();
            return make_shared<NumberNode>(to_string(decimalValue));
        }
        // Xử lý hệ 16
        else if (value[0] == 'h')
        {
            value = value.substr(1);                     // Bỏ tiền tố 'h'
            int decimalValue = stoi(value, nullptr, 16); // Chuyển đổi sang thập phân
            advance();
            return make_shared<NumberNode>(to_string(decimalValue));
        }
        else
        {
            // Xử lý số thập phân thông thường
            advance();
            return make_shared<NumberNode>(value);
        }
    }
    else if (currentToken()->getType() == "Id")
    {
        auto node = make_shared<IdentifierNode>(currentToken()->getValue());
        advance();
        return node;
    }
    else if (currentToken()->getType() == "Tpd")
    {
        string type = currentToken()->getValue();
        advance();

        if (currentToken() && currentToken()->getType() == "Id")
        {
            string variableName = currentToken()->getValue();
            advance();

            if (currentToken() && currentToken()->getValue() == "=")
            {
                advance(); // Skip '='
                auto value = parseExpression();
                return make_shared<OperatorNode>(
                    "declare",
                    make_shared<TypeDeclNode>(type),
                    make_shared<OperatorNode>("=", make_shared<IdentifierNode>(variableName), value));
            }

            return make_shared<OperatorNode>(
                "declare",
                make_shared<TypeDeclNode>(type),
                make_shared<IdentifierNode>(variableName));
        }
        else
        {
            throw runtime_error("Expected identifier after type declaration");
        }
    }
    else if (currentToken()->getValue() == "(")
    {
        advance();
        auto node = parseExpression();
        if (currentToken() && currentToken()->getValue() == ")")
        {
            advance();
            return node;
        }
        else
        {
            throw runtime_error("Expected ')'");
        }
    }
    else
    {
        throw runtime_error("Unexpected token: " + currentToken()->getValue());
    }
}

shared_ptr<ASTNode> Parser::parseAssignment()
{
    if (currentToken()->getType() == "Id")
    {
        string variableName = currentToken()->getValue();
        advance();

        if (currentToken() && currentToken()->getValue() == "=")
        {
            advance(); // Bỏ qua dấu "="
            auto value = parseExpression();
            return make_shared<OperatorNode>("=", make_shared<IdentifierNode>(variableName), value);
        }
        else
        {
            // Nếu không có '=', trả về IdentifierNode
            return make_shared<IdentifierNode>(variableName);
        }
    }

    throw runtime_error("Invalid assignment syntax");
}
