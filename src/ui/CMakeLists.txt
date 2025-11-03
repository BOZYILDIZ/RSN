# ==============================================================================
# RecoverySoftNetz UI (Qt6) — CMakeLists.txt
# ==============================================================================
#
# Manages build configuration for UI components:
# - Device Wizard
# - Progress Monitor
# - Results View
# - Main Application Window
#
# Requires: Qt6 (Core, Gui, Widgets)

# ==============================================================================
# UI Library Configuration
# ==============================================================================

project(RSN_UI LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt6 MOC (Meta-Object Compiler) automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ==============================================================================
# Find Qt6
# ==============================================================================

find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    REQUIRED
)

# ==============================================================================
# UI Sources
# ==============================================================================

set(UI_SOURCES
    device_wizard.h
    device_wizard.cpp
    progress_monitor.h
    progress_monitor.cpp
    results_view.h
    results_view.cpp
)

# ==============================================================================
# Main Window (will be created from main.cpp)
# ==============================================================================

set(MAINWINDOW_SOURCES
    ../ui/mainwindow.h
    ../ui/mainwindow.cpp
)

# ==============================================================================
# Resources (icons, stylesheets, images)
# ==============================================================================

set(UI_RESOURCES
    ui_resources.qrc
)

# ==============================================================================
# Create UI Library
# ==============================================================================

add_library(RSN_UI STATIC
    ${UI_SOURCES}
    ${MAINWINDOW_SOURCES}
    ${UI_RESOURCES}
)

# Link Qt6 libraries
target_link_libraries(RSN_UI
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

# Include directories for UI library
target_include_directories(RSN_UI
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# ==============================================================================
# Compiler Flags
# ==============================================================================

if(MSVC)
    # Windows: Add MSVC-specific flags
    target_compile_options(RSN_UI PRIVATE /W4 /WX)
elseif(APPLE)
    # macOS: Add Apple Clang flags
    target_compile_options(RSN_UI PRIVATE -Wall -Wextra -Wpedantic)
else()
    # Linux: Add GCC/Clang flags
    target_compile_options(RSN_UI PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ==============================================================================
# Installation (optional)
# ==============================================================================

# Install UI headers
install(FILES
    device_wizard.h
    progress_monitor.h
    results_view.h
    DESTINATION include/rsn/ui
)

# Install resources
install(FILES
    ui_resources.qrc
    DESTINATION share/rsn/ui
)

# ==============================================================================
# Summary
# ==============================================================================

message(STATUS "✓ UI Module Configuration:")
message(STATUS "  • Device Wizard: device_wizard.{h,cpp}")
message(STATUS "  • Progress Monitor: progress_monitor.{h,cpp}")
message(STATUS "  • Results View: results_view.{h,cpp}")
message(STATUS "  • Main Window: mainwindow.{h,cpp}")
message(STATUS "  • Resources: ui_resources.qrc")
message(STATUS "  • Library: RSN_UI (static)")
message(STATUS "  • Qt6 Components: Core, Gui, Widgets")
