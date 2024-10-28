#include <map>

#include "RoboCaddie.h"

const uint8_t RoboCaddie::STOP;
const uint8_t RoboCaddie::FORWARD;
const uint8_t RoboCaddie::BACKWARD;
const uint8_t RoboCaddie::RIGHT;

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
  // PWMValues: {status, {power, steer}}
  static const std::map<uint8_t, std::pair<uint16_t, uint16_t>> pwmValues = {
      {STOP, {0, 0}},
      {FORWARD, {100, 0}},
      {BACKWARD, {-100, 0}},
      {RIGHT, {0, 100}}};
  int16_t power = 0;
  int16_t steer = 0;

  auto iterator = pwmValues.find(status);
  if (iterator != pwmValues.end()) {
    power = iterator->second.first;
    steer = iterator->second.second;
  }

  hover.sendPWM(power, steer, PROTOCOL_SOM_NOACK);
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
