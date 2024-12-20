#ifdef UNIT_TEST

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "RoboCaddie.h"
#include "RoboCaddie/RoboCaddieTests.h"

#include "RoboCaddie/InputControllerTests.h"

// Fix arduino/ArduinoCore-mbed issue:
// https://github.com/platformio/platformio-core/issues/3980#issuecomment-1500895461
#ifndef RENODE_ENVIRONMENT
REDIRECT_STDOUT_TO(Serial);
#endif

void setup() {
  delay(2000);

  Serial.begin(115200);

  ::testing::InitGoogleTest();

  if (RUN_ALL_TESTS()) {
  };
}

void loop() { delay(1000); }

#endif
