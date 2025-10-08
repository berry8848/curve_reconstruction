#include "interpolators/line_segment.h"

void extractPoints(const std::vector<LineSegment>& segments, 
                   std::vector<std::pair<double, double>>& points) {
    for (const auto& seg : segments) {
        double mid_x = (seg.x1 + seg.x2) / 2.0;
        double mid_y = (seg.y1 + seg.y2) / 2.0;
        points.push_back({mid_x, mid_y});
        
        points.push_back({seg.x1, seg.y1});
        points.push_back({seg.x2, seg.y2});
    }
}