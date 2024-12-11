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

#endif