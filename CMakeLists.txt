cmake_minimum_required(VERSION 3.16)
project(SymbolicMath)

set(CMAKE_CXX_STANDARD 20)

add_executable(SymbolicMath main.cpp symbolicMath.h parser/parser.h parser/select.h symbols/operators.h symbols/functions.h symbols/terms.h symbols/constants.h parser/stack.h parser/fixedString.h)
