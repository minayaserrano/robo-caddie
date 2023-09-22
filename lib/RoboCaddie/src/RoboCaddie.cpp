#include "RoboCaddie.h"

RoboCaddie::RoboCaddie() {}

RoboCaddie::~RoboCaddie() {}

const int RoboCaddie::getStatus() { return STOP; }

void RoboCaddie::transmission(UART &uart) { uart.transmit({}, 0); }
