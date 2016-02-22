#include "PointDetectorWidget.hpp"

// -----------------------------------------------------------------------------
IntSliderSpinBoxCombo::IntSliderSpinBoxCombo(QWidget* parent)
  : IntSliderSpinBoxCombo(QStringLiteral("Label"), 100, parent) {}

IntSliderSpinBoxCombo::IntSliderSpinBoxCombo
(const QString& label, int maxValue, QWidget* parent)
  : QWidget(parent),
    _label(new QLabel(label, this)),
    _slider(new QSlider(Qt::Horizontal, this)),
    _spinbox(new QSpinBox(this))
{
  _slider->setMaximum(maxValue);
  _spinbox->setMaximum(maxValue);

  auto layout = new QGridLayout(this);
  layout->addWidget(_label, 0, 0, 1, 2, Qt::AlignCenter);
  layout->addWidget(_slider, 1, 0);
  layout->addWidget(_spinbox, 1, 1);

  connect(_slider, SIGNAL(valueChanged(int)), _spinbox, SLOT(setValue(int)));
  connect(_spinbox, SIGNAL(valueChanged(int)), _slider, SLOT(setValue(int)));

  connect(_slider, SIGNAL(valueChanged(int)), this, SLOT(notifyValueChanged(int)));
  connect(_spinbox, SIGNAL(valueChanged(int)), this, SLOT(notifyValueChanged(int)));
}

void IntSliderSpinBoxCombo::notifyValueChanged(int value)
{
  emit valueChanged(value);
}

void IntSliderSpinBoxCombo::setMinimum(int value)
{
  _slider->setMinimum(value);
  _spinbox->setMinimum(value);
  emit minimumChanged(value);
}

void IntSliderSpinBoxCombo::setMaximum(int value)
{
  _slider->setMaximum(value);
  _spinbox->setMaximum(value);
  emit maximumChanged(value);
}

// -----------------------------------------------------------------------------
DoubleSliderSpinBoxCombo::DoubleSliderSpinBoxCombo(QWidget* parent)
  : DoubleSliderSpinBoxCombo(QStringLiteral("Label"), 100.0, parent) {}

DoubleSliderSpinBoxCombo::DoubleSliderSpinBoxCombo
(const QString& label, double maxValue, QWidget* parent)
  : QWidget(parent),
    _label(new QLabel(label, this)),
    _slider(new DoubleSlider(Qt::Horizontal, this)),
    _spinbox(new QDoubleSpinBox(this))
{
  _slider->setDoubleMaximum(maxValue);
  _spinbox->setMaximum(maxValue);
  _spinbox->setSingleStep(0.01);

  auto layout = new QGridLayout(this);

  layout->addWidget(_label, 0, 0, 1, 2, Qt::AlignCenter);
  layout->addWidget(_slider, 1, 0);
  layout->addWidget(_spinbox, 1, 1);

  connect(
    _slider, SIGNAL(doubleValueChanged(double)),
    _spinbox, SLOT(setValue(double))
  );
  connect(
    _spinbox, SIGNAL(valueChanged(double)),
    _slider, SLOT(setDoubleValue(double))
  );

  connect(
    _slider, SIGNAL(doubleValueChanged(double)),
    this, SLOT(notifyValueChanged(double))
  );

  connect(
    _spinbox, SIGNAL(valueChanged(double)),
    this, SLOT(notifyValueChanged(double))
  );
}

void DoubleSliderSpinBoxCombo::notifyValueChanged(double value)
{
  emit valueChanged(value);
}

void DoubleSliderSpinBoxCombo::setMinimum(double value)
{
  _slider->setDoubleMinimum(value);
  _spinbox->setMinimum(value);
  emit minimumChanged(value);
}

void DoubleSliderSpinBoxCombo::setMaximum(double value)
{
  _slider->setDoubleMaximum(value);
  _spinbox->setMaximum(value);
  emit maximumChanged(value);
}

void DoubleSliderSpinBoxCombo::setSingleStep(double value)
{
  _spinbox->setSingleStep(value);
}

// -----------------------------------------------------------------------------
PointDetectorWidget::PointDetectorWidget(QWidget* parent)
  : QGroupBox(QStringLiteral("Detect Lattice Points"), parent),
    _max_corners(
      new IntSliderSpinBoxCombo(QStringLiteral("Max Points"), 500, this)),
    _qualityLevel(
      new DoubleSliderSpinBoxCombo(QStringLiteral("Quality"), 1.0, this)),
    _minDistance(
      new DoubleSliderSpinBoxCombo(QStringLiteral("Minimum Distance"), 20.0, this)),
    _blockSize(
      new IntSliderSpinBoxCombo(QStringLiteral("Block Size"), 30, this)),
    _pointRadius(
      new IntSliderSpinBoxCombo(QStringLiteral("Point Radius"), 10, this))
{
  setCheckable(true);
  setChecked(false);

  _max_corners->setValue(25);
  _qualityLevel->setValue(0.01);
  _qualityLevel->setMinimum(0.01);
  _minDistance->setValue(10.0);
  _blockSize->setMinimum(2);
  _blockSize->setValue(3);
  _pointRadius->setValue(4);

  auto layout = new QVBoxLayout(this);
  layout->setSpacing(0);
  layout->addWidget(_max_corners);
  layout->addWidget(_qualityLevel);
  layout->addWidget(_minDistance);
  layout->addWidget(_blockSize);
  layout->addWidget(_pointRadius);

  createConnections();
}

void PointDetectorWidget::notifyParameterChanged()
{
  emit parameterChanged();
}

void PointDetectorWidget::createConnections()
{
  connect(_max_corners, SIGNAL(valueChanged(int)),
          this, SLOT(notifyParameterChanged()));

  connect(_qualityLevel, SIGNAL(valueChanged(double)),
          this, SLOT(notifyParameterChanged()));

  connect(_minDistance, SIGNAL(valueChanged(double)),
          this, SLOT(notifyParameterChanged()));

  connect(_blockSize, SIGNAL(valueChanged(int)),
          this, SLOT(notifyParameterChanged()));

  connect(_pointRadius, SIGNAL(valueChanged(int)),
          this, SLOT(notifyParameterChanged()));
}
