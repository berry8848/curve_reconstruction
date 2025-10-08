# Lane Curve Restoration - Docker実行ガイド

このプロジェクトをDocker環境で実行するためのガイドです。

## 前提条件

- Docker がインストールされていること

## ディレクトリ構成

```
.
├── CMakeLists.txt
├── Dockerfile
├── .dockerignore
├── README.md
├── docker/
├── docs/
├── include/
│   ├── interpolators/
│   │   ├── interpolator.h
│   │   ├── kalman_interpolator.h
│   │   └── line_segment.h
│   └── utils/
│       └── image_utils.h
└── src/
    ├── interpolators/
    │   ├── CMakeLists.txt
    │   └── kalman_interpolator.cpp
    ├── utils/
    │   ├── CMakeLists.txt
    │   ├── image_utils.cpp
    │   └── line_segment.cpp
    └── main.cpp
```

## 実行方法

### 1. Dockerイメージをビルド

プロジェクトのルートディレクトリで以下を実行：

```bash
docker build -t lane_curve:latest .
```

### 2. コンテナを実行して結果を取得

```bash
docker run --rm --name lane_curve_container lane_curve:latest
```

このコマンドはプログラムを実行し、コンソールに結果を表示します。

### 3. 出力ファイル（PPM画像）を取得

プログラムが生成するPPMファイルをホストにコピーします：

```bash
# コンテナを起動（バックグラウンド）
docker run -d --name lane_curve_container lane_curve:latest sleep infinity

# プログラムを実行
docker exec lane_curve_container ./lane_curve

# 生成されたPPMファイルをホストにコピー
docker cp lane_curve_container:/app/build/lane_curve.ppm ./lane_curve.ppm

# コンテナを停止・削除
docker stop lane_curve_container
docker rm lane_curve_container
```

または、ボリュームマウントを使用する方法：

```bash
# 現在のディレクトリをマウントして実行
docker run --rm -v $(pwd)/output:/app/build/output lane_curve:latest sh -c "./lane_curve && cp lane_curve.ppm output/"
```

**注意**: 事前に`output`ディレクトリを作成してください：
```bash
mkdir -p output
```

## 便利なコマンド

### コンテナ内でシェルを起動（デバッグ用）

```bash
docker run --rm -it lane_curve:latest /bin/bash
```

コンテナ内で手動実行：
```bash
# コンテナ内で
cd /app/build
./lane_curve
ls -l lane_curve.ppm
```

### イメージの確認

```bash
# ビルドしたイメージを確認
docker images | grep lane_curve
```

### イメージの削除

```bash
docker rmi lane_curve:latest
```

### キャッシュをクリアして再ビルド

```bash
docker build --no-cache -t lane_curve:latest .
```

## 簡単な実行スクリプト

以下の内容で`run.sh`を作成すると便利です：

```bash
#!/bin/bash

# 出力ディレクトリを作成
mkdir -p output

# Dockerイメージをビルド
echo "Building Docker image..."
docker build -t lane_curve:latest .

# コンテナを実行
echo "Running container..."
docker run -d --name lane_curve_tmp lane_curve:latest sleep 10

# プログラムを実行
echo "Executing program..."
docker exec lane_curve_tmp ./lane_curve

# 結果をコピー
echo "Copying output file..."
docker cp lane_curve_tmp:/app/build/lane_curve.ppm ./output/

# コンテナをクリーンアップ
echo "Cleaning up..."
docker stop lane_curve_tmp
docker rm lane_curve_tmp

echo "Done! Output saved to ./output/lane_curve.ppm"
```

実行権限を付与して実行：
```bash
chmod +x run.sh
./run.sh
```

## 出力ファイル

プログラムは以下のファイルを生成します：

- **lane_curve.ppm**: 曲線補間の結果画像（PPM形式）
  - 赤い点: 入力線分から抽出された点
  - 青い線: カルマンフィルタで復元された曲線

## PPM画像の表示方法

PPMファイルは以下の方法で表示できます：

### ImageMagick（Linux/Mac）
```bash
# インストール
sudo apt-get install imagemagick  # Ubuntu/Debian
brew install imagemagick          # macOS

# 表示
display lane_curve.ppm

# PNG/JPGに変換
convert lane_curve.ppm lane_curve.png
```

### GIMP（Windows/Linux/Mac）
GIMPで直接開くことができます。

### オンラインビューア
PPMファイルをブラウザで表示できるオンラインツールを使用できます。

## トラブルシューティング

### ビルドエラーが発生する場合

```bash
# キャッシュをクリアして再ビルド
docker build --no-cache -t lane_curve:latest .

# ビルドログを詳細表示
docker build --progress=plain -t lane_curve:latest .
```

### 権限エラーが発生する場合

```bash
# outputディレクトリの権限を設定
mkdir -p output
chmod 777 output
```

### コンテナが残っている場合

```bash
# 実行中のコンテナを確認
docker ps -a | grep lane_curve

# 停止して削除
docker stop lane_curve_container
docker rm lane_curve_container
```

### Eigenライブラリが見つからない場合

Dockerイメージ内でEigenが正しくインストールされているか確認：
```bash
docker run --rm lane_curve:latest dpkg -l | grep eigen
```

## 開発ワークフロー

### ソースコードを変更した場合

1. ソースコードを編集
2. Dockerイメージを再ビルド
```bash
docker build -t lane_curve:latest .
```
3. 再実行

### パラメータの調整

`src/main.cpp`の以下の部分を編集：

```cpp
// カルマンフィルタのパラメータ
// 第1引数: プロセスノイズ（デフォルト: 0.01）
// 第2引数: 観測ノイズ（デフォルト: 10.0）
std::make_unique<KalmanInterpolator>(0.01, 10.0);
```

### 入力データの変更

`src/main.cpp`の`segments`変数を編集：

```cpp
std::vector<LineSegment> segments = {
    {10, 50, 15, 52},
    {20, 54, 25, 57},
    // 新しい線分を追加...
};
```

## 技術スタック

- **C++17**
- **CMake 3.10+**
- **Eigen 3.3+**: 線形代数ライブラリ（カルマンフィルタの計算に使用）
- **Ubuntu 22.04**: ベースDockerイメージ

## プロジェクト構成

- **interpolator.h**: 補間の基底インターフェース
- **kalman_interpolator.h/cpp**: カルマンフィルタによる2次曲線補間
- **line_segment.h/cpp**: 線分データの処理
- **image_utils.h/cpp**: PPM画像出力
- **main.cpp**: メインプログラム