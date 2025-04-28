# Simple_Scripting_Language
Simple Scripting Language là một dự án mô phỏng cách hoạt động của một ngôn ngữ lập trình đơn giản. Hệ thống bao gồm các thành phần cơ bản như Lexer, Parser và Interpreter để thực hiện phân tích cú pháp và thực thi các tập lệnh ngắn. Dự án nhằm mục tiêu minh họa cách một ngôn ngữ lập trình hoạt động từ lúc đọc mã nguồn cho tới lúc thực thi.
### Cấu trúc dự án
- Lexer/: Chịu trách nhiệm đọc mã nguồn đầu vào và chuyển đổi thành các tokens (từ khóa, số, toán tử, v.v.).
- Parsing/: Chịu trách nhiệm phân tích token thành cây cú pháp trừu tượng (AST - Abstract Syntax Tree).
- Interpreter/: Thực thi AST để thực hiện các phép toán, gán giá trị và xử lý logic.
- build/: Thư mục chứa các file biên dịch đầu ra (CMake build system).
- main.cpp: Chương trình chính để khởi chạy Lexer, Parser và Interpreter trên một tập lệnh đầu vào.
### Cách hoạt động
- Lexer đọc mã nguồn dạng text, tách thành từng token (ví dụ: từ khóa let, dấu =, số 5, toán tử +, v.v.).
- Parser nhận các token từ Lexer, phân tích cú pháp và xây dựng một cây AST đại diện cho chương trình.
- Interpreter đi qua cây AST để thực thi các lệnh, tính toán biểu thức và lưu trữ giá trị biến.\
**Ví dụ**:
```
Input: let x = 5 + 3;
Lexer => Tokens: [LET, IDENT(x), EQUAL, NUMBER(5), PLUS, NUMBER(3), SEMICOLON]
Parser => AST: Gán x bằng (5 + 3)
Interpreter => Kết quả: x = 8
```
