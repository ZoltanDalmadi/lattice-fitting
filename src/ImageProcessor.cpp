#include <QtWidgets>
#include "ImageProcessor.hpp"

ImageProcessor::ImageProcessor(QObject* parent)
  : QObject(parent) {}

bool ImageProcessor::loadImage(const QString& fileName, cv::Mat& dest)
{
  dest = cv::imread(fileName.toStdString());
  return dest.data != nullptr;
}

QPixmap ImageProcessor::mat2QPixmap(const cv::Mat& image)
{
  cv::Mat rgb;
  cv::cvtColor(image, rgb, CV_BGR2RGB);
  return QPixmap::fromImage(
           QImage(static_cast<uchar*>(rgb.data),
                  rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888));
}