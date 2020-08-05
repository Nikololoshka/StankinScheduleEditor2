# Project StankinScheduleEditor2
# ******************************************************************************
cmake_minimum_required(VERSION 3.15)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/functions.cmake)

set(TEMPLATE_APPLICATION_NAME "StankinScheduleEditor2" CACHE STRING "Application name")
set(PROJECT_NAME ${TEMPLATE_APPLICATION_NAME})

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
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
set(TEMPLATE_QT_VERSION "5.15.0" CACHE STRING "Qt Version")
set(TEMPLATE_QT_MODULES Core Gui Widgets)

find_package(Qt5 ${TEMPLATE_QT_VERSION} EXACT CONFIG REQUIRED ${TEMPLATE_QT_MODULES})

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# OpenCV
# ******************************************************************************
if (WIN32)
    set(OpenCV_DIR "D:/opencv/install")
endif()

find_package(OpenCV REQUIRED)

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

AddSources("${SOURCE_DIR}/view" "view" list_app_src)

AddSources("${SOURCE_DIR}/model" "schedule" list_app_src)
AddSources("${SOURCE_DIR}/model/pair" "pair" list_app_src)

AddSources("${SOURCE_DIR}/ui" "ui" list_app_src)
AddSources("${SOURCE_DIR}/ui/editor" "editor" list_app_src)
AddSources("${SOURCE_DIR}/ui/import" "import" list_app_src)

set(list_app_res "res/res.qrc")

# Tests
# ******************************************************************************

enable_testing()
include(GoogleTest)
add_subdirectory(test)

# Target
# ******************************************************************************
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories("${SOURCE_DIR}/model")
include_directories("${SOURCE_DIR}/view")

add_executable(${PROJECT_NAME} ${list_app_src} ${list_app_res})
target_include_directories(${PROJECT_NAME} PUBLIC ${BINARY_DIR})

# Link
# *********************************************************************
target_link_libraries(${PROJECT_NAME}
    PUBLIC Qt5::Core
    PUBLIC Qt5::Widgets
    PUBLIC Qt5::Gui
    ${OpenCV_LIBS}
)

