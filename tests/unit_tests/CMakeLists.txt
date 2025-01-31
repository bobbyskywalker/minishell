cmake_minimum_required(VERSION 3.11)

project(tokenizer_tests LANGUAGES CXX C)

include(FetchContent)

FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)

FetchContent_MakeAvailable(gtest)

# First test executable (tokenizer_tests)
add_executable(${PROJECT_NAME} tokenizer_tests.cpp 
    ../../src/parser/lexer.c 
    ../../src/parser/tokenizer_utils.c 
    ../../lib/libft/ft_calloc.c 
    ../../lib/libft/ft_strlen.c 
    ../../lib/libft/ft_strdup.c 
    ../../lib/libft/ft_bzero.c)

target_link_libraries(${PROJECT_NAME} PRIVATE gtest gtest_main)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Discover and add tests for the first test executable
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

# Second test executable (AST_tests)
add_executable(AST_tests AST_tests.cpp 
    ../../src/parser/lexer.c 
    ../../src/parser/tokenizer_utils.c 
    ../../src/parser/AST_parser.c 
    ../../src/parser/AST_utils.c 
    ../../src/parser/parser_utils.c
    ../../lib/libft/ft_calloc.c 
    ../../lib/libft/ft_strlen.c 
    ../../lib/libft/ft_strdup.c 
    ../../lib/libft/ft_bzero.c
    ../../lib/libft/ft_strncmp.c)

target_link_libraries(AST_tests PRIVATE gtest gtest_main)
target_compile_features(AST_tests PRIVATE cxx_std_17)

# Discover and add tests for the second test executable
gtest_discover_tests(AST_tests)
