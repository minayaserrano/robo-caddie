#ifndef __INPUTCONTROLLER_H
#define __INPUTCONTROLLER_H

class InputController {
public:
  InputController() {}
  virtual ~InputController() {}
  virtual void init() = 0;
  virtual void connect() = 0;
  virtual bool isConnected() = 0;
  virtual Command readCommand() = 0;
};

class DummyController : public InputController {
public:
  DummyController(TimeService &time) : InputController(), time(time) {}

  void init() {}
  void connect() {}
  bool isConnected() { return true; }

  Command readCommand() {
    if (time.isTick(3000)) {
      if (command == Command::STOP) {
        command = Command::FORWARD;
      } else {
        command = Command::STOP;
      }
    }
    return command;
  }

private:
  Command command = Command::STOP;
  TimeService &time;
};

#ifdef ARDUINO

#include <ArduinoBLE.h>

// TODO: https://github.com/arduino-libraries/ArduinoBLE/issues/182
volatile static Command inputControllerCommand = Command::STOP;

class ACGAMR1Controller : public InputController {
public:
  ACGAMR1Controller() : InputController() {}

  void init() {
    Serial.begin(115200);
    if (!BLE.begin()) {
      Serial.println("BLE init failed!");
      return;
    }
    BLE.setSupervisionTimeout(0x000A);
    BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
    Serial.println("BLE scanning...");
    BLE.scanForName(deviceName);
  }

  Command readCommand() {
    BLE.poll();
    return inputControllerCommand;
  }

  void connect() {
    peripheral = BLE.available();
    if (peripheral && peripheral.localName() == deviceName) {
      BLE.stopScan();
      if (!peripheral.connect()) {
        Serial.println("Peripheral connect failed");
        return;
      }

      if (!peripheral.discoverAttributes()) {
        Serial.println("Attribute discovery failed");
        peripheral.disconnect();
        return;
      }

      if (!peripheral.discoverService(hidServiceId)) {
        Serial.println("0x1812 disc failed");
        peripheral.disconnect();
        return;
      }

      BLEService hidService = peripheral.service(hidServiceId);

      // There are multiple 2A4D characteristics
      // We need to subscribe to all of them
      int iCount = hidService.characteristicCount();
      for (int i = 0; i < iCount; i++) {
        BLECharacteristic bc = hidService.characteristic(i);
        if (strcasecmp(bc.uuid(), hidServiceReportId) == 0) {
          bc.subscribe();
          bc.setEventHandler(BLEWritten, HIDReportWritten);
        }
      }
    }
  }

  bool isConnected() { return peripheral.connected(); }

  static Command parseHIDReport(const uint8_t *reportData, size_t length) {
    if (length != 2) {
      return Command::STOP;
    }

    const uint16_t BUTTON_LEFT = 0x100;
    const uint16_t BUTTON_RIGHT = 0x200;
    const uint16_t BUTTON_UP = 0x400;
    const uint16_t BUTTON_DOWN = 0x800;

    // Extract buttons and axis information
    uint16_t buttonsState = reportData[0];
    uint8_t yAxis = reportData[1] & 0xC0;
    uint8_t xAxis = reportData[1] & 0x30;

    if (yAxis == 0x00) {
      buttonsState |= BUTTON_UP;
    } else if (yAxis == 0x80) {
      buttonsState |= BUTTON_DOWN;
    }

    if (xAxis == 0x00) {
      buttonsState |= BUTTON_LEFT;
    } else if (xAxis == 0x20) {
      buttonsState |= BUTTON_RIGHT;
    }

    Command command = Command::STOP;

    if (buttonsState == BUTTON_UP) {
      command = Command::FORWARD;
    }
    if (buttonsState == BUTTON_DOWN) {
      command = Command::BACKWARD;
    }
    if (buttonsState == BUTTON_LEFT) {
      command = Command::LEFT;
    }
    if (buttonsState == BUTTON_RIGHT) {
      command = Command::RIGHT;
    }

    inputControllerCommand = command;

    return command;
  }

private:
  BLEDevice peripheral;
  const char *deviceName = "ACGAM R1          ";
  const char *hidServiceId = "1812";
  const char *hidServiceReportId = "2A4D";

  static void blePeripheralDisconnectHandler(BLEDevice central) {
    Serial.println("blePeripheralDisconnectHandler");
    Serial.println("BLE scanning...");
    BLE.scanForName("ACGAM R1          ");
  }

  static void HIDReportWritten(BLEDevice device,
                               BLECharacteristic characteristic) {
    uint8_t lenght;
    uint8_t reportData[2];

    lenght = characteristic.readValue(reportData, sizeof(reportData));

    inputControllerCommand = parseHIDReport(reportData, lenght);
  }
};

#endif

#endif
