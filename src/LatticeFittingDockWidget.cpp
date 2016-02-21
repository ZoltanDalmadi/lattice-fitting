#include <QtWidgets>
#include "LatticeFittingDockWidget.hpp"
#include "ThresholdWidget.hpp"

LatticeFittingDockWidget::LatticeFittingDockWidget(QWidget* parent)
  : QDockWidget(QStringLiteral("Image Tools"), parent),
    _thresholdWidget(new ThresholdWidget(this))
{
  setFeatures(QDockWidget::DockWidgetMovable);

  auto contents = new QWidget();
  auto layout = new QVBoxLayout(contents);

  layout->addWidget(_thresholdWidget);

  layout->addStretch();
  setWidget(contents);
}
