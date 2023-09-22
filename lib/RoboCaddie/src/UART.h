#ifndef __UART_H
#define __UART_H

#include <stdint.h>
#include <vector>

class UART {
public:
  UART() {}
  ~UART() {}
  virtual const int transmit(const uint8_t *message, int length) = 0;
};

class SpyUART : public UART {
private:
  std::vector<uint8_t> lastSentMessage = {};

public:
  SpyUART() : UART() {}

  const int transmit(const uint8_t *message, int length) {
    lastSentMessage = {0x04, 0x01, 0x0A, 0x57, 0x0E, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90};
    return 14;
  }

  std::vector<uint8_t> getLastSentMessage() { return lastSentMessage; }
};

#endif