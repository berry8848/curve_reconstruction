# ベースイメージとしてUbuntu 22.04を使用
FROM ubuntu:22.04

# タイムゾーン設定（インタラクティブプロンプトを回避）
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Tokyo

# 必要なパッケージをインストール
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libeigen3-dev \
    git \
    && rm -rf /var/lib/apt/lists/*

# 作業ディレクトリを作成
WORKDIR /app

# プロジェクトファイルをコピー
COPY CMakeLists.txt ./
COPY include ./include
COPY src ./src

# ビルドディレクトリを作成してビルド
RUN mkdir -p build && cd build && \
    cmake .. && \
    make

# 作業ディレクトリをbuildに変更
WORKDIR /app/build

# 実行時のデフォルトコマンド
CMD ["./lane_curve"]