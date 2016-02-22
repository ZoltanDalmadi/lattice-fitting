#include "LatticeFittingDockWidget.hpp"

LatticeFittingDockWidget::LatticeFittingDockWidget(QWidget* parent)
  : QDockWidget(QStringLiteral("Image Tools"), parent),
    _thresholdWidget(new ThresholdWidget(this)),
    _pointDetectorWidget(new PointDetectorWidget(this))
{
  setFeatures(QDockWidget::DockWidgetMovable);

  auto contents = new QWidget();
  auto layout = new QVBoxLayout(contents);

  layout->addWidget(_thresholdWidget);
  layout->addWidget(_pointDetectorWidget);

  layout->addStretch();
  setWidget(contents);
}
