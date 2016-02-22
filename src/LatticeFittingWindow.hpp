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

private:
  void createActions();
  void createMenus();

  QLabel* _imageLabel;
  QScrollArea* _scrollArea;

  LatticeFittingDockWidget* _dockWidget;

  ImageProcessor* _imageProcessor;
  PointDetector* _pointDetector;

  QFileDialog* _openDialog;
  QAction* _openAct;
  QAction* _exitAct;
  QAction* _detectAct;

  QMenu* _fileMenu;
  QMenu* _detectMenu;

  cv::Mat _cvImage;
  cv::Mat _cvGray;
};
