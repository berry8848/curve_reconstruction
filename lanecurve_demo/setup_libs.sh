#!/bin/bash

set -e

LIBRARY_SOURCE="../liblanecurve/dist"

echo "=========================================="
echo "Setting up external libraries"
echo "=========================================="

# ライブラリソースの確認
if [ ! -d "$LIBRARY_SOURCE" ]; then
    echo "Error: Library source not found at: $LIBRARY_SOURCE"
    echo ""
    echo "Please build the library first:"
    echo "  cd ../liblanecurve"
    echo "  ./build_library.sh"
    exit 1
fi

echo "Source: $LIBRARY_SOURCE"
echo ""

# libsディレクトリを作成
mkdir -p libs/lib
mkdir -p libs/include

# ライブラリをコピー
echo "Copying libraries..."
cp -v $LIBRARY_SOURCE/lib/*.so libs/lib/

# ヘッダーファイルをコピー
echo ""
echo "Copying header files..."
cp -rv $LIBRARY_SOURCE/include/* libs/include/

# バージョン情報をコピー
if [ -f "$LIBRARY_SOURCE/VERSION" ]; then
    cp $LIBRARY_SOURCE/VERSION libs/
fi

echo ""
echo "=========================================="
echo "✓ Setup Complete!"
echo "=========================================="
echo ""
echo "Libraries installed:"
ls -lh libs/lib/

echo ""
echo "Headers installed:"
tree libs/include/ 2>/dev/null || ls -R libs/include/

echo ""
echo "Next: Run ./build_demo.sh to build applications"