#ifndef __TIMESERVICE_H
#define __TIMESERVICE_H

#include <stdint.h>

class TimeService {
public:
  TimeService() {}
  ~TimeService() {}
  virtual uint64_t getCurrentTime() = 0;
  virtual uint64_t getStartTime() = 0;
};

class FakeTimeService : public TimeService {
private:
  uint64_t currentTime;
  uint64_t startTime;

public:
  FakeTimeService() : TimeService() {}
  void setCurrentTime(uint64_t time) { currentTime = time; }
  void setStartTime(uint64_t time) { startTime = time; }
  uint64_t getCurrentTime() { return currentTime; }
  uint64_t getStartTime() { return startTime; }
};

#endif