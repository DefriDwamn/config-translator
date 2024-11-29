cmake_minimum_required(VERSION 3.20)
project(config_translator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
)
FetchContent_MakeAvailable(googletest)

find_package(Boost REQUIRED program_options)

include_directories("include" ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES "src/*.cpp" "include/*.hpp" "main.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE Boost::boost Boost::program_options)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()
add_subdirectory(tests)