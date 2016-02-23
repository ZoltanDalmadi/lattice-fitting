#pragma once
#include <QPixmap>
#include <opencv2/opencv.hpp>

bool loadImage(const QString& fileName, cv::Mat& dest);
QPixmap mat2QPixmap(const cv::Mat& image);
QPixmap mat2QPixmapGray(const cv::Mat& image);
void applyThreshold(const cv::Mat& image, cv::Mat& dest, double value);
