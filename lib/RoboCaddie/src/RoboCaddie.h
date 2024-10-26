#ifndef __ROBOCADDIE_H
#define __ROBOCADDIE_H

#include <HoverboardAPI.h>

#include "TimeService.h"
#include "UART.h"

class RoboCaddie {
private:
  RoboCaddieUART::UART &uart;
  TimeService &time;
  HoverboardAPI hover;
  const uint8_t TRANSMISSION_TICKER_INTERVAL_IN_MILLISECONDS = 30;

  int UARTWrapper(unsigned char *data, int len);
  friend int UARTWrapperStatic(unsigned char *data, int len);

public:
  static const int STOP = 0;

  RoboCaddie(RoboCaddieUART::UART &, TimeService &);
  ~RoboCaddie();
  int getStatus();
  void init();
  void transmission();
  void run();
};

static RoboCaddie *g_instance = nullptr;

#endif
