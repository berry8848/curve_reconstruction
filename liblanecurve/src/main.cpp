#include <iostream>
#include <fstream>
#include <memory>
#include "interpolators/interpolator.h"
#include "interpolators/kalman_interpolator.h"
#include "interpolators/line_segment.h"
#include "utils/image_utils.h"

int main() {
    std::vector<LineSegment> segments = {
        {-1.2, 12.2, -0.8, 11.8},
        {-0.7, 11.65, -0.3, 10.85},
        {-0.2, 10.6, 0.2, 9.4},
        {0.8, 7, 1.2, 5},
        {1.5, 3.29, 1.9, 0.73},
    };
    
    std::vector<std::pair<double, double>> points;
    extractPoints(segments, points);
    
    std::unique_ptr<Interpolator> interpolator = 
        std::make_unique<KalmanInterpolator>(0.01, 10.0);
    
    interpolator->fit(points);
    
    std::string equation = interpolator->getDescription();
    
    std::cout << "推定された曲線:" << std::endl;
    std::cout << equation << std::endl;
    
    // 関数式をファイルに保存
    std::ofstream eq_file("output/equation.txt");
    eq_file << equation << std::endl;
    eq_file.close();
    
    std::vector<std::pair<double, double>> curve_points;
    for (double x = -2; x <= 2; x += 0.01) {
        double y = interpolator->predict(x);
        curve_points.push_back({x, y});
    }
    
    saveToPPM("output/lane_curve.ppm", segments, curve_points, 1000, 800);
    
    std::cout << "曲線画像を lane_curve.ppm として保存しました。" << std::endl;
    std::cout << "関数式を equation.txt として保存しました。" << std::endl;
    std::cout << "緑の線: 元の線分" << std::endl;
    std::cout << "青い線: 復元された曲線" << std::endl;
    
    return 0;
}