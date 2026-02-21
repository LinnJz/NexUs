cmake_minimum_required(VERSION 3.5)

project(NXPacketIO VERSION 2.0.0 LANGUAGES CXX)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_definitions(-DNXPACKETIO_LIBRARY)
option(NXPACKETIO_BUILD_STATIC_LIB "Build static library." OFF)

FILE(GLOB ORIGIN *.h *.cpp)
FILE(GLOB PacketIO PacketIO/*.h PacketIO/*.cpp)
FILE(GLOB Genio Genio/*.h Genio/*.cpp)
FILE(GLOB Util Util/*.h Util/*.cpp)
FILE(GLOB XIO XIO/*.h XIO/*.cpp)

source_group(PacketIO FILES ${PacketIO})
source_group(Genio FILES ${Genio})
source_group(Util FILES ${Util})
source_group(XIO FILES ${XIO})

set(PROJECT_SOURCES
    ${ORIGIN}
    ${PacketIO}
    ${Genio}
    ${Util}
    ${XIO}
)

if (NXPACKETIO_BUILD_STATIC_LIB)
    set(LIB_TYPE "STATIC")
else ()
    set(LIB_TYPE "SHARED")
endif ()

add_library(${PROJECT_NAME} ${LIB_TYPE}
    ${PROJECT_SOURCES}
)

if (MINGW)
    set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
endif ()
if (MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)

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

FILE(GLOB ORIGIN_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
FILE(GLOB XIO_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/XIO/*.h)
FILE(GLOB GENIO_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/GenIO/*.h)
FILE(GLOB PACKETIO_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/PacketIO/*.h)
FILE(GLOB UTIL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/Util/*.h)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/GenIO
    ${CMAKE_CURRENT_SOURCE_DIR}/PacketIO
    ${CMAKE_CURRENT_SOURCE_DIR}/Util
    ${CMAKE_CURRENT_SOURCE_DIR}/XIO
)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        ws2_32
    )
endif ()

# 设置安装路径
set(INSTALL_INCLUDE_DIR include/${PROJECT_NAME})
set(INSTALL_LIB_DIR lib/${PROJECT_NAME})
set(INSTALL_BIN_DIR bin/${PROJECT_NAME})
set(INSTALL_SHARED_DIR shared/${PROJECT_NAME})

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
)

# 安装头文件
install(FILES ${ORIGIN_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR})
install(FILES ${XIO_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR}/XIO)
install(FILES ${GENIO_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR}/GenIO)
install(FILES ${PACKETIO_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR}/PacketIO)
install(FILES ${UTIL_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR}/Util)

# 安装PDB文件（仅Windows MSVC）
if(MSVC)
    install(FILES 
        $<TARGET_PDB_FILE:${PROJECT_NAME}>
        DESTINATION ${INSTALL_BIN_DIR}
        CONFIGURATIONS Debug RelWithDebInfo
    )
endif()

set(INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/${INSTALL_INCLUDE_DIR})
set(LIBRARIES ${PROJECT_NAME})
set(LIB_DIR ${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR})

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

if (AUTO_INSTALL_AFTER_BUILD)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} --install . --config $<CONFIG>
        COMMENT "Installing ${PROJECT_NAME}"
    )
endif()
