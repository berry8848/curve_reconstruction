#!/bin/bash

# エラーが発生したら終了
set -e

echo "=========================================="
echo "Lane Curve Restoration - Docker Runner"
echo "=========================================="
echo ""

# 出力ディレクトリを作成
echo "[1/5] Creating output directory..."
mkdir -p output

# Dockerイメージをビルド
echo "[2/5] Building Docker image..."
docker build -t lane_curve:latest .

# 一時コンテナを起動
echo "[3/5] Starting container..."
docker rm -f lane_curve_tmp > /dev/null 2>&1 || true
docker run -d --name lane_curve_tmp lane_curve:latest sleep 10

# プログラムを実行
echo "[4/5] Executing program..."
docker exec lane_curve_tmp ./lane_curve

# 結果をコピー
echo "[5/5] Copying output files..."
docker cp lane_curve_tmp:/app/build/lane_curve.ppm ./output/
docker cp lane_curve_tmp:/app/build/equation.txt ./output/

# コンテナをクリーンアップ
echo "Cleaning up..."
docker stop lane_curve_tmp > /dev/null 2>&1
docker rm lane_curve_tmp > /dev/null 2>&1

echo ""
echo "=========================================="
echo "✓ Done!"
echo "Output saved to:"
echo "  - ./output/lane_curve.ppm"
echo "  - ./output/equation.txt"
echo "=========================================="
echo ""
echo "To view the image:"
echo "  display output/lane_curve.ppm"
echo "Or convert to PNG:"
echo "  convert output/lane_curve.ppm output/lane_curve.png"

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