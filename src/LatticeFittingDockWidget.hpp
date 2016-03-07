#pragma once
#include <QtWidgets>
#include "ThresholdWidget.hpp"
#include "PointDetectorWidget.hpp"
#include "LatticeFitterWidget.hpp"

class LatticeFittingDockWidget : public QDockWidget
{
  Q_OBJECT
public:
  explicit LatticeFittingDockWidget(QWidget* parent = 0);

private:
  friend class LatticeFittingWindow;
  ThresholdWidget* _thresholdWidget;
  PointDetectorWidget* _pointDetectorWidget;
  LatticeFitterWidget* _latticeFitterWidget;
};
