#include <algorithm>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/linestring.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <NTL/mat_ZZ.h>
#include <NTL/LLL.h>
#include <QVector2D>
#include "functions.hpp"
#include "LatticeFitter.hpp"

BOOST_GEOMETRY_REGISTER_POINT_2D_GET_SET(
    QPoint, int, boost::geometry::cs::cartesian,
    x, y, setX, setY)

BOOST_GEOMETRY_REGISTER_LINESTRING_TEMPLATED(QVector)
BOOST_GEOMETRY_REGISTER_LINESTRING(QPolygon)

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

QPolygon convexHull(const std::vector<cv::Point2f>& points)
{
  std::vector<cv::Point2f> hull;
  cv::convexHull(points, hull);

  QPolygon poly;

  for (const auto& point : hull)
    poly.append(QPoint(point.x, point.y));

  return poly;
}

QVector<QPoint> generateGrid(const Lattice& lattice, const QPolygon& hull)
{
  QPoint origin (
    NTL::to_int(lattice.origin[0]),
    NTL::to_int(lattice.origin[1])
  );

  QPoint baseVectorU {
    NTL::to_int(lattice.bases(1, 1)),
    NTL::to_int(lattice.bases(1, 2))
  };

  QPoint baseVectorV {
    NTL::to_int(lattice.bases(2, 1)),
    NTL::to_int(lattice.bases(2, 2))
  };

  QVector<QPoint> line {
    {0, 500}, {1000, 100}
  };

  QVector<QPoint> intersection_points;

  boost::geometry::intersection(hull, line, intersection_points);

  return intersection_points;
}
