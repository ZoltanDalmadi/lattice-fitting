#pragma once
#include <QObject>
#include <QPixmap>
#include <opencv2/opencv.hpp>

class ImageProcessor : public QObject
{
  Q_OBJECT
public:
  explicit ImageProcessor(QObject* parent = 0);

signals:

public slots:
  bool loadImage(const QString& fileName, cv::Mat& dest);
  QPixmap mat2QPixmap(const cv::Mat& image);
};