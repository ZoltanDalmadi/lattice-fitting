#include "PointDetector.hpp"

PointDetector::PointDetector
(PointDetectorWidget& widget, QObject* parent)
  : QObject(parent),
    _widget(widget) {}

void PointDetector::detect(const cv::Mat& gray)
{
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
}

void PointDetector::drawPoints(QPixmap& pixmap)
{
  QPainter painter(&pixmap);
  painter.setPen(QPen(Qt::red, 2));
  auto radius = _widget._pointRadius->value();

  for (const auto& point : _points) {
    painter.drawEllipse(QPoint(point.x, point.y), radius, radius);
  }
}
