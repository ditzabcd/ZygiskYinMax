cmake_minimum_required(VERSION 3.18)
project(ZygiskYin CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s -fvisibility=hidden -fno-exceptions -fno-rtti -Wl,--gc-sections -fno-unwind-tables -fno-asynchronous-unwind-tables")
set(CMAKE_C_FLAGS_RELEASE "-O3 -s -fvisibility=hidden -Wl,--gc-sections")

add_executable(zygiskd src/zygiskd.cpp)
target_link_libraries(zygiskd PRIVATE pthread)

add_library(zygisk SHARED src/libzygisk.cpp)
set_target_properties(zygisk PROPERTIES OUTPUT_NAME "zygisk")
target_link_libraries(zygisk PRIVATE dl)