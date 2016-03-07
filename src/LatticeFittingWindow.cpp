#include <QtWidgets>
#include "LatticeFittingWindow.hpp"
#include "PointDetector.hpp"
#include "functions.hpp"
#include "LatticeFitter.hpp"
#include "LatticeFittingDockWidget.hpp"

LatticeFittingWindow::LatticeFittingWindow()
  : _imageLabel(new QLabel(this)),
    _scrollArea(new QScrollArea(this)),
    _dockWidget(new LatticeFittingDockWidget(this)),
    _pointDetector(new PointDetector(*_dockWidget->_pointDetectorWidget, this)),
    _latticeFitter(new LatticeFitter(this))
{
  _imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  _imageLabel->setScaledContents(true);

  _scrollArea->setBackgroundRole(QPalette::Dark);
  _scrollArea->setWidget(_imageLabel);

  auto layout = new QHBoxLayout;
  layout->addWidget(_scrollArea);

  auto centralWidget = new QWidget(this);
  centralWidget->setLayout(layout);
  setCentralWidget(centralWidget);

  createActions();
  createMenus();

  // dialog --------------------------------------------------------------------
  const auto picturesLocations =
    QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);

  _openDialog = new QFileDialog(
    this, tr("Open File"),
    picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last()
  );

  _openDialog->setAcceptMode(QFileDialog::AcceptOpen);
  _openDialog->selectMimeTypeFilter("image/jpeg");

  setStatusBar(new QStatusBar);

  addDockWidget(Qt::RightDockWidgetArea, _dockWidget);

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

  _originalPixmap = mat2QPixmap(_cvImage);
  resetImage();

  cv::cvtColor(_cvImage, _cvGray, cv::COLOR_BGR2GRAY);

  setWindowFilePath(fileName);

  _dockWidget->_thresholdWidget->setChecked(false);
  _dockWidget->_pointDetectorWidget->setChecked(false);

  statusBar()->showMessage(QStringLiteral("Image Loaded successfully."), 10000);

  return true;
}

void LatticeFittingWindow::open()
{
  while (_openDialog->exec() && !loadFile(_openDialog->selectedFiles().first()));

  if (!_openDialog->selectedFiles().isEmpty())
    _openDialog->setDirectory(
      QFileInfo(_openDialog->selectedFiles().first()).path());

  _dockWidget->setEnabled(true);
}

void LatticeFittingWindow::detect()
{
  QPixmap pixmap;

  if (_dockWidget->_thresholdWidget->isChecked()) {
    _pointDetector->detect(_cvThresh);
    pixmap = mat2QPixmapGray(_cvThresh);
  } else {
    _pointDetector->detect(_cvGray);
    pixmap = _originalPixmap;
  }

  _pointDetector->drawPoints(pixmap);
  _imageLabel->setPixmap(pixmap);
  _lastPixmap = pixmap;
  _imageLabel->adjustSize();
}

void LatticeFittingWindow::toggleDetect(bool toggle)
{
  if (toggle) {
    detect();
  } else {
    resetImage();

    if (_dockWidget->_thresholdWidget->isChecked())
      threshold(_dockWidget->_thresholdWidget->level());
  }
}

void LatticeFittingWindow::threshold(int value)
{
  applyThreshold(_cvGray, _cvThresh, static_cast<double>(value));

  if (_dockWidget->_pointDetectorWidget->isChecked()) {
    detect();
  } else {
    _imageLabel->setPixmap(mat2QPixmapGray(_cvThresh));
    _imageLabel->adjustSize();
  }
}

void LatticeFittingWindow::toggleThreshold(bool toggle)
{
  if (toggle) {
    threshold(_dockWidget->_thresholdWidget->level());
  } else {
    resetImage();
  }

  if (_dockWidget->_pointDetectorWidget->isChecked())
    detect();
}

void LatticeFittingWindow::drawGrid()
{
  QPainter painter(&_lastPixmap);
  painter.setPen(QPen(Qt::red, 2));

  std::vector<cv::Point2f> hull;
  cv::convexHull(_pointDetector->_points, hull);

  QPolygon poly;

  for(const auto& point : hull)
    poly.append(QPoint(point.x, point.y));

  painter.drawConvexPolygon(poly);

  _imageLabel->setPixmap(_lastPixmap);
  _imageLabel->adjustSize();

  QString message(QStringLiteral("Lattice found. Total error: "));
  message.append(QString::number(_latticeFitter->best_lattice.total_error));
  statusBar()->showMessage(message);
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

  connect(_dockWidget->_pointDetectorWidget, SIGNAL(parameterChanged()), this,
          SLOT(detect()));
  connect(_dockWidget->_pointDetectorWidget, SIGNAL(toggled(bool)), this,
          SLOT(toggleDetect(bool)));
  connect(_dockWidget->_thresholdWidget, SIGNAL(levelChanged(int)), this,
          SLOT(threshold(int)));
  connect(_dockWidget->_thresholdWidget, SIGNAL(toggled(bool)), this,
          SLOT(toggleThreshold(bool)));

  connect(_dockWidget->_latticeFitterWidget->button(), &QPushButton::released,
          _latticeFitter,
  [this]() {
    _latticeFitter->findBestLattice(_pointDetector->_points);
  });

  connect(_latticeFitter, &LatticeFitter::foundBestLattice,
          this, &LatticeFittingWindow::drawGrid);
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

void LatticeFittingWindow::resetImage()
{
  _imageLabel->setPixmap(_originalPixmap);
  _imageLabel->adjustSize();
}
