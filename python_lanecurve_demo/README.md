# lanecurve_python_demo

Python版レーンカーブ推定デモアプリケーション

## 概要

このプロジェクトは、`liblanecurve_python`で生成された共有ライブラリ(.so)を使用して、
車載カメラから得られた線分データから曲線を推定するデモプログラムです。

## 必要な環境

- Python 3.6以上
- liblanecurve_pythonでビルドされた共有ライブラリ
  - `libinterpolators.so`
  - `libutils.so`

## セットアップ

### 1. 共有ライブラリのビルド

まず、`liblanecurve_python`プロジェクトをビルドします:

```bash
cd /path/to/liblanecurve_python
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

### 2. 共有ライブラリの配置

ビルドされた`.so`ファイルを`lib/`ディレクトリにコピーします:

```bash
# lanecurve_python_demoディレクトリで実行
mkdir -p lib
cp /path/to/liblanecurve_python/build/libinterpolators.so lib/
cp /path/to/liblanecurve_python/build/libutils.so lib/
```

または、ビルドディレクトリから直接コピー:

```bash
cd lanecurve_python_demo
cp ../liblanecurve_python/build/*.so lib/
```

## ディレクトリ構造

```
lanecurve_python_demo/
├── demo_main.py          # デモプログラム本体
├── lib/                  # 共有ライブラリ配置ディレクトリ
│   ├── libinterpolators.so
│   └── libutils.so
├── output/               # 出力ファイル（自動生成）
│   ├── equation.txt      # 推定された曲線の方程式
│   └── lane_curve.ppm    # 可視化画像
└── README.md
```

## 実行方法

### 基本的な実行

```bash
python3 demo_main.py
```

または実行権限を付与して直接実行:

```bash
chmod +x demo_main.py
./demo_main.py
```

### 実行結果

プログラムは以下の処理を実行します:

1. サンプルの線分データ（5本）を読み込み
2. 線分から点を抽出（各線分から3点: 中点、始点、終点）
3. カルマンフィルタで2次曲線を推定
4. 推定結果を`output/`ディレクトリに出力

**出力ファイル:**
- `output/equation.txt` - 推定された曲線の方程式（例: y = -0.523456x^2 + 1.234567x + 10.123456）
- `output/lane_curve.ppm` - 可視化画像（PPMフォーマット）

### 出力例

```
========================================
Lane Curve Demo Application
========================================

Input: 5 line segments
Extracted: 15 points

推定された曲線:
y = -0.523456x^2 + 1.234567x + 10.123456

Output files:
  - output/equation.txt
  - output/lane_curve.ppm

凡例:
  緑の線: 元の線分
  青い線: 復元された曲線

========================================
✓ Demo completed successfully!
========================================
```

## 画像の確認

生成されたPPM画像は、以下のツールで確認できます:

```bash
# ImageMagickで表示
display output/lane_curve.ppm

# ImageMagickでPNGに変換
convert output/lane_curve.ppm output/lane_curve.png

# GIMPで開く
gimp output/lane_curve.ppm
```

## プログラムの構造

### 主要コンポーネント

1. **LineSegment / Point 構造体**
   - C++の構造体とPythonのctypes.Structureをマッピング

2. **load_libraries()**
   - 共有ライブラリを動的にロード

3. **setup_function_signatures()**
   - C++関数のシグネチャを定義

4. **main()**
   - デモの実行フロー

### 使用するライブラリ関数

#### utils.so
- `extractPoints()` - 線分から点群を抽出
- `saveToPPM()` - 画像としてPPM形式で保存

#### interpolators.so
- `createKalmanInterpolator()` - カルマン補間器を作成
- `fit()` - 点群をフィッティング
- `predict()` - x座標からy座標を予測
- `getDescription()` - 曲線の方程式を取得
- `deleteKalmanInterpolator()` - メモリ解放

## カスタマイズ

### サンプルデータの変更

`demo_main.py`の`segments_data`を変更することで、異なる線分データを試すことができます:

```python
segments_data = [
    (x1, y1, x2, y2),  # 線分1
    (x1, y1, x2, y2),  # 線分2
    # ...
]
```

### カルマンフィルタのパラメータ調整

```python
# プロセスノイズとオブザベーションノイズを調整
interpolator = interpolators_lib.createKalmanInterpolator(
    0.01,   # process_noise: 小さいほどスムーズ
    10.0    # observation_noise: 大きいほどノイズに強い
)
```

### 画像サイズの変更

```python
utils_lib.saveToPPM(
    output_path.encode('utf-8'),
    segments, len(segments_data),
    curve_points, len(curve_points_list),
    1000,  # width: 画像幅
    800    # height: 画像高さ
)
```

## トラブルシューティング

### エラー: libinterpolators.so が見つかりません

**原因**: 共有ライブラリが`lib/`ディレクトリにない

**解決策**:
```bash
# ライブラリをコピー
cp ../liblanecurve_python/build/*.so lib/

# または、LD_LIBRARY_PATHを設定
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
python3 demo_main.py
```

### エラー: undefined symbol

**原因**: 共有ライブラリに必要なシンボル（関数）が含まれていない

**解決策**:
```bash
# シンボルを確認
nm -D lib/libinterpolators.so | grep create
nm -D lib/libutils.so | grep extract

# 見つからない場合は、liblanecurve_pythonを再ビルド
cd ../liblanecurve_python
rm -rf build
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

### エラー: Eigen3が見つからない

**原因**: liblanecurve_pythonのビルド時にEigen3が見つからなかった

**解決策**:
```bash
# Eigen3をインストール
sudo apt-get install libeigen3-dev  # Ubuntu/Debian
brew install eigen                   # macOS

# 再ビルド
cd ../liblanecurve_python/build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## 技術的な詳細

### ctypesによるFFI

このプログラムは、Pythonの`ctypes`モジュールを使用してC++ライブラリを呼び出します。

**メリット:**
- 追加の依存関係が不要（標準ライブラリ）
- C/C++ライブラリを直接利用可能
- パフォーマンスが高い

**デメリット:**
- 型安全性が低い（実行時エラー）
- メモリ管理を手動で行う必要がある

### メモリ管理

オペークポインタパターンを使用して、C++オブジェクトのライフサイクルを管理:

```python
# オブジェクト作成
interpolator = interpolators_lib.createKalmanInterpolator(...)

try:
    # 使用
    interpolators_lib.fit(interpolator, ...)
finally:
    # 必ずクリーンアップ
    interpolators_lib.deleteKalmanInterpolator(interpolator)
```

## 参考資料

- [Python ctypes公式ドキュメント](https://docs.python.org/3/library/ctypes.html)
- [カルマンフィルタの基礎](https://en.wikipedia.org/wiki/Kalman_filter)
- [PPM画像フォーマット](http://netpbm.sourceforge.net/doc/ppm.html)

## ライセンス

このプロジェクトはデモ目的で提供されています。
