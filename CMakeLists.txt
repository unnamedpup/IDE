cmake_minimum_required(VERSION 3.12)
project(json-IDE CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

find_package(PkgConfig REQUIRED)
pkg_search_module(NCURSESW REQUIRED ncursesw)
include_directories(${NCURSESW_INCLUDE_DIRS})


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB SOURCES "src/*.cpp" "src/*.hpp")

add_executable(json-IDE ${SOURCES})
target_link_libraries(json-IDE ncursesw)



add_subdirectory(tests)

 target_include_directories(tests PRIVATE 
     ${CMAKE_CURRENT_SOURCE_DIR}
     ${CMAKE_SOURCE_DIR}
)