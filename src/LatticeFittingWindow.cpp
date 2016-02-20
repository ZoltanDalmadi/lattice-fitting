#include <QtWidgets>
#include "LatticeFittingWindow.hpp"

LatticeFittingWindow::LatticeFittingWindow()
  : _layout(new QGridLayout),
    _imageLabel(new QLabel(this)),
    _scrollArea(new QScrollArea(this)),
    _slider(new QSlider(Qt::Horizontal, this)),
    _imageProcessor(new ImageProcessor(this))
{
  _imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  _imageLabel->setScaledContents(true);

  _scrollArea->setBackgroundRole(QPalette::Dark);
  _scrollArea->setWidget(_imageLabel);

  _slider->setMinimum(1);
  _slider->setMaximum(250);

  _layout->addWidget(_scrollArea, 0, 0);
  _layout->addWidget(_slider, 1, 0);

  QWidget* centralWidget = new QWidget(this);
  centralWidget->setLayout(_layout);
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

  setStatusBar(new QStatusBar());
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

  _imageLabel->setPixmap(_imageProcessor->mat2QPixmap(_cvImage));
  _imageLabel->adjustSize();

  cv::cvtColor(_cvImage, _cvGray, cv::COLOR_BGR2GRAY);

  setWindowFilePath(fileName);

  statusBar()->showMessage("Image Loaded successfully.");

  return true;
}

void LatticeFittingWindow::open()
{
  while (_openDialog->exec() && !loadFile(_openDialog->selectedFiles().first()));

  if (!_openDialog->selectedFiles().isEmpty())
    _openDialog->setDirectory(
      QFileInfo(_openDialog->selectedFiles().first()).path());
}

void LatticeFittingWindow::detect()
{
  cv::Mat points_image = _cvImage.clone();
  _pointDetector.detect(_cvGray, points_image, _points);

  _imageLabel->setPixmap(_imageProcessor->mat2QPixmap(points_image));
  _imageLabel->adjustSize();
}

void LatticeFittingWindow::updatePointsDisplay()
{
  auto value = _slider->value();
  statusBar()->showMessage("Max points: " + QString::number(value));
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
  connect(_slider, &QSlider::valueChanged, this,
          &LatticeFittingWindow::updatePointsDisplay);
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
