# -----------------------------------------------------------------------------
# Compiler-Specific Configuration(General)
# -----------------------------------------------------------------------------
if(COMPILER_MSVC)
    # MSVC Runtime Library Configuration
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    
    # Debug Information Format
    if(POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug>:EditAndContinue>,$<$<CONFIG:RelWithDebInfo>:ProgramDatabase>>")
    endif()

    # 所有 MSVC 编译选项（完整列表）
    set(MSVC_COMPILE_GENERAL_OPTIONS
        /utf-8                # 字符集 UTF-8
        /permissive-          # 严格标准一致性
        /experimental:module  # 启用 C++20 模块支持（若库以模块方式提供接口则必须）
        /Zc:__cplusplus       # 正确设置 __cplusplus 宏
        /Zc:wchar_t           # wchar_t 作为内置类型
        /Zc:forScope          # 标准 for 循环作用域
        /Zc:inline            # 移除未使用的函数（优化选项，不对外）
        /Zc:preprocessor      # 标准预处理器行为
        /W4                   # 警告级别 4
        /WX-                  # 不将警告视为错误
        /MP                   # 多进程编译（仅加速）
        /Zp8                  # 8 字节结构体对齐
        /GF                   # 启用字符串池（优化）
        /Gd                   # __cdecl 调用约定
        /fp:precise           # 精确浮点模型
        /FC                   # 诊断信息中显示完整路径
        /openmp:experimental  # 实验性 OpenMP 支持
        /sdl                  # 安全检查
        /EHsc                 # C++ 同步异常处理模型
        /errorReport:prompt   # 错误报告模式
        /diagnostics:column   # 诊断信息格式（列号）
        /nologo               # 隐藏版权信息
        /Gm-                  # 禁用最小重建
        /arch:AVX2            # 启用 AVX2 指令集
    )

    # 必须传播给使用者的选项（PUBLIC / INTERFACE）
    set(PUBLIC_MSVC_COMPILE_OPTIONS
        /utf-8
        /permissive-
        /experimental:module      # 若库导出模块接口
        /Zc:__cplusplus
        /Zc:wchar_t
        /Zc:forScope
        /Zc:preprocessor
        /Zp8                      # 对齐方式影响结构体布局
        /Gd                        # 调用约定影响函数符号
        /fp:precise                # 浮点行为影响内联计算
        /EHsc                       # 异常处理模型必须匹配
        /arch:AVX2                  # 若头文件使用 AVX2 intrinsic
        /wd5103
    )

    # Derive private MSVC options by removing public ones from the full list
    set(PRIVATE_MSVC_COMPILE_GENERAL_OPTIONS ${MSVC_COMPILE_GENERAL_OPTIONS})
    list(REMOVE_ITEM PRIVATE_MSVC_COMPILE_GENERAL_OPTIONS ${PUBLIC_MSVC_COMPILE_OPTIONS})

    # Apply public MSVC options to the target
    target_compile_options(${PROJECT_NAME} PUBLIC
        "$<$<CXX_COMPILER_ID:MSVC>:${PUBLIC_MSVC_COMPILE_OPTIONS}>"
    )

    # Apply private MSVC options to the target
    target_compile_options(${PROJECT_NAME} PRIVATE
        "$<$<CXX_COMPILER_ID:MSVC>:${PRIVATE_MSVC_COMPILE_GENERAL_OPTIONS}>"
    )

    # Compiler definitions (all private)
    target_compile_definitions(${PROJECT_NAME} PUBLIC
        _UNICODE
        UNICODE
        NOMINMAX
        _CRT_SECURE_NO_WARNINGS
        _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
    )
else()
    # GCC/Clang Configuration
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
        message(STATUS "Found ccache program: ${CCACHE_PROGRAM}")
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PROGRAM})
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_PROGRAM})
    endif()

    # g++:
    # g++ -fmodules-ts -x c++-system-header iostream
    # g++ -fmodules-ts fig16_01.cpp -o fig16_01

    
    # msvc module file extension is ixx, clang is cppm, 
    # clang needs to use a command to recognize the ixx suffix, like [ -x c++-module welcome.ixx ]
    # g++ -fmodules-ts -c -x c++ deitel.math.ixx [ -x c++ ]

    # clang++ (-16 may need to be removed or changed based on your clang++ version):
    # clang++-16 -std=c++20 -x c++-system-header --precompile string -o string.pcm
    # clang++-16 -std=c++20 -x c++-system-header --precompile iostream -o iostream.pcm
    # clang++-16 -std=c++20 -fmodule-file=string.pcm -x c++-module welcome.ixx --precompile -o welcome.pcm
    # clang++-16 -std=c++20 -fmodule-file=iostream.pcm fig16_03.cpp -fprebuilt-module-path=. string-pcm welcome.pcm -o fig16_03

    # GCC/Clang General Compilation Options (equivalent to MSVC)
    set(GCC_CLANG_COMPILE_GENERAL_OPTIONS
        # Character set (equivalent to /utf-8)
        -finput-charset=UTF-8
        -fexec-charset=UTF-8
        
        # Strict standard compliance (equivalent to /permissive-)
        -pedantic-errors
        
        # C++20 Modules Support (equivalent to /experimental:module)
        # -fmodules-ts will be added per compiler below
        
        # Correct __cplusplus macro (equivalent to /Zc:__cplusplus)
        # Handled by -std=c++20
        
        # Warning levels (equivalent to /W4)
        -Wall
        -Wextra
        -Wpedantic
        
        # Treat warnings as errors (equivalent to /WX- but we enable it)
        -Werror
        
        # Alignment (equivalent to /Zp8)
        -malign-double
        
        # String pooling (equivalent to /GF)
        -fmerge-all-constants
        
        # Floating point precision (equivalent to /fp:precise)
        -fno-fast-math
        
        # OpenMP support (equivalent to /openmp:experimental)
        -fopenmp
        
        # Security checks (equivalent to /sdl)
        -fstack-protector-strong
        -D_FORTIFY_SOURCE=2
        
        # Exception handling (equivalent to /EHsc, default in GCC/Clang)
        -fexceptions
        
        # Diagnostic format (equivalent to /diagnostics:column)
        -fdiagnostics-color=always
        -fdiagnostics-show-option
        -fno-omit-frame-pointer
        
        # AVX2 instruction set (equivalent to /arch:AVX2)
        -mavx2
        -mfma
    )

    # 必须传播给使用者的选项（PUBLIC / INTERFACE）
    set(PUBLIC_GCC_CLANG_COMPILE_OPTIONS
        -finput-charset=UTF-8          # 字符集
        -fexec-charset=UTF-8
        -pedantic-errors                # 严格标准一致性
        -malign-double                   # 结构体对齐
        -fno-fast-math                    # 精确浮点模型
        -fexceptions                       # 异常处理模型
        -mavx2                              # AVX2 指令集（若头文件使用 intrinsic）
        -mfma
    )

    # 仅内部使用的选项（PRIVATE）= 全部选项 - 公共选项
    set(PRIVATE_GCC_CLANG_COMPILE_OPTIONS ${GCC_CLANG_COMPILE_GENERAL_OPTIONS})
    list(REMOVE_ITEM PRIVATE_GCC_CLANG_COMPILE_OPTIONS ${PUBLIC_GCC_CLANG_COMPILE_OPTIONS})
    
     set(EXTRA_WARNING_SUPPRESSIONS
        -Wno-unused-parameter
        -Wno-unused-variable
        -Wno-unused-local-typedef
    )

    # 模块支持选项（若项目不使用模块接口，设为 PRIVATE；否则需考虑传播）
    # 这里假设项目不使用模块，故设为 PRIVATE
    # GCC/Clang Modules Support - disabled as project doesn't use modules
    set(MODULE_SUPPORT_OPTIONS_GCC
        -fmodules-ts
        -fmodule-mapper=inline
    )
    set(MODULE_SUPPORT_OPTIONS_CLANG
        -fmodules
        -fbuiltin-module-map
        -fimplicit-module-maps
        -fprebuilt-module-path=${CMAKE_CURRENT_BINARY_DIR}
    )

    # Apply GCC/Clang general options (all private)
    target_compile_options(${PROJECT_NAME} PUBLIC
        "$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:${PUBLIC_GCC_CLANG_COMPILE_OPTIONS}>"
    )
    target_compile_options(${PROJECT_NAME} PRIVATE
        "$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:${PRIVATE_GCC_CLANG_COMPILE_OPTIONS}>"
    )

    # Additional warning suppressions (private)
    target_compile_options(${PROJECT_NAME} PRIVATE
        "$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:${EXTRA_WARNING_SUPPRESSIONS}>"
    )

    # Module support options (private)
    if(COMPILER_GCC)
        target_compile_options(${PROJECT_NAME} PUBLIC
            "$<$<CXX_COMPILER_ID:GNU>:${MODULE_SUPPORT_OPTIONS_GCC}>"
        )
    elseif(COMPILER_CLANG)
        target_compile_options(${PROJECT_NAME} PUBLIC
            "$<$<CXX_COMPILER_ID:Clang>:${MODULE_SUPPORT_OPTIONS_CLANG}>"
        )
    endif()
