## TODO list

- ~~RoboCaddie is stopped at startup~~
  - ~~RoboCaddie sends a STOP message to the motor when it is stopped~~
  - ~~RoboCaddie transmits a message to the motor every 30ms~~
  - ~~Arduino Nano 33 BLE support~~
  - ~~Go to metal:~~
    - ~~Main loop~~
    - ~~Arduino UART implementation~~
      - ~~UART baud rate initialization~~
    - ~~Arduino TimeService implementation~~
    - ~~Consecutive messages should increase CI (Continuity counter) and decrease CS (CheckSum) values~~
- ~~RoboCaddie goes forward when it receives a forward command~~
- ~~RoboCaddie goes backward when it receives a backward command~~
- ~~RoboCaddie goes left when it receives a left command~~
- ~~RoboCaddie goes right when it receives a right command~~
- ~~RoboCaddie goes {direction} when it receives a {direction} command~~
- ~~RoboCaddie stops when it receives a stop command~~
- ~~Disallow invalid commands and statuses~~
- ~~RoboCaddie stops on disconnect~~
- RoboCaddie goes forward left if it is going forward and it receives a left command
- RoboCaddie goes forward left if it is going left and it receives a forward command
- ~~InputController interface (ACGAMR1Controller/...)~~
- Output feedback interface (Serial/Display...) to show RoboCaddie feedback:
  - Transmission: power and steer
  - RoboCaddie status
  - InputController connection status
  - Command received
- Output feedback: show hoverboard feedback (speed and battery voltage)
- Output feedback: show InputController feedback (battery voltage)
- Motor gears support
- Smooth aceleration (incremental PWM)
- Speed mode
- Follow me mode

## TECH debt

- Avoid volatile static Command inputControllerCommand: https://github.com/arduino-libraries/ArduinoBLE/issues/182 
- Refactor: Extract transmission to MotorDriver interface
- ACGAMR1 controller codes documentation
