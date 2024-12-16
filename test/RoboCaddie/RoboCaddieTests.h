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

class MockInputController : public InputController {
public:
  MOCK_METHOD(void, init, (), (override));
  MOCK_METHOD(void, connect, (), (override));
  MOCK_METHOD(bool, isConnected, (), (override));
  MOCK_METHOD(Command, readCommand, (), (override));
};

class MockOutputDevice : public OutputDevice {
public:
  MOCK_METHOD(void, transmission, (int16_t power, int16_t steer), (override));
  MOCK_METHOD(void, status, (Status status), (override));
};

class RoboCaddieFixture : public ::testing::Test {
protected:
  MockUART uart;
  MockTimeService time;
  MockInputController inputController;
  MockOutputDevice outputDevice;
  RoboCaddie robocaddie;

  RoboCaddieFixture() : robocaddie(uart, time, inputController, outputDevice) {}
};

TEST_F(RoboCaddieFixture, RoboCaddieIsStoppedOnStartup) {
  std::vector<uint8_t> stopMsg1 = {0x04, 0x01, 0x0A, 0x57, 0x0E, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90};
  EXPECT_CALL(uart, transmit(stopMsg1)).Times(1);
  EXPECT_CALL(time, isTick(_)).WillOnce(Return(true));
  EXPECT_CALL(inputController, connect()).Times(1);
  EXPECT_CALL(inputController, isConnected())
      .WillOnce(Return(true))
      .WillOnce(Return(false));
  EXPECT_CALL(inputController, readCommand()).WillOnce(Return(Command::STOP));
  EXPECT_CALL(outputDevice, status(Status::STOP)).Times(1);
  EXPECT_CALL(outputDevice, transmission(0, 0)).Times(1);

  robocaddie.run();
}

struct RoboCaddieStatusFixtureData {
  const Command command;
  const Status status;
};

struct RoboCaddieMessageFixtureData {
  const Command command;
  std::vector<uint8_t> expectedMessage;
  Status expectedStatus;
  int16_t expectedPower;
  int16_t expectedSteer;
};

class RoboCaddieMessageFixture
    : public RoboCaddieFixture,
      public testing::WithParamInterface<RoboCaddieMessageFixtureData> {};

INSTANTIATE_TEST_SUITE_P(
    RoboCaddieMessage, RoboCaddieMessageFixture,
    testing::Values(
        RoboCaddieMessageFixtureData{Command::STOP,
                                     {0x04, 0x01, 0x0A, 0x57, 0x0E, 0x00, 0x00,
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90},
                                     Status::STOP,
                                     0,
                                     0},
        RoboCaddieMessageFixtureData{Command::FORWARD,
                                     {0x04, 0x01, 0x0A, 0x57, 0x0E, 0x0A, 0x00,
                                      0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x7C},
                                     Status::FORWARD,
                                     10,
                                     0},
        RoboCaddieMessageFixtureData{Command::BACKWARD,
                                     {0x04, 0x01, 0x0A, 0x57, 0x0E, 0xF6, 0xFF,
                                      0xFF, 0xFF, 0xF6, 0xFF, 0xFF, 0xFF, 0xAA},
                                     Status::BACKWARD,
                                     -10,
                                     0},
        RoboCaddieMessageFixtureData{Command::RIGHT,
                                     {0x04, 0x01, 0x0A, 0x57, 0x0E, 0x0A, 0x00,
                                      0x00, 0x00, 0xF6, 0xFF, 0xFF, 0xFF, 0x93},
                                     Status::RIGHT,
                                     0,
                                     10},
        RoboCaddieMessageFixtureData{Command::LEFT,
                                     {0x04, 0x01, 0x0A, 0x57, 0x0E, 0xF6, 0xFF,
                                      0xFF, 0xFF, 0x0A, 0x00, 0x00, 0x00, 0x93},
                                     Status::LEFT,
                                     0,
                                     -10},
        // Invalid command
        RoboCaddieMessageFixtureData{static_cast<Command>(126),
                                     {0x04, 0x01, 0x0A, 0x57, 0x0E, 0x00, 0x00,
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90},
                                     Status::STOP,
                                     0,
                                     0}));

TEST_P(RoboCaddieMessageFixture,
       AMessageIsSentToTheMotorWhenRoboCaddieStatusIsX) {
  EXPECT_CALL(uart, transmit(GetParam().expectedMessage)).Times(1);
  EXPECT_CALL(time, isTick(_)).WillOnce(Return(true));
  EXPECT_CALL(inputController, connect()).Times(1);
  EXPECT_CALL(inputController, isConnected())
      .WillOnce(Return(true))
      .WillOnce(Return(false));
  EXPECT_CALL(inputController, readCommand())
      .WillOnce(Return(GetParam().command));
  EXPECT_CALL(outputDevice, status(GetParam().expectedStatus)).Times(1);
  EXPECT_CALL(outputDevice,
              transmission(GetParam().expectedPower, GetParam().expectedSteer))
      .Times(1);

  robocaddie.run();
}

TEST_F(RoboCaddieFixture,
       RoboCaddieDoesNotSendTransmissionIfIntervalIsShorterThan30Ms) {
  EXPECT_CALL(time, isTick(30)).WillOnce(Return(false));
  EXPECT_CALL(uart, transmit(_)).Times(0);
  EXPECT_CALL(inputController, connect()).Times(1);
  EXPECT_CALL(inputController, isConnected())
      .WillOnce(Return(true))
      .WillOnce(Return(false));
  EXPECT_CALL(inputController, readCommand()).WillOnce(Return(Command::STOP));
  EXPECT_CALL(outputDevice, status(_)).Times(0);
  EXPECT_CALL(outputDevice, transmission(_, _)).Times(0);

  robocaddie.run();
}

TEST_F(RoboCaddieFixture,
       RoboCaddieSendsATransmissionIfIntervalIsLargerThan30Ms) {
  EXPECT_CALL(time, isTick(30)).WillOnce(Return(true));
  EXPECT_CALL(uart, transmit(_)).Times(1);
  EXPECT_CALL(inputController, connect()).Times(1);
  EXPECT_CALL(inputController, isConnected())
      .WillOnce(Return(true))
      .WillOnce(Return(false));
  EXPECT_CALL(inputController, readCommand()).WillOnce(Return(Command::STOP));
  EXPECT_CALL(outputDevice, status(_)).Times(1);
  EXPECT_CALL(outputDevice, transmission(_, _)).Times(1);

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
  EXPECT_CALL(outputDevice, status(_)).Times(3);
  EXPECT_CALL(outputDevice, transmission(_, _)).Times(3);
  EXPECT_CALL(inputController, connect()).Times(10);
  EXPECT_CALL(inputController, isConnected())
      .WillOnce(Return(true))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .WillOnce(Return(false));
  EXPECT_CALL(inputController, readCommand())
      .WillRepeatedly(Return(Command::STOP));

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
  EXPECT_CALL(time, isTick(_))
      .WillOnce(Return(true))
      .WillOnce(Return(true))
      .WillOnce(Return(true));
  EXPECT_CALL(inputController, connect()).Times(3);
  EXPECT_CALL(outputDevice, status(_)).Times(3);
  EXPECT_CALL(outputDevice, transmission(_, _)).Times(3);
  EXPECT_CALL(inputController, isConnected())
      .WillOnce(Return(true))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .WillOnce(Return(false));
  EXPECT_CALL(inputController, readCommand())
      .WillRepeatedly(Return(Command::STOP));

  robocaddie.run();
  robocaddie.run();
  robocaddie.run();
}

#endif
