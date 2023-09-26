#ifdef UNIT_TEST

#include <Arduino.h>
#include <RoboCaddie.h>
#include <unity.h>

#include "RoboCaddie/RoboCaddieTests.h"

// Fix arduino/ArduinoCore-mbed issue:
// https://github.com/platformio/platformio-core/issues/3980#issuecomment-1500895461
#ifndef RENODE_ENVIRONMENT
REDIRECT_STDOUT_TO(Serial);
#endif

void setup() {
  delay(2000);

  run_tests();
}

void loop() { delay(1000); }

#endif
