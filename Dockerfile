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
COPY . .

# ビルドディレクトリを作成してビルド
RUN mkdir -p build && cd build && \
    cmake .. && \
    make

# 実行時のデフォルトコマンド
CMD ["./build/lane_curve"]