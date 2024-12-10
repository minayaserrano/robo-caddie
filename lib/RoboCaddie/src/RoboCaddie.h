#ifndef __ROBOCADDIE_H
#define __ROBOCADDIE_H

#include "HoverboardAPI.h"

#include "TimeService.h"
#include "UART.h"

class RoboCaddie {
public:
  enum class Status {
    STOP = 0,
    FORWARD = 1,
    BACKWARD = 2,
    RIGHT = 3,
    LEFT = 4
  };
  enum class Command {
    STOP = 0,
    FORWARD = 1,
    BACKWARD = 2,
    RIGHT = 3,
    LEFT = 4
  };

  RoboCaddie(RoboCaddieUART::UART &, TimeService &);
  ~RoboCaddie();
  void setStatus(const Command command);
  void init();
  void run();

private:
  RoboCaddieUART::UART &uart;
  TimeService &time;
  HoverboardAPI hover;
  Status status = Status::STOP;
  const uint8_t TRANSMISSION_TICKER_INTERVAL_IN_MILLISECONDS = 30;

  void transmission();
  int UARTWrapper(unsigned char *data, int len);
  friend int UARTWrapperStatic(unsigned char *data, int len);
};

static RoboCaddie *g_instance = nullptr;

#endif
