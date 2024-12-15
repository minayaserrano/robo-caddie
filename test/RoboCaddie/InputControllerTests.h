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

#ifdef ARDUINO

class ACGAMR1ControllerFixture : public ::testing::Test {
protected:
  void testCommandInterpretation(const std::vector<uint8_t> &inputData,
                                 Command expectedCommand,
                                 const std::string &testCaseName) {
    EXPECT_EQ(
        ACGAMR1Controller::parseHIDReport(inputData.data(), inputData.size()),
        expectedCommand)
        << "Failed test case: " << static_cast<int>(inputData[0])
        << static_cast<int>(inputData[1]) << testCaseName;
  }
};

TEST_F(ACGAMR1ControllerFixture, InvalidInputHandling) {
  testCommandInterpretation({}, Command::STOP, "Empty input");
  testCommandInterpretation({0x00}, Command::STOP, "Single byte input");
  testCommandInterpretation({0x00, 0x00, 0x00}, Command::STOP,
                            "Oversized input");
  testCommandInterpretation({0x00, 0x00}, Command::STOP,
                            "Joystick up and left at same time, no buttons");
}

TEST_F(ACGAMR1ControllerFixture, ForwardCommand) {
  testCommandInterpretation({0x00, 0x10}, Command::FORWARD, "Just joystick up");
}

TEST_F(ACGAMR1ControllerFixture, BackwardCommand) {
  testCommandInterpretation({0x00, 0x90}, Command::BACKWARD,
                            "Just joystick down");
}

TEST_F(ACGAMR1ControllerFixture, LeftCommand) {
  testCommandInterpretation({0x00, 0x40}, Command::LEFT, "Just joystick left");
}

TEST_F(ACGAMR1ControllerFixture, RightCommand) {
  testCommandInterpretation({0x00, 0x60}, Command::RIGHT,
                            "Just joystick right");
}

TEST_F(ACGAMR1ControllerFixture, StopCommands) {
  testCommandInterpretation({0x00, 0x50}, Command::STOP,
                            "Centered joystick, no buttons");
  testCommandInterpretation({0x01, 0x10}, Command::STOP, "Up but 1 button");
  testCommandInterpretation({0x03, 0x90}, Command::STOP, "Down but 2 button");
  testCommandInterpretation({0x01, 0x40}, Command::STOP, "Left but 1 button");
  testCommandInterpretation({0x07, 0x60}, Command::STOP, "Right but 3 Buttons");
}

TEST_F(ACGAMR1ControllerFixture, BoundaryInputTest) {
  testCommandInterpretation({0xFF, 0xFF}, Command::STOP,
                            "Maximum Input Values");
}

#endif

#endif
