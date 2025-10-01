cmake_minimum_required(VERSION 3.16)
project(curve_reconstruction CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_library(cr_lane
    src/lane/bev_warp.cpp
    src/lane/line_detect.cpp
    src/lane/lane_fit.cpp
    src/lane/lane_track.cpp
    src/utils/calibration.cpp
    src/utils/viz.cpp
)
target_include_directories(cr_lane PUBLIC include)
target_link_libraries(cr_lane PUBLIC ${OpenCV_LIBS})

add_executable(cr_demo src/main.cpp)
target_link_libraries(cr_demo PRIVATE cr_lane)

enable_testing()
add_subdirectory(tests)
