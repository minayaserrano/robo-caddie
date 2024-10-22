#ifndef __UART_H
#define __UART_H

#include <stdint.h>
#include <vector>

namespace RoboCaddieUART {

class UART {
public:
  UART() {}
  ~UART() {}
  virtual void init(void) = 0;
  virtual int transmit(std::vector<uint8_t> message, int length) = 0;
};

} // namespace RoboCaddieUART

#endif