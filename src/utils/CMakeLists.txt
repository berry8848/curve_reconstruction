# ユーティリティライブラリ
add_library(utils STATIC
    line_segment.cpp
    image_utils.cpp
)

target_include_directories(utils 
    PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)