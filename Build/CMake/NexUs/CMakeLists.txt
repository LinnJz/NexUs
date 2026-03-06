cmake_minimum_required(VERSION 3.5)

project(NexUs VERSION 2.0.0 LANGUAGES CXX)

add_definitions(-DNEXUS_LIBRARY)
option(NEXUS_BUILD_STATIC_LIB "Build static library." OFF)

FILE(GLOB ORIGIN Source/include/*.h Source/*.cpp)
FILE(GLOB PRIVATE Source/private/*.h Source/private/*.cpp)
FILE(GLOB DEVELOPER Source/DeveloperComponents/*.h Source/DeveloperComponents/*.cpp)

source_group(include FILES ${ORIGIN})
source_group(private FILES ${PRIVATE})
source_group(DeveloperComponents FILES ${DEVELOPER})

set(PROJECT_SOURCES
    ${ORIGIN}
    ${PRIVATE}
    ${DEVELOPER}
    ${CMAKE_CURRENT_SOURCE_DIR}/NexUs.qrc
)

if (NEXUS_BUILD_STATIC_LIB)
    set(LIB_TYPE "STATIC")
else ()
    set(LIB_TYPE "SHARED")
endif ()

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library(${PROJECT_NAME} ${LIB_TYPE}
        ${PROJECT_SOURCES}
    )
else ()
    add_library(${PROJECT_NAME} ${LIB_TYPE}
        ${PROJECT_SOURCES}
    )
endif ()

FILE(GLOB EXPORT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/Source/include/*.h)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source/private>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source/DeveloperComponents>
    $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
)

if (MINGW)
    set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
endif ()
if (MSVC)
    set(PRIVATE_MSVC_COMPILE_GENERAL_OPTIONS
        # /ifcOutput ${CMAKE_BINARY_DIR}/  # Generate interface definition file
        /permissive-
        # C++20 Modules Support
        /experimental:module  # Enable module support (MSVC 2019 16.8+)
        # /std:c++latest        # Use latest C++ standard for modules
        /Zc:wchar_t           # wchar_t as a primitive type
        /Zc:forScope          # for loop range consistency
        /Zc:inline            # Remove unused functions
        /W4                   # Warning Level 4
        /WX-                  # Treat warnings as errors, "WX" will not compile, due to the way the standard library's internal code is written
        /Zp8                  # 8-byte alignment
        /GF                   # Enable string pool
        /Gd                   # __cdecl calling convention
        /fp:precise           
        /FC                   # Full Path Diagnosis
        # BUG
        # If the settings, we cannot use coroutines and what Microsoft officially describes may not be consistent.
        # /await:strict         # Enable cooperative routine support, can use "/await", VS 2026 should replace it as "/await:strict"
        /openmp:experimental  # OpenMP support
        /errorReport:prompt   # Error Reporting Mode
        /diagnostics:column   # Diagnostic Information Format
        /Gm-                  # Disable minimal rebuild
        # https://learn.microsoft.com/zh-cn/cpp/build/reference/arch-x64?view=msvc-170
        /arch:AVX2            # Enable AVX2 instruction set
    )
    set(PUBLIC_MSVC_COMPILE_GENERAL_OPTIONS
        /utf-8                # character set
        /Zc:__cplusplus       # Correctly set the __cplusplus macroX
        /Zc:preprocessor
        /MP                   # Multiprocessor Compilation
        /sdl                  # SDL checks
        /EHsc                 # C Exception Handling
        /nologo
    )
    target_compile_options(${PROJECT_NAME} PUBLIC ${PUBLIC_MSVC_COMPILE_GENERAL_OPTIONS})
    target_compile_options(${PROJECT_NAME} PRIVATE ${PRIVATE_MSVC_COMPILE_GENERAL_OPTIONS})
    target_compile_options(${PROJECT_NAME} PRIVATE
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/JMC>"    # Just My Code debugging
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/ZI>"     # Edit and Continue
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/Od>"     # Disable optimization
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/RTC1>"   # Runtime error checks
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/O2>"    # Maximize speed
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/Ob2>"   # Inline any suitable function
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/GL>"    # Whole program optimization
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/GS>"    # Buffer security check
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/Qpar>"  # Auto-parallelization
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/Gy>"    # Function-level linking
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/Oi>"    # Intrinsic functions
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/O2>" 
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/Ob1>"
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/Zi>" 
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/GL>" 
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/GS>" 
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/Qpar>"
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/Gy>" 
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/Oi>" 
        "$<$<CONFIG:MinSizeRel>:/Zi>"
        "$<$<CONFIG:MinSizeRel>:/O1>"
    )
    
    target_link_options(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:/DEBUG>"
        "$<$<CONFIG:RelWithDebInfo>:/DEBUG:FULL>"
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/LTCG>"
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/LTCG>"
        "$<$<CONFIG:Release>:/DEBUG:NONE>"
        "$<$<CONFIG:MinSizeRel>:/DEBUG:FASTLINK>"
    )
    
    set_target_properties(${PROJECT_NAME} PROPERTIES
        DEBUG_POSTFIX "d"
        COMPILE_PDB_NAME ${PROJECT_NAME}
        COMPILE_PDB_NAME_DEBUG ${PROJECT_NAME}d
    )
endif ()

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::WidgetsPrivate
        D3D11
        DXGI
    )
else ()
    target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::WidgetsPrivate
    )
endif ()

set(INSTALL_INCLUDE_DIR include/${PROJECT_NAME})
set(INSTALL_LIB_DIR lib/${PROJECT_NAME})
set(INSTALL_BIN_DIR bin/${PROJECT_NAME})
set(INSTALL_SHARED_DIR shared/${PROJECT_NAME})

# 安装目标文件
install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
)

# 安装头文件
install(FILES ${EXPORT_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR})

# 安装PDB文件（仅Windows MSVC）
if(MSVC)
    install(FILES 
        $<TARGET_PDB_FILE:${PROJECT_NAME}>
        DESTINATION ${INSTALL_BIN_DIR}
        CONFIGURATIONS Debug RelWithDebInfo
    )
endif()

# 设置配置变量
set(INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/${INSTALL_INCLUDE_DIR})
set(LIBRARIES ${PROJECT_NAME})
set(LIB_DIR ${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR})

# 安装导出目标
install(
    EXPORT ${PROJECT_NAME}
    FILE ${PROJECT_NAME}Targets.cmake
    DESTINATION ${INSTALL_SHARED_DIR}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION 2.0.0
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${INSTALL_SHARED_DIR}
    PATH_VARS INCLUDE_DIRS LIBRARIES LIB_DIR
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake 
          ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake 
    DESTINATION ${INSTALL_SHARED_DIR}
)

# 自动安装后构建命令
if (AUTO_INSTALL_AFTER_BUILD)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} --install "${CMAKE_BINARY_DIR}" --config $<CONFIG>
        COMMENT "Installing ${PROJECT_NAME} to ${CMAKE_INSTALL_PREFIX}"
        VERBATIM
    )
endif()
