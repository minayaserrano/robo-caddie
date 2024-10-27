#include "RoboCaddie.h"

const uint8_t RoboCaddie::STOP;
const uint8_t RoboCaddie::FORWARD;
const uint8_t RoboCaddie::BACKWARD;

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

uint8_t RoboCaddie::getStatus() { return status; }

void RoboCaddie::setStatus(const uint8_t command) { status = command; }

void RoboCaddie::init() { uart.init(); }

void RoboCaddie::transmission() {
  if (status == FORWARD) {
    hover.sendPWM(100, 0, PROTOCOL_SOM_NOACK);
  } else {
    hover.sendPWM(0, 0, PROTOCOL_SOM_NOACK);
  }
}

void RoboCaddie::run() {
  if (time.isTick(TRANSMISSION_TICKER_INTERVAL_IN_MILLISECONDS)) {
    transmission();
  }
}

int RoboCaddie::UARTWrapper(unsigned char *data, int len) {
  std::vector<uint8_t> message(std::move(data), std::move(data + len));
  return (int)uart.transmit(message);
}
