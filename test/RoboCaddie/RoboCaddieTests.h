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
  EXPECT_EQ(RoboCaddie::Status::STOP, robocaddie.getStatus());
}

struct RoboCaddieStatusFixtureData {
  const RoboCaddie::Command command;
  const RoboCaddie::Status status;
};

class RoboCaddieStatusFixture
    : public RoboCaddieFixture,
      public testing::WithParamInterface<RoboCaddieStatusFixtureData> {};

INSTANTIATE_TEST_SUITE_P(
    RoboCaddieStatus, RoboCaddieStatusFixture,
    testing::Values(RoboCaddieStatusFixtureData{RoboCaddie::Command::STOP,
                                                RoboCaddie::Status::STOP},
                    RoboCaddieStatusFixtureData{RoboCaddie::Command::FORWARD,
                                                RoboCaddie::Status::FORWARD},
                    RoboCaddieStatusFixtureData{RoboCaddie::Command::BACKWARD,
                                                RoboCaddie::Status::BACKWARD},
                    RoboCaddieStatusFixtureData{RoboCaddie::Command::RIGHT,
                                                RoboCaddie::Status::RIGHT},
                    RoboCaddieStatusFixtureData{RoboCaddie::Command::LEFT,
                                                RoboCaddie::Status::LEFT},
                    // Invalid command
                    RoboCaddieStatusFixtureData{
                        static_cast<RoboCaddie::Command>(127),
                        RoboCaddie::Status::STOP}));

TEST_P(RoboCaddieStatusFixture, RoboCaddieStatusIsXOnCommandX) {
  robocaddie.setStatus(GetParam().command);

  EXPECT_EQ(GetParam().status, robocaddie.getStatus());
}

struct RoboCaddieMessageFixtureData {
  const RoboCaddie::Command command;
  std::vector<uint8_t> expectedMessage;
};

class RoboCaddieMessageFixture
    : public RoboCaddieFixture,
      public testing::WithParamInterface<RoboCaddieMessageFixtureData> {};

INSTANTIATE_TEST_SUITE_P(
    RoboCaddieMessage, RoboCaddieMessageFixture,
    testing::Values(RoboCaddieMessageFixtureData{RoboCaddie::Command::STOP,
                                                 {0x04, 0x01, 0x0A, 0x57, 0x0E,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x90}},
                    RoboCaddieMessageFixtureData{RoboCaddie::Command::FORWARD,
                                                 {0x04, 0x01, 0x0A, 0x57, 0x0E,
                                                  0x64, 0x00, 0x00, 0x00, 0x64,
                                                  0x00, 0x00, 0x00, 0xC8}},
                    RoboCaddieMessageFixtureData{RoboCaddie::Command::BACKWARD,
                                                 {0x04, 0x01, 0x0A, 0x57, 0x0E,
                                                  0x9C, 0xFF, 0xFF, 0xFF, 0x9C,
                                                  0xFF, 0xFF, 0xFF, 0x5E}},
                    RoboCaddieMessageFixtureData{RoboCaddie::Command::RIGHT,
                                                 {0x04, 0x01, 0x0A, 0x57, 0x0E,
                                                  0x64, 0x00, 0x00, 0x00, 0x9C,
                                                  0xFF, 0xFF, 0xFF, 0x93}},
                    RoboCaddieMessageFixtureData{RoboCaddie::Command::LEFT,
                                                 {0x04, 0x01, 0x0A, 0x57, 0x0E,
                                                  0x9C, 0xFF, 0xFF, 0xFF, 0x64,
                                                  0x00, 0x00, 0x00, 0x93}},
                    // Invalid command
                    RoboCaddieMessageFixtureData{
                        static_cast<RoboCaddie::Command>(126),
                        {0x04, 0x01, 0x0A, 0x57, 0x0E, 0x00, 0x00, 0x00, 0x00,
                         0x00, 0x00, 0x00, 0x00, 0x90}}));

TEST_P(RoboCaddieMessageFixture,
       AMessageIsSentToTheMotorWhenRoboCaddieStatusIsX) {
  EXPECT_CALL(uart, transmit(GetParam().expectedMessage)).Times(1);

  robocaddie.setStatus(GetParam().command);
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
