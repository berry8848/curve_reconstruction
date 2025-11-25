#!/bin/bash

# エラーが発生したら終了
set -e

echo "==================================="
echo "Building shared libraries first..."
echo "==================================="

# buildフォルダを削除
rm -rf ./build/

# ビルド＆実行
cmake -S . -B ./build
cmake --build ./build

echo ""
echo "==================================="
echo "Compiling external app..."
echo "==================================="

# 共有ライブラリを使って外部アプリをコンパイル
g++ -std=c++17 \
    -I./include \
    -I/usr/include/eigen3 \
    -L./build/src/interpolators \
    -L./build/src/utils \
    -linterpolators \
    -lutils \
    -Wl,-rpath,./build/src/interpolators \
    -Wl,-rpath,./build/src/utils \

echo "✓ Compilation successful"

# 共有ライブラリのパスを設定（rpath使用時は不要だが念のため）
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    export LD_LIBRARY_PATH=./build/src/interpolators:./build/src/utils:$LD_LIBRARY_PATH
elif [[ "$OSTYPE" == "darwin"* ]]; then
    export DYLD_LIBRARY_PATH=./build/src/interpolators:./build/src/utils:$DYLD_LIBRARY_PATH
fi

echo "==================================="
echo "✓ All done!"
echo "==================================="