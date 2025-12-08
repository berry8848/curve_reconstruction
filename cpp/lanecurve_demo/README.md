# Lane Curve Demo Applications

LaneCurve共有ライブラリを使用したデモアプリケーション

## セットアップ

### 1. ライブラリの準備
```bash
./setup_libs.sh
```

このスクリプトは `../liblanecurve/dist/` からライブラリとヘッダーをコピーします。

### 2. ビルド
```bash
./build_demo.sh
```

## アプリケーション

### demo_main
標準的な車線曲線復元のデモ
```bash
./build/demo_main
```

### custom_app
カスタム設定での曲線推定例
```bash
./build/custom_app
```

## 出力

結果は `output/` ディレクトリに保存されます：
- `equation.txt`: 推定された曲線の方程式
- `lane_curve.ppm`: 可視化画像

## 独自アプリケーションの作成

1. `src/` に新しい `.cpp` ファイルを作成
2. `CMakeLists.txt` に実行ファイルを追加
3. `./build_demo.sh` で再ビルド