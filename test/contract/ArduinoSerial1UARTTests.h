#ifdef UNIT_TEST

TEST(UART, ArduinoSerial1UARTSendsInformation) {
  ArduinoSerial1UART uart;

  std::vector<uint8_t> message = {0x01, 0x02, 0x03};

  uart.init();

  EXPECT_EQ(uart.transmit(message), 3);
}

#endif
