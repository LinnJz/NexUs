cmake_minimum_required(VERSION 3.5)

project(NexUs VERSION 1.0.0 LANGUAGES CXX)

FILE(GLOB ORIGIN Source/include/*.h Source/*.cpp)
FILE(GLOB PRIVATE Source/private/*.h Source/private/*.cpp)
FILE(GLOB DEVELOPER Source/DeveloperComponents/*.h Source/DeveloperComponents/*.cpp)

FILE(GLOB EXPORT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/Source/include/*.h)

source_group(include FILES ${ORIGIN})
source_group(private FILES ${PRIVATE})
source_group(DeveloperComponents FILES ${DEVELOPER})

set(PROJECT_SOURCES
    ${ORIGIN}
    ${PRIVATE}
    ${DEVELOPER}
    ${CMAKE_CURRENT_SOURCE_DIR}/NexUs.qrc
)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library(${PROJECT_NAME} ${NEXUS_LIB_TYPE}
        ${PROJECT_SOURCES}
    )
else ()
    add_library(${PROJECT_NAME} ${NEXUS_LIB_TYPE}
        ${PROJECT_SOURCES}
    )
endif ()

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source/private>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source/DeveloperComponents>
    $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
)

target_compile_definitions(${PROJECT_NAME} PRIVATE 
  -DNEXUS_LIBRARY
)

include(../CMake/TargetCompilerConfig.cmake)
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
    VERSION 1.0.0
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
