# 首先创建目标目录
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Resource/Image)

# 定义要下载的图片列表
set(IMAGE_FILES
    "Cirno.jpg"
    "MicaBase.png"
    "Moon.jpg"
    "noise.png"
)

# 逐个下载图片（仅当文件不存在时）
foreach(IMG_FILE IN LISTS IMAGE_FILES)
    set(DEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Resource/Image/${IMG_FILE}")
    
    if(NOT EXISTS ${DEST_PATH})
        message(STATUS "Downloading ${IMG_FILE}...")
        file(DOWNLOAD
            "https://raw.githubusercontent.com/Liniyous/ElaWidgetTools/main/ElaWidgetTools/Image/${IMG_FILE}"
            "${DEST_PATH}"
            SHOW_PROGRESS
            TLS_VERIFY OFF
        )
    else()
        message(STATUS "${IMG_FILE} already exists, skipping download.")
    endif()
endforeach()