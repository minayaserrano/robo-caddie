#ifdef ARDUINO

#include <Arduino.h>

#include "RoboCaddie.h"

ArduinoSerial1UART uart;
ArduinoTimeService timeservice;
ArduinoTimeService arduinoTimeService;
RoboCaddie robocaddie(uart, timeservice);

RoboCaddie::Command command = RoboCaddie::Command::STOP;

void setup() {
  delay(2000);
  robocaddie.init();
  delay(1000);
}

void loop() {
  if (arduinoTimeService.isTick(3000)) {
    if (command == RoboCaddie::Command::STOP) {
      command = RoboCaddie::Command::FORWARD;
    } else {
      command = RoboCaddie::Command::STOP;
    }
  }
  robocaddie.run(command);
}
#endif

#ifndef ARDUINO
int main() { return 0; }
#endif