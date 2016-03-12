#include "LatticeFitterWidget.hpp"
#include <QVBoxLayout>

LatticeFitterWidget::LatticeFitterWidget(QWidget* parent)
  : QGroupBox(QStringLiteral("Find best lattice"), parent),
    _button(new QPushButton(QStringLiteral("Start"), parent)),
    _progressBar(new QProgressBar(parent))
{
  setEnabled(false);
  _progressBar->hide();
  auto layout = new QVBoxLayout(this);
  layout->addWidget(_button);
  layout->addWidget(_progressBar);
}
