cmake_minimum_required(VERSION 3.0)

#if(MSVC)
#  add_compile_options(/W4 /WX)
#else(MSVC)
#  add_compile_options(-Wall -Wextra -pedantic -Werror)
#endif(MSVC)

add_library(ast ast.cpp)

add_library(tokeniser tokeniser.cpp)
target_link_libraries(tokeniser ast)

add_library(shuntingyard shuntingyard.cpp)
target_link_libraries(shuntingyard ast)
target_link_libraries(shuntingyard tokeniser)

add_executable(main main.cpp)
target_link_libraries(main ast)
target_link_libraries(main tokeniser)
target_link_libraries(main shuntingyard)