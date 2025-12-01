#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Lane Curve Demo Application (Python版)
C++の共有ライブラリ(.so)を使用して曲線推定を行うデモプログラム
"""

import ctypes
import os
import sys
from pathlib import Path


# ========================================
# C++構造体とクラスのPythonラッパー
# ========================================

class LineSegment(ctypes.Structure):
    """線分を表す構造体"""
    _fields_ = [
        ("x1", ctypes.c_double),
        ("y1", ctypes.c_double),
        ("x2", ctypes.c_double),
        ("y2", ctypes.c_double),
    ]


class Point(ctypes.Structure):
    """点を表す構造体"""
    _fields_ = [
        ("x", ctypes.c_double),
        ("y", ctypes.c_double),
    ]


# ========================================
# 共有ライブラリの読み込み
# ========================================

def load_libraries():
    """共有ライブラリを読み込む"""
    # ライブラリのパスを取得
    lib_dir = Path(__file__).parent
    
    # libutils.soを読み込み
    utils_lib_path = lib_dir / "libutils.so"
    if not utils_lib_path.exists():
        raise FileNotFoundError(f"libutils.so が見つかりません: {utils_lib_path}")
    utils_lib = ctypes.CDLL(str(utils_lib_path))
    
    # libinterpolators.soを読み込み
    interpolators_lib_path = lib_dir / "libinterpolators.so"
    if not interpolators_lib_path.exists():
        raise FileNotFoundError(f"libinterpolators.so が見つかりません: {interpolators_lib_path}")
    interpolators_lib = ctypes.CDLL(str(interpolators_lib_path))
    
    return utils_lib, interpolators_lib


# ========================================
# C++関数のラッパー関数
# ========================================

def setup_function_signatures(utils_lib, interpolators_lib):
    """C++関数のシグネチャを設定"""
    
    # extractPoints関数
    # void extractPoints(const std::vector<LineSegment>& segments, 
    #                    std::vector<std::pair<double, double>>& points);
    utils_lib.extractPoints.argtypes = [
        ctypes.POINTER(LineSegment),
        ctypes.c_int,
        ctypes.POINTER(Point),
        ctypes.POINTER(ctypes.c_int)
    ]
    utils_lib.extractPoints.restype = None
    
    # saveToPPM関数
    # void saveToPPM(const std::string& filename, 
    #                const std::vector<LineSegment>& segments,
    #                const std::vector<std::pair<double, double>>& curve_points,
    #                int width, int height);
    utils_lib.saveToPPM.argtypes = [
        ctypes.c_char_p,
        ctypes.POINTER(LineSegment),
        ctypes.c_int,
        ctypes.POINTER(Point),
        ctypes.c_int,
        ctypes.c_int,
        ctypes.c_int
    ]
    utils_lib.saveToPPM.restype = None
    
    # KalmanInterpolatorの作成
    # KalmanInterpolator* createKalmanInterpolator(double process_noise, double observation_noise);
    interpolators_lib.createKalmanInterpolator.argtypes = [
        ctypes.c_double,
        ctypes.c_double
    ]
    interpolators_lib.createKalmanInterpolator.restype = ctypes.c_void_p
    
    # fit関数
    # void fit(KalmanInterpolator* interpolator, const Point* points, int num_points);
    interpolators_lib.fit.argtypes = [
        ctypes.c_void_p,
        ctypes.POINTER(Point),
        ctypes.c_int
    ]
    interpolators_lib.fit.restype = None
    
    # predict関数
    # double predict(KalmanInterpolator* interpolator, double x);
    interpolators_lib.predict.argtypes = [
        ctypes.c_void_p,
        ctypes.c_double
    ]
    interpolators_lib.predict.restype = ctypes.c_double
    
    # getDescription関数
    # const char* getDescription(KalmanInterpolator* interpolator);
    interpolators_lib.getDescription.argtypes = [ctypes.c_void_p]
    interpolators_lib.getDescription.restype = ctypes.c_char_p
    
    # deleteKalmanInterpolator関数
    # void deleteKalmanInterpolator(KalmanInterpolator* interpolator);
    interpolators_lib.deleteKalmanInterpolator.argtypes = [ctypes.c_void_p]
    interpolators_lib.deleteKalmanInterpolator.restype = None


# ========================================
# メイン処理
# ========================================

def main():
    print("=" * 40)
    print("Lane Curve Demo Application")
    print("=" * 40)
    print()
    
    # 共有ライブラリを読み込み
    try:
        utils_lib, interpolators_lib = load_libraries()
        setup_function_signatures(utils_lib, interpolators_lib)
    except Exception as e:
        print(f"エラー: ライブラリの読み込みに失敗しました: {e}")
        return 1
    
    # サンプルデータ
    segments_data = [
        (-1.2, 12.2, -0.8, 11.8),
        (-0.7, 11.65, -0.3, 10.85),
        (-0.2, 10.6, 0.2, 9.4),
        (0.8, 7, 1.2, 5),
        (1.5, 3.29, 1.9, 0.73),
    ]
    
    # LineSegment配列を作成
    segments = (LineSegment * len(segments_data))()
    for i, (x1, y1, x2, y2) in enumerate(segments_data):
        segments[i] = LineSegment(x1, y1, x2, y2)
    
    print(f"Input: {len(segments_data)} line segments")
    
    # 点を抽出
    max_points = len(segments_data) * 3  # 中点、始点、終点
    points = (Point * max_points)()
    num_points = ctypes.c_int(0)
    
    utils_lib.extractPoints(
        segments,
        len(segments_data),
        points,
        ctypes.byref(num_points)
    )
    
    print(f"Extracted: {num_points.value} points")
    print()
    
    # カルマンフィルタで曲線推定
    interpolator = interpolators_lib.createKalmanInterpolator(0.01, 10.0)
    
    try:
        # フィッティング
        interpolators_lib.fit(interpolator, points, num_points.value)
        
        # 曲線の説明を取得
        equation_bytes = interpolators_lib.getDescription(interpolator)
        equation = equation_bytes.decode('utf-8')
        
        print("推定された曲線:")
        print(equation)
        print()
        
        # 出力ディレクトリを作成
        output_dir = Path("output")
        output_dir.mkdir(exist_ok=True)
        
        # 関数式をファイルに保存
        with open(output_dir / "equation.txt", "w", encoding="utf-8") as f:
            f.write(equation + "\n")
        
        # 曲線点を生成
        curve_points_list = []
        x_start, x_end, x_step = -2.0, 2.0, 0.01
        x = x_start
        while x <= x_end:
            y = interpolators_lib.predict(interpolator, x)
            curve_points_list.append((x, y))
            x += x_step
        
        # 曲線点を配列に変換
        curve_points = (Point * len(curve_points_list))()
        for i, (x, y) in enumerate(curve_points_list):
            curve_points[i] = Point(x, y)
        
        # 画像として保存
        output_path = str(output_dir / "lane_curve.ppm")
        utils_lib.saveToPPM(
            output_path.encode('utf-8'),
            segments,
            len(segments_data),
            curve_points,
            len(curve_points_list),
            1000,  # width
            800    # height
        )
        
        print("Output files:")
        print("  - output/equation.txt")
        print("  - output/lane_curve.ppm")
        print()
        print("凡例:")
        print("  緑の線: 元の線分")
        print("  青い線: 復元された曲線")
        print()
        print("=" * 40)
        print("✓ Demo completed successfully!")
        print("=" * 40)
        
    finally:
        # メモリ解放
        interpolators_lib.deleteKalmanInterpolator(interpolator)
    
    return 0


if __name__ == "__main__":
    sys.exit(main())
