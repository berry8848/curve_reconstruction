# Lane Curve Library - Python版デモ

## 概要

このプロジェクトは、C++で実装されたカルマンフィルタベースの曲線推定ライブラリをPythonから利用できるようにしたものです。

## ファイル構成

```
.
├── CMakeLists.txt              # C++ビルド設定
├── CMakeLists_with_python.txt  # Pythonラッパー付きビルド設定
├── demo_main.cpp               # C++版デモプログラム
├── demo_main.py                # Python版デモプログラム
├── include/
│   ├── interpolators/
│   │   ├── interpolator.h
│   │   ├── kalman_interpolator.h
│   │   └── line_segment.h
│   ├── utils/
│   │   └── image_utils.h
│   └── python_wrapper.h        # Pythonバインディング用ヘッダー
├── src/
│   ├── interpolators/
│   │   └── kalman_interpolator.cpp
│   ├── utils/
│   │   ├── line_segment.cpp
│   │   └── image_utils.cpp
│   └── wrappers/               # Pythonバインディング実装
│       ├── interpolators_wrapper.cpp
│       └── utils_wrapper.cpp
├── libinterpolators.so         # 補間ライブラリ
└── libutils.so                 # ユーティリティライブラリ
```

## 必要な環境

### C++ライブラリのビルド（既に完了している場合は不要）

- CMake 3.10以上
- C++17対応コンパイラ (g++ 7以上)
- Eigen3ライブラリ

### Python実行環境

- Python 3.6以上
- ctypesモジュール（標準ライブラリ）

## ビルド方法

### オプション1: 既存のsoファイルを使用（推奨）

既に`libinterpolators.so`と`libutils.so`がある場合、Pythonラッパー関数を追加してリビルドする必要があります:

```bash
# Pythonラッパー付きでリビルド
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -f ../CMakeLists_with_python.txt ..
make
```

### オプション2: ゼロからビルド

```bash
# ディレクトリ構造を作成
mkdir -p src/interpolators src/utils src/wrappers include/interpolators include/utils

# ソースファイルを配置（省略）

# ビルド
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make

# ビルド成果物を確認
ls -la libinterpolators.so libutils.so demo_cpp
```

## 実行方法

### Python版デモの実行

```bash
# demo_main.pyと.soファイルが同じディレクトリにあることを確認
python3 demo_main.py
```

または実行権限を付与して:

```bash
chmod +x demo_main.py
./demo_main.py
```

### C++版デモの実行（比較用）

```bash
# LD_LIBRARY_PATHを設定して実行
LD_LIBRARY_PATH=. ./demo_cpp
```

## 出力結果

プログラムを実行すると、`output/`ディレクトリに以下のファイルが生成されます:

- `equation.txt` - 推定された曲線の方程式
- `lane_curve.ppm` - 可視化画像（PPMフォーマット）

## 技術的な詳細

### Pythonバインディングの仕組み

1. **C言語インターフェース**: C++クラスをC言語の関数として公開
   - `python_wrapper.h`: 関数宣言
   - `*_wrapper.cpp`: 実装

2. **ctypesによる呼び出し**: Pythonから共有ライブラリを動的にロード
   - `ctypes.CDLL()`: soファイルの読み込み
   - `argtypes/restype`: 関数シグネチャの定義
   - `Structure`: C構造体のマッピング

3. **メモリ管理**:
   - オペークポインタパターンでC++オブジェクトを管理
   - `create*/delete*`関数でライフサイクル制御

### ラッパー関数一覧

#### utils.so

- `extractPoints()` - 線分から点を抽出
- `saveToPPM()` - PPM画像として保存

#### interpolators.so

- `createKalmanInterpolator()` - インスタンス作成
- `deleteKalmanInterpolator()` - インスタンス削除
- `fit()` - モデルのフィッティング
- `predict()` - 予測値の計算
- `getDescription()` - 曲線方程式の取得

## トラブルシューティング

### soファイルが見つからない

```bash
# エラー: libinterpolators.so が見つかりません
# 解決策: LD_LIBRARY_PATHを設定
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
python3 demo_main.py
```

### 関数シンボルが見つからない

```bash
# ラッパー関数が含まれているか確認
nm -D libinterpolators.so | grep create
nm -D libutils.so | grep extract

# 期待される出力:
# 00000xxx T createKalmanInterpolator
# 00000xxx T extractPoints
```

ラッパー関数が見つからない場合は、Pythonラッパーを含めて再ビルドしてください。

## ライセンス

このプロジェクトはデモ目的で提供されています。

## 参考資料

- [ctypes公式ドキュメント](https://docs.python.org/3/library/ctypes.html)
- [Eigenライブラリ](https://eigen.tuxfamily.org/)
- [カルマンフィルタ](https://en.wikipedia.org/wiki/Kalman_filter)
