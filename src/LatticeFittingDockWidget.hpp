#pragma once
#include <QtWidgets>

class ThresholdWidget;

class LatticeFittingDockWidget : public QDockWidget
{
  Q_OBJECT
public:
  explicit LatticeFittingDockWidget(QWidget* parent = 0);

private:
  ThresholdWidget* _thresholdWidget;
};
