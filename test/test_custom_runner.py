# Modify GoogletestTestCaseParser to allow Renode log format
# https://github.com/platformio/platformio-core/blob/master/platformio/test/runners/googletest.py

import os
import re

from platformio.public import GoogletestTestRunner
from platformio.test.runners.googletest import GoogletestTestCaseParser


class CustomGoogletestTestCaseParser(GoogletestTestCaseParser):
    # Examples:
    # 12:02:09.8136 [INFO] uart0: [host: 2.12s (+11.59ms)|virt: 1.03s (+0.21ms)] [ RUN      ] FooTest.Bar
    # ...
    # 12:02:09.8136 [INFO] uart0: [host: 2.12s (+11.59ms)|virt: 1.03s (+0.21ms)] [  FAILED  ] FooTest.Bar (0 ms)
    STATUS__NAME_RE = r".*\[\s+(?P<status>[A-Z]+)\s+\]\s+(?P<name>[^\(\s]+)"

    # Examples:
    # 12:02:09.8136 [INFO] uart0: [host: 2.12s (+11.59ms)|virt: 1.03s (+0.21ms)] [ RUN      ] FooTest.Bar
    # 12:02:09.8136 [INFO] uart0: [host: 2.12s (+11.59ms)|virt: 1.03s (+0.21ms)] test/test_gtest/test_main.cpp:26: Failure
    # 12:02:09.8136 [INFO] uart0: [host: 2.12s (+11.59ms)|virt: 1.03s (+0.21ms)] Y:\core\examples\unit-testing\googletest\test\test_gtest\test_main.cpp:26: Failure
    SOURCE_MESSAGE_RE = r".*(?P<source_file>.+):(?P<source_line>\d+):(?P<message>.*)$"

    def _parse_status_and_name(self, line):
        result = (None, None)
        line = line.strip()
        # if not line.startswith("["):
        #     return result
        match = re.search(self.STATUS__NAME_RE, line)
        if not match:
            return result
        return match.group("status"), match.group("name")


class CustomTestRunner(GoogletestTestRunner):
    EXTRA_LIB_DEPS = ["google/googletest@^1.15.2"]

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._tc_parser = CustomGoogletestTestCaseParser()
        os.environ["GTEST_COLOR"] = "no"  # disable ANSI symbols
