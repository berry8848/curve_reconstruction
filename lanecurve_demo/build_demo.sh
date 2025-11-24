#!/bin/bash

set -e

echo "=========================================="
echo "Building Lane Curve Demo Applications"
echo "=========================================="

# ライブラリの確認
if [ ! -f "libs/lib/libinterpolators.so" ]; then
    echo "Error: External libraries not found!"
    echo "Please run: ./setup_libs.sh first"
    exit 1
fi

echo "✓ External libraries found"
echo ""

# 出力ディレクトリを作成
mkdir -p output

# ビルドディレクトリをクリーン
rm -rf build
mkdir -p build

# CMakeビルド
echo "Running CMake..."
cmake -S . -B ./build

echo ""
echo "Building applications..."
cmake --build ./build

echo ""
echo "=========================================="
echo "✓ Build Complete!"
echo "=========================================="
echo ""
echo "Built applications:"
ls -lh build/demo_main build/custom_app

echo ""
echo "=========================================="
echo "Running demo_main..."
echo "=========================================="

# 環境変数設定（念のため）
export LD_LIBRARY_PATH=$(pwd)/libs/lib:$LD_LIBRARY_PATH

# 実行
./build/demo_main

# 結果を表示
echo ""
echo "=========================================="
echo "Results:"
echo "=========================================="
if [ -f "output/equation.txt" ]; then
    cat output/equation.txt
fi

echo ""
echo "Output files:"
ls -lh output/

echo ""
echo "=========================================="
echo "To run custom_app:"
echo "  ./build/custom_app"
echo "=========================================="