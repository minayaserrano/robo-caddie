#ifndef __TIMESERVICE_H
#define __TIMESERVICE_H

#include <stdint.h>

class TimeService {
public:
  TimeService() {}
  ~TimeService() {}
};

class FakeTimeService : public TimeService {
private:
public:
  FakeTimeService() : TimeService() {}
  void setCurrentTime(uint64_t) {}
  void setStartTime(uint64_t) {}
};

#endif