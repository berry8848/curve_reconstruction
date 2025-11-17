#!/bin/bash

set -e

# 出力ディレクトリを作成
mkdir -p output

echo "==================================="
echo "Building shared libraries first..."
echo "==================================="

# まず共有ライブラリをビルド（まだビルドされていない場合）
if [ ! -f "./build/src/interpolators/libinterpolators.so" ]; then
    echo "Shared libraries not found. Building..."
    ./cmake-run.sh
else
    echo "Shared libraries already exist. Skipping build."
fi

echo ""
echo "==================================="
echo "Compiling external app..."
echo "==================================="

# 共有ライブラリを使って外部アプリをコンパイル
g++ -std=c++17 \
    -I./include \
    -I/usr/include/eigen3 \
    ./src/external_app.cpp \
    -L./build/src/interpolators \
    -L./build/src/utils \
    -linterpolators \
    -lutils \
    -Wl,-rpath,./build/src/interpolators \
    -Wl,-rpath,./build/src/utils \
    -o ./build/external_app

echo "✓ Compilation successful"

# 共有ライブラリのパスを設定（rpath使用時は不要だが念のため）
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    export LD_LIBRARY_PATH=./build/src/interpolators:./build/src/utils:$LD_LIBRARY_PATH
elif [[ "$OSTYPE" == "darwin"* ]]; then
    export DYLD_LIBRARY_PATH=./build/src/interpolators:./build/src/utils:$DYLD_LIBRARY_PATH
fi

echo ""
echo "==================================="
echo "Running external app..."
echo "==================================="

./build/external_app

# PPMをPNGに変換
if command -v convert &> /dev/null; then
    echo ""
    echo "Converting to PNG..."
    convert ./output/lane_curve_external.ppm ./output/lane_curve_external.png
    echo "✓ PNG conversion complete"
else
    echo ""
    echo "⚠ ImageMagick not found. Skipping PNG conversion."
fi

# 結果を表示
echo ""
echo "==================================="
echo "Comparison Results:"
echo "==================================="
echo ""
echo "Original (lane_curve):"
cat ./output/equation.txt
echo ""
echo "External App:"
cat ./output/equation_external.txt
echo ""

# 差分チェック
echo "==================================="
echo "Difference check:"
echo "==================================="
if diff -q ./output/equation.txt ./output/equation_external.txt > /dev/null; then
    echo "✓ Results are IDENTICAL!"
else
    echo "⚠ Results differ:"
    diff ./output/equation.txt ./output/equation_external.txt || true
fi
echo ""

# 生成されたファイルを確認
echo "==================================="
echo "Generated files:"
echo "==================================="
ls -lh ./output/ | grep -E "external"
echo ""

# 共有ライブラリの確認
echo "==================================="
echo "Shared libraries used:"
echo "==================================="
if command -v ldd &> /dev/null; then
    ldd ./build/external_app | grep -E "(interpolators|utils|eigen)"
elif command -v otool &> /dev/null; then
    # macOS
    otool -L ./build/external_app | grep -E "(interpolators|utils)"
fi
echo ""

echo "==================================="
echo "✓ All done!"
echo "==================================="