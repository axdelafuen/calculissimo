cmake_minimum_required(VERSION 3.10)
project(calculissimo VERSION 1.0.0 LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TST_DIR ${CMAKE_SOURCE_DIR}/tests)

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE LIB_SOURCES "${SRC_DIR}/*.cpp")
list(FILTER LIB_SOURCES EXCLUDE REGEX "main\\.cpp$")
file(GLOB_RECURSE TEST "${TST_DIR}/*.cpp")

FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5)
FetchContent_MakeAvailable(raylib)

# Core library (without main.cpp)
add_library(core ${LIB_SOURCES})
target_include_directories(core PUBLIC ${INCLUDE_DIR})
target_link_libraries(core PUBLIC raylib)

# Executable
add_executable(calculissimo ${SOURCES})
target_include_directories(calculissimo PRIVATE ${INCLUDE_DIR})
target_link_libraries(calculissimo PRIVATE core)

# Tests
if(TEST)
    add_executable(tests ${TEST})
    target_include_directories(tests PRIVATE ${INCLUDE_DIR} ${TST_DIR})
    target_link_libraries(tests PRIVATE core)
endif()

if(UNIX)
    target_compile_options(calculissimo PRIVATE -Wall -Wextra -pthread)
    target_compile_options(core PRIVATE -Wall -Wextra -pthread)
endif()

#add_custom_command(
#    TARGET xale-db-cli
#    POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#            ${CMAKE_SOURCE_DIR}/appconfig.json
#            $<TARGET_FILE_DIR:xale-db-cli>/appconfig.json
#)
