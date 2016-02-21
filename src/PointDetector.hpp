#pragma once
#include <QObject>
#include <opencv2/opencv.hpp>

class PointDetector : public QObject
{
  Q_OBJECT

public:
  PointDetector
  (const cv::Mat& image, const cv::Mat& image_gray, QObject* parent = 0);

public slots:
  void detect();
  void setMaxCorners(size_t value);
  //  void setQualityLevel(double value);
  //  void setMinDistance(double value);
  //  void setBlockSize(size_t value);
  //  void setPointRadius(int value);
  //  void useHarrisDetector(bool value);
  //  void setK(double value);

private:
  friend class LatticeFittingWindow;

  const cv::Mat& _image;
  const cv::Mat& _image_gray;
  cv::Mat _points_image;
  std::vector<cv::Point2f> _points;

  size_t _max_corners = 23;
  double _qualityLevel = 0.01;
  double _minDistance = 10;
  size_t _blockSize = 3;
  int _pointRadius = 4;
  bool _useHarrisDetector = false;
  double _k = 0.04;
};

inline void PointDetector::setMaxCorners(size_t value)
{
  _max_corners = value;
}
