#ifdef ARDUINO

#include <Arduino.h>

#include "RoboCaddie.h"

ArduinoSerial1UART uart;
ArduinoTimeService timeservice;
ArduinoTimeService arduinoTimeService;
RoboCaddie robocaddie(uart, timeservice);

void setup() {
  delay(2000);
  robocaddie.init();
  delay(1000);
}

void loop() {
  if (arduinoTimeService.isTick(3000)) {
    if (robocaddie.getStatus() == 0) {
      robocaddie.setStatus(RoboCaddie::FORWARD);
    } else {
      robocaddie.setStatus(RoboCaddie::STOP);
    }
  }
  robocaddie.run();
}
#endif

#ifndef ARDUINO
int main() { return 0; }
#endif