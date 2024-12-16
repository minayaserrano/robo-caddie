#ifndef __ROBOCADDIE_H
#define __ROBOCADDIE_H

#include "HoverboardAPI.h"

#include "TimeService.h"
#include "UART.h"

#include "Command.h"
#include "Status.h"

#include "InputController.h"
#include "OutputDevice.h"

class RoboCaddie {
public:
  RoboCaddie(RoboCaddieUART::UART &, TimeService &, InputController &,
             OutputDevice &);
  ~RoboCaddie();
  void init();
  void run();

private:
  RoboCaddieUART::UART &uart;
  TimeService &time;
  InputController &inputController;
  OutputDevice &outputDevice;
  HoverboardAPI hover;
  Status status = Status::STOP;
  const uint8_t TRANSMISSION_TICKER_INTERVAL_IN_MILLISECONDS = 30;

  void execute(const Command command);
  void transmission();
  int UARTWrapper(unsigned char *data, int len);
  friend int UARTWrapperStatic(unsigned char *data, int len);
};

static RoboCaddie *g_instance = nullptr;

#endif
