#pragma once
#include <QMainWindow>
#include <opencv2/opencv.hpp>

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
class QSlider;
class QGridLayout;
class QFileDialog;
class ImageProcessor;
class PointDetector;
class LatticeFittingDockWidget;

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
  void createActions();
  void createMenus();

  QGridLayout* _layout;
  QLabel* _imageLabel;
  QScrollArea* _scrollArea;
  QSlider* _slider;

  ImageProcessor* _imageProcessor;

  PointDetector* _pointDetector;

  QFileDialog* _openDialog;
  QAction* _openAct;
  QAction* _exitAct;
  QAction* _detectAct;

  QMenu* _fileMenu;
  QMenu* _detectMenu;

  LatticeFittingDockWidget* _dockWidget;

  cv::Mat _cvImage;
  cv::Mat _cvGray;
};
