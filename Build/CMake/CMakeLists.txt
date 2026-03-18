cmake_minimum_required(VERSION 3.5)

project(NexUs VERSION 1.0.0 LANGUAGES CXX)

#在这里配置QT路径 例如 D:/Qt/6.6.2/msvc2019_64 D:/Qt/5.15.2/msvc2019_64 /home/liniyous/Qt/5.15.2/gcc_64
set(QT_SDK_DIR E:/Qt/6.6.2/msvc2019_64 CACHE PATH "QT SDK DIR" FORCE)
list(APPEND CMAKE_PREFIX_PATH ${QT_SDK_DIR})

option(AUTO_INSTALL_AFTER_BUILD "Automatically install after building" OFF)
message(STATUS "Auto install after build: ${AUTO_INSTALL_AFTER_BUILD}")

option(BUILD_NXPACKETIO "Build NXPacketIO" ON)

option(NXPACKETIO_BUILD_STATIC_LIB "Build static library." OFF)
option(NEXUS_BUILD_STATIC_LIB "Build static library." OFF)

if (NXPACKETIO_BUILD_STATIC_LIB)
    set(NXPACKETIO_LIB_TYPE "STATIC")
else ()
    set(NXPACKETIO_LIB_TYPE "SHARED")
endif ()
if (NEXUS_BUILD_STATIC_LIB)
    set(NEXUS_LIB_TYPE "STATIC")
else ()
    set(NEXUS_LIB_TYPE "SHARED")
endif ()

set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
include(CMake/Global.cmake)
include(CMake/Functions.cmake)

# Comes with third-party libraries and vcpkg management; whichever is added to the search path first will be found first.
# include(CMake/Dependencies.cmake) # vcpkg 3rdparty
# include(CMake/CompilerConfigs.cmake) # global compiler configs

set(NexUs_BIN_DIR "${CMAKE_SOURCE_DIR}/Bin/${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_${PLATFORM_SUFFIX}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${NexUs_BIN_DIR} CACHE PATH "Runtime output directory")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${NexUs_BIN_DIR} CACHE PATH "Library output directory")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${NexUs_BIN_DIR} CACHE PATH "Archive output directory")
set(CMAKE_PCH_OUTPUT_DIRECTORY ${NexUs_BIN_DIR} CACHE PATH "Precompiled header output directory")
set(CMAKE_ASM_LISTING_OUTPUT_DIRECTORY ${NexUs_BIN_DIR} CACHE PATH "Assembly listing output directory")
set(CMAKE_CXX_MODULE_OUTPUT_DIRECTORY ${NexUs_BIN_DIR} CACHE PATH "C++ module output directory")
if (COMPILER_MSVC)
  set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY ${NexUs_BIN_DIR} CACHE PATH "PDB compile output directory")
  set(CMAKE_PDB_OUTPUT_DIRECTORY ${NexUs_BIN_DIR} CACHE PATH "PDB output directory")
endif()
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/Install/${PROJECT_NAME}_${PLATFORM_SUFFIX}/${CMAKE_BUILD_TYPE}" CACHE PATH "Installation path" FORCE)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_subdirectory(NexUs)
if (WIN32 AND BUILD_NXPACKETIO)
    add_definitions(-DBUILD_WITH_NXPACKETIO)
    add_subdirectory(NXPacketIO)
endif()
add_subdirectory(NexUsExample)

if (${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.NexUs)
endif()