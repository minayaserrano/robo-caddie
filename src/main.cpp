#ifdef ARDUINO

#include <Arduino.h>

#include "RoboCaddie.h"

ArduinoSerial1UART uart;
ArduinoTimeService timeservice;
ArduinoTimeService timeserviceInputController;
#ifdef DUMMYCONTROLLER
DummyController inputController(timeserviceInputController);
#else
ACGAMR1Controller inputController;
#endif
RoboCaddie robocaddie(uart, timeservice, inputController);

void setup() {
  delay(2000);
  robocaddie.init();
  delay(1000);
}

void loop() { robocaddie.run(); }

#endif

#ifndef ARDUINO
int main() { return 0; }
#endif