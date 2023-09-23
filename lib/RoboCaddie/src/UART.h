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
  std::vector<uint8_t> lastSentMessage;
  uint64_t numberOfExecutions = 0;
  const int MAX_UART_MESSAGE_LENGTH = 258;

public:
  SpyUART() : UART() {
    lastSentMessage.reserve(MAX_UART_MESSAGE_LENGTH);
    lastSentMessage = {};
  }

  const int transmit(const uint8_t *message, int length) {
    lastSentMessage.assign(message, message + length);
    numberOfExecutions = numberOfExecutions + 1;
    return length;
  }

  std::vector<uint8_t> getLastSentMessage() { return lastSentMessage; }
  uint8_t getNumbersOfExecutions() { return numberOfExecutions; }
};

#endif