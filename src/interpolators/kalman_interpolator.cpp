#include "kalman_interpolator.h"
#include <sstream>
#include <iomanip>

using namespace Eigen;

KalmanInterpolator::KalmanInterpolator(double process_noise, double observation_noise) {
    x = VectorXd::Zero(3);
    P = MatrixXd::Identity(3, 3) * 100.0;
    Q = MatrixXd::Identity(3, 3) * process_noise;
    R = MatrixXd::Identity(2, 2) * observation_noise;
}

void KalmanInterpolator::predict() {
    P = P + Q;
}

void KalmanInterpolator::update(double x_obs, double y_obs) {
    MatrixXd H(1, 3);
    H << x_obs * x_obs, x_obs, 1.0;
    
    MatrixXd S = H * P * H.transpose() + R.block(0, 0, 1, 1);
    MatrixXd K = P * H.transpose() * S.inverse();
    
    VectorXd z(1);
    z << y_obs;
    VectorXd y_pred(1);
    y_pred << H * x;
    VectorXd innovation = z - y_pred;
    
    x = x + K * innovation;
    
    MatrixXd I = MatrixXd::Identity(3, 3);
    P = (I - K * H) * P;
}

void KalmanInterpolator::fit(const std::vector<std::pair<double, double>>& points) {
    for (const auto& pt : points) {
        predict();
        update(pt.first, pt.second);
    }
}

double KalmanInterpolator::predict(double x_val) const {
    return x(0) * x_val * x_val + x(1) * x_val + x(2);
}

std::string KalmanInterpolator::getDescription() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6);
    oss << "Kalman Filter (Quadratic): y = " 
        << x(0) << "x^2 + " << x(1) << "x + " << x(2);
    return oss.str();
}