#pragma once
#include <opencv2/opencv.hpp>

class PointDetector
{
public:
  void detect(const cv::Mat& image,
              cv::Mat& points_image,
              std::vector<cv::Point2f>& points);
private:
  friend class LatticeFittingWindow;
  size_t _max_corners = 23;
  double _qualityLevel = 0.01;
  double _minDistance = 10;
  size_t _blockSize = 3;
  int _pointRadius = 4;
  bool _useHarrisDetector = false;
  double _k = 0.04;
};
