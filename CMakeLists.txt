cmake_minimum_required(VERSION 3.18...3.31)
project(ZygiskYin CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fvisibility=hidden -fno-exceptions -fno-rtti -fno-unwind-tables -fno-asynchronous-unwind-tables")
set(CMAKE_C_FLAGS_RELEASE "-O3 -fvisibility=hidden")

add_executable(zygiskd src/zygiskd.cpp)
target_link_options(zygiskd PRIVATE -s -Wl,--gc-sections)

add_library(zygisk SHARED src/libzygisk.cpp)
set_target_properties(zygisk PROPERTIES OUTPUT_NAME "zygisk")
target_link_libraries(zygisk PRIVATE dl)
target_link_options(zygisk PRIVATE -s -Wl,--gc-sections)