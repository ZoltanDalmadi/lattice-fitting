#pragma once

#include <QGroupBox>
#include <QPushButton>

class LatticeFitterWidget : public QGroupBox
{
  Q_OBJECT
public:
  explicit LatticeFitterWidget(QWidget* parent = 0);

private:
  QPushButton* _button;
};
