cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CheckIPOSupported)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if (NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/debug)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/release)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/lib/minsizerel)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/lib/relwithdebinfo)
endif()

if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/bin/minsizerel)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/bin/relwithdebinfo)
endif()

if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/bin/minsizerel)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/bin/relwithdebinfo)
endif()

project(PokeSim)

check_ipo_supported(RESULT IpoSupported OUTPUT error)

set (SOURCES
  src/Battle/Actions/ResolveDecision.cpp
  src/Battle/Setup/BattleStateSetup.cpp
  src/Battle/Setup/MoveStateSetup.cpp
  src/Battle/Setup/PokemonStateSetup.cpp
  src/Battle/Setup/SideStateSetup.cpp
  src/Components/Tags/AbilityTags.cpp
  src/Components/Tags/ItemTags.cpp
  src/Components/Tags/NatureTags.cpp
  src/Components/Tags/StatusTags.cpp
  src/Pokedex/Names.cpp
  src/Pokedex/Pokedex.cpp
  src/Pokedex/Setup/GetItemBuild.cpp
  src/Pokedex/Setup/GetMoveBuild.cpp
  src/Pokedex/Setup/GetSpeciesBuild.cpp
  src/Pokedex/Setup/ItemDexDataSetup.cpp
  src/Pokedex/Setup/MoveDexDataSetup.cpp
  src/Pokedex/Setup/SpeciesDexDataSetup.cpp
  src/Simulation/SimulationSetup.cpp
  src/Simulation/SimulationResults.cpp
)

add_library(PokeSim STATIC ${SOURCES})

target_include_directories(PokeSim PUBLIC
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/external
)

if (MSVC)
  set(compiler_warning_options /W4 /WX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
else()
  set(compiler_warning_options -Wshadow -Wall)
endif()

target_compile_options(PokeSim PRIVATE ${compiler_warning_options})

############################################################
# Tests
############################################################

add_executable(PokeSimTests
  tests/BattleStateTest.cpp
  tests/PokedexTest.cpp
  tests/Tests.cpp
  external/Catch2/catch_amalgamated.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  if (IpoSupported)
    cmake_host_system_information(RESULT SystemMemory QUERY TOTAL_PHYSICAL_MEMORY)
    if (SystemMemory GREATER 4000)
      message(STATUS "IPO / LTO enabled")
      set_property(TARGET PokeSim PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
      set_property(TARGET PokeSimTests PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
      message(STATUS "System memory of ${SystemMemory} is too low for LTO")
    endif()
  else()
    message(STATUS "IPO / LTO not supported: <${error}>")
  endif()
endif()

option(USE_SINGLE_HEADER_FILE "Use the Single Header File version of the library when running tests." OFF)

target_compile_options(PokeSimTests PRIVATE ${compiler_warning_options})

if (USE_SINGLE_HEADER_FILE)
  message(STATUS "Using single header file instead of library")
  target_include_directories(PokeSimTests PUBLIC ${PROJECT_SOURCE_DIR}/external)
  target_compile_definitions(PokeSimTests PRIVATE USE_SINGLE_HEADER)
else()
  target_link_libraries(PokeSimTests PRIVATE PokeSim)
endif()
