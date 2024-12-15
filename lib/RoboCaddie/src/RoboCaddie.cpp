#include <map>

#include "RoboCaddie.h"

int UARTWrapperStatic(unsigned char *data, int len) {
  if (g_instance) {
    return g_instance->UARTWrapper(data, len);
  }
  return -1;
}

RoboCaddie::RoboCaddie(RoboCaddieUART::UART &uart, TimeService &time,
                       InputController &inputController)
    : uart(uart), time(time), inputController(inputController),
      hover(UARTWrapperStatic) {
  g_instance = this;
}

RoboCaddie::~RoboCaddie() {
  if (g_instance == this) {
    g_instance = nullptr;
  }
}

void RoboCaddie::execute(const Command command) {
  static const std::map<Command, Status> statusValues = {
      {Command::STOP, Status::STOP},
      {Command::FORWARD, Status::FORWARD},
      {Command::BACKWARD, Status::BACKWARD},
      {Command::RIGHT, Status::RIGHT},
      {Command::LEFT, Status::LEFT}};

  auto iterator = statusValues.find(command);

  if (iterator != statusValues.end()) {
    status = iterator->second;
  } else {
    status = Status::STOP;
  }
}

void RoboCaddie::init() {
  uart.init();
  inputController.init();
}

void RoboCaddie::transmission() {
  // PWMValues: {status, {power, steer}}
  static const std::map<Status, std::pair<int16_t, int16_t>> pwmValues = {
      {Status::STOP, {0, 0}},
      {Status::FORWARD, {100, 0}},
      {Status::BACKWARD, {-100, 0}},
      {Status::RIGHT, {0, 100}},
      {Status::LEFT, {0, -100}}};

  auto iterator = pwmValues.find(status);
  if (iterator != pwmValues.end()) {
    hover.sendPWM(iterator->second.first, iterator->second.second,
                  PROTOCOL_SOM_NOACK);
  } else {
    hover.sendPWM(0, 0, PROTOCOL_SOM_NOACK);
  }
}

void RoboCaddie::run() {
  inputController.connect();
  while (inputController.isConnected()) {
    Command command = inputController.readCommand();
    execute(command);
    if (time.isTick(TRANSMISSION_TICKER_INTERVAL_IN_MILLISECONDS)) {
      transmission();
    }
  }
}

int RoboCaddie::UARTWrapper(unsigned char *data, int len) {
  std::vector<uint8_t> message(std::move(data), std::move(data + len));
  return (int)uart.transmit(message);
}
