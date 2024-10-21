#ifdef UNIT_TEST

#include <RoboCaddie.h>
#include <gtest/gtest.h>

#include "RoboCaddie/RoboCaddieTests.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  if (RUN_ALL_TESTS()) {
  };

  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}

#endif
