cmake_minimum_required(VERSION 3.10)
project(LaneCurveRestoration VERSION 1.0 LANGUAGES CXX)

# C++17を要求
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 共有ライブラリのビルド設定
option(BUILD_SHARED_LIBS "Build shared libraries (.dll/.so)" ON)

# Windows DLLのエクスポート設定
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Eigenライブラリを探す
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# インクルードディレクトリを指定
include_directories(${PROJECT_SOURCE_DIR}/include)

# ========================================
# ライブラリの定義
# ========================================

# 補間手法のライブラリ
add_library(interpolators SHARED
    src/interpolators/kalman_interpolator.cpp
)

target_include_directories(interpolators 
    PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(interpolators 
    PUBLIC 
    Eigen3::Eigen
)

# ライブラリのバージョン設定
set_target_properties(interpolators PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
)

# ユーティリティライブラリ
add_library(utils SHARED
    src/utils/line_segment.cpp
    src/utils/image_utils.cpp
)

target_include_directories(utils 
    PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)

# ライブラリのバージョン設定
set_target_properties(utils PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
)

# ========================================
# 実行ファイルの定義
# ========================================

# メイン実行ファイル
add_executable(lane_curve src/main.cpp)

target_link_libraries(lane_curve 
    PRIVATE 
    interpolators
    utils
    Eigen3::Eigen
)

# External Application（共有ライブラリを使用）
add_executable(external_app src/external_app.cpp)

target_include_directories(external_app 
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(external_app 
    PRIVATE 
    interpolators
    utils
    Eigen3::Eigen
)

# RPATH設定（実行時にライブラリを見つけられるようにする）
set_target_properties(external_app PROPERTIES
    BUILD_RPATH "${CMAKE_BINARY_DIR}"
    INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
)

# ========================================
# インストール設定
# ========================================

# 実行ファイルのインストール
install(TARGETS lane_curve external_app 
    DESTINATION bin
)

# 共有ライブラリのインストール
install(TARGETS interpolators utils
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# ヘッダーファイルのインストール
install(DIRECTORY include/ 
    DESTINATION include
)