#pragma once
#include <QObject>
#include <opencv2/opencv.hpp>
#include "PointDetectorWidget.hpp"

class PointDetector : public QObject
{
  Q_OBJECT

public:
  PointDetector
  (const cv::Mat& image, const cv::Mat& image_gray,
   PointDetectorWidget& widget, QObject* parent = 0);

public slots:
  void detect();

private:
  friend class LatticeFittingWindow;

  const cv::Mat& _image;
  const cv::Mat& _image_gray;
  PointDetectorWidget& _widget;
  cv::Mat _points_image;
  std::vector<cv::Point2f> _points;
};
