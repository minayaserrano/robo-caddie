#ifndef __UART_H
#define __UART_H

#include <stdint.h>
#include <vector>

namespace RoboCaddieUART {

class UART {
public:
  UART() {}
  virtual ~UART() {}
  virtual void init(void) = 0;
  virtual int transmit(std::vector<uint8_t> message, int length) = 0;
};

} // namespace RoboCaddieUART

#ifdef ARDUINO

#include <Arduino.h>

class ArduinoSerial1UART : public RoboCaddieUART::UART {
public:
  ArduinoSerial1UART() : RoboCaddieUART::UART() {}

  void init(void) { Serial1.begin(115200); }

  int transmit(std::vector<uint8_t> message, int length) {
    return Serial1.write(message.data(), message.size());
  }
};

#endif

#endif