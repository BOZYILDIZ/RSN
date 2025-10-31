cmake_minimum_required(VERSION 3.18)
project(RecoverySoftNetz
  VERSION 0.1.0
  LANGUAGES CXX C
  DESCRIPTION "Universal AI-Powered Data Recovery Solution"
)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wno-deprecated-declarations")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find packages
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
find_package(GTest)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ═══════════════════════════════════════════════════════════════════
# Core Recovery Engine
# ═══════════════════════════════════════════════════════════════════

add_library(recovery_core
  src/core/recovery_engine.h
  src/core/recovery_engine.cpp
  src/core/file_registry.h
  src/core/file_registry.cpp
  src/filesystems/filesystem_interface.h
)

target_include_directories(recovery_core PUBLIC src/)
target_link_libraries(recovery_core PUBLIC Qt6::Core)

# ═══════════════════════════════════════════════════════════════════
# UI (Qt6)
# ═══════════════════════════════════════════════════════════════════

add_executable(RecoverySoftNetz
  src/ui/main.cpp
  src/ui/mainwindow.h
  src/ui/mainwindow.cpp
)

target_link_libraries(RecoverySoftNetz
  PRIVATE
  recovery_core
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
)

# ═══════════════════════════════════════════════════════════════════
# Tests
# ═══════════════════════════════════════════════════════════════════

if(GTEST_FOUND)
  enable_testing()

  add_executable(recovery_tests
    tests/unit/recovery_engine_test.cpp
  )

  target_link_libraries(recovery_tests
    PRIVATE
    recovery_core
    GTest::GTest
    GTest::Main
  )

  add_test(NAME RecoveryTests COMMAND recovery_tests)
else()
  message(STATUS "GoogleTest not found - tests disabled")
endif()

# ═══════════════════════════════════════════════════════════════════
# Output summary
# ═══════════════════════════════════════════════════════════════════

message(STATUS "")
message(STATUS "═══════════════════════════════════════════════════════")
message(STATUS "RecoverySoftNetz v${PROJECT_VERSION}")
message(STATUS "═══════════════════════════════════════════════════════")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Qt Version: ${Qt6_VERSION}")
message(STATUS "GTest: ${GTEST_FOUND}")
message(STATUS "")
