#pragma once

#include <QGroupBox>
#include <QPushButton>
#include <QProgressBar>

class LatticeFitterWidget : public QGroupBox
{
  Q_OBJECT
public:
  explicit LatticeFitterWidget(QWidget* parent = 0);

  QPushButton* _button;
  QProgressBar* _progressBar;
};
