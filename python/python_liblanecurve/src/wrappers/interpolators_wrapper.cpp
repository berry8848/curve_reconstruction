#include "python_wrapper.h"
#include "interpolators/kalman_interpolator.h"
#include <vector>
#include <utility>
#include <string>
#include <cstring>

extern "C" {

// 説明文字列を保持するための静的バッファ
static thread_local char description_buffer[1024];

KalmanInterpolatorHandle createKalmanInterpolator(double process_noise, 
                                                   double observation_noise) {
    KalmanInterpolator* interpolator = new KalmanInterpolator(process_noise, observation_noise);
    return static_cast<void*>(interpolator);
}

void deleteKalmanInterpolator(KalmanInterpolatorHandle handle) {
    if (handle) {
        KalmanInterpolator* interpolator = static_cast<KalmanInterpolator*>(handle);
        delete interpolator;
    }
}

void fit(KalmanInterpolatorHandle handle, const CPoint* points, int num_points) {
    if (!handle) return;
    
    KalmanInterpolator* interpolator = static_cast<KalmanInterpolator*>(handle);
    
    // C配列からC++のvectorに変換
    std::vector<std::pair<double, double>> cpp_points;
    cpp_points.reserve(num_points);
    
    for (int i = 0; i < num_points; i++) {
        cpp_points.push_back({points[i].x, points[i].y});
    }
    
    // フィッティング
    interpolator->fit(cpp_points);
}

double predict(KalmanInterpolatorHandle handle, double x) {
    if (!handle) return 0.0;
    
    KalmanInterpolator* interpolator = static_cast<KalmanInterpolator*>(handle);
    return interpolator->predict(x);
}

const char* getDescription(KalmanInterpolatorHandle handle) {
    if (!handle) return "";
    
    KalmanInterpolator* interpolator = static_cast<KalmanInterpolator*>(handle);
    std::string desc = interpolator->getDescription();
    
    // バッファに安全にコピー
    std::strncpy(description_buffer, desc.c_str(), sizeof(description_buffer) - 1);
    description_buffer[sizeof(description_buffer) - 1] = '\0';
    
    return description_buffer;
}

} // extern "C"
