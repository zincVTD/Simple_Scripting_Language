#ifndef __LEXER_H__
#define __LEXER_H__

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Token
{
private:
    string type;
    string value;

public:
    Token(string type, string value);
    string getType() const;
    string getValue() const;
};

class Lexer
{
private:
    string input;
    size_t pos;
    char currentChar;

    void advance();
    void skipWhitespace();
    string collectNumber();
    string collectOperator();
    string collectIdentifier();

public:
    Lexer(string input);
    vector<Token *> tokenize();
};

class Integer : public Token
{
public:
    Integer(string value);
};

class Float : public Token
{
public:
    Float(string value);
};

class Operator : public Token
{
public:
    Operator(string value);
};

class Identifier : public Token
{
public:
    Identifier(string value);
};

class TypeDecl : public Token
{
public:
    TypeDecl(string value);
};

#endif