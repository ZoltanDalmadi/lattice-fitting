#pragma once
#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include "PointDetector.hpp"

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
class QSlider;
class QGridLayout;

class LatticeFittingWindow : public QMainWindow
{
  Q_OBJECT

public:
  LatticeFittingWindow();
  bool loadFile(const QString&);

private slots:
  void open();
  void detect();
  void updatePointsDisplay();

private:
  friend class PointDetector;
  PointDetector _pointDetector;

  void createActions();
  void createMenus();

  QLabel* _imageLabel;
  QLabel* _pointsLabel;
  QScrollArea* _scrollArea;

  QAction* _openAct;
  QAction* _exitAct;
  QAction* _detectAct;

  QMenu* _fileMenu;
  QMenu* _detectMenu;

  cv::Mat _cvImage;
  cv::Mat _cvGray;
  std::vector<cv::Point2f> _points;
  QSlider* _slider;
  QGridLayout* _layout;
};
