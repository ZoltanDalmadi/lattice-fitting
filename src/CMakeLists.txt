add_executable(lattice-fitting
  lattice-fitting.cpp
  ImageProcessor.cpp
  LatticeFittingWindow.cpp
  PointDetector.cpp
)

target_link_libraries(lattice-fitting
  Qt5::Widgets
  ${OpenCV_LIBS}
)
