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
    lastSentMessage.assign(message, message + length);
    return length;
  }

  std::vector<uint8_t> getLastSentMessage() { return lastSentMessage; }
};

#endif