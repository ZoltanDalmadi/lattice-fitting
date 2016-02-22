#pragma once
#include <QtWidgets>

class ThresholdWidget : public QGroupBox
{
  Q_OBJECT
public:
  explicit ThresholdWidget(QWidget* parent = 0);
  int level() const { return _level; }

signals:
  void levelChanged(int level);

private slots:
  void setLevel(int level);

private:
  void createConnections();

  QLabel* _label;
  QSlider* _slider;
  QSpinBox* _spinbox;
  int _level;
};
