#pragma once
#include <QObject>
#include <opencv2/opencv.hpp>
#include "PointDetectorWidget.hpp"

class PointDetector : public QObject
{
  Q_OBJECT

public:
  PointDetector(PointDetectorWidget& widget, QObject* parent = 0);

public slots:
  void detect(const cv::Mat& gray);
  void drawPoints(QPixmap& pixmap);

private:
  friend class LatticeFittingWindow;

  PointDetectorWidget& _widget;
  cv::Mat _points_image;
  std::vector<cv::Point2f> _points;
};
