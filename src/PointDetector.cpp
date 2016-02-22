#include <opencv2/imgproc.hpp>
#include "PointDetector.hpp"

PointDetector::PointDetector
(const cv::Mat& image, const cv::Mat& image_gray,
 PointDetectorWidget& widget, QObject* parent)
  : QObject(parent),
    _image(image),
    _image_gray(image_gray),
    _widget(widget) {}

void PointDetector::detect()
{
  _points_image = _image.clone();

  cv::goodFeaturesToTrack(
    _image_gray, _points,
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
