#ifdef UNIT_TEST

#include <RoboCaddie.h>
#include <unity.h>

void test_robocaddie_is_stopped_on_startup() {
  RoboCaddie robocaddie;

  TEST_ASSERT_EQUAL(RoboCaddie::STOP, robocaddie.getStatus());
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_robocaddie_is_stopped_on_startup);

  UNITY_END();
}

#endif
