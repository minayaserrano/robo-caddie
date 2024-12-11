#ifdef UNIT_TEST

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "RoboCaddie.h"
#include "RoboCaddie/RoboCaddieTests.h"

#include "RoboCaddie/InputControllerTests.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  if (RUN_ALL_TESTS()) {
  };

  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}

#endif
