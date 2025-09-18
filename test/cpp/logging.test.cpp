#include "logging.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>

using ::testing::HasSubstr;
using ::testing::Not;

class ConsoleLoggerTest : public ::testing::Test {
protected:
    std::ostringstream buffer;
    Fortest::Logger logger{buffer}; // inject test buffer

    std::string get_output() {
        return buffer.str();
    }

    void clear_output() {
        buffer.str("");
        buffer.clear();
    }
};

// -----------------------------------------------------------------------------
// Tag behavior
// -----------------------------------------------------------------------------
TEST_F(ConsoleLoggerTest, LogPass) {
    logger.log("all good", "PASS");
    std::string out = get_output();
    EXPECT_THAT(out, HasSubstr("[PASS] all good"));
    EXPECT_THAT(out, HasSubstr("\033[32m")); // green present

    std::ostringstream oss;
    oss << logger;
    EXPECT_EQ(oss.str(), "[PASS] all good");
}

TEST_F(ConsoleLoggerTest, LogFail) {
    logger.log("something broke", "FAIL");
    std::string out = get_output();
    EXPECT_THAT(out, HasSubstr("[FAIL] something broke"));
    EXPECT_THAT(out, HasSubstr("\033[31m")); // red present
}

TEST_F(ConsoleLoggerTest, LogInfo) {
    logger.log("details", "INFO");
    std::string out = get_output();
    EXPECT_THAT(out, HasSubstr("[INFO] details"));
    EXPECT_THAT(out, HasSubstr("\033[0m")); // reset present
}

TEST_F(ConsoleLoggerTest, LogTrue) {
    logger.log("condition ok", "TRUE");
    std::string out = get_output();
    EXPECT_THAT(out, HasSubstr("[TRUE] condition ok"));
    EXPECT_THAT(out, HasSubstr("\033[32m")); // green present
}

TEST_F(ConsoleLoggerTest, LogFalse) {
    logger.log("condition bad", "FALSE");
    std::string out = get_output();
    EXPECT_THAT(out, HasSubstr("[FALSE] condition bad"));
    EXPECT_THAT(out, HasSubstr("\033[31m")); // red present
}

TEST_F(ConsoleLoggerTest, LogUnknownTagPrintsRawMessage) {
    logger.log("just text", "OTHER");
    EXPECT_EQ(get_output(), "just text\n");
}

TEST_F(ConsoleLoggerTest, LogEmptyTagPrintsRawMessage) {
    logger.log("raw", "");
    EXPECT_EQ(get_output(), "raw\n");
}

TEST_F(ConsoleLoggerTest, LowercaseTagDoesNotFormat) {
    logger.log("case check", "pass");
    EXPECT_EQ(get_output(), "case check\n");
}

// -----------------------------------------------------------------------------
// Border behavior
// -----------------------------------------------------------------------------
TEST_F(ConsoleLoggerTest, BorderIsPrintedAroundMessage) {
    Fortest::Logger border_logger(
        buffer, "====", Fortest::Logger::Color::DEFAULT
    );
    border_logger.log("border test", "INFO");
    std::string out = get_output();
    EXPECT_THAT(out, HasSubstr("===="));
    EXPECT_THAT(out, HasSubstr("[INFO] border test"));
}

TEST_F(ConsoleLoggerTest, BorderAppliedForEveryMessage) {
    Fortest::Logger border_logger(buffer, "****");
    border_logger.log("first", "PASS");
    border_logger.log("second", "FAIL");

    std::string out = get_output();

    // Borders appear
    EXPECT_THAT(out, HasSubstr("****"));
    // Messages are present
    EXPECT_THAT(out, HasSubstr("[PASS] first"));
    EXPECT_THAT(out, HasSubstr("[FAIL] second"));
    // Colors are present
    EXPECT_THAT(out, HasSubstr("\033[32m")); // green
    EXPECT_THAT(out, HasSubstr("\033[31m")); // red
}

