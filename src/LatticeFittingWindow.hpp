#pragma once
#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include "PointDetector.hpp"
#include "ImageProcessor.hpp"

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
class QSlider;
class QGridLayout;
class QFileDialog;

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

  QGridLayout* _layout;
  QLabel* _imageLabel;
  QScrollArea* _scrollArea;
  QSlider* _slider;

  ImageProcessor* _imageProcessor;
  QFileDialog* _openDialog;
  QAction* _openAct;
  QAction* _exitAct;
  QAction* _detectAct;

  QMenu* _fileMenu;
  QMenu* _detectMenu;

  cv::Mat _cvImage;
  cv::Mat _cvGray;
  std::vector<cv::Point2f> _points;
};
