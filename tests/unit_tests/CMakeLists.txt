cmake_minimum_required(VERSION 3.11)

project(strtok_tests LANGUAGES CXX C)

include(FetchContent)

FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)

FetchContent_MakeAvailable(gtest)

add_executable(${PROJECT_NAME} strtok_test.cpp ../../src/parser/lexer.c ../../lib/libft/ft_isspace.c ../../lib/libft/ft_strlen.c)

target_link_libraries(${PROJECT_NAME} PRIVATE gtest gtest_main)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
