#ifndef __ROBOCADDIE_H
#define __ROBOCADDIE_H

#include <UART.h>

class RoboCaddie {
private:
public:
  static const int STOP = 0;

  RoboCaddie();
  ~RoboCaddie();
  const int getStatus();
  void transmission(UART &);
};

#endif
