function(GetVcpkgPackageSearchPath out_var linkage)
    string(TOUPPER "${linkage}" LINKAGE_MODE)
    if(LINKAGE_MODE STREQUAL "STATIC")
        set(search_path "${VCPKG_STATIC_CMAKE_PATH}")
    elseif(LINKAGE_MODE STREQUAL "DYNAMIC")
        set(search_path "${VCPKG_DYNAMIC_CMAKE_PATH}")
    else()
        message(FATAL_ERROR "Unsupported linkage mode: ${linkage}")
    endif()

    if(NOT EXISTS "${search_path}")
        message(FATAL_ERROR "Vcpkg package path not found: ${search_path}")
    endif()

    set(${out_var} "${search_path}" PARENT_SCOPE)
endfunction()

function(FindVcpkgPackage package_name)
    set(options REQUIRED QUIET CONFIG)
    set(one_value_args LINKAGE VERSION)
    set(multi_value_args COMPONENTS)
    cmake_parse_arguments(FVP "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

    if(NOT FVP_LINKAGE)
        set(FVP_LINKAGE "DYNAMIC")
    endif()

    string(TOUPPER "${FVP_LINKAGE}" LINKAGE_MODE)
    if(LINKAGE_MODE STREQUAL "STATIC")
        set(vcpkg_triplet "${VCPKG_STATIC_TRIPLET}")
    else()
        set(vcpkg_triplet "${VCPKG_DYNAMIC_TRIPLET}")
    endif()

    set(vcpkg_installed_dir "${VCPKG_ROOT}/installed")
    set(VCPKG_INSTALLED_DIR "${vcpkg_installed_dir}")
    set(_VCPKG_INSTALLED_DIR "${vcpkg_installed_dir}")
    set(VCPKG_TARGET_TRIPLET "${vcpkg_triplet}")
    set(VCPKG_INSTALLED_DIR "${vcpkg_installed_dir}" CACHE PATH "" FORCE)
    set(_VCPKG_INSTALLED_DIR "${vcpkg_installed_dir}" CACHE PATH "" FORCE)
    set(VCPKG_TARGET_TRIPLET "${vcpkg_triplet}" CACHE STRING "" FORCE)

    GetVcpkgPackageSearchPath(package_search_path "${FVP_LINKAGE}")

    set(find_args "${package_name}")
    if(FVP_VERSION)
        list(APPEND find_args "${FVP_VERSION}")
    endif()
    if(FVP_REQUIRED)
        list(APPEND find_args REQUIRED)
    endif()
    if(FVP_QUIET)
        list(APPEND find_args QUIET)
    endif()
    if(FVP_CONFIG)
        list(APPEND find_args CONFIG)
    endif()
    if(FVP_COMPONENTS)
        list(APPEND find_args COMPONENTS ${FVP_COMPONENTS})
    endif()
    list(APPEND find_args HINTS "${package_search_path}" NO_DEFAULT_PATH)

    unset(${package_name}_DIR CACHE)
    unset(${package_name}_ROOT CACHE)
    find_package(${find_args})
endfunction()

function(CopyTargetDependentLibs target_name lib_base_names release_lib_path)
    if(NOT TARGET ${target_name})
        message(WARNING "Target ${target_name} does not exist, skipping library copy")
        return()
    endif()

    set(debug_lib_path "${ARGV3}")
    if(NOT debug_lib_path)
        set(debug_lib_path "${release_lib_path}")
    endif()

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(search_path "${debug_lib_path}")
    else()
        set(search_path "${release_lib_path}")
    endif()

    if(NOT EXISTS "${search_path}")
        message(WARNING "Library search path does not exist: ${search_path}")
        return()
    endif()

    # Process each library
    foreach(LIB_BASE_NAME ${lib_base_names})
        # Platform-specific file pattern (handle versioned .so on Linux)
        if(UNIX AND NOT APPLE)  # Linux: match .so, .so.1, .so.1.2.3 etc.
            set(LIB_PATTERN "${LIB_BASE_NAME}${SHARED_LIB_SUFFIX}*")
        else()  # Windows/macOS: exact suffix match (no version wildcard)
            set(LIB_PATTERN "${LIB_BASE_NAME}${SHARED_LIB_SUFFIX}")
        endif()

        # Find all matching library files
        file(GLOB LIB_FILES 
            "${search_path}/${LIB_PATTERN}"
            LIST_DIRECTORIES false  # Exclude directories from results
        )

        unset(FOUND_LIBS)
        # Filter out invalid files (only keep regular files)
        foreach(LIB_FILE ${LIB_FILES})
            if(EXISTS "${LIB_FILE}" AND NOT IS_DIRECTORY "${LIB_FILE}")
                get_filename_component(FILE_EXT "${LIB_FILE}" LAST_EXT)
                if(FILE_EXT MATCHES "\\.(dll|so|dylib|lib|a)$" OR 
                   (WIN32 AND (NOT FILE_EXT OR FILE_EXT STREQUAL ".dll")))
                    # Copy library to target output directory (only if different)
                    add_custom_command(TARGET ${target_name} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            "${LIB_FILE}"
                            $<TARGET_FILE_DIR:${target_name}>
                        COMMENT "Copying ${LIB_FILE} to output directory"
                    )
                    list(APPEND FOUND_LIBS "${LIB_FILE}")
                endif()
            endif()
        endforeach()

        if(NOT FOUND_LIBS)
            message(WARNING "No shared library files found for pattern: ${search_path}/${LIB_PATTERN}")
        else()
            message(STATUS "Copied libraries for ${LIB_BASE_NAME}: ${FOUND_LIBS}")
        endif()
    endforeach()
endfunction()

function(SetTargetRuntimeDependencyPath target_name)
  if(WIN32)
    # For Visual Studio, set debug environment path
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      set(vcpkg_bin_path "${VCPKG_DEBUG_BIN_PATH}")
    else()
      set(vcpkg_bin_path "${VCPKG_BIN_PATH}")
    endif()

    set_target_properties(${target_name} PROPERTIES
      VS_DEBUGGER_ENVIRONMENT "PATH=${vcpkg_bin_path};$ENV{PATH}"
    )
  else()
    # # For Unix-like (Linux/macOS), set RPATH
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      set(rpath "${VCPKG_DEBUG_BIN_PATH}:${VCPKG_DEBUG_BIN_PATH}/../lib")
    else()
      set(rpath "${VCPKG_BIN_PATH}:${VCPKG_BIN_PATH}/../lib")
    endif()
        
    set_target_properties(${target_name} PROPERTIES
      BUILD_WITH_INSTALL_RPATH TRUE
      INSTALL_RPATH "${rpath}"
    )
  endif()
endfunction()


function(IMPORTED_LIBRARY target_name lib_base_name library_dir)
    if(TARGET ${target_name})
        return()
    endif()

    set(lib_path "${library_dir}/${lib_base_name}${LIB_SUFFIX}")
    set(shared_lib_base "${library_dir}/${lib_base_name}${SHARED_LIB_SUFFIX}")

    # Handle wildcard for versioned .so files (Linux only)
    set(shared_lib_path "${shared_lib_base}")
    if(UNIX AND NOT APPLE)
        # Use glob to find any versioned .so (e.g., .so, .so.3, .so.3.3.0)
        file(GLOB versioned_libs "${shared_lib_base}*")
        if(versioned_libs)
            # Pick the first matched library (prioritize full version if exists)
            list(SORT versioned_libs COMPARE NATURAL ORDER DESCENDING)
            list(GET versioned_libs 0 shared_lib_path)
        endif()
    endif()

    if(NOT EXISTS "${shared_lib_path}")
        message(WARNING "❌ Shared library not found: ${shared_lib_base}*")
        return()
    endif()

    # Create imported target
    add_library(${target_name} SHARED IMPORTED)
    
    # Set platform-specific properties
    if(WIN32)
        set_target_properties(${target_name} PROPERTIES
            IMPORTED_LOCATION "${shared_lib_path}"
            IMPORTED_IMPLIB "${lib_path}"
            IMPORTED_LOCATION_DEBUG "${shared_lib_path}"
            IMPORTED_IMPLIB_DEBUG "${lib_path}"
            IMPORTED_LOCATION_RELEASE "${shared_lib_path}"
            IMPORTED_IMPLIB_RELEASE "${lib_path}"
            IMPORTED_LOCATION_RELWITHDEBINFO "${shared_lib_path}"
            IMPORTED_IMPLIB_RELWITHDEBINFO "${lib_path}"
            IMPORTED_LOCATION_MINSIZEREL "${shared_lib_path}"
            IMPORTED_IMPLIB_MINSIZEREL "${lib_path}"
        )
    else()
        set_target_properties(${target_name} PROPERTIES
            IMPORTED_LOCATION "${shared_lib_path}"  # .so/.dylib path
        )
    endif()

    message(STATUS "✅ Found library: ${shared_lib_path}")
endfunction()
