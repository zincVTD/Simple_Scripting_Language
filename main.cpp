#include "Lexer/lexer.hpp"
#include "Parsing/parser.hpp"
#include "Interpreter/interpreter.hpp"

int main()
{
    Interpreter interpreter; // Khởi tạo Interpreter duy nhất

    while (1)
    {
        // Chuỗi đầu vào
        cout << "Input Code: ";
        string input;
        getline(cin, input);

        try
        {
            Lexer lexer(input);
            auto tokens = lexer.tokenize();

            for (auto token : tokens)
            {
                cout << "Type: " << token->getType() << ", Value: " << token->getValue() << endl;
            }

            Parser parser(tokens);
            auto ast = parser.parse();
            ast->print();
            cout << endl;

            double result = interpreter.interpret(ast);
            cout << "Final Result: " << result << endl;

            // Cleanup tokens
            for (auto token : tokens)
            {
                delete token;
            }
        }
        catch (const exception &e)
        {
            cerr << "Error: " << e.what() << endl;
        }
    }
    return 0;
}

