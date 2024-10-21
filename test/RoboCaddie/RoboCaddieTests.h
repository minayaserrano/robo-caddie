#ifdef UNIT_TEST

TEST(DummyTest, ShouldPass) { EXPECT_EQ(1, 1); }

TEST(DummyFailedTest, ShouldNotPass) { EXPECT_EQ(1, 0); }

TEST(SkipTest, DoesSkip) {
  GTEST_SKIP() << "Skipping single test";
  EXPECT_EQ(0, 1); // Won't fail; it won't be executed
}

/*
void test_robocaddie_is_stopped_on_startup();
void test_a_STOP_message_is_sent_to_the_motor_when_robocaddie_status_is_STOP();
void test_robocaddie_sends_a_transmission_every_30_ms();
void test_uart_baud_rate_should_be_115200();

int run_tests() {
  UNITY_BEGIN();

  RUN_TEST(test_robocaddie_is_stopped_on_startup);
  RUN_TEST(
      test_a_STOP_message_is_sent_to_the_motor_when_robocaddie_status_is_STOP);
  RUN_TEST(test_robocaddie_sends_a_transmission_every_30_ms);
  RUN_TEST(test_uart_baud_rate_should_be_115200);

  return UNITY_END();
}

void test_robocaddie_is_stopped_on_startup() {
  SpyUART uart;
  FakeTimeService time;
  RoboCaddie robocaddie(uart, time);

  TEST_ASSERT_EQUAL(RoboCaddie::STOP, robocaddie.getStatus());
}

void test_a_STOP_message_is_sent_to_the_motor_when_robocaddie_status_is_STOP() {
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
  TEST_ASSERT_NULL(uart.getLastSentMessage().data());

  robocaddie.transmission();

  TEST_ASSERT_EQUAL_UINT8_ARRAY(
      stop_msg.data(), uart.getLastSentMessage().data(), stop_msg.size());
}

void test_robocaddie_sends_a_transmission_every_30_ms() {
  SpyUART uart;
  FakeTimeService time;
  RoboCaddie robocaddie(uart, time);

  time.setCurrentTime(25);
  time.setStartTime(0);

  robocaddie.run();

  TEST_ASSERT_EQUAL_UINT64(0, uart.getNumbersOfExecutions());

  time.setCurrentTime(30);
  time.setStartTime(0);

  robocaddie.run();

  TEST_ASSERT_EQUAL_UINT64(1, uart.getNumbersOfExecutions());

  time.setCurrentTime(45);
  time.setStartTime(31);

  robocaddie.run();

  TEST_ASSERT_EQUAL_UINT64(1, uart.getNumbersOfExecutions());

  time.setCurrentTime(62);
  time.setStartTime(31);

  robocaddie.run();

  TEST_ASSERT_EQUAL_UINT64(2, uart.getNumbersOfExecutions());
}

void test_uart_baud_rate_should_be_115200() {
  SpyUART uart;
  FakeTimeService time;
  RoboCaddie robocaddie(uart, time);

  TEST_ASSERT_EQUAL_UINT64(0, uart.getBaudRate());

  robocaddie.init();

  TEST_ASSERT_EQUAL_UINT64(115200, uart.getBaudRate());
}
*/

#endif
