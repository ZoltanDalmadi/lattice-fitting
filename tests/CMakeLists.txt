enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(functions_test functions_test.cpp
    ${CMAKE_SOURCE_DIR}/src/functions.cpp)

target_link_libraries(functions_test
  Qt5::Widgets
  ${OpenCV_LIBS}
  ${GTEST_BOTH_LIBRARIES}
  ${NTL_LIBRARIES}
)

add_test(FunctionsTest functions_test)

add_executable(LatticeFitterTest LatticeFitterTest.cpp
    ${CMAKE_SOURCE_DIR}/src/LatticeFitter.cpp)

target_link_libraries(LatticeFitterTest
  Qt5::Widgets
  ${OpenCV_LIBS}
  ${GTEST_BOTH_LIBRARIES}
  ${NTL_LIBRARIES}
)

add_test(LatticeFitterTest LatticeFitterTest)
