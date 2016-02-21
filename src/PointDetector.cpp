#include <opencv2/imgproc.hpp>
#include "PointDetector.hpp"

PointDetector::PointDetector
(const cv::Mat& image, const cv::Mat& image_gray, QObject* parent)
  : QObject(parent),
    _image(image),
    _image_gray(image_gray) {}

void PointDetector::detect()
{
  _points_image = _image.clone();

  cv::goodFeaturesToTrack(
    _image_gray, _points, _max_corners, _qualityLevel, _minDistance, cv::Mat(),
    _blockSize, _useHarrisDetector, _k
  );

  for (const auto& point : _points)
    cv::circle(_points_image, point, _pointRadius, cv::Scalar(0, 0, 255), -1, 8, 0);
}
