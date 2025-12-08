#include "interpolators/kalman_interpolator.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

KalmanInterpolator::KalmanInterpolator(double process_noise, double observation_noise) {
    // 状態ベクトル [a, b, c] を初期化
    x = Eigen::VectorXd::Zero(3);
    
    // 誤差共分散行列を初期化（大きな不確実性）
    P = Eigen::MatrixXd::Identity(3, 3) * 1000.0;
    
    // プロセスノイズ
    Q = Eigen::MatrixXd::Identity(3, 3) * process_noise;
    
    // 観測ノイズ
    R = Eigen::MatrixXd::Identity(1, 1) * observation_noise;
}

void KalmanInterpolator::predict() {
    // 状態遷移なし（静的モデル）
    // x = F * x;  (F = I なので x は変わらない)
    
    // 誤差共分散の予測
    P = P + Q;
}

void KalmanInterpolator::update(double x_obs, double y_obs) {
    // 観測行列 H = [x^2, x, 1]
    Eigen::RowVectorXd H(3);
    H << x_obs * x_obs, x_obs, 1.0;
    
    // イノベーション（観測残差）
    double y_pred = H * x;
    double innovation = y_obs - y_pred;
    
    // イノベーション共分散
    double S = (H * P * H.transpose())(0, 0) + R(0, 0);
    
    // カルマンゲイン
    Eigen::VectorXd K = P * H.transpose() / S;
    
    // 状態更新
    x = x + K * innovation;
    
    // 誤差共分散更新
    Eigen::MatrixXd I = Eigen::MatrixXd::Identity(3, 3);
    P = (I - K * H) * P;
}

void KalmanInterpolator::fit(const std::vector<std::pair<double, double>>& points) {
    // 各点に対してカルマンフィルタを適用
    for (const auto& point : points) {
        predict();
        update(point.first, point.second);
    }
}

double KalmanInterpolator::predict(double x_val) const {
    // y = ax^2 + bx + c
    double a = x(0);
    double b = x(1);
    double c = x(2);
    
    return a * x_val * x_val + b * x_val + c;
}

std::string KalmanInterpolator::getDescription() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6);
    
    double a = x(0);
    double b = x(1);
    double c = x(2);
    
    oss << "y = ";
    
    if (std::abs(a) > 1e-10) {
        oss << a << "x^2";
        if (b >= 0) oss << " + ";
    }
    
    if (std::abs(b) > 1e-10 || std::abs(a) < 1e-10) {
        if (b < 0 || std::abs(a) < 1e-10) {
            oss << b << "x";
        } else {
            oss << b << "x";
        }
        if (c >= 0) oss << " + ";
    }
    
    if (std::abs(c) > 1e-10 || (std::abs(a) < 1e-10 && std::abs(b) < 1e-10)) {
        oss << c;
    }
    
    return oss.str();
}
