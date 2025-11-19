#!/bin/bash

# エラーが発生したら終了
set -e

# 出力ディレクトリを作成
mkdir -p output

# buildフォルダを削除
rm -rf ./build/

# ビルド＆実行
cmake -S . -B ./build
cmake --build ./build
./build/lane_curve

# PPMをPNGに変換
if command -v convert &> /dev/null; then
    echo "Converting to PNG..."
    convert ./output/lane_curve.ppm ./output/lane_curve.png
    echo "✓ PNG conversion complete"
fi

# 関数式を表示
echo ""
echo "Estimated curve equation:"
cat ./output/equation.txt