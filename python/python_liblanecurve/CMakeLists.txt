cmake_minimum_required(VERSION 3.10)
project(LibLaneCurvePython VERSION 1.0 LANGUAGES CXX)

# C++17を要求
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Eigenライブラリを探す
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# インクルードディレクトリを指定
include_directories(${PROJECT_SOURCE_DIR}/include)

# ========================================
# 補間手法ライブラリ (Pythonラッパー付き)
# ========================================
add_library(interpolators SHARED
    src/interpolators/kalman_interpolator.cpp
    src/wrappers/interpolators_wrapper.cpp
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

set_target_properties(interpolators PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
    OUTPUT_NAME "interpolators"
)

# ========================================
# ユーティリティライブラリ (Pythonラッパー付き)
# ========================================
add_library(utils SHARED
    src/utils/line_segment.cpp
    src/utils/image_utils.cpp
    src/wrappers/utils_wrapper.cpp
)

target_include_directories(utils 
    PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(utils PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
    OUTPUT_NAME "utils"
)

# ========================================
# インストール設定
# ========================================
install(TARGETS interpolators utils
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ 
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# ========================================
# ビルド情報の表示
# ========================================
message(STATUS "========================================")
message(STATUS "LibLaneCurvePython Configuration")
message(STATUS "========================================")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Eigen3 version: ${Eigen3_VERSION}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "========================================")
