#include <opencv2/imgproc.hpp>
#include "PointDetector.hpp"

PointDetector::PointDetector
(PointDetectorWidget& widget, QObject* parent)
  : QObject(parent),
    _widget(widget) {}

void PointDetector::detect(const cv::Mat& image, const cv::Mat& gray)
{
  _points_image = image.clone();

  cv::goodFeaturesToTrack(
    gray, _points,
    _widget._max_corners->value(),
    _widget._qualityLevel->value(),
    _widget._minDistance->value(),
    cv::Mat(),
    _widget._blockSize->value(),
    false,
    0.04
  );

  for (const auto& point : _points)
    cv::circle(_points_image, point, _widget._pointRadius->value(),
               cv::Scalar(0, 0, 255), -1, 8, 0);
}
