cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CheckCXXCompilerFlag)
include(CheckIPOSupported)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

  set_property(CACHE
    CMAKE_BUILD_TYPE
    PROPERTY
    STRINGS
    "Debug"
    "Release"
    "MinSizeRel"
    "RelWithDebInfo"
  )
endif()

if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/debug)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/release)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL
      ${CMAKE_BINARY_DIR}/lib/minsizerel)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO
      ${CMAKE_BINARY_DIR}/lib/relwithdebinfo)
endif()

if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL
      ${CMAKE_BINARY_DIR}/bin/minsizerel)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO
      ${CMAKE_BINARY_DIR}/bin/relwithdebinfo)
endif()

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL
      ${CMAKE_BINARY_DIR}/bin/minsizerel)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO
      ${CMAKE_BINARY_DIR}/bin/relwithdebinfo)
endif()

project(PokeSim)

option(ENABLE_TESTING
  "Build the tests along with PokeSim. This will negatively effect the library's performance."
  ON
)

option(ENABLE_BENCHMARKING "Build the benchmarks along with PokeSim." OFF)

option(USE_COMBINED_SOURCE_AND_HEADER_FILES
  "Use the combined source and combined header file version of the library when running tests and benchmarks."
  OFF
)

if(USE_COMBINED_SOURCE_AND_HEADER_FILES)
  set(ENTITY_VIEWER OFF)
else()
  option(ENTITY_VIEWER "Build the registry with component debugging tools" OFF)
endif()

function(setup_target)
  set(options DEFS)
  set(oneValueArgs NAME)
  set(multiValueArgs SOURCES)
  cmake_parse_arguments(CUR_TARGET
    "${options}"
    "${oneValueArgs}"
    "${multiValueArgs}"
    ${ARGN}
  )

  set(is_library_target OFF)
  if(CUR_TARGET_NAME STREQUAL "PokeSim")
    set(is_library_target ON)
  endif()

  if(is_library_target)
    add_library(PokeSim STATIC ${CUR_TARGET_SOURCES})
  else()
    add_executable(${CUR_TARGET_NAME} ${CUR_TARGET_SOURCES})
    target_include_directories(${CUR_TARGET_NAME}
      PRIVATE
      ${PROJECT_SOURCE_DIR}/external
    )
  endif()

  target_compile_options(${CUR_TARGET_NAME} PRIVATE ${compiler_warning_options})

  if(ipo_enabled)
    set_property(TARGET
      ${CUR_TARGET_NAME}
      PROPERTY
      INTERPROCEDURAL_OPTIMIZATION
      TRUE
    )
  endif()

  if(NOT is_library_target)
    if(USE_COMBINED_SOURCE_AND_HEADER_FILES)
      message(STATUS
        "Using combined source and header files instead of library for ${CUR_TARGET_NAME}"
      )

      target_compile_definitions(${CUR_TARGET_NAME}
        PRIVATE
        POKESIM_USE_COMBINED_SOURCE_AND_HEADER_FILES
      )
      target_sources(${CUR_TARGET_NAME} PRIVATE extras/PokeSim.cpp)
    else()
      target_link_libraries(${CUR_TARGET_NAME} PRIVATE PokeSim)
    endif()
  endif()
endfunction()

check_ipo_supported(RESULT ipo_supported OUTPUT error)
set(ipo_enabled OFF)

if(ENABLE_TESTING)
  add_compile_definitions(POKESIM_ENABLE_TESTING)
endif()

if(ENTITY_VIEWER AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(POKESIM_ENTITY_VIEWER)
  set(ENTITY_VIEWER src/Utilities/EntityViewer.cpp)
else()
  set(ENTITY_VIEWER "")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  CHECK_CXX_COMPILER_FLAG("-march=native" arch_native_supported)
  if(arch_native_supported)
    option(OPTIMIZE_FOR_NATIVE "Build code that uses all of your computer's feature. Resulting program may not run on other computers." OFF)
    if(OPTIMIZE_FOR_NATIVE)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    endif()
  endif()

  if(ipo_supported)
    cmake_host_system_information(RESULT
      SystemMemory
      QUERY
      TOTAL_PHYSICAL_MEMORY
    )
    if(SystemMemory GREATER 4000)
      message(STATUS "IPO / LTO enabled")
      set(ipo_enabled ON)
    else()
      message(STATUS "System memory of ${SystemMemory} is too low for LTO")
    endif()
  else()
    message(STATUS "IPO / LTO not supported: <${error}>")
  endif()
endif()

if(MSVC)
  set(compiler_warning_options /W4 /WX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
else()
  set(compiler_warning_options -Wshadow -Wall)
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(
      CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -ffile-prefix-map=${CMAKE_SOURCE_DIR}=."
    )
  endif()
endif()

setup_target(
  NAME PokeSim
  SOURCES
    src/AnalyzeEffect/AnalyzeEffect.cpp
    src/AnalyzeEffect/Setup/AnalyzeEffectInputSetup.cpp
    src/Battle/Clone/Clone.cpp
    src/Battle/Helpers/Helpers.cpp
    src/Battle/ManageBattleState.cpp
    src/Battle/Pokemon/ManagePokemonState.cpp
    src/Battle/Setup/BattleStateSetup.cpp
    src/Battle/Setup/EmplaceTagFromEnum.cpp
    src/Battle/Setup/MoveStateSetup.cpp
    src/Battle/Setup/PokemonStateSetup.cpp
    src/Battle/Setup/SideStateSetup.cpp
    src/Battle/Side/ManageSideState.cpp
    src/CalcDamage/CalcDamage.cpp
    src/CalcDamage/Setup/CalcDamageInputSetup.cpp
    src/Pokedex/Abilities/AbilityEvents.cpp
    src/Pokedex/Effects/EffectEvents.cpp
    src/Pokedex/Items/ItemEvents.cpp
    src/Pokedex/Moves/MoveEvents.cpp
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

target_include_directories(PokeSim PUBLIC
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/external
)

if(ENABLE_TESTING)
  setup_target(NAME PokeSimTests
    SOURCES
      external/Catch2/catch_amalgamated.cpp
      tests/AnalyzeEffectTest.cpp
      tests/BattleStateTest.cpp
      tests/CalcDamageTest.cpp
      tests/Effects/ChoiceLock.cpp
      tests/Effects/Paralysis.cpp
      tests/Moves/FuryAttack.cpp
      tests/Moves/KnockOff.cpp
      tests/PokedexTest.cpp
      tests/RegistryContainerTests.cpp
      tests/SimulateTurnsTest.cpp
      tests/SimulationSetupTest.cpp
      tests/Tests.cpp
      tests/VerticalSlices.cpp
  )

  target_compile_definitions(PokeSimTests PRIVATE CATCH_AMALGAMATED_CUSTOM_MAIN)
endif()

if(ENABLE_BENCHMARKING)
  setup_target(NAME PokeSimBenchmarks
    SOURCES
      external/Catch2/catch_amalgamated.cpp
      benchmarks/VerticalSlices.cpp
  )
  target_compile_definitions(PokeSimBenchmarks PRIVATE CATCH_CONFIG_DEFAULT_REPORTER="benchmark")
endif()
