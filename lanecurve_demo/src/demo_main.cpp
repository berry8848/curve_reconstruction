#include <iostream>
#include <fstream>
#include <memory>
#include "interpolators/interpolator.h"
#include "interpolators/kalman_interpolator.h"
#include "interpolators/line_segment.h"
#include "utils/image_utils.h"

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Lane Curve Demo Application" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    // サンプルデータ
    std::vector<LineSegment> segments = {
        {-1.2, 12.2, -0.8, 11.8},
        {-0.7, 11.65, -0.3, 10.85},
        {-0.2, 10.6, 0.2, 9.4},
        {0.8, 7, 1.2, 5},
        {1.5, 3.29, 1.9, 0.73},
    };
    
    std::cout << "Input: " << segments.size() << " line segments" << std::endl;
    
    // 点を抽出
    std::vector<std::pair<double, double>> points;
    extractPoints(segments, points);
    
    std::cout << "Extracted: " << points.size() << " points" << std::endl;
    std::cout << std::endl;
    
    // カルマンフィルタで曲線推定
    std::unique_ptr<Interpolator> interpolator = 
        std::make_unique<KalmanInterpolator>(0.01, 10.0);
    
    interpolator->fit(points);
    
    std::string equation = interpolator->getDescription();
    
    std::cout << "推定された曲線:" << std::endl;
    std::cout << equation << std::endl;
    std::cout << std::endl;
    
    // 関数式をファイルに保存
    std::ofstream eq_file("output/equation.txt");
    eq_file << equation << std::endl;
    eq_file.close();
    
    // 曲線点を生成
    std::vector<std::pair<double, double>> curve_points;
    for (double x = -2; x <= 2; x += 0.01) {
        double y = interpolator->predict(x);
        curve_points.push_back({x, y});
    }
    
    // 画像として保存
    saveToPPM("output/lane_curve.ppm", segments, curve_points, 1000, 800);
    
    std::cout << "Output files:" << std::endl;
    std::cout << "  - output/equation.txt" << std::endl;
    std::cout << "  - output/lane_curve.ppm" << std::endl;
    std::cout << std::endl;
    std::cout << "凡例:" << std::endl;
    std::cout << "  緑の線: 元の線分" << std::endl;
    std::cout << "  青い線: 復元された曲線" << std::endl;
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "✓ Demo completed successfully!" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}