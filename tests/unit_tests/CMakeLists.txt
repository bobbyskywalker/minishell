cmake_minimum_required(VERSION 3.11)

project(tokenizer_tests LANGUAGES CXX C)

include(FetchContent)

FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)

FetchContent_MakeAvailable(gtest)

add_executable(${PROJECT_NAME} tokenizer_tests.cpp ../../src/parser/lexer.c ../../src/parser/tokenizer_utils.c 
../../lib/libft/ft_calloc.c ../../lib/libft/ft_strlen.c ../../lib/libft/ft_strdup.c ../../lib/libft/ft_bzero.c)

target_link_libraries(${PROJECT_NAME} PRIVATE gtest gtest_main)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
