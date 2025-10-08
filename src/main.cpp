#include <iostream>
#include <memory>
#include "interpolators/interpolator.h"
#include "interpolators/kalman_interpolator.h"
#include "interpolators/line_segment.h"
#include "utils/image_utils.h"

int main() {
    std::vector<LineSegment> segments = {
        {10, 50, 15, 52},
        {20, 54, 25, 57},
        {30, 59, 35, 63},
        {40, 66, 45, 71},
        {50, 75, 55, 80},
        {60, 83, 65, 88},
        {70, 90, 75, 94},
    };
    
    std::vector<std::pair<double, double>> points;
    extractPoints(segments, points);
    
    std::unique_ptr<Interpolator> interpolator = 
        std::make_unique<KalmanInterpolator>(0.01, 10.0);
    
    interpolator->fit(points);
    
    std::cout << "推定された曲線:" << std::endl;
    std::cout << interpolator->getDescription() << std::endl;
    
    std::vector<std::pair<double, double>> curve_points;
    for (double x = 0; x <= 100; x += 0.5) {
        double y = interpolator->predict(x);
        curve_points.push_back({x, y});
    }
    
    saveToPPM("lane_curve.ppm", points, curve_points);
    
    std::cout << "曲線画像を lane_curve.ppm として保存しました。" << std::endl;
    std::cout << "赤い点: 入力線分から抽出された点" << std::endl;
    std::cout << "青い線: 復元された曲線" << std::endl;
    
    return 0;
}