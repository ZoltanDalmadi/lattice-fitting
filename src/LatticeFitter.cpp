#include "LatticeFitter.hpp"
#include <algorithm>
#include <NTL/LLL.h>

LatticeFitter::LatticeFitter(QObject* parent)
  : QObject(parent) {}

void LatticeFitter::findBestLattice(const std::vector<cv::Point2f>& points)
{
  std::vector<Lattice> bestLattices;

  for (const auto& p : points)
    bestLattices.emplace_back(bestLatticeForOrigin(p, points));

  best_lattice = *std::min_element(bestLattices.cbegin(), bestLattices.cend(),
  [](const Lattice& l1, const Lattice& l2) {
    return l1.total_error < l2.total_error;
  });

  emit foundBestLattice(best_lattice);
}

Lattice LatticeFitter::bestLatticeForOrigin(
    const cv::Point2f& origin, const std::vector<cv::Point2f>& points) const
{
  std::vector<NTL::vec_ZZ> vecs;

  for (const auto& point : points) {
    if (point == origin) continue;

    vecs.push_back(cvPoint2fToNTLVec(point - origin));
  }

  std::vector<Lattice> lattices;

  for (size_t i = 0; i < vecs.size(); ++i) {
    for (size_t j = i + 1; j < vecs.size(); ++j) {
      lattices.emplace_back();
      auto& current_lattice = lattices.back();

      current_lattice.origin = cvPoint2fToNTLVec(origin);
      current_lattice.bases.SetDims(2, 2);
      current_lattice.bases[0] = vecs[i];
      current_lattice.bases[1] = vecs[j];

      NTL::LLL_FP(current_lattice.bases);
      calculateErrorForLattice(current_lattice, vecs);
    }
  }

  return *std::min_element(lattices.cbegin(), lattices.cend(),
  [](const Lattice& l1, const Lattice& l2) {
    return l1.total_error < l2.total_error;
  });
}

void LatticeFitter::calculateErrorForLattice(Lattice& lattice,
    const std::vector<NTL::vec_ZZ>& points) const
{
  lattice.total_error = 0;

  for (const auto& vec : points) {
    NTL::vec_ZZ v;
    NTL::NearVector(v, lattice.bases, vec);
    auto u = v - vec;

    lattice.total_error += std::hypot(NTL::to_double(u[0]), NTL::to_double(u[1]));
  }
}
