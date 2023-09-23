#include "RoboCaddie.h"

RoboCaddie::RoboCaddie(UART &uart, TimeService &time)
    : uart(uart), time(time) {}

RoboCaddie::~RoboCaddie() {}

const int RoboCaddie::getStatus() { return STOP; }

void RoboCaddie::transmission() {
  std::vector<uint8_t> stopMsg = {0x04, 0x01, 0x0A, 0x57, 0x0E, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90};
  uart.transmit(stopMsg.data(), stopMsg.size());
}

void RoboCaddie::run() {
  if (time.isTick(30)) {
    transmission();
  }
}
