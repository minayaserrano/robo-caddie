#ifndef __ROBOCADDIE_H
#define __ROBOCADDIE_H

#include <TimeService.h>
#include <UART.h>

class RoboCaddie {
private:
  UART &uart;
  TimeService &time;
  const uint8_t TRANSMISSION_TICKER_INTERVAL_IN_MILLISECONDS = 30;

public:
  static const int STOP = 0;

  RoboCaddie(UART &, TimeService &);
  ~RoboCaddie();
  const int getStatus();
  void transmission();
  void run();
};

#endif
