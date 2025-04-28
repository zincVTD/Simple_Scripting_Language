#include "Lexer/lexer.hpp"

// Token class implementation
Token::Token(string type, string value) : type(type), value(value) {}

string Token::getType() const
{
    return type;
}

string Token::getValue() const
{
    return value;
}

// Integer class implementation
Integer::Integer(string value) : Token("Int", value) {}

// Float class implementation
Float::Float(string value) : Token("Flt", value) {}

// Operator class implementation
Operator::Operator(string value) : Token("Op", value) {}

// Identifier class implementation
Identifier::Identifier(string value) : Token("Id", value) {}

// Type declaration class implementation
TypeDecl::TypeDecl(string value) : Token("Tpd", value) {}

// Lexer class implementation
Lexer::Lexer(string input) : input(input), pos(0)
{
    currentChar = input[pos];
}

// Advance to the next character in the input string
void Lexer::advance()
{
    pos++;
    if (pos < input.length())
    {
        currentChar = input[pos];
    }
    else
    {
        currentChar = '\0'; // End of input
    }
}

// Skip whitespace characters
void Lexer::skipWhitespace()
{
    while (isspace(currentChar))
    {
        advance();
    }
}

// Collect a number (Integer or Float) from the input string
string Lexer::collectNumber()
{
    string result;

    // Nhận diện hệ nhị phân (b...) hoặc hệ 16 (h...)
    if (currentChar == 'b' || currentChar == 'h')
    {
        char prefix = currentChar;
        result += prefix; // Thêm tiền tố 'b' hoặc 'h'
        advance();

        // Đọc giá trị nhị phân hoặc hệ 16
        while ((prefix == 'b' && (currentChar == '0' || currentChar == '1')) ||
               (prefix == 'h' && isxdigit(currentChar))) // `isxdigit` kiểm tra 0-9, A-F, a-f
        {
            result += currentChar;
            advance();
        }

        return result; // Trả về số nhị phân hoặc hệ 16
    }

    // Xử lý số thập phân thông thường
    while (isdigit(currentChar) || currentChar == '.')
    {
        result += currentChar;
        advance();
    }

    return result;
}

// Collect an operator from the input string
string Lexer::collectOperator()
{
    string result;
    if (!isalnum(currentChar) && currentChar != '_') // Ignore alphanumeric characters and underscore
    {
        result += currentChar;
        advance();
    }
    return result;
}

// Collect an identifier (variable name)
string Lexer::collectIdentifier()
{
    string result;
    while (isalnum(currentChar) || currentChar == '_') // Ký tự hợp lệ: chữ cái, số, hoặc gạch dưới
    {
        result += currentChar;
        advance();
    }

    return result;
}

// Tokenize the input string into a vector of Tokens
vector<Token *> Lexer::tokenize()
{
    vector<Token *> tokens;

    while (currentChar != '\0')
    {
        if (isspace(currentChar))
        {
            skipWhitespace(); // Bỏ qua khoảng trắng
        }
        else if (isdigit(currentChar)) // Xử lý số hoặc TypeDecl
        {
            string number = collectNumber();

            // Kiểm tra nếu đây là TypeDecl (e.g., "8b", "16b", "32b")
            if (currentChar == 'b' || currentChar == 'B')
            {
                number += currentChar;                      // Thêm 'b'
                advance();                                  // Bỏ qua ký tự 'b'
                tokens.push_back(new Token("Tpd", number)); // TypeDecl
            }
            else
            {
                tokens.push_back(new Integer(number)); // Số thông thường
            }
        }
        else if (currentChar == 'b' || currentChar == 'h') // Xử lý số nhị phân và hệ 16
        {
            string number = collectNumber();       // Thu thập giá trị
            tokens.push_back(new Integer(number)); // Tạo token kiểu Integer
        }
        else if (isalpha(currentChar)) // Xử lý tên biến
        {
            string identifier = collectIdentifier();
            tokens.push_back(new Token("Id", identifier));
        }
        else // Xử lý các ký tự đặc biệt như '=', ';'
        {
            string op = collectOperator();
            if (!op.empty())
            {
                tokens.push_back(new Operator(op));
            }
        }
    }

    return tokens;
}
