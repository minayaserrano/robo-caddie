#ifdef ARDUINO

#include <Arduino.h>

#include "RoboCaddie.h"

ArduinoSerial1UART uart;
ArduinoTimeService timeservice;
#ifdef DUMMYCONTROLLER
ArduinoTimeService timeserviceInputController;
DummyController inputController(timeserviceInputController);
#else
ACGAMR1Controller inputController;
#endif
ArduinoSerialOutputDevice outputDevice;
RoboCaddie robocaddie(uart, timeservice, inputController, outputDevice);

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