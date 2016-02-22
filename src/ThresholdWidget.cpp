#include "ThresholdWidget.hpp"

ThresholdWidget::ThresholdWidget(QWidget* parent)
  : QGroupBox(QStringLiteral("Threshold Image"), parent),
    _label(new QLabel(QStringLiteral("Level"), this)),
    _slider(new QSlider(Qt::Horizontal, this)),
    _spinbox(new QSpinBox(this))
{
  setCheckable(true);
  setChecked(false);

  _slider->setMaximum(255);
  _spinbox->setMaximum(255);
  setLevel(128);

  auto layout = new QHBoxLayout(this);
  layout->addWidget(_label);
  layout->addWidget(_slider);
  layout->addWidget(_spinbox);

  createConnections();
}

void ThresholdWidget::setLevel(int level)
{
  _level = level;
  _slider->setValue(_level);
  _spinbox->setValue(_level);
  emit levelChanged(_level);
}

void ThresholdWidget::createConnections()
{
  connect(_slider, &QSlider::valueChanged, _spinbox, &QSpinBox::setValue);
  connect(_slider, &QSlider::valueChanged, this, &ThresholdWidget::setLevel);

  // weird: it doesn't compile with the above syntax
  connect(_spinbox, SIGNAL(valueChanged(int)), _slider, SLOT(setValue(int)));
  connect(_spinbox, SIGNAL(valueChanged(int)), this, SLOT(setLevel(int)));
}
