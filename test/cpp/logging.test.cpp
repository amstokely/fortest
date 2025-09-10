#include "logging.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>

using ::testing::HasSubstr;

class ConsoleLoggerTest : public ::testing::Test {
protected:
    std::ostringstream buffer;
    ConsoleLogger logger{buffer};  // inject test buffer

    std::string get_output() {
        return buffer.str();
    }

    void clear_output() {
        buffer.str("");
        buffer.clear();
    }
};

TEST_F(ConsoleLoggerTest, LogPass) {
    logger.log("all good", "PASS");
    std::string out = get_output();
    EXPECT_THAT(out, HasSubstr("[PASS] all good"));
    EXPECT_THAT(out, HasSubstr("\033[32m"));  // green

    std::ostringstream oss;
    oss << logger;
    EXPECT_EQ(oss.str(), "[PASS] all good");
}


TEST_F(ConsoleLoggerTest, LogFail) {
    logger.log("something broke", "FAIL");
    std::string out = get_output();
    EXPECT_THAT(out, HasSubstr("[FAIL] something broke"));
    EXPECT_THAT(out, HasSubstr("\033[31m"));  // red
}

TEST_F(ConsoleLoggerTest, LogInfo) {
    logger.log("details", "INFO");
    std::string out = get_output();
    EXPECT_THAT(out, HasSubstr("[INFO] details"));
    EXPECT_THAT(out, HasSubstr("\033[0m"));   // default reset
}

TEST_F(ConsoleLoggerTest, LogTrue) {
    logger.log("condition ok", "TRUE");
    std::string out = get_output();
    EXPECT_THAT(out, HasSubstr("[TRUE] condition ok"));
    EXPECT_THAT(out, HasSubstr("\033[32m"));  // green
}

TEST_F(ConsoleLoggerTest, LogFalse) {
    logger.log("condition bad", "FALSE");
    std::string out = get_output();
    EXPECT_THAT(out, HasSubstr("[FALSE] condition bad"));
    EXPECT_THAT(out, HasSubstr("\033[31m"));  // red
}

TEST_F(ConsoleLoggerTest, LogUnknownTagPrintsRawMessage) {
    logger.log("just text", "OTHER");
    std::string out = get_output();
    EXPECT_EQ(out, "just text\n");
}

TEST_F(ConsoleLoggerTest, BorderIsPrintedAroundMessage) {
    ConsoleLogger border_logger(buffer, "====", Color::DEFAULT);
    border_logger.log("border test", "INFO");
    std::string out = get_output();
    EXPECT_THAT(out, HasSubstr("===="));
    EXPECT_THAT(out, HasSubstr("[INFO] border test"));
}

TEST_F(ConsoleLoggerTest, OperatorStreamOutputsLastMessage) {
    logger.log("stream test", "INFO");
    std::ostringstream oss;
    oss << logger;
    EXPECT_EQ(oss.str(), "[INFO] stream test");
}

TEST_F(ConsoleLoggerTest, OperatorStreamWhenNoMessage) {
    ConsoleLogger empty_logger(buffer);
    std::ostringstream oss;
    oss << empty_logger;
    EXPECT_EQ(oss.str(), "(no log yet)");
}

TEST(ConsoleLoggerColorTest, ColorToCodeReturnsCorrectEscapeCodes) {
    EXPECT_EQ(ConsoleLogger::color_to_code(Color::RED),     "\033[31m");
    EXPECT_EQ(ConsoleLogger::color_to_code(Color::GREEN),   "\033[32m");
    EXPECT_EQ(ConsoleLogger::color_to_code(Color::YELLOW),  "\033[33m");
    EXPECT_EQ(ConsoleLogger::color_to_code(Color::BLUE),    "\033[34m");
    EXPECT_EQ(ConsoleLogger::color_to_code(Color::MAGENTA), "\033[35m");
    EXPECT_EQ(ConsoleLogger::color_to_code(Color::CYAN),    "\033[36m");
    EXPECT_EQ(ConsoleLogger::color_to_code(Color::WHITE),   "\033[37m");
    EXPECT_EQ(ConsoleLogger::color_to_code(Color::DEFAULT), "\033[0m");
}
