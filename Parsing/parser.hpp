#ifndef __PARSER_H__
#define __PARSER_H__

#include "Lexer/lexer.hpp"
#include <stdexcept>
#include <memory>

class ASTNode
{
public:
    virtual ~ASTNode() = default;
    virtual void print() const = 0; // Pure virtual function to print the AST
};

class NumberNode : public ASTNode
{
private:
    string value;

public:
    NumberNode(const string &value);
    void print() const override;
    string getValue() const;
};

class OperatorNode : public ASTNode
{
private:
    string op;
    shared_ptr<ASTNode> left;
    shared_ptr<ASTNode> right;

public:
    OperatorNode(const string &op, shared_ptr<ASTNode> left, shared_ptr<ASTNode> right);
    void print() const override;
    const string &getOp() const;
    const shared_ptr<ASTNode> &getLeft() const;
    const shared_ptr<ASTNode> &getRight() const;
};

class IdentifierNode : public ASTNode
{
private:
    string name;

public:
    IdentifierNode(const string &name);
    void print() const override;
    string getName() const;
};

class TypeDeclNode : public ASTNode
{
private:
    string type;

public:
    TypeDeclNode(const string &type);
    void print() const override;
    string getType() const;
};

class Parser
{
private:
    vector<Token *> tokens;
    size_t currentIndex;

    Token *currentToken();
    void advance();

    shared_ptr<ASTNode> parseExpression();
    shared_ptr<ASTNode> parseTerm();
    shared_ptr<ASTNode> parseFactor();
    shared_ptr<ASTNode> parseAssignment();

public:
    Parser(const vector<Token *> &tokens);
    shared_ptr<ASTNode> parse();
};

#endif
