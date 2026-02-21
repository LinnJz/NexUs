cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0087 NEW)
project(NexUsExample VERSION 0.1 LANGUAGES CXX)

include(Dependencies.cmake)
FILE(GLOB ORIGIN main.cpp Source/*.cpp Source/*.h)
FILE(GLOB MODELVIEW Source/ModelView/*.h Source/ModelView/*.cpp)
FILE(GLOB EXAMPLEPAGE Source/ExamplePage/*.h Source/ExamplePage/*.cpp)

source_group(ModelView FILES ${MODELVIEW})
source_group(ExamplePage FILES ${EXAMPLEPAGE})

set(PROJECT_SOURCES
    ${ORIGIN}
    ${MODELVIEW}
    ${EXAMPLEPAGE}
)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    #遍历所有资源文件
    file(GLOB_RECURSE RES_PATHS *.png *.jpg *.svg *.ico *.ttf *.webp *.js)
    foreach (filepath ${RES_PATHS})
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
        list(APPEND resource_files ${filename})
    endforeach (filepath)

    qt_add_resources(${PROJECT_NAME} "NexUsExample"
        RESOURCES PREFIX "/"
        FILES
        ${resource_files}
    )
else ()
    qt5_add_big_resources(PROJECT_SOURCES
        NexUsExample.qrc
    )
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
    )
endif ()

if (WIN32)
    target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source/ExamplePage>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source/ModelView>
    )
    if (BUILD_NXPACKETIO)
        target_link_libraries(${PROJECT_NAME} PRIVATE
            Qt${QT_VERSION_MAJOR}::Widgets
            NexUs
            NXPacketIO
        )
    else ()
        target_link_libraries(${PROJECT_NAME} PRIVATE
            Qt${QT_VERSION_MAJOR}::Widgets
            NexUs
        )
    endif ()

else ()
    target_include_directories(${PROJECT_NAME} PUBLIC
        ExamplePage
        ModelView
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        NexUs
    )
endif ()

if (MINGW)
    set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
endif ()
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:/ZI>"
        "$<$<CONFIG:Debug>:/Od>"
        "$<$<CONFIG:Release>:/O2>"
        "$<$<CONFIG:RelWithDebInfo>:/Zi>"
        "$<$<CONFIG:RelWithDebInfo>:/O2>"
        "$<$<CONFIG:MinSizeRel>:/Zi>"
        "$<$<CONFIG:MinSizeRel>:/O1>"
    )
    
    target_link_options(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:/DEBUG>"
        "$<$<CONFIG:RelWithDebInfo>:/DEBUG:FULL>"
        "$<$<CONFIG:Release>:/DEBUG:NONE>"
        "$<$<CONFIG:MinSizeRel>:/DEBUG:FASTLINK>"
    )
    
    set_target_properties(${PROJECT_NAME} PROPERTIES
        DEBUG_POSTFIX "d"
        COMPILE_PDB_NAME ${PROJECT_NAME}
        COMPILE_PDB_NAME_DEBUG ${PROJECT_NAME}d
        COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    )
endif ()

if (${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME})
endif ()
set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    VS_DEBUGGER_WORKING_DIRECTORY "${NEXUS_BIN_DIR}"
    VS_DEBUGGER_COMMAND "${NEXUS_BIN_DIR}/${PROJECT_NAME}.exe"
)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif ()

option(PACKAGE_NEXUSEXAMPLE "Package NexUsExample" OFF)
find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_SDK_DIR}/bin")
if (WIN32 AND WINDEPLOYQT_EXECUTABLE AND PACKAGE_NEXUSEXAMPLE)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                "$<TARGET_FILE:${PROJECT_NAME}>"
                --no-compiler-runtime
                --no-system-d3d-compiler
                --no-opengl-sw
        COMMENT "Running windeployqt for ${PROJECT_NAME}"
        VERBATIM
    )
endif ()
