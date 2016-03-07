#pragma once
#include <QObject>
#include <opencv2/opencv.hpp>
#include <NTL/mat_ZZ.h>

struct Lattice {
  NTL::vec_ZZ origin;
  NTL::mat_ZZ bases;
  double total_error;
};

inline bool operator==(const Lattice& lhs, const Lattice& rhs)
{
  return lhs.origin == rhs.origin &&
         lhs.bases == rhs.bases &&
         lhs.total_error == rhs.total_error;
}

inline bool operator!=(const Lattice& lhs, const Lattice& rhs)
{
  return !(lhs == rhs);
}

class LatticeFitter : public QObject
{
  Q_OBJECT

public:
  explicit LatticeFitter(QObject* parent = 0);

  Lattice best_lattice;

signals:
  void foundBestLattice(const Lattice&);

public slots:
  void findBestLattice(const std::vector<cv::Point2f>& points);

private:
  Lattice bestLatticeForOrigin
  (const cv::Point2f& origin, const std::vector<cv::Point2f>& points) const;

  void calculateErrorForLattice
  (Lattice& lattice, const std::vector<NTL::vec_ZZ>& points) const;

  NTL::vec_ZZ cvPoint2fToNTLVec(const cv::Point2f& point) const;
  double sq(double value) const;
};

inline NTL::vec_ZZ LatticeFitter::cvPoint2fToNTLVec(const cv::Point2f& point) const
{
  NTL::vec_ZZ result;
  result.append(NTL::to_ZZ(point.x));
  result.append(NTL::to_ZZ(point.y));
  return result;
}

inline double LatticeFitter::sq(double value) const
{
  return value * value;
}
