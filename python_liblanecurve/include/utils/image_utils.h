#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <string>
#include <vector>
#include <utility>
#include "interpolators/line_segment.h"

// PPM画像として出力
void saveToPPM(const std::string& filename, 
               const std::vector<LineSegment>& segments,
               const std::vector<std::pair<double, double>>& curve_points,
               int width = 800, int height = 600);

#endif // IMAGE_UTILS_H