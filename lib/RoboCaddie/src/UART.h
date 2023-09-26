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
  virtual int transmit(const uint8_t *message, int length) = 0;
};

} // namespace RoboCaddieUART

class SpyUART : public RoboCaddieUART::UART {
private:
  std::vector<uint8_t> lastSentMessage;
  uint64_t numberOfExecutions = 0;
  uint64_t baudRate = 0;
  const int MAX_UART_MESSAGE_LENGTH = 258;

public:
  SpyUART() : RoboCaddieUART::UART() {
    lastSentMessage.reserve(MAX_UART_MESSAGE_LENGTH);
    lastSentMessage = {};
  }

  void init() { baudRate = 115200; }

  int transmit(const uint8_t *message, int length) {
    lastSentMessage.assign(message, message + length);
    numberOfExecutions = numberOfExecutions + 1;
    return length;
  }

  std::vector<uint8_t> getLastSentMessage() { return lastSentMessage; }
  uint8_t getNumbersOfExecutions() { return numberOfExecutions; }
  uint64_t getBaudRate() { return baudRate; }
};

#endif