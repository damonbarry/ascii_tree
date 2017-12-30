cmake_minimum_required(VERSION 3.9)

project(ascii_tree)

include_directories(. spec)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

add_executable(ascii_tree_tests
    spec/can_generate_a_syntax_tree.cpp
    spec/can_parse_chars.cpp
    spec/can_recognize_ascii_tree_chars.cpp
    spec/can_recognize_ascii_tree_tokens.cpp
    spec/can_reject_invalid_char_sequences.cpp
    spec/tests.cpp
)