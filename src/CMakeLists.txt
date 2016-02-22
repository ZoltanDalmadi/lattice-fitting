add_executable(lattice-fitting
  lattice-fitting.cpp
  ImageProcessor.cpp
  LatticeFittingDockWidget.cpp
  LatticeFittingWindow.cpp
  PointDetector.cpp
  ThresholdWidget.cpp
  PointDetectorWidget.cpp
)

target_link_libraries(lattice-fitting
  Qt5::Widgets
  ${OpenCV_LIBS}
)
