#include "RoboCaddie.h"

using namespace RoboCaddieUART;

const int RoboCaddie::STOP;

int UARTWrapperStatic(unsigned char *data, int len) {
  if (g_instance) {
    return g_instance->UARTWrapper(data, len);
  }
  return -1;
}

RoboCaddie::RoboCaddie(RoboCaddieUART::UART &uart, TimeService &time)
    : uart(uart), time(time), hover(UARTWrapperStatic) {
  g_instance = this;
}

RoboCaddie::~RoboCaddie() {
  if (g_instance == this) {
    g_instance = nullptr;
  }
}

int RoboCaddie::getStatus() { return STOP; }

void RoboCaddie::init() { uart.init(); }

void RoboCaddie::transmission() { hover.sendPWM(0, 0, PROTOCOL_SOM_NOACK); }

void RoboCaddie::run() {
  if (time.isTick(TRANSMISSION_TICKER_INTERVAL_IN_MILLISECONDS)) {
    transmission();
  }
}

int RoboCaddie::UARTWrapper(unsigned char *data, int len) {
  std::vector<uint8_t> message(std::move(data), std::move(data + len));
  return (int)uart.transmit(message);
}
