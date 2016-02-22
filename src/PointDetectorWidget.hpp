#pragma once
#include <QtWidgets>

// -----------------------------------------------------------------------------
class DoubleSlider : public QSlider
{
  Q_OBJECT

public:
  DoubleSlider(Qt::Orientation orientation, QWidget* parent = 0)
    : QSlider(orientation, parent)
  {
    connect(this, SIGNAL(valueChanged(int)),
            this, SLOT(notifyValueChanged(int)));
  }

  DoubleSlider(QWidget* parent = 0)
    : DoubleSlider(Qt::Vertical, parent)
  {}

  double resolution() const { return _resolution; }
  void setResolution(double resolution) { _resolution = resolution; }

signals:
  void doubleValueChanged(double value);

public slots:
  void notifyValueChanged(int value)
  {
    double doubleValue = value / _resolution;
    emit doubleValueChanged(doubleValue);
  }

  void setDoubleValue(double value)
  {
    setValue(static_cast<int>(value * _resolution));
  }

  void setDoubleMaximum(double value)
  {
    setMaximum(static_cast<int>(value * _resolution));
  }

private:
  double _resolution = 100.0;
};

// -----------------------------------------------------------------------------
class IntSliderSpinBoxCombo : public QWidget
{
  Q_OBJECT
public:
  IntSliderSpinBoxCombo(QWidget* parent = 0);
  IntSliderSpinBoxCombo(const QString& label, int maxValue, QWidget* parent = 0);

  int value() const { return _slider->value(); }
  int maximum() const { return _slider->maximum(); }

signals:
  void valueChanged(int);
  void maximumChanged(int);

public slots:
//  void notifyValueChanged(int value);
  void setValue(int value);
  void setMaximum(int value);

private:
  QLabel* _label;
  QSlider* _slider;
  QSpinBox* _spinbox;
};

// -----------------------------------------------------------------------------
class DoubleSliderSpinBoxCombo : public QWidget
{
  Q_OBJECT
public:
  DoubleSliderSpinBoxCombo(QWidget* parent = 0);
  DoubleSliderSpinBoxCombo(const QString& label, double maxValue, QWidget* parent = 0);

  double value() const { return _slider->value(); }
  double maximum() const { return _slider->maximum(); }

signals:
  void valueChanged(double);
  void maximumChanged(double);

public slots:
//  void notifyValueChanged(double value);
  void setValue(double value);
  void setMaximum(double value);

private:
  QLabel* _label;
  DoubleSlider* _slider;
  QDoubleSpinBox* _spinbox;
};

// -----------------------------------------------------------------------------
class PointDetectorWidget : public QGroupBox
{
  Q_OBJECT
public:
  explicit PointDetectorWidget(QWidget* parent = 0);

signals:
  void parameterChanged();

private slots:
  void notifyParameterChanged();
//  void setMaxCorners(int maxCorners);
//  void setQualityLevel(double quality);
//  void setMinDistance(double distance);
//  void setBlockSize(int blockSize);
//  void setPointRadius(int pointRadius);
//  void setUseHarrisDetector(bool value);
//  void setK(double k);

private:
  void createConnections();

  QCheckBox* _useHarrisDetector;
  IntSliderSpinBoxCombo* _max_corners; // 25
  DoubleSliderSpinBoxCombo* _qualityLevel; // 0.01
  DoubleSliderSpinBoxCombo* _minDistance; // 10.0
  IntSliderSpinBoxCombo* _blockSize; // 3
  IntSliderSpinBoxCombo* _pointRadius; // 4
  DoubleSliderSpinBoxCombo* _k; // 0.04
};
