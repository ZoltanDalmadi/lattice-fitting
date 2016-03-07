#include "LatticeFitterWidget.hpp"
#include <QHBoxLayout>

LatticeFitterWidget::LatticeFitterWidget(QWidget* parent)
  : QGroupBox(QStringLiteral("Find best lattice"), parent),
    _button(new QPushButton(QStringLiteral("Start"), parent))
{
  setEnabled(false);
  auto layout = new QHBoxLayout(this);
  layout->addWidget(_button);
}
