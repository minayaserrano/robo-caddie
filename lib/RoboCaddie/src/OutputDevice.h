#ifndef __OUTPUT_DEVICE_H
#define __OUTPUT_DEVICE_H

class OutputDevice {
public:
  OutputDevice() {}
  virtual ~OutputDevice() {}
  virtual void outputTransmission(int16_t power, int16_t steer) = 0;
};

#ifdef ARDUINO

class ArduinoSerialOutputDevice : public OutputDevice {
public:
  ArduinoSerialOutputDevice() : OutputDevice() {}

  void outputTransmission(int16_t power, int16_t steer) override {
    Serial.print(power);
    Serial.print(" - ");
    Serial.println(steer);
  }
};

#endif

#endif
