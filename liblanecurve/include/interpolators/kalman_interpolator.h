#ifndef KALMAN_INTERPOLATOR_H
#define KALMAN_INTERPOLATOR_H

#include "interpolator.h"
#include <Eigen/Dense>

class KalmanInterpolator : public Interpolator {
private:
    Eigen::VectorXd x;  // 状態ベクトル [a, b, c] (y = ax^2 + bx + c)
    Eigen::MatrixXd P;  // 誤差共分散行列
    Eigen::MatrixXd Q;  // プロセスノイズ
    Eigen::MatrixXd R;  // 観測ノイズ
    
    void predict();
    void update(double x_obs, double y_obs);
    
public:
    KalmanInterpolator(double process_noise = 0.01, double observation_noise = 10.0);
    
    void fit(const std::vector<std::pair<double, double>>& points) override;
    double predict(double x) const override;
    std::string getDescription() const override;
    
    // カルマンフィルタ固有のメソッド
    Eigen::VectorXd getState() const { return x; }
};

#endif // KALMAN_INTERPOLATOR_H