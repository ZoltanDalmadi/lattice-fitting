#pragma once
#include <QPixmap>
#include <QPolygon>
#include <QVector>
#include <QPoint>
#include <opencv2/opencv.hpp>

struct Lattice;

QPixmap mat2QPixmap(const cv::Mat& image);
QPixmap mat2QPixmapGray(const cv::Mat& image);
void applyThreshold(const cv::Mat& image, cv::Mat& dest, double value);
QPolygon convexHull(const std::vector<cv::Point2f>& points);
QVector<QPoint> generateGrid(const Lattice& lattice, const QPolygon& poly);
