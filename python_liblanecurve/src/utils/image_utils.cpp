#include "utils/image_utils.h"
#include <array>
#include <fstream>
#include <algorithm>
#include <cmath>

void saveToPPM(const std::string& filename, 
               const std::vector<LineSegment>& segments,
               const std::vector<std::pair<double, double>>& curve_points,
               int width, int height) {
    
    // 画像バッファを初期化（白背景）
    std::vector<std::vector<std::array<int, 3>>> image(
        height, std::vector<std::array<int, 3>>(width, {255, 255, 255})
    );
    
    // データの範囲を計算
    double min_x = 1e9, max_x = -1e9, min_y = 1e9, max_y = -1e9;
    
    for (const auto& seg : segments) {
        min_x = std::min({min_x, seg.x1, seg.x2});
        max_x = std::max({max_x, seg.x1, seg.x2});
        min_y = std::min({min_y, seg.y1, seg.y2});
        max_y = std::max({max_y, seg.y1, seg.y2});
    }
    
    for (const auto& pt : curve_points) {
        min_x = std::min(min_x, pt.first);
        max_x = std::max(max_x, pt.first);
        min_y = std::min(min_y, pt.second);
        max_y = std::max(max_y, pt.second);
    }
    
    // マージンを追加
    double margin_x = (max_x - min_x) * 0.1;
    double margin_y = (max_y - min_y) * 0.1;
    min_x -= margin_x;
    max_x += margin_x;
    min_y -= margin_y;
    max_y += margin_y;
    
    // 座標変換関数
    auto to_pixel = [&](double x, double y) -> std::pair<int, int> {
        int px = static_cast<int>((x - min_x) / (max_x - min_x) * (width - 1));
        int py = static_cast<int>((max_y - y) / (max_y - min_y) * (height - 1));
        px = std::max(0, std::min(width - 1, px));
        py = std::max(0, std::min(height - 1, py));
        return {px, py};
    };
    
    // 線分を描画（緑色）
    for (const auto& seg : segments) {
        auto [x1, y1] = to_pixel(seg.x1, seg.y1);
        auto [x2, y2] = to_pixel(seg.x2, seg.y2);
        
        // Bresenhamアルゴリズムで線分を描画
        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;
        
        int x = x1, y = y1;
        while (true) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                image[y][x] = {0, 255, 0};  // 緑色
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
    
    // 曲線を描画（青色）
    for (size_t i = 0; i < curve_points.size(); ++i) {
        auto [px, py] = to_pixel(curve_points[i].first, curve_points[i].second);
        
        if (px >= 0 && px < width && py >= 0 && py < height) {
            image[py][px] = {0, 0, 255};  // 青色
            
            // 太い線にするために周辺ピクセルも塗る
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int nx = px + dx;
                    int ny = py + dy;
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        image[ny][nx] = {0, 0, 255};
                    }
                }
            }
        }
    }
    
    // PPMファイルとして保存
    std::ofstream file(filename, std::ios::binary);
    file << "P6\n" << width << " " << height << "\n255\n";
    
    for (const auto& row : image) {
        for (const auto& pixel : row) {
            file.put(static_cast<char>(pixel[0]));
            file.put(static_cast<char>(pixel[1]));
            file.put(static_cast<char>(pixel[2]));
        }
    }
    
    file.close();
}