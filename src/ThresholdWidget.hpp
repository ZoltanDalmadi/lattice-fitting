#pragma once
#include <QtWidgets>

class ThresholdWidget : public QGroupBox
{
  Q_OBJECT
public:
  explicit ThresholdWidget(QWidget* parent = 0);

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
