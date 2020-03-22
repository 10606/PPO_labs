cmake_minimum_required(VERSION 3.0)

project(lab_3 VERSION 1.0 LANGUAGES C CXX)

set(release_flags
    "-Os -march=native -Wall -ftemplate-depth=10000 -std=c++17 -lpthread"
)

set(compile_flags "-fsanitize=address -fsanitize=leak -fsanitize=undefined \
-fdiagnostics-color=always -fdiagnostics-generate-patch \
-g -Wall -ftemplate-depth=10000 -std=c++17"
)

#httpparser https://github.com/nekipelov/httpparser

link_directories(
    .
    /usr/lib/
)

#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${compile_flags}")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${release_flags}")

message("CMAKE_CXX_FLAGS" ${CMAKE_CXX_FLAGS})

add_executable(main.cpp.elf
    main.cpp
    dao/postgres.cpp
    processing/processing.cpp
    utils/data.cpp
    server/https_server.cpp
    server/uri_parser.cpp
    utils/time_convert.cpp
)

target_include_directories(main.cpp.elf 
    PRIVATE 
)

set_target_properties(main.cpp.elf
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY .
)

target_link_libraries(main.cpp.elf
    curl
    mongoose
    pqxx
    gtest
)





add_executable(test.cpp.elf
    test/test.cpp
    test/common_test.cpp
    test/curl_wrap.cpp
    dao/postgres.cpp
    processing/processing.cpp
    utils/data.cpp
    server/https_server.cpp
    server/uri_parser.cpp
    utils/time_convert.cpp
)

target_include_directories(test.cpp.elf 
    PRIVATE 
)

set_target_properties(test.cpp.elf
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY .
)

target_link_libraries(test.cpp.elf
    curl
    mongoose
    pqxx
    gtest
)


add_custom_target(test ALL DEPENDS test.cpp.elf
    COMMAND ./test.cpp.elf
)


