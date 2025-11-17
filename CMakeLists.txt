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

# サブディレクトリを追加
add_subdirectory(src/interpolators)
add_subdirectory(src/utils)

# 実行ファイル
add_executable(lane_curve src/main.cpp)

# ライブラリをリンク
target_link_libraries(lane_curve 
    PRIVATE 
    interpolators
    utils
    Eigen3::Eigen
)

# Simple External Application（共有ライブラリを使用）
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

# RPATH設定
set_target_properties(external_app PROPERTIES
    BUILD_RPATH "${CMAKE_BINARY_DIR}/src/interpolators:${CMAKE_BINARY_DIR}/src/utils"
    INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
)

# インストール設定
install(TARGETS lane_curve external_app external_app DESTINATION bin)

# 共有ライブラリもインストール
install(TARGETS interpolators utils
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# ヘッダーファイルもインストール
install(DIRECTORY include/ DESTINATION include)