## 概要
- カルマンフィルタを用いて、与えられた線分から曲線を予測するプラグラム。

## 環境構築
### 開発時(WSLローカル上)
```bash
sudo apt update
sudo apt install build-essential imagemagick libeigen3-dev -y
```

## 実行方法
### 開発時(WSLローカル上)
```bash
./cmake-run.sh 
```

### docker環境
```bash
./docker-run.sh
```