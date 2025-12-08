#ifndef PYTHON_WRAPPER_H
#define PYTHON_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

// ========================================
// LineSegment構造体 (C互換)
// ========================================
typedef struct {
    double x1, y1, x2, y2;
} CLineSegment;

// ========================================
// Point構造体 (C互換)
// ========================================
typedef struct {
    double x, y;
} CPoint;

// ========================================
// utils.so用のラッパー関数
// ========================================

// 線分から点を抽出
void extractPoints(const CLineSegment* segments, int num_segments, 
                   CPoint* points, int* num_points);

// PPM画像として保存
void saveToPPM(const char* filename, 
               const CLineSegment* segments, int num_segments,
               const CPoint* curve_points, int num_curve_points,
               int width, int height);

// ========================================
// interpolators.so用のラッパー関数
// ========================================

// KalmanInterpolatorのオペークポインタ型
typedef void* KalmanInterpolatorHandle;

// KalmanInterpolatorを作成
KalmanInterpolatorHandle createKalmanInterpolator(double process_noise, 
                                                   double observation_noise);

// KalmanInterpolatorを削除
void deleteKalmanInterpolator(KalmanInterpolatorHandle handle);

// フィッティング
void fit(KalmanInterpolatorHandle handle, const CPoint* points, int num_points);

// 予測
double predict(KalmanInterpolatorHandle handle, double x);

// 説明を取得
const char* getDescription(KalmanInterpolatorHandle handle);

#ifdef __cplusplus
}
#endif

#endif // PYTHON_WRAPPER_H
