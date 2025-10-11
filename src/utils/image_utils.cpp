#include "utils/image_utils.h"
#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <array>

void saveToPPM(const std::string& filename, 
               const std::vector<LineSegment>& segments,
               const std::vector<std::pair<double, double>>& curve_points,
               int width, int height) {
    
    std::vector<std::vector<std::array<int, 3>>> image(
        height, std::vector<std::array<int, 3>>(width, {255, 255, 255}));
    
    // データ範囲を計算
    double min_x = 1e9, max_x = -1e9;
    double min_y = 1e9, max_y = -1e9;
    
    for (const auto& seg : segments) {
        min_x = std::min({min_x, seg.x1, seg.x2});
        max_x = std::max({max_x, seg.x1, seg.x2});
        min_y = std::min({min_y, seg.y1, seg.y2});
        max_y = std::max({max_y, seg.y1, seg.y2});
    }
    
    // マージンを追加
    double range_x = max_x - min_x;
    double range_y = max_y - min_y;
    min_x -= range_x * 0.1;
    max_x += range_x * 0.1;
    min_y -= range_y * 0.1;
    max_y += range_y * 0.1;
    
    auto transform = [&](double x, double y) -> std::pair<int, int> {
        int img_x = static_cast<int>((x - min_x) / (max_x - min_x) * (width - 1));
        int img_y = static_cast<int>((1.0 - (y - min_y) / (max_y - min_y)) * (height - 1));
        return {img_x, img_y};
    };
    
    // Bresenhamの直線描画アルゴリズム
    auto drawLine = [&](int x1, int y1, int x2, int y2, int r, int g, int b, int thickness = 1) {
        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;
        
        int x = x1, y = y1;
        while (true) {
            // 太さを持たせる
            for (int tx = -thickness; tx <= thickness; tx++) {
                for (int ty = -thickness; ty <= thickness; ty++) {
                    int px = x + tx;
                    int py = y + ty;
                    if (px >= 0 && px < width && py >= 0 && py < height) {
                        image[py][px] = {r, g, b};
                    }
                }
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
    };
    
    // 元の線分を緑で描画
    for (const auto& seg : segments) {
        auto [x1, y1] = transform(seg.x1, seg.y1);
        auto [x2, y2] = transform(seg.x2, seg.y2);
        drawLine(x1, y1, x2, y2, 0, 200, 0, 2);
    }
    
    // 復元曲線を青で描画
    for (size_t i = 1; i < curve_points.size(); i++) {
        auto [x1, y1] = transform(curve_points[i-1].first, curve_points[i-1].second);
        auto [x2, y2] = transform(curve_points[i].first, curve_points[i].second);
        drawLine(x1, y1, x2, y2, 0, 0, 255, 1);
    }
    
    // PPMファイルとして保存
    std::ofstream ofs(filename);
    ofs << "P3\n" << width << " " << height << "\n255\n";
    for (const auto& row : image) {
        for (const auto& pixel : row) {
            ofs << pixel[0] << " " << pixel[1] << " " << pixel[2] << " ";
        }
        ofs << "\n";
    }
}