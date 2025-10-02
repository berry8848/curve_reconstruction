#include "image_utils.h"
#include <fstream>
#include <array>
#include <algorithm>
#include <cmath>

void saveToPPM(const std::string& filename, 
               const std::vector<std::pair<double, double>>& original_points,
               const std::vector<std::pair<double, double>>& curve_points, 
               int width, int height) {
    
    std::vector<std::vector<std::array<int, 3>>> image(
        height, std::vector<std::array<int, 3>>(width, {255, 255, 255}));
    
    double min_x = 0, max_x = 100;
    double min_y = 0, max_y = 100;
    
    auto transform = [&](double x, double y) -> std::pair<int, int> {
        int img_x = static_cast<int>((x - min_x) / (max_x - min_x) * (width - 1));
        int img_y = static_cast<int>((1.0 - (y - min_y) / (max_y - min_y)) * (height - 1));
        return {img_x, img_y};
    };
    
    // 元の点を赤でプロット
    for (const auto& pt : original_points) {
        auto [img_x, img_y] = transform(pt.first, pt.second);
        if (img_x >= 0 && img_x < width && img_y >= 0 && img_y < height) {
            for (int dx = -2; dx <= 2; dx++) {
                for (int dy = -2; dy <= 2; dy++) {
                    if (std::abs(dx) + std::abs(dy) <= 2) {
                        int nx = img_x + dx;
                        int ny = img_y + dy;
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            image[ny][nx] = {255, 0, 0};
                        }
                    }
                }
            }
        }
    }
    
    // 復元曲線を青でプロット
    for (size_t i = 1; i < curve_points.size(); i++) {
        auto [x1, y1] = transform(curve_points[i-1].first, curve_points[i-1].second);
        auto [x2, y2] = transform(curve_points[i].first, curve_points[i].second);
        
        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;
        
        int x = x1, y = y1;
        while (true) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                image[y][x] = {0, 0, 255};
            }
            
            if (x == x2 && y == y2) break;
            
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x += sx;
            }
            if (e2 < dx) {
                err += dx;
                y += sy;
            }
        }
    }
    
    std::ofstream ofs(filename);
    ofs << "P3\n" << width << " " << height << "\n255\n";
    for (const auto& row : image) {
        for (const auto& pixel : row) {
            ofs << pixel[0] << " " << pixel[1] << " " << pixel[2] << " ";
        }
        ofs << "\n";
    }
}