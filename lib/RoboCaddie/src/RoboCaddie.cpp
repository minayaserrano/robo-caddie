#include "RoboCaddie.h"

RoboCaddie::RoboCaddie(UART &uart, TimeService &time)
    : uart(uart), time(time) {}

RoboCaddie::~RoboCaddie() {}

const int RoboCaddie::getStatus() { return STOP; }

void RoboCaddie::transmission() {
  if (time.getCurrentTime() - time.getStartTime() >= 30) {
    std::vector<uint8_t> stopMsg = {0x04, 0x01, 0x0A, 0x57, 0x0E, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90};
    uart.transmit(stopMsg.data(), stopMsg.size());
  }
}