TEST_F(ConsoleLoggerTest, EmptyBorderDoesNotPrintBorder) {
    Fortest::Logger no_border_logger(buffer, "");
    no_border_logger.log("no border", "INFO");

    std::string out = get_output();
    EXPECT_THAT(out, Not(HasSubstr("====")));
    EXPECT_THAT(out, HasSubstr("[INFO] no border"));
}

// -----------------------------------------------------------------------------
// Operator<< behavior
// -----------------------------------------------------------------------------
TEST_F(ConsoleLoggerTest, OperatorStreamOutputsLastMessage) {
    logger.log("stream test", "INFO");
    std::ostringstream oss;
    oss << logger;
    EXPECT_EQ(oss.str(), "[INFO] stream test");
}

TEST_F(ConsoleLoggerTest, OperatorStreamWhenNoMessage) {
    Fortest::Logger empty_logger(buffer);
    std::ostringstream oss;
    oss << empty_logger;
    EXPECT_EQ(oss.str(), "(no log yet)");
}

TEST_F(ConsoleLoggerTest, OperatorStreamShowsLastMessageOnly) {
    logger.log("first", "INFO");
    logger.log("second", "FAIL");

    std::ostringstream oss;
    oss << logger;
    EXPECT_EQ(oss.str(), "[FAIL] second");
}

// -----------------------------------------------------------------------------
// Output integrity
// -----------------------------------------------------------------------------
TEST_F(ConsoleLoggerTest, EachMessageEndsWithReset) {
    logger.log("colored", "PASS");
    std::string out = get_output();
    EXPECT_THAT(out, HasSubstr("\033[0m")); // reset code exists
    EXPECT_TRUE(out.ends_with("\n"));
}

TEST_F(ConsoleLoggerTest, MultipleLogsAccumulateOutput) {
    logger.log("first", "INFO");
    logger.log("second", "PASS");

    std::string out = get_output();
    EXPECT_THAT(out, HasSubstr("[INFO] first"));
    EXPECT_THAT(out, HasSubstr("[PASS] second"));
}

TEST(ConsoleLoggerIsolationTest, InstancesKeepIndependentState) {
    std::ostringstream buf1, buf2;
    Fortest::Logger logger1(buf1);
    Fortest::Logger logger2(buf2);

    logger1.log("one", "PASS");
    logger2.log("two", "FAIL");

    std::ostringstream oss1, oss2;
    oss1 << logger1;
    oss2 << logger2;

    EXPECT_EQ(oss1.str(), "[PASS] one");
    EXPECT_EQ(oss2.str(), "[FAIL] two");
}

// -----------------------------------------------------------------------------
// Color utility
// -----------------------------------------------------------------------------
TEST(ConsoleLoggerColorTest, ColorToCodeReturnsCorrectEscapeCodes) {
    EXPECT_EQ(
        Fortest::Logger::color_to_code(Fortest::Logger::Color::RED),
        "\033[31m"
    );
    EXPECT_EQ(
        Fortest::Logger::color_to_code(Fortest::Logger::Color::GREEN),
        "\033[32m"
    );
    EXPECT_EQ(
        Fortest::Logger::color_to_code(Fortest::Logger::Color::YELLOW),
        "\033[33m"
    );
    EXPECT_EQ(
        Fortest::Logger::color_to_code(Fortest::Logger::Color::BLUE),
        "\033[34m"
    );
    EXPECT_EQ(
        Fortest::Logger::color_to_code(Fortest::Logger::Color::MAGENTA),
        "\033[35m"
    );
    EXPECT_EQ(
        Fortest::Logger::color_to_code(Fortest::Logger::Color::CYAN),
        "\033[36m"
    );
    EXPECT_EQ(
        Fortest::Logger::color_to_code(Fortest::Logger::Color::WHITE),
        "\033[37m"
    );
    EXPECT_EQ(
        Fortest::Logger::color_to_code(Fortest::Logger::Color::DEFAULT),
        "\033[0m"
    );
}
