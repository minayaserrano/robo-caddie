#ifndef __OUTPUT_DEVICE_H
#define __OUTPUT_DEVICE_H

#include <map>

class OutputDevice {
public:
  OutputDevice() {}
  virtual ~OutputDevice() {}
  virtual void transmission(int16_t power, int16_t steer) = 0;
  virtual void status(Status status) = 0;
};

#ifdef ARDUINO

class ArduinoSerialOutputDevice : public OutputDevice {
public:
  ArduinoSerialOutputDevice() : OutputDevice() {}

  void transmission(int16_t power, int16_t steer) override {
    Serial.print(power);
    Serial.print(" - ");
    Serial.println(steer);
  }

  void status(Status status) override {
    static const std::map<Status, const char *> statusNames = {
        {Status::STOP, "STOP"},
        {Status::FORWARD, "FORWARD"},
        {Status::BACKWARD, "BACKWARD"},
        {Status::RIGHT, "RIGHT"},
        {Status::LEFT, "LEFT"}};

    const char *statusName = "STOP";

    auto it = statusNames.find(status);
    if (it != statusNames.end()) {
      statusName = it->second;
    }

    Serial.println(statusName);
  }
};

#endif

#endif
