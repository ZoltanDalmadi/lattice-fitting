file(GLOB SOURCES *.cpp)

add_executable(lattice-fitting ${SOURCES})

target_link_libraries(lattice-fitting
  Qt5::Widgets
  ${OpenCV_LIBS}
  ${NTL_LIBRARIES}
)
