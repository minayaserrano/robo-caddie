#ifdef UNIT_TEST

TEST(ArduinoTimeService, ArduinoTimeServiceIsTickEvery1000ms) {
  ArduinoTimeService timeservice;

  EXPECT_EQ(timeservice.isTick(1000), false);
  delay(50);
  EXPECT_EQ(timeservice.isTick(1000), false);
  delay(1000);
  EXPECT_EQ(timeservice.isTick(1000), true);
  delay(500);
  EXPECT_EQ(timeservice.isTick(1000), false);
  delay(500);
  EXPECT_EQ(timeservice.isTick(1000), true);
  delay(1000);
  EXPECT_EQ(timeservice.isTick(1000), true);
  EXPECT_EQ(timeservice.isTick(1000), false);
}

#endif
