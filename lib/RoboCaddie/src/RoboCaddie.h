#ifndef __ROBOCADDIE_H
#define __ROBOCADDIE_H

#include "HoverboardAPI.h"

#include "TimeService.h"
#include "UART.h"

class RoboCaddie {
private:
  RoboCaddieUART::UART &uart;
  TimeService &time;
  HoverboardAPI hover;
  uint8_t status = STOP;
  const uint8_t TRANSMISSION_TICKER_INTERVAL_IN_MILLISECONDS = 30;

  int UARTWrapper(unsigned char *data, int len);
  friend int UARTWrapperStatic(unsigned char *data, int len);

public:
  static const uint8_t STOP = 0;
  static const uint8_t FORWARD = 1;
  static const uint8_t BACKWARD = 2;

  RoboCaddie(RoboCaddieUART::UART &, TimeService &);
  ~RoboCaddie();
  uint8_t getStatus();
  void setStatus(const uint8_t command);
  void init();
  void transmission();
  void run();
};

static RoboCaddie *g_instance = nullptr;

#endif
