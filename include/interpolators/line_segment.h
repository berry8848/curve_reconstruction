#ifndef LINE_SEGMENT_H
#define LINE_SEGMENT_H

#include <vector>
#include <utility>

// 線分を表す構造体
struct LineSegment {
    double x1, y1, x2, y2;
};

// 線分の中点を抽出
void extractPoints(const std::vector<LineSegment>& segments, 
                   std::vector<std::pair<double, double>>& points);

#endif // LINE_SEGMENT_H
