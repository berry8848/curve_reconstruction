cmake_minimum_required(VERSION 3.10)
project(LaneCurveDemo VERSION 1.0 LANGUAGES CXX)

# C++17を要求
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Eigenライブラリを探す
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# ========================================
# 外部ライブラリのパス設定
# ========================================

set(EXTERNAL_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/lib")
set(EXTERNAL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/include")

# ライブラリの存在確認
if(NOT EXISTS "${EXTERNAL_LIB_DIR}/libinterpolators.so")
    message(FATAL_ERROR 
        "libinterpolators.so not found!\n"
        "Please run: ./setup_libs.sh to copy libraries from liblanecurve/dist/")
endif()

if(NOT EXISTS "${EXTERNAL_LIB_DIR}/libutils.so")
    message(FATAL_ERROR 
        "libutils.so not found!\n"
        "Please run: ./setup_libs.sh to copy libraries from liblanecurve/dist/")
endif()

message(STATUS "Using libraries from: ${EXTERNAL_LIB_DIR}")
message(STATUS "Using headers from: ${EXTERNAL_INCLUDE_DIR}")

# インクルードディレクトリを指定
include_directories(${EXTERNAL_INCLUDE_DIR})

# ========================================
# 既存の共有ライブラリをインポート
# ========================================

# interpolatorsライブラリ
add_library(interpolators SHARED IMPORTED)
set_target_properties(interpolators PROPERTIES
    IMPORTED_LOCATION "${EXTERNAL_LIB_DIR}/libinterpolators.so"
    INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_INCLUDE_DIR}"
)

# utilsライブラリ
add_library(utils SHARED IMPORTED)
set_target_properties(utils PROPERTIES
    IMPORTED_LOCATION "${EXTERNAL_LIB_DIR}/libutils.so"
    INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_INCLUDE_DIR}"
)

# ========================================
# デモアプリケーション
# ========================================

# メインデモ
add_executable(demo_main src/demo_main.cpp)

target_link_libraries(demo_main 
    PRIVATE 
    interpolators
    utils
    Eigen3::Eigen
)

# RPATH設定
set_target_properties(demo_main PROPERTIES
    BUILD_RPATH "${EXTERNAL_LIB_DIR}"
    INSTALL_RPATH "$ORIGIN/../libs/lib"
)

# カスタムアプリケーション
add_executable(custom_app src/custom_app.cpp)

target_link_libraries(custom_app 
    PRIVATE 
    interpolators
    utils
    Eigen3::Eigen
)

set_target_properties(custom_app PROPERTIES
    BUILD_RPATH "${EXTERNAL_LIB_DIR}"
    INSTALL_RPATH "$ORIGIN/../libs/lib"
)

# ========================================
# インストール設定
# ========================================

install(TARGETS demo_main custom_app DESTINATION bin)