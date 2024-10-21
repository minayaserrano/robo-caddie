#ifdef UNIT_TEST

TEST(RoboCaddieTest, RoboCaddieIsStoppedOnStartup) {
  SpyUART uart;
  FakeTimeService time;
  RoboCaddie robocaddie(uart, time);

  EXPECT_EQ(RoboCaddie::STOP, robocaddie.getStatus());
}

TEST(RoboCaddieTest, AStopMessageIsSentToTheMotorWhenRoboCaddieStatusIsSTOP) {
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

  SpyUART uart;
  FakeTimeService time;
  RoboCaddie robocaddie(uart, time);

  // Precondition: no message sent yet on initialization
  EXPECT_EQ(uart.getLastSentMessage().data(), nullptr);

  robocaddie.transmission();

  const auto &lastMessage = uart.getLastSentMessage();
  EXPECT_THAT(std::vector<uint8_t>(lastMessage.data(),
                                   lastMessage.data() + lastMessage.size()),
              testing::ElementsAreArray(stop_msg));
}

TEST(RoboCaddieTest, RoboCaddieSendsATransmissionEvery30Ms) {
  SpyUART uart;
  FakeTimeService time;
  RoboCaddie robocaddie(uart, time);

  time.setCurrentTime(25);
  time.setStartTime(0);

  robocaddie.run();

  EXPECT_EQ(0, uart.getNumbersOfExecutions());

  time.setCurrentTime(30);
  time.setStartTime(0);

  robocaddie.run();

  EXPECT_EQ(1, uart.getNumbersOfExecutions());

  time.setCurrentTime(45);
  time.setStartTime(31);

  robocaddie.run();

  EXPECT_EQ(1, uart.getNumbersOfExecutions());

  time.setCurrentTime(62);
  time.setStartTime(31);

  robocaddie.run();

  EXPECT_EQ(2, uart.getNumbersOfExecutions());
}

TEST(RoboCaddieTest, UARTBaudRateShouldBe115200) {
  SpyUART uart;
  FakeTimeService time;
  RoboCaddie robocaddie(uart, time);

  EXPECT_EQ(0, uart.getBaudRate());

  robocaddie.init();

  EXPECT_EQ(115200, uart.getBaudRate());
}

#endif
