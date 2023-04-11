cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(PokeSim)

add_library(${PROJECT_NAME} STATIC
  src/Dex/Dex.cpp
  src/Dex/Names.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/external
)

if (MSVC)
  set(compiler_warning_options /W4 /WX)
else()
  set(compiler_warning_options -Wshadow -Wall)
endif()

target_compile_options(${PROJECT_NAME} PRIVATE ${compiler_warning_options})

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()


############################################################
# Tests
############################################################

add_executable(PokeSimTests
  tests/Tests.cpp
  tests/Catch2/catch_amalgamated.cpp
)

option(USE_SINGLE_HEADER_FILE "Use the Single Header File version of the library when running tests." OFF)

target_link_libraries(PokeSimTests PRIVATE PokeSim)
target_compile_options(PokeSimTests PRIVATE ${compiler_warning_options})

if (USE_SINGLE_HEADER_FILE)
  target_compile_definitions(PokeSimTests PRIVATE USE_SINGLE_HEADER)
else()
  target_link_libraries(PokeSimTests PRIVATE PokeSim)
endif()
