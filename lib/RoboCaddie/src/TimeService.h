#ifndef __TIMESERVICE_H
#define __TIMESERVICE_H

#include <stdint.h>

class TimeService {
public:
  TimeService() {}
  ~TimeService() {}
  virtual bool isTick(const uint8_t milliseconds) = 0;
};

class FakeTimeService : public TimeService {
private:
  uint64_t currentTime;
  uint64_t startTime;

public:
  FakeTimeService() : TimeService() {}

  void setCurrentTime(uint64_t time) { currentTime = time; }

  void setStartTime(uint64_t time) { startTime = time; }

  bool isTick(const uint8_t milliseconds) {
    return (currentTime - startTime) >= milliseconds;
  }
};

#endif