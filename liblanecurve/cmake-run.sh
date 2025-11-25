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


# # 共有ライブラリのパスを設定（rpath使用時は不要だが念のため）
# if [[ "$OSTYPE" == "linux-gnu"* ]]; then
#     export LD_LIBRARY_PATH=./build/src/interpolators:./build/src/utils:$LD_LIBRARY_PATH
# elif [[ "$OSTYPE" == "darwin"* ]]; then
#     export DYLD_LIBRARY_PATH=./build/src/interpolators:./build/src/utils:$DYLD_LIBRARY_PATH
# fi

echo "==================================="
echo "✓ All done!"
echo "==================================="