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

namespace
{

template <typename Geometry, typename Point>
void translate(Geometry& g, const Point& d)
{
  boost::geometry::for_each_point(g, [&d](Point& p) {
    boost::geometry::add_point(p, d);
  });
};

template <typename Point>
Point extend_vector(Point origin, Point base_vector, const int scalar)
{
  // just extend the origin in the base vector's direction
  boost::geometry::multiply_value(base_vector, scalar * 5);
  boost::geometry::add_point(origin, base_vector);
  return origin;
}

template <typename Point, typename Line>
void translate_and_intersect(
    const Point& base_vector,
    Line extended_line,
    const Line& image,
    Line& output)
{
  // do the first intersection
  Line intersection_points;
  boost::geometry::intersection(extended_line, image, intersection_points);

  // it has to be 2 intersection points to be a valid line
  while (intersection_points.size() > 1) {
    // output.insert(
    //     output.end(),
    //     intersection_points.begin(),
    //     intersection_points.end());
    output.append(intersection_points);

    // translate "cutting" line with base vector
    translate(extended_line, base_vector);

    // and repeat until no valid lines can be created
    boost::geometry::clear(intersection_points);
    boost::geometry::intersection(extended_line, image, intersection_points);
  }
}

template <typename Point, typename Ring>
QVector<Point> grid_base_image_intersections(
    const Point& origin,
    Point base_vector_u,
    Point base_vector_v,
    const Ring& image,
    const int edge)
{
  using boost::geometry::multiply_value;

  QVector<Point> line;

  // STEP 1: ------------------------------------------------------------------
  // Obtain a long line laying on our base vector.
  line.push_back(extend_vector(origin, base_vector_u, edge));

  // Flip base vector direction
  multiply_value(base_vector_u, -1);
  line.push_back(extend_vector(origin, base_vector_u, edge));

  // STEP 2: ------------------------------------------------------------------
  // translate the line in both directions, and calculate intersections
  // with the image.
  QVector<Point> result;
  translate_and_intersect(base_vector_v, line, image, result);

  // flip base vector direction
  multiply_value(base_vector_v, -1);

  // pre-translate, as the two intersection points of the base vector's line
  // have already been found
  translate(line, base_vector_v);
  translate_and_intersect(base_vector_v, line, image, result);

  return result;
}

} // end unnamed namespace

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

QVector<QPoint> generateGrid(
    const Lattice& lattice,
    int width, int height)
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

  QVector<QPoint> image_rectangle {
    { 0, 0 },
    { 0, height },
    { width, height },
    { width, 0 },
    { 0, 0 },
  };

  // get intersection points with first base vector
  auto lines_u = grid_base_image_intersections(
      origin, baseVectorU, baseVectorV, image_rectangle, width);

  // and just swap the base vectors for the other set of points
  auto lines_v = grid_base_image_intersections(
      origin, baseVectorV, baseVectorU, image_rectangle, height);

  QVector<QPoint> intersection_points;
  intersection_points.resize(lines_u.size() + lines_v.size());
  intersection_points.append(lines_u);
  intersection_points.append(lines_v);

  return intersection_points;
}
