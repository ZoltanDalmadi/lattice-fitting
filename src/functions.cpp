#include "functions.hpp"

bool loadImage(const QString& fileName, cv::Mat& dest)
{
  dest = cv::imread(fileName.toStdString());
  return dest.data != nullptr;
}

QPixmap mat2QPixmap(const cv::Mat& image)
{
  cv::Mat rgb;
  cv::cvtColor(image, rgb, CV_BGR2RGB);
  return QPixmap::fromImage(
           QImage(static_cast<uchar*>(rgb.data),
                  rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888));
}

QPixmap mat2QPixmapGray(const cv::Mat& image)
{
  return QPixmap::fromImage(
           QImage(static_cast<uchar*>(image.data),
                  image.cols, image.rows, image.step, QImage::Format_Grayscale8));
}

void applyThreshold(const cv::Mat& image, cv::Mat& dest, double value)
{
  cv::threshold(image, dest, value, 255.0, cv::THRESH_BINARY);
}

float diam(std::vector<cv::Point2f>& points)
{
  float maxX = 0.0f;
  float maxY = 0.0f;
  auto size = points.size();

  for (size_t i = 0; i < size; ++i) {
    for (size_t j = i + 1; j < size; ++j) {
      auto xValue = std::abs(points[i].x - points[j].x);
      auto yValue = std::abs(points[i].y - points[j].y);

      if (xValue > maxX)
        maxX = xValue;

      if (yValue > maxY)
        maxY = yValue;
    }
  }

  return std::max(maxX, maxY);
}
