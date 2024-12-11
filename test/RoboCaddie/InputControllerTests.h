#ifdef UNIT_TEST

using ::testing::Return;

class MockDummyControllerTimeService : public TimeService {
public:
  MOCK_METHOD(bool, isTick, (const uint16_t milliseconds), (override));
};

class DummyControllerFixture : public ::testing::Test {
protected:
  MockDummyControllerTimeService time;
  DummyController inputController;

  DummyControllerFixture() : inputController(time) {}
};

TEST_F(DummyControllerFixture, InitialStateIsStopped) {
  EXPECT_EQ(inputController.readCommand(), Command::STOP);
}

TEST_F(DummyControllerFixture, CommandAlternatesBetweenStopAndForward) {
  EXPECT_CALL(time, isTick(3000)).WillOnce(Return(true));
  EXPECT_EQ(inputController.readCommand(), Command::FORWARD);

  EXPECT_CALL(time, isTick(3000)).WillOnce(Return(true));
  EXPECT_EQ(inputController.readCommand(), Command::STOP);
}

TEST_F(DummyControllerFixture, DoesNotChangeCommandWithoutTick) {
  EXPECT_CALL(time, isTick(3000)).WillRepeatedly(Return(false));

  Command firstCommand = inputController.readCommand();
  Command secondCommand = inputController.readCommand();

  EXPECT_EQ(firstCommand, secondCommand);
}

#endif
