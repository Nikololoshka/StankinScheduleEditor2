# Project StankinScheduleEditor2
# ******************************************************************************
cmake_minimum_required(VERSION 3.0)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/functions.cmake)

set(TEMPLATE_APPLICATION_NAME "StankinScheduleEditor2" CACHE STRING "Application name")
set(PROJECT_NAME ${TEMPLATE_APPLICATION_NAME})

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project directories
# ******************************************************************************
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(PROJECT_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
set(BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)

# Output directories
# ******************************************************************************
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BUILD_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${BUILD_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${BUILD_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BUILD_DIR})

# Meta information
# ******************************************************************************
set(TEMPLATE_VERSION_MAJOR "0" CACHE STRING "Version Major")
set(TEMPLATE_VERSION_MINOR "1" CACHE STRING "Version Minor")
set(TEMPLATE_VERSION_PATCH "0" CACHE STRING "Version Patch")
set(TEMPLATE_ORGANIZATION "Nikolay Vereshchagin" CACHE STRING "Organization")
set(TEMPLATE_DOMAIN "github.com/nikololoshka" CACHE STRING "Domain")

# Qt
# ******************************************************************************
set(TEMPLATE_QT_VERSION "5.13.2" CACHE STRING "Qt Version")
set(TEMPLATE_QT_MODULES Core Gui Widgets)

find_package(Qt5 ${TEMPLATE_QT_VERSION} EXACT CONFIG REQUIRED ${TEMPLATE_QT_MODULES})

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Configure files
# ******************************************************************************
configure_file(${ROOT_DIR}/cmake/meta.hpp.in ${BINARY_DIR}/meta.hpp)

# Definitions
# ******************************************************************************
add_definitions(${QT_DEFINITIONS})
add_definitions(-DUSE_INSTALL_TARGET)

# Sources
# ******************************************************************************
AddSources("${SOURCE_DIR}" "app" list_app_src)
AddSources("${SOURCE_DIR}/forms" "forms" list_app_src)
AddSources("${SOURCE_DIR}/schedule" "schedule" list_app_src)
AddSources("${SOURCE_DIR}/schedule/pair" "pair" list_app_src)

set(list_app_res "res/res.qrc")

# Tests
# ******************************************************************************

enable_testing()
include(GoogleTest)
add_subdirectory(test)

# Target
# ******************************************************************************
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(${PROJECT_NAME} ${list_app_src} ${list_app_res})
target_include_directories(${PROJECT_NAME} PUBLIC ${BINARY_DIR})

# Link
# *********************************************************************
target_link_libraries(${PROJECT_NAME}
    PUBLIC Qt5::Core
    PUBLIC Qt5::Widgets
    PUBLIC Qt5::Gui
)
