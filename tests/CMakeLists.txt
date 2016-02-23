enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(functions_test functions_test.cpp)
target_link_libraries(functions_test
  Qt5::Widgets
  ${OpenCV_LIBS}
  ${GTEST_BOTH_LIBRARIES}
)

add_test(FunctionsTest functions_test)
