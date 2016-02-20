#include <opencv2/imgproc.hpp>
#include "PointDetector.hpp"

void PointDetector::detect(
  const cv::Mat& image, cv::Mat& points_image, std::vector<cv::Point2f>& points)
{
  cv::goodFeaturesToTrack(
    image, points, _max_corners, _qualityLevel, _minDistance, cv::Mat(),
    _blockSize, _useHarrisDetector, _k
  );

  for (const auto& point : points)
    cv::circle(points_image, point, _pointRadius, cv::Scalar(0, 0, 255), -1, 8, 0);
}
