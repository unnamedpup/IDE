cmake_minimum_required(VERSION 3.12)
project(tests)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_search_module(NCURSESW REQUIRED ncursesw)
include_directories(${NCURSESW_INCLUDE_DIRS})

include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.4.0 
)

FetchContent_MakeAvailable(Catch2)

# include_directories(${CMAKE_SOURCE_DIR})

# file(GLOB TEST_SOURCES "main_test.cpp")
set(TEST_SOURCES
    main_test.cpp
    ../src/runner.hpp
    ../src/output.hpp
    ../src/output.cpp
    ../src/utils.cpp
    ../src/utils.hpp
)
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain ncursesw)
add_custom_target(run_tests COMMAND tests DEPENDS tests)

# file(GLOB OUTPUT_SOURCES "../src/*.cpp" "../src/*.hpp")
target_sources(tests PRIVATE ${TEST_SOURCES})
