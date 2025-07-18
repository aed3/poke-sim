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

option(ENABLE_TESTING "Build the tests along with PokeSim. This will negatively effect the library's performance." ON)
if (ENABLE_TESTING)
  add_compile_definitions(POKESIM_ENABLE_TESTING)
endif()

option(ENTITY_VIEWER "Build the registry with component debugging tools" OFF)

if (ENTITY_VIEWER AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(POKESIM_ENTITY_VIEWER)
  set(ENTITY_VIEWER src/Utilities/EntityViewer.cpp)
else()
  set(ENTITY_VIEWER "")
endif()

set (SOURCES
  src/AnalyzeEffect/AnalyzeEffect.cpp
  src/AnalyzeEffect/Setup/AnalyzeEffectInputSetup.cpp
  src/Battle/Clone/Clone.cpp
  src/Battle/Helpers/Helpers.cpp
  src/Battle/ManageBattleState.cpp
  src/Battle/Pokemon/ManagePokemonState.cpp
  src/Battle/Setup/BattleStateSetup.cpp
  src/Battle/Setup/EnumToTag.cpp
  src/Battle/Setup/MoveStateSetup.cpp
  src/Battle/Setup/PokemonStateSetup.cpp
  src/Battle/Setup/SideStateSetup.cpp
  src/CalcDamage/CalcDamage.cpp
  src/CalcDamage/Setup/CalcDamageInputSetup.cpp
  src/Pokedex/Abilities/AbilityEvents.cpp
  src/Pokedex/Effects/EffectEvents.cpp
  src/Pokedex/Items/ItemEvents.cpp
  src/Pokedex/Pokedex.cpp
  src/Pokedex/Setup/AbilityDexDataSetup.cpp
  src/Pokedex/Setup/GetAbilityBuild.cpp
  src/Pokedex/Setup/GetItemBuild.cpp
  src/Pokedex/Setup/GetMoveBuild.cpp
  src/Pokedex/Setup/GetSpeciesBuild.cpp
  src/Pokedex/Setup/ItemDexDataSetup.cpp
  src/Pokedex/Setup/MoveDexDataSetup.cpp
  src/Pokedex/Setup/SpeciesDexDataSetup.cpp
  src/SimulateTurn/CalcDamageSpecifics.cpp
  src/SimulateTurn/ManageActionQueue.cpp
  src/SimulateTurn/RandomChance.cpp
  src/SimulateTurn/SimulateTurn.cpp
  src/Simulation/MoveHitSteps.cpp
  src/Simulation/RunEvent.cpp
  src/Simulation/Simulation.cpp
  src/Simulation/SimulationResults.cpp
  src/Simulation/SimulationSetup.cpp
  src/Utilities/ArgumentChecks.cpp
  src/Utilities/AssertComponentsEqual.cpp
  ${ENTITY_VIEWER}
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
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffile-prefix-map=${CMAKE_SOURCE_DIR}=/PokeSim")
endif()

target_compile_options(PokeSim PRIVATE ${compiler_warning_options})

############################################################
# Tests
############################################################

if (ENABLE_TESTING)
  add_executable(PokeSimTests
    tests/AnalyzeEffectTest.cpp
    tests/BattleStateTest.cpp
    tests/PokedexTest.cpp
    tests/CalcDamageTest.cpp
    tests/RegistryContainerTests.cpp
    tests/SimulationSetupTest.cpp
    tests/SimulateTurnsTest.cpp
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
    target_compile_definitions(PokeSimTests PRIVATE POKESIM_USE_SINGLE_HEADER_FILE)
  else()
    target_link_libraries(PokeSimTests PRIVATE PokeSim)
  endif()
endif()