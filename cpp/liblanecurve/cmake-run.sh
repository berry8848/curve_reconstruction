#!/bin/bash

# エラーが発生したら終了
set -e

echo "==================================="
echo "Building shared libraries ..."
echo "==================================="

# buildフォルダを削除
rm -rf ./build/

# ビルド＆実行
cmake -S . -B ./build
cmake --build ./build

echo "==================================="
echo "✓ All done!"
echo "==================================="