# RoboCaddie

[![CI-native](https://github.com/minayaserrano/robo-caddie/actions/workflows/ci-native.yml/badge.svg)](https://github.com/minayaserrano/robo-caddie/actions/workflows/ci-native.yml)
[![CI-Arduino-Nano-33-BLE](https://github.com/minayaserrano/robo-caddie/actions/workflows/ci-arduino-nano-33-ble.yml/badge.svg)](https://github.com/minayaserrano/robo-caddie/actions/workflows/ci-arduino-nano-33-ble.yml)

A hoverboard-powered golf trolley.

## Features

- Remote Mode: Bluetooth BLE remote control.
- Follow Mode: Trolley follows you by uwb sensors.

[TODO list](https://github.com/minayaserrano/robo-caddie/blob/main/TODO.md)

## Acknowledgments

Thank you to all the contributors from the hoverboard hack community. In particular:

- Niklas Fauth and Jan Henrik for starting the community: https://github.com/NiklasFauth/hoverboard-firmware-hack
- phail for his awesome Bipropellant project. This project use bipropellant-protocol through bipropellant-hoveboard-api: https://github.com/bipropellant
- EmanuelFeru for his awesome contribution to the community: https://github.com/EmanuelFeru/hoverboard-firmware-hack-FOC
- All the people involved in telegram group hooover

## Contributing

If you'd like to contribute, please fork the repository and use a feature branch. Pull requests are warmly welcome.

## Licensing

The code in this project is licensed under MIT license.

## Development environment: Pre-commit instalation
```
poetry install --no-root
pre-commit install
```

## Protocol definition
```
const uint8_t PROTOCOL_MSG2_SOM = 0x04;  // PROTOCOL_SOM_NOACK
const uint8_t PROTOCOL_MSG2_CI = 0x01;   // Continuity Counter
const uint8_t PROTOCOL_MSG2_LEN = 0x0A;  // Len of bytes to follow,
                                         // NOT including CS
                                         // (CheckSum)
const uint8_t PROTOCOL_MSG2_CMD = 0x57;  // PROTOCOL_CMD_WRITEVAL ('W')
const uint8_t PROTOCOL_MSG2_CODE = 0x0E; // setPointPWM

// PWM Values on little endian. Examples:
// (0x00 0x00 0x00 0x00 => PWM = 0)
// (0x64 0x00 0x00 0x00 => PWM = 100)
const uint8_t PROTOCOL_MSG2_RIGHT_WHEEL1 = 0x00;
const uint8_t PROTOCOL_MSG2_RIGHT_WHEEL2 = 0x00;
const uint8_t PROTOCOL_MSG2_RIGHT_WHEEL3 = 0x00;
const uint8_t PROTOCOL_MSG2_RIGHT_WHEEL4 = 0x00;
const uint8_t PROTOCOL_MSG2_LEFT_WHEEL1 = 0x00;
const uint8_t PROTOCOL_MSG2_LEFT_WHEEL2 = 0x00;
const uint8_t PROTOCOL_MSG2_LEFT_WHEEL3 = 0x00;
const uint8_t PROTOCOL_MSG2_LEFT_WHEEL4 = 0x00;
const uint8_t PROTOCOL_MSG2_CS = 0x90; // CheckSum (From CI to WHEEL2)

const std::vector<uint8_t> stop_msg = {
    PROTOCOL_MSG2_SOM,          PROTOCOL_MSG2_CI,
    PROTOCOL_MSG2_LEN,          PROTOCOL_MSG2_CMD,
    PROTOCOL_MSG2_CODE,         PROTOCOL_MSG2_RIGHT_WHEEL1,
    PROTOCOL_MSG2_RIGHT_WHEEL2, PROTOCOL_MSG2_RIGHT_WHEEL3,
    PROTOCOL_MSG2_RIGHT_WHEEL4, PROTOCOL_MSG2_LEFT_WHEEL1,
    PROTOCOL_MSG2_LEFT_WHEEL2,  PROTOCOL_MSG2_LEFT_WHEEL3,
    PROTOCOL_MSG2_LEFT_WHEEL4,  PROTOCOL_MSG2_CS};
```

## Basic moves examples
```
power = 0,
steer = 0;
std::vector<uint8_t> stopMsg     = {0x04, 0x01, 0x0A, 0x57, 0x0E, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90};

power = 100;
steer = 0;
std::vector<uint8_t> forwardMsg  = {0x04, 0x01, 0x0A, 0x57, 0x0E, 0x64, 0x00,
                                    0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0xC8};

power = -100;
steer = 0;
std::vector<uint8_t> backwardMsg = {0x04, 0x01, 0x0A, 0x57, 0x0E, 0x9C, 0xFF,
                                    0xFF, 0xFF, 0x9C, 0xFF, 0xFF, 0xFF, 0x5E};

power = 0,
steer = 100;
std::vector<uint8_t> rightMsg    = {0x04, 0x01, 0x0A, 0x57, 0x0E, 0x64, 0x00,
                                    0x00, 0x00, 0x9C, 0xFF, 0xFF, 0xFF, 0x5E};

power = 0,
steer = -100;
std::vector<uint8_t> backwardMsg = {0x04, 0x01, 0x0A, 0x57, 0x0E, 0x9C, 0xFF,
                                    0xFF, 0xFF, 0x64, 0x00, 0x00, 0x00, 0x5E};
```
