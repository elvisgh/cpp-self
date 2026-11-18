cmake_minimum_required(VERSION 3.16)
project(cpp-self VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 手写系列：header-only，测试是唯一消费者
add_library(cppself INTERFACE)
target_include_directories(cppself INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(cppself INTERFACE -Wall -Wextra -Wpedantic)

option(CPPSELF_SANITIZE "Build tests with ASan/UBSan" OFF)

enable_testing()

# 每个用例独立进程跑：崩了一个不会掩盖后面的
function(cppself_add_case_test target cases)
  add_executable(${target} tests/${target}.cpp)
  target_link_libraries(${target} PRIVATE cppself)
  if (CPPSELF_SANITIZE)
    target_compile_options(${target} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer -g)
    target_link_options(${target} PRIVATE -fsanitize=address,undefined)
  endif()
  foreach(c ${cases})
    add_test(NAME ${target}.case${c} COMMAND ${target} ${c})
  endforeach()
endfunction()

cppself_add_case_test(test_string "1;2;3;4;5;6;7;8;9;10;11;12")
