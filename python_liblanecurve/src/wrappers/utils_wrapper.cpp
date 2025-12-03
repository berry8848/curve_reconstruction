#include "python_wrapper.h"
#include "interpolators/line_segment.h"
#include "utils/image_utils.h"
#include <vector>
#include <utility>

extern "C" {

void extractPoints(const CLineSegment* segments, int num_segments, 
                   CPoint* points, int* num_points) {
    // C配列からC++のvectorに変換
    std::vector<LineSegment> cpp_segments;
    cpp_segments.reserve(num_segments);
    
    for (int i = 0; i < num_segments; i++) {
        cpp_segments.push_back({
            segments[i].x1,
            segments[i].y1,
            segments[i].x2,
            segments[i].y2
        });
    }
    
    // C++の関数を呼び出し
    std::vector<std::pair<double, double>> cpp_points;
    ::extractPoints(cpp_segments, cpp_points);
    
    // 結果をC配列に変換
    *num_points = static_cast<int>(cpp_points.size());
    for (size_t i = 0; i < cpp_points.size(); i++) {
        points[i].x = cpp_points[i].first;
        points[i].y = cpp_points[i].second;
    }
}

void saveToPPM(const char* filename, 
               const CLineSegment* segments, int num_segments,
               const CPoint* curve_points, int num_curve_points,
               int width, int height) {
    // C配列からC++のvectorに変換
    std::vector<LineSegment> cpp_segments;
    cpp_segments.reserve(num_segments);
    
    for (int i = 0; i < num_segments; i++) {
        cpp_segments.push_back({
            segments[i].x1,
            segments[i].y1,
            segments[i].x2,
            segments[i].y2
        });
    }
    
    std::vector<std::pair<double, double>> cpp_curve_points;
    cpp_curve_points.reserve(num_curve_points);
    
    for (int i = 0; i < num_curve_points; i++) {
        cpp_curve_points.push_back({curve_points[i].x, curve_points[i].y});
    }
    
    // C++の関数を呼び出し
    ::saveToPPM(std::string(filename), cpp_segments, cpp_curve_points, width, height);
}

} // extern "C"
