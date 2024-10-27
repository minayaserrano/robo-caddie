#ifdef UNIT_TEST

using ::testing::_;
using ::testing::Return;

class MockUART : public RoboCaddieUART::UART {
public:
  MOCK_METHOD(void, init, (), (override));
  MOCK_METHOD(int, transmit, (std::vector<uint8_t> message), (override));
};

class MockTimeService : public TimeService {
public:
  MOCK_METHOD(bool, isTick, (const uint16_t milliseconds), (override));
};

class RoboCaddieFixture : public ::testing::Test {
protected:
  MockUART uart;
  MockTimeService time;
  RoboCaddie robocaddie;

  RoboCaddieFixture() : robocaddie(uart, time) {}
};

TEST_F(RoboCaddieFixture, RoboCaddieIsStoppedOnStartup) {
  EXPECT_EQ(RoboCaddie::STOP, robocaddie.getStatus());
}

TEST_F(RoboCaddieFixture, RoboCaddieStatusIsFORWARDOnCommandFORWARD) {
  robocaddie.setStatus(RoboCaddie::FORWARD);

  EXPECT_EQ(RoboCaddie::FORWARD, robocaddie.getStatus());
}

TEST_F(RoboCaddieFixture, RoboCaddieStatusIsBACKWARDOnCommandBACKWARD) {
  robocaddie.setStatus(RoboCaddie::BACKWARD);

  EXPECT_EQ(RoboCaddie::BACKWARD, robocaddie.getStatus());
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

  EXPECT_CALL(uart, transmit(stop_msg)).Times(1);

  robocaddie.transmission();
}

TEST_F(RoboCaddieFixture,
       AForwardMessageIsSentToTheMotorWhenRoboCaddieStatusIsFORWARD) {

  std::vector<uint8_t> forwardMsg = {0x04, 0x01, 0x0A, 0x57, 0x0E, 0x64, 0x00,
                                     0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0xC8};

  EXPECT_CALL(uart, transmit(forwardMsg)).Times(1);

  robocaddie.setStatus(RoboCaddie::FORWARD);
  robocaddie.transmission();
}

TEST_F(RoboCaddieFixture,
       RoboCaddieDoesNotSendTransmissionIfIntervalIsShorterThan30Ms) {
  EXPECT_CALL(time, isTick(30)).WillOnce(Return(false));

  EXPECT_CALL(uart, transmit(_)).Times(0);

  robocaddie.run();
}

TEST_F(RoboCaddieFixture,
       RoboCaddieSendsATransmissionIfIntervalIsLargerThan30Ms) {
  EXPECT_CALL(time, isTick(30)).WillOnce(Return(true));

  EXPECT_CALL(uart, transmit(_)).Times(1);

  robocaddie.run();
}

TEST_F(RoboCaddieFixture, RoboCaddieSendsATransmissionEveryTick) {
  EXPECT_CALL(time, isTick(30))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .WillOnce(Return(false))
      .WillOnce(Return(false))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .WillOnce(Return(false))
      .WillOnce(Return(false));

  EXPECT_CALL(uart, transmit(_)).Times(3);

  for (uint8_t counter = 0; counter < 10; ++counter) {
    robocaddie.run();
  }
}

TEST_F(RoboCaddieFixture, UARTInitializedOnRoboCaddieInitialization) {
  EXPECT_CALL(uart, init()).Times(1);

  robocaddie.init();
}

TEST_F(RoboCaddieFixture, ConsecutiveMessagesIncreaseCIAndDecreaseCSvalues) {
  std::vector<uint8_t> stopMsg1 = {0x04, 0x01, 0x0A, 0x57, 0x0E, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90};

  std::vector<uint8_t> stopMsg2 = {0x04, 0x02, 0x0A, 0x57, 0x0E, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8F};

  std::vector<uint8_t> stopMsg3 = {0x04, 0x03, 0x0A, 0x57, 0x0E, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8E};

  EXPECT_CALL(uart, transmit(stopMsg1)).Times(1);
  EXPECT_CALL(uart, transmit(stopMsg2)).Times(1);
  EXPECT_CALL(uart, transmit(stopMsg3)).Times(1);

  robocaddie.transmission();
  robocaddie.transmission();
  robocaddie.transmission();
}

#endif
