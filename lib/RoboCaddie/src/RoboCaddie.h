#ifndef __ROBOCADDIE_H
#define __ROBOCADDIE_H

class RoboCaddie {
private:
public:
  static const int STOP = 0;

  RoboCaddie();
  ~RoboCaddie();
  const int getStatus();
};

#endif
