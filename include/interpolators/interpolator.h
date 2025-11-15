#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <vector>
#include <utility>
#include <string>

// 補間インターフェース
class Interpolator {
public:
    virtual ~Interpolator() = default;
    
    // 点群をもとに補間モデルを学習
    virtual void fit(const std::vector<std::pair<double, double>>& points) = 0;
    
    // x座標からy座標を予測
    virtual double predict(double x) const = 0;
    
    // 補間結果の説明を取得
    virtual std::string getDescription() const = 0;
};
#endif // INTERPOLATOR_H