#ifndef __TIMESERVICE_H
#define __TIMESERVICE_H

#include <stdint.h>

class TimeService {
public:
  TimeService() {}
  ~TimeService() {}
  virtual bool isTick(const uint8_t milliseconds) = 0;
};

#endif