endif()

# Stack size linker options (private)
if(WIN32)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_link_options(${PROJECT_NAME} PRIVATE "$<$<CXX_COMPILER_ID:Clang>:-Wl,/STACK:4194304>")
    else()
        target_link_options(${PROJECT_NAME} PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/STACK:4194304>")
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_link_options(${PROJECT_NAME} PRIVATE "$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wl,-z,stack-size=4194304>")
endif()

# -----------------------------------------------------------------------------
# Build Type Configuration
# -----------------------------------------------------------------------------

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Build Type: Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG _DEBUG)
    
    if(COMPILER_MSVC)
        # MSVC Debug compilation options (private)
        target_compile_options(${PROJECT_NAME} PRIVATE
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/JMC>"    # Just My Code debugging
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/ZI>"     # Edit and Continue
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/Od>"     # Disable optimization
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/RTC1>"   # Runtime error checks
        )
        
        # MSVC Debug link options (private)
        target_link_options(${PROJECT_NAME} PRIVATE
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/DEBUG>"
        )
    else()
        # GCC/Clang Debug compilation options (private)
        target_compile_options(${PROJECT_NAME} PRIVATE
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Debug>>:-g>"
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Debug>>:-O0>"
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Debug>>:-fno-omit-frame-pointer>"
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Debug>>:-fno-inline>"
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Debug>>:-fno-optimize-sibling-calls>"
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Debug>>:-fno-eliminate-unused-debug-types>"
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Debug>>:-fno-common>"
            #"$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Debug>>:-fno-strict-aliasing>"
        )
        
        # GCC/Clang Debug link options (private)
        target_link_options(${PROJECT_NAME} PRIVATE
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Debug>>:-g>"
        )
    endif()
    
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Build Type: Release")
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG _NDEBUG)
    
    if(COMPILER_MSVC)
        # MSVC Release compilation options (private)
        target_compile_options(${PROJECT_NAME} PRIVATE
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/O2>"    # Maximize speed
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/Ob2>"   # Inline any suitable function
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/GL>"    # Whole program optimization
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/GS>"    # Buffer security check
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/Qpar>"  # Auto-parallelization
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/Gy>"    # Function-level linking
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/Oi>"    # Intrinsic functions
        )
        
        # MSVC Release link options (private)
        target_link_options(${PROJECT_NAME} PRIVATE
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/LTCG>"  # Link-time code generation
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/DEBUG:NONE>"
        )
    else()
        # GCC/Clang Release compilation options (private)
        target_compile_options(${PROJECT_NAME} PRIVATE
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Release>>:-O3>"
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Release>>:-flto>"
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Release>>:-finline-functions>"
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Release>>:-funroll-loops>"
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Release>>:-fomit-frame-pointer>"
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Release>>:-ffast-math>"
        )
        
        # GCC/Clang Release link options (private)
        target_link_options(${PROJECT_NAME} PRIVATE
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Release>>:-flto>"
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Release>>:-s>"
        )
    endif()
    
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    message(STATUS "Build Type: RelWithDebInfo")
    # No specific definitions added in original for RelWithDebInfo
    
    if(COMPILER_MSVC)
        # MSVC RelWithDebInfo compilation options (private)
        target_compile_options(${PROJECT_NAME} PRIVATE
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/O2>" 
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/Ob1>"
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/Zi>" 
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/GL>" 
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/GS>" 
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/Qpar>"
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/Gy>" 
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/Oi>" 
        )
        
        # MSVC RelWithDebInfo link options (private)
        target_link_options(${PROJECT_NAME} PRIVATE
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/LTCG>"
            "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/DEBUG:FULL>"
        )
    else()
        # GCC/Clang RelWithDebInfo compilation options (private)
        target_compile_options(${PROJECT_NAME} PRIVATE
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:RelWithDebInfo>>:-O2>"
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:RelWithDebInfo>>:-g>"
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:RelWithDebInfo>>:-fno-omit-frame-pointer>"
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:RelWithDebInfo>>:-flto>"
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:RelWithDebInfo>>:-finline-functions-called-once>"
        )
        
        # GCC/Clang RelWithDebInfo link options (private)
        target_link_options(${PROJECT_NAME} PRIVATE
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:RelWithDebInfo>>:-flto>"
            "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:RelWithDebInfo>>:-g>"
        )
    endif()
    
else()
    message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
endif()

# -----------------------------------------------------------------------------
# Sanitizers Configuration
# -----------------------------------------------------------------------------
if(USE_SANITIZERS)
    if(COMPILER_CLANG OR COMPILER_GCC)
        target_compile_options(${PROJECT_NAME} PRIVATE
            "$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-fsanitize=address>"
            "$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-fsanitize=undefined>"
            "$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-fno-omit-frame-pointer>"
        )
        target_link_options(${PROJECT_NAME} PRIVATE
            "$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-fsanitize=address>"
            "$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-fsanitize=undefined>"
        )
        message(STATUS "Sanitizers enabled")
    else()
        message(WARNING "Sanitizers only supported for Clang/GCC")
    endif()
endif()


if (MINGW)
    set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
endif ()
if (COMPILER_MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        DEBUG_POSTFIX "d"
        COMPILE_PDB_NAME ${PROJECT_NAME}
        COMPILE_PDB_NAME_DEBUG ${PROJECT_NAME}d
    )
endif()