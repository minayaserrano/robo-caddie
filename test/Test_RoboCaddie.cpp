#ifdef UNIT_TEST

#include <RoboCaddie.h>
#include <unity.h>

void test_robocaddie_is_stopped_on_startup() {
  RoboCaddie robocaddie;

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

  std::vector<uint8_t> stop_msg = {
      PROTOCOL_MSG2_SOM,          PROTOCOL_MSG2_CI,
      PROTOCOL_MSG2_LEN,          PROTOCOL_MSG2_CMD,
      PROTOCOL_MSG2_CODE,         PROTOCOL_MSG2_RIGHT_WHEEL1,
      PROTOCOL_MSG2_RIGHT_WHEEL2, PROTOCOL_MSG2_RIGHT_WHEEL3,
      PROTOCOL_MSG2_RIGHT_WHEEL4, PROTOCOL_MSG2_LEFT_WHEEL1,
      PROTOCOL_MSG2_LEFT_WHEEL2,  PROTOCOL_MSG2_LEFT_WHEEL3,
      PROTOCOL_MSG2_LEFT_WHEEL4,  PROTOCOL_MSG2_CS};

  RoboCaddie robocaddie;
  SpyUART uart;

  // Precondition: no message sent yet on initialization
  TEST_ASSERT_NULL(uart.getLastSentMessage().data());

  robocaddie.transmission(uart);

  TEST_ASSERT_EQUAL_UINT8_ARRAY(
      stop_msg.data(), uart.getLastSentMessage().data(), stop_msg.size());
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_robocaddie_is_stopped_on_startup);
  RUN_TEST(
      test_a_STOP_message_is_sent_to_the_motor_when_robocaddie_status_is_STOP);

  UNITY_END();
}

#endif
