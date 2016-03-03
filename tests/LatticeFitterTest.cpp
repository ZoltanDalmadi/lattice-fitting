#include <gtest/gtest.h>
#include <opencv2/core.hpp>
#include "LatticeFitter.hpp"

namespace
{

class LatticeFitterTest : public ::testing::Test
{
protected:
  LatticeFitterTest() : latticeFitter(new LatticeFitter) {}
  virtual ~LatticeFitterTest() { delete latticeFitter; }

  LatticeFitter* latticeFitter;
};

TEST_F(LatticeFitterTest, test_fitLattice)
{
  std::vector<cv::Point2f> points;
  points.emplace_back(2, 6);
  points.emplace_back(8, 13);
  points.emplace_back(13, 17);
  points.emplace_back(18, 18);
  points.emplace_back(24, 22);
  points.emplace_back(32, 20);

  latticeFitter->findBestLattice(points);

  Lattice known_best_lattice;
  known_best_lattice.origin.append(NTL::to_ZZ(13));
  known_best_lattice.origin.append(NTL::to_ZZ(17));

  known_best_lattice.bases.SetDims(2, 2);
  known_best_lattice.bases.put(0, 0, NTL::to_ZZ(-1));
  known_best_lattice.bases.put(0, 1, NTL::to_ZZ(-1));
  known_best_lattice.bases.put(1, 0, NTL::to_ZZ(2));
  known_best_lattice.bases.put(1, 1, NTL::to_ZZ(-2));

  known_best_lattice.total_error = 3;

  EXPECT_EQ(latticeFitter->best_lattice, known_best_lattice);
}

}
