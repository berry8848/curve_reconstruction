# 補間手法のライブラリ
add_library(interpolators STATIC
    kalman_interpolator.cpp
)

target_include_directories(interpolators 
    PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(interpolators 
    PUBLIC 
    Eigen3::Eigen
)