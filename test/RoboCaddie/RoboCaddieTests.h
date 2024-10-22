#ifdef UNIT_TEST

using ::testing::_;

class MockUART : public RoboCaddieUART::UART {
public:
  MOCK_METHOD(void, init, (), (override));
  MOCK_METHOD(int, transmit, (std::vector<uint8_t> message, int length),
              (override));
};

class RoboCaddieFixture : public ::testing::Test {
protected:
  MockUART uart;
  FakeTimeService time;
  RoboCaddie robocaddie;

  RoboCaddieFixture() : robocaddie(uart, time) {}
};

TEST_F(RoboCaddieFixture, RoboCaddieIsStoppedOnStartup) {
  EXPECT_EQ(RoboCaddie::STOP, robocaddie.getStatus());
}

TEST_F(RoboCaddieFixture,
       AStopMessageIsSentToTheMotorWhenRoboCaddieStatusIsSTOP) {
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

  EXPECT_CALL(uart, transmit(stop_msg, stop_msg.size())).Times(1);

  robocaddie.transmission();
}

TEST_F(RoboCaddieFixture,
       RoboCaddieDoesNotSendTransmissionIfIntervalIsShorterThan30Ms) {
  time.setCurrentTime(25);
  time.setStartTime(0);

  EXPECT_CALL(uart, transmit(_, _)).Times(0);

  robocaddie.run();
}

TEST_F(RoboCaddieFixture,
       RoboCaddieSendsATransmissionIfIntervalIsLargerThan30Ms) {
  time.setCurrentTime(30);
  time.setStartTime(0);

  EXPECT_CALL(uart, transmit(_, _)).Times(1);

  robocaddie.run();
}

TEST_F(RoboCaddieFixture, RoboCaddieSendsATransmissionEvery30Ms) {
  EXPECT_CALL(uart, transmit(_, _)).Times(3);

  time.setCurrentTime(0);
  time.setStartTime(0);

  uint8_t time_elapsed = 0;
  while (time_elapsed <= 119) {
    time.setCurrentTime(time_elapsed);
    robocaddie.run();
    time_elapsed += 1;
  }
}

TEST_F(RoboCaddieFixture, UARTBaudRateShouldBe115200) {
  EXPECT_CALL(uart, init()).Times(1);

  robocaddie.init();
}

#endif
