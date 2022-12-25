cmake_minimum_required(VERSION 3.14)

project(RISCVEM VERSION 0.1 LANGUAGES CXX)
option(ENABLE_DOCTESTS "Include tests in the library. 
  Setting this to OFF will remove alldoctest related code.
  Tests in tests/*.cpp will still be enabled." ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-anon-enum-enum-conversion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a+fp+simd+crypto+crc")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include(Doctest)

set(SOURCES
  src/cpu.cpp
  src/registers.cpp
  src/memory.cpp
  src/utils.cpp
  src/conditionals.cpp
  src/arithmetics.cpp
)

set(TESTFILES
  tests/main.cpp)

set(LIBRARY_NAME riscpp)
add_library(${LIBRARY_NAME} SHARED ${SOURCES})

include_directories(riscv-tests)

target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${LIBRARY_NAME} PUBLIC doctest)

add_executable(riscvem app/main.cpp)
target_link_libraries(riscvem PUBLIC ${LIBRARY_NAME})

set_target_properties(
  ${LIBRARY_NAME} riscvem
  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

add_subdirectory(tests)
