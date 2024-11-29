project(tests)

file(GLOB_RECURSE TESTSOURCES "*.cpp")
add_executable(${PROJECT_NAME} ${TESTSOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE gtest gtest_main Boost::boost)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})