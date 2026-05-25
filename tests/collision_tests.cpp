#include <gtest/gtest.h>

#include "CollisionUtils.h"

TEST(CollisionTest, SamePositionCollides) {
  EXPECT_TRUE(colisionaAABB(0, 0, 32, 32, 0, 0, 32, 32));
}

TEST(CollisionTest, AdjacentRightDoesNotCollide) {
  EXPECT_FALSE(colisionaAABB(0, 0, 32, 32, 32, 0, 32, 32));
}

TEST(CollisionTest, OverlappingByOnePixelCollides) {
  EXPECT_TRUE(colisionaAABB(0, 0, 32, 32, 31, 31, 32, 32));
}

TEST(CollisionTest, FarApartDoesNotCollide) {
  EXPECT_FALSE(colisionaAABB(0, 0, 32, 32, 100, 0, 32, 32));
  EXPECT_FALSE(colisionaAABB(0, 0, 32, 32, 0, 100, 32, 32));
}

TEST(CollisionTest, SmallerInsideLargerCollides) {
  EXPECT_TRUE(colisionaAABB(0, 0, 32, 32, 5, 5, 16, 16));
  EXPECT_TRUE(colisionaAABB(5, 5, 16, 16, 0, 0, 32, 32));
}

TEST(CollisionTest, NegativePositionCollides) {
  EXPECT_TRUE(colisionaAABB(-10, -10, 32, 32, -9, -9, 32, 32));
}

TEST(CollisionTest, NegativePositionDoesNotCollide) {
  EXPECT_FALSE(colisionaAABB(-10, -10, 32, 32, 50, 50, 32, 32));
}
