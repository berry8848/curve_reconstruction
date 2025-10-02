cmake_minimum_required(VERSION 3.10)
project(LaneCurveRestoration VERSION 1.0 LANGUAGES CXX)

# C++17を要求
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# インストール設定（オプション）
install(TARGETS lane_curve DESTINATION bin)