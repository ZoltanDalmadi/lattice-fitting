#pragma once
#include <QtWidgets>
#include "ThresholdWidget.hpp"
#include "PointDetectorWidget.hpp"

class LatticeFittingDockWidget : public QDockWidget
{
  Q_OBJECT
public:
  explicit LatticeFittingDockWidget(QWidget* parent = 0);

private:
  friend class LatticeFittingWindow;
  ThresholdWidget* _thresholdWidget;
  PointDetectorWidget* _pointDetectorWidget;
};
