#include <QtWidgets>
#include <QtConcurrent/QtConcurrent>
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
  createDialog();

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
  if (_dockWidget->_thresholdWidget->isChecked()) {
    _pointDetector->detect(_cvThresh);
    _lastPixmap = mat2QPixmapGray(_cvThresh);
  } else {
    _pointDetector->detect(_cvGray);
    _lastPixmap = _originalPixmap;
  }

  _pointDetector->drawPoints(_lastPixmap);
  setImageToLast();
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

void LatticeFittingWindow::drawGrid(const Lattice& lattice)
{
  QPainter painter(&_lastPixmap);
  painter.setPen(QPen(Qt::red, 2));

  auto gridPoints = generateGrid(lattice,
      _lastPixmap.width(), _lastPixmap.height());

  painter.setPen(QPen(Qt::blue, 1));
  painter.drawLines(gridPoints);

  setImageToLast();

  QString message(QStringLiteral("Lattice found. Total error: "));
  message.append(QString::number(lattice.total_error));
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

  connect(_dockWidget->_pointDetectorWidget, &PointDetectorWidget::parameterChanged,
          this, &LatticeFittingWindow::detect);

  connect(_dockWidget->_pointDetectorWidget, &PointDetectorWidget::toggled,
          this, &LatticeFittingWindow::toggleDetect);

  connect(_dockWidget->_thresholdWidget, &ThresholdWidget::levelChanged,
          this, &LatticeFittingWindow::threshold);

  connect(_dockWidget->_thresholdWidget, &ThresholdWidget::toggled,
          this, &LatticeFittingWindow::toggleThreshold);

  connect(_dockWidget->_latticeFitterWidget->_button, &QPushButton::released,
          _latticeFitter,
  [this]() {
    QtConcurrent::run(
          _latticeFitter, &LatticeFitter::findBestLattice, _pointDetector->_points);
  });

  qRegisterMetaType<Lattice>("Lattice");

  connect(_latticeFitter, &LatticeFitter::foundBestLattice,
          this, &LatticeFittingWindow::drawGrid);

  connect(_latticeFitter, &LatticeFitter::latticeFittingStarted,
          _dockWidget->_latticeFitterWidget->_progressBar, &QProgressBar::show);

  connect(_latticeFitter, &LatticeFitter::progressUpdated,
          _dockWidget->_latticeFitterWidget->_progressBar, &QProgressBar::setValue);

  connect(_latticeFitter, &LatticeFitter::foundBestLattice,
          _dockWidget->_latticeFitterWidget->_progressBar, &QProgressBar::hide);
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

void LatticeFittingWindow::createDialog()
{
  const auto picturesLocations =
    QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);

  _openDialog = new QFileDialog(
    this, tr("Open File"),
    picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last()
  );

  _openDialog->setAcceptMode(QFileDialog::AcceptOpen);
  _openDialog->selectMimeTypeFilter("image/jpeg");
}

void LatticeFittingWindow::resetImage()
{
  _imageLabel->setPixmap(_originalPixmap);
  _imageLabel->adjustSize();
}

void LatticeFittingWindow::setImageToLast()
{
  _imageLabel->setPixmap(_lastPixmap);
  _imageLabel->adjustSize();
}
