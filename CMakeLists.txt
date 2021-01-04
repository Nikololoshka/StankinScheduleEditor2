# Project StankinScheduleEditor2
# ******************************************************************************
cmake_minimum_required(VERSION 3.16)

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

# Qt 5
# ******************************************************************************
if(WIN32)
    set(TEMPLATE_QT_VERSION "5.15.1" CACHE STRING "Qt Version")
else()
    set(TEMPLATE_QT_VERSION "5.15.0" CACHE STRING "Qt Version")
endif()

set(TEMPLATE_QT_MODULES Core Gui Widgets)
#set(CMAKE_PREFIX_PATH "C:\\Qt\\5.15.1\\msvc2019_64\\lib\\cmake\\Qt5")

if(WIN32)
    set(Qt5_DIR "C:/Qt/5.15.1/msvc2019_64/lib/cmake/Qt5" CACHE STRING "Qt5 Windows Dir")
endif()

find_package(Qt5 ${TEMPLATE_QT_VERSION} EXACT CONFIG REQUIRED ${TEMPLATE_QT_MODULES} )

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# OpenCV
# ******************************************************************************
find_package(OpenCV CONFIG REQUIRED)

# Tesseract
# ******************************************************************************
if(WIN32)
    find_package(Tesseract CONFIG REQUIRED)
endif()

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
include_directories("${ROOT_DIR}/lib")
include_directories("${ROOT_DIR}/include")

add_executable(${PROJECT_NAME} ${list_app_src} ${list_app_res})
target_include_directories(${PROJECT_NAME} PUBLIC ${BINARY_DIR})

# Copy resources
# *********************************************************************
set(COPY_RES OFF)
if (COPY_RES)
    if(APPLE)
        execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${MACOSX_BUNDLE_RESOURCES})
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${MACOSX_BUNDLE_ICON} ${MACOSX_BUNDLE_RESOURCES})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND}
            -E copy_directory "${ROOT_DIR}/res" ${BUILD_DIR}/${PROJECT_NAME}.app/contents/)
    else()
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND}
            -E copy_directory "${ROOT_DIR}/res" ${BUILD_DIR}/)
        add_custom_target(CopyResources ALL COMMAND ${CMAKE_COMMAND}
            -E copy_directory "${ROOT_DIR}/res" ${BUILD_DIR}/)
    endif()
endif()

message(DEBUG "Paths:")
message(DEBUG "${Tesseract_LIBS}")
message(DEBUG "${Leptonica_LIBS}")

# Link
# *********************************************************************
target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt5::Core
    PRIVATE Qt5::Widgets
    PRIVATE Qt5::Gui
    PRIVATE ${OpenCV_LIBS}
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE libtesseract
    )
else()
    target_link_libraries(${PROJECT_NAME}
        PRIVATE tesseract
        PRIVATE lept
    )
endif()

target_precompile_headers(${PROJECT_NAME}
    PUBLIC "${ROOT_DIR}/include/stdafx.h"
)
