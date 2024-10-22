#include "RoboCaddie.h"

using namespace RoboCaddieUART;

const int RoboCaddie::STOP;

RoboCaddie::RoboCaddie(UART &uart, TimeService &time)
    : uart(uart), time(time) {}

RoboCaddie::~RoboCaddie() {}

int RoboCaddie::getStatus() { return STOP; }

void RoboCaddie::init() { uart.init(); }

void RoboCaddie::transmission() {
  std::vector<uint8_t> stopMsg = {0x04, 0x01, 0x0A, 0x57, 0x0E, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90};
  uart.transmit(stopMsg, stopMsg.size());
}

void RoboCaddie::run() {
  if (time.isTick(TRANSMISSION_TICKER_INTERVAL_IN_MILLISECONDS)) {
    transmission();
  }
}
