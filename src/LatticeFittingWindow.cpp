#include <QtWidgets>
#include "LatticeFittingWindow.hpp"

LatticeFittingWindow::LatticeFittingWindow()
{
  _layout = new QGridLayout(this);

  _imageLabel = new QLabel(this);
//  _imageLabel->setBackgroundRole(QPalette::Base);
//  _imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  _imageLabel->setScaledContents(true);

  _pointsLabel = new QLabel(this);

  _scrollArea = new QScrollArea(this);
  _scrollArea->setBackgroundRole(QPalette::Dark);
  _scrollArea->setWidget(_imageLabel);

  _slider = new QSlider(Qt::Horizontal, this);
  _slider->setMinimum(1);
  _slider->setMaximum(250);

  _layout->addWidget(_scrollArea, 0, 0);
  _layout->addWidget(_slider, 1, 0);
  _layout->addWidget(_pointsLabel, 1, 1);

  QWidget* centralWidget = new QWidget(this);
  centralWidget->setLayout(_layout);
  setCentralWidget(centralWidget);

  createActions();
  createMenus();

  resize(QGuiApplication::primaryScreen()->availableSize() / 1.5);
}

bool LatticeFittingWindow::loadFile(const QString& fileName)
{
  _cvImage = cv::imread(fileName.toStdString());

  if (!_cvImage.data) {
    QMessageBox::information(
      this, QGuiApplication::applicationDisplayName(),
      tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
    setWindowFilePath(QString());
    _imageLabel->setPixmap(QPixmap());
    _imageLabel->adjustSize();
    return false;
  }

  cv::Mat rgb;
  cv::cvtColor(_cvImage, rgb, CV_BGR2RGB);
  auto pixmap = QPixmap::fromImage(
                  QImage(static_cast<uchar*>(rgb.data),
                         rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888));

  _imageLabel->setPixmap(pixmap);
  _imageLabel->adjustSize();

  cv::cvtColor(_cvImage, _cvGray, cv::COLOR_BGR2GRAY);

  setWindowFilePath(fileName);

  return true;
}

void LatticeFittingWindow::open()
{
  const auto picturesLocations =
    QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);

  QFileDialog dialog(
    this, tr("Open File"),
    picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last()
  );

  dialog.setAcceptMode(QFileDialog::AcceptOpen);
  dialog.selectMimeTypeFilter("image/jpeg");

  while (dialog.exec() && !loadFile(dialog.selectedFiles().first()));
}

void LatticeFittingWindow::detect()
{
  cv::Mat points_image = _cvImage.clone();
  _pointDetector.detect(_cvGray, points_image, _points);

  cv::cvtColor(points_image, points_image, CV_BGR2RGB);
  auto pixmap = QPixmap::fromImage(
                  QImage(static_cast<uchar*>(points_image.data),
                         points_image.cols, points_image.rows,
                         points_image.step, QImage::Format_RGB888));

  _imageLabel->setPixmap(pixmap);
  _imageLabel->adjustSize();
}

void LatticeFittingWindow::updatePointsDisplay()
{
  auto value = _slider->value();
  _pointsLabel->setText(QString::number(value));
  _pointDetector._max_corners = value;
  detect();
}

void LatticeFittingWindow::createActions()
{
  _openAct = new QAction(tr("&Open Image..."), this);
  _openAct->setShortcut(tr("Ctrl+O"));
  connect(_openAct, &QAction::triggered, this, &LatticeFittingWindow::open);

  _exitAct = new QAction(tr("E&xit"), this);
  _exitAct->setShortcut(tr("Ctrl+Q"));
  connect(_exitAct, &QAction::triggered, this, &LatticeFittingWindow::close);

  _detectAct = new QAction(tr("Detect Grid &Points"), this);
  _detectAct->setShortcut(tr("Ctrl+P"));
  connect(_detectAct, &QAction::triggered, this, &LatticeFittingWindow::detect);

  connect(_slider, &QSlider::valueChanged, this, &LatticeFittingWindow::updatePointsDisplay);
}

void LatticeFittingWindow::createMenus()
{
  _fileMenu = new QMenu(tr("&File"), this);
  _fileMenu->addAction(_openAct);
  _fileMenu->addSeparator();
  _fileMenu->addAction(_exitAct);

  _detectMenu = new QMenu(tr("&Detection"), this);
  _detectMenu->addAction(_detectAct);

  menuBar()->addMenu(_fileMenu);
  menuBar()->addMenu(_detectMenu);
}
