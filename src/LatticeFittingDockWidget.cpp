#include "LatticeFittingDockWidget.hpp"

LatticeFittingDockWidget::LatticeFittingDockWidget(QWidget* parent)
  : QDockWidget(QStringLiteral("Image Tools"), parent),
    _thresholdWidget(new ThresholdWidget(this)),
    _pointDetectorWidget(new PointDetectorWidget(this)),
    _latticeFitterWidget(new LatticeFitterWidget(this))
{
  setEnabled(false);
  setFeatures(QDockWidget::DockWidgetMovable);

  auto contents = new QWidget();
  auto layout = new QVBoxLayout(contents);

  layout->addWidget(_thresholdWidget);
  layout->addWidget(_pointDetectorWidget);
  layout->addWidget(_latticeFitterWidget);

  layout->addStretch();
  setWidget(contents);

  connect(_pointDetectorWidget, SIGNAL(toggled(bool)),
          _latticeFitterWidget, SLOT(setEnabled(bool)));
}
