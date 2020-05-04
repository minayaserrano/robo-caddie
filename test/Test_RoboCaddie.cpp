#ifdef UNIT_TEST

#include <unity.h>

void test_environment() { TEST_ASSERT_TRUE(true); }

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_environment);

  UNITY_END();
}

#endif
