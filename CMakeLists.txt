# Đặt version tối thiểu cho CMake
cmake_minimum_required(VERSION 3.10)

# Tên dự án
project(main)

# Sử dụng chuẩn C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tìm tất cả các file .cpp trong dự án
file(GLOB_RECURSE SOURCES
    Lexer/*.cpp
    Parsing/*.cpp
    Interpreter/*.cpp
    main.cpp
)

# Include tất cả các thư mục header
include_directories(${CMAKE_SOURCE_DIR})

# Tạo file thực thi từ SOURCES
add_executable(${PROJECT_NAME} ${SOURCES})
