#ifndef __TIMESERVICE_H
#define __TIMESERVICE_H

#include <stdint.h>

class TimeService {
public:
  TimeService() {}
  virtual ~TimeService() {}
  virtual bool isTick(const uint16_t milliseconds) = 0;
};

#ifdef ARDUINO

#include <Arduino.h>

class ArduinoTimeService : public TimeService {
private:
  unsigned long lastTickTime;

public:
  ArduinoTimeService() : lastTickTime(millis()) {}

  bool isTick(const uint16_t milliseconds) override {
    unsigned long currentTime = millis();
    if (currentTime - lastTickTime >= milliseconds) {
      lastTickTime = currentTime;
      return true;
    }
    return false;
  }
};

#endif

#endif