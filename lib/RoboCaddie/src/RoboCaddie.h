#ifndef __ROBOCADDIE_H
#define __ROBOCADDIE_H

#include <UART.h>

class RoboCaddie {
private:
  UART &uart;

public:
  static const int STOP = 0;

  RoboCaddie(UART &);
  ~RoboCaddie();
  const int getStatus();
  void transmission();
};

#endif
