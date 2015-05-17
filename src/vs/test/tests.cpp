#include <iostream>

#include <gtest/gtest.h>

#include "../glm/glm.hpp"

#include "voronoi_tests.cpp"
#include "priqueue_tests.cpp"

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
