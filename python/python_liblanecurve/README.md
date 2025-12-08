# liblanecurve_python

Python対応のレーンカーブ推定共有ライブラリ

## 概要

このライブラリは、カルマンフィルタを用いた2次曲線推定機能を提供します。
Pythonから利用可能なC言語インターフェースを備えています。

## ビルド要件

- CMake 3.10以上
- C++17対応コンパイラ (g++ 7以上推奨)
- Eigen3ライブラリ (3.3以上)

### Eigenのインストール

```bash
# Ubuntu/Debian
sudo apt-get install libeigen3-dev

# macOS
brew install eigen

# または手動インストール
wget https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
tar xzf eigen-3.4.0.tar.gz
cd eigen-3.4.0
mkdir build && cd build
cmake ..
sudo make install
```

## ビルド方法

```bash
# ビルドディレクトリを作成
mkdir build
cd build

# CMakeで設定
cmake -DCMAKE_BUILD_TYPE=Release ..

# ビルド
make

# インストール（オプション）
sudo make install
```

## 生成される成果物

- `libinterpolators.so` - カルマン補間ライブラリ
- `libutils.so` - ユーティリティライブラリ

## ディレクトリ構造

```
liblanecurve_python/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── interpolators/
│   │   ├── interpolator.h          # 補間インターフェース
│   │   ├── kalman_interpolator.h   # カルマン補間実装
│   │   └── line_segment.h          # 線分構造体
│   ├── utils/
│   │   └── image_utils.h           # 画像出力ユーティリティ
│   └── python_wrapper.h            # Pythonバインディング
└── src/
    ├── interpolators/
    │   └── kalman_interpolator.cpp
    ├── utils/
    │   ├── line_segment.cpp
    │   └── image_utils.cpp
    └── wrappers/                   # Pythonラッパー実装
        ├── interpolators_wrapper.cpp
        └── utils_wrapper.cpp
```

## API リファレンス

### C++ API

#### KalmanInterpolator クラス

```cpp
#include "interpolators/kalman_interpolator.h"

// コンストラクタ
KalmanInterpolator(double process_noise = 0.01, 
                   double observation_noise = 10.0);

// 点群をフィッティング
void fit(const std::vector<std::pair<double, double>>& points);

// x座標からy座標を予測
double predict(double x) const;

// 曲線方程式の文字列表現を取得
std::string getDescription() const;
```

### Python C API

#### interpolators.so

```c
// KalmanInterpolatorを作成
void* createKalmanInterpolator(double process_noise, 
                                double observation_noise);

// インスタンスを削除
void deleteKalmanInterpolator(void* handle);

// フィッティング
void fit(void* handle, const CPoint* points, int num_points);

// 予測
double predict(void* handle, double x);

// 方程式を取得
const char* getDescription(void* handle);
```

#### utils.so

```c
// 線分から点を抽出
void extractPoints(const CLineSegment* segments, int num_segments,
                   CPoint* points, int* num_points);

// PPM画像として保存
void saveToPPM(const char* filename,
               const CLineSegment* segments, int num_segments,
               const CPoint* curve_points, int num_curve_points,
               int width, int height);
```

## 使用例

### C++から使用

```cpp
#include "interpolators/kalman_interpolator.h"

std::vector<std::pair<double, double>> points = {
    {-1.0, 12.0}, {-0.5, 11.0}, {0.0, 10.0}
};

KalmanInterpolator interpolator(0.01, 10.0);
interpolator.fit(points);

double y = interpolator.predict(0.5);
std::string equation = interpolator.getDescription();
```

### Pythonから使用

```python
import ctypes

# ライブラリをロード
lib = ctypes.CDLL("./libinterpolators.so")

# KalmanInterpolatorを作成
interpolator = lib.createKalmanInterpolator(0.01, 10.0)

# ... フィッティングと予測 ...

# クリーンアップ
lib.deleteKalmanInterpolator(interpolator)
```

詳細な使用例は `lanecurve_python_demo` プロジェクトを参照してください。

## トラブルシューティング

### Eigenが見つからない

```bash
# Eigen3の場所を確認
find /usr -name "Eigen" -type d 2>/dev/null

# CMakeにパスを指定
cmake -DEIGEN3_INCLUDE_DIR=/usr/include/eigen3 ..
```

### 共有ライブラリのシンボルを確認

```bash
# エクスポートされているシンボルを確認
nm -D libinterpolators.so | grep -E "(create|delete|fit|predict)"
nm -D libutils.so | grep -E "(extract|saveToPPM)"
```

## ライセンス

このプロジェクトはデモ目的で提供されています。

## 関連プロジェクト

- `lanecurve_python_demo` - このライブラリを使用したPythonデモアプリケーション
