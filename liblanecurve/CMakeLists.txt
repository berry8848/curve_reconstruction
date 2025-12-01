cmake_minimum_required(VERSION 3.10)
project(LaneCurveLibrary VERSION 1.0 LANGUAGES CXX)

# C++17を要求
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Eigenライブラリを探す
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# インクルードディレクトリを指定
include_directories(${PROJECT_SOURCE_DIR}/include)

# ========================================
# 補間手法ライブラリ
# ========================================
add_library(interpolators SHARED
    src/interpolators/kalman_interpolator.cpp
)

target_include_directories(interpolators 
    PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(interpolators 
    PUBLIC 
    Eigen3::Eigen
)

# set_target_properties(interpolators PROPERTIES
#     VERSION 1.0.0
#     SOVERSION 1
#     PUBLIC_HEADER "include/interpolators/interpolator.h;include/interpolators/kalman_interpolator.h;include/interpolators/line_segment.h"
# )

# ========================================
# ユーティリティライブラリ
# ========================================
add_library(utils SHARED
    src/utils/line_segment.cpp
    src/utils/image_utils.cpp
)

target_include_directories(utils 
    PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# set_target_properties(utils PROPERTIES
#     VERSION 1.0.0
#     SOVERSION 1
#     PUBLIC_HEADER "include/utils/image_utils.h"
# )

# ========================================
# インストール設定
# ========================================
# install(TARGETS interpolators utils
#     LIBRARY DESTINATION lib
#     ARCHIVE DESTINATION lib
#     RUNTIME DESTINATION bin
#     PUBLIC_HEADER DESTINATION include
# )

# install(DIRECTORY include/ 
#     DESTINATION include
#     FILES_MATCHING PATTERN "*.h"
# )