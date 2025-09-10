// test_assert.cpp
#include "assert.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

// Mock logger that satisfies LoggerLike
class MockLogger {
public:
    MOCK_METHOD(void, log, (const std::string &message, const std::string &type), ());
};

// Define an Assert type using MockLogger
using TestAssert = Assert<MockLogger>;

class AssertTest : public ::testing::Test {
protected:
    std::shared_ptr<MockLogger> mock_logger;
    TestAssert test_assert;

    void SetUp() override {
        mock_logger = std::make_shared<MockLogger>();
        test_assert.reset();
    }
};

TEST_F(AssertTest, AssertEqual_Pass_Int) {
    EXPECT_CALL(*mock_logger, log("expected == actual (5)", "TRUE")).Times(1);

    test_assert.assert_equal(5, 5, mock_logger);

    EXPECT_EQ(test_assert.get_num_passed(), 1);
    EXPECT_EQ(test_assert.get_num_failed(), 0);
}

TEST_F(AssertTest, AssertEqual_Fail_Int) {
    EXPECT_CALL(*mock_logger, log("expected: 5, actual: 6", "FALSE")).Times(1);

    test_assert.assert_equal(5, 6, mock_logger);

    EXPECT_EQ(test_assert.get_num_passed(), 0);
    EXPECT_EQ(test_assert.get_num_failed(), 1);
}

TEST_F(AssertTest, AssertEqual_Pass_String) {
    EXPECT_CALL(*mock_logger, log("expected == actual (hello)", "TRUE")).Times(1);

    test_assert.assert_equal(std::string("hello"), std::string("hello"), mock_logger);

    EXPECT_EQ(test_assert.get_num_passed(), 1);
    EXPECT_EQ(test_assert.get_num_failed(), 0);
}

TEST_F(AssertTest, AssertEqual_Fail_String) {
    EXPECT_CALL(*mock_logger, log("expected: hello, actual: world", "FALSE")).Times(1);

    test_assert.assert_equal(std::string("hello"), std::string("world"), mock_logger);

    EXPECT_EQ(test_assert.get_num_passed(), 0);
    EXPECT_EQ(test_assert.get_num_failed(), 1);
}

TEST_F(AssertTest, AssertNotEqual_Pass_Int) {
    EXPECT_CALL(*mock_logger, log("expected != actual (5)", "TRUE")).Times(1);

    test_assert.assert_not_equal(5, 6, mock_logger);

    EXPECT_EQ(test_assert.get_num_passed(), 1);
    EXPECT_EQ(test_assert.get_num_failed(), 0);
}

TEST_F(AssertTest, AssertNotEqual_Fail_Int) {
    EXPECT_CALL(*mock_logger, log("expected: 5, actual: 5", "FALSE")).Times(1);

    test_assert.assert_not_equal(5, 5, mock_logger);

    EXPECT_EQ(test_assert.get_num_passed(), 0);
    EXPECT_EQ(test_assert.get_num_failed(), 1);
}

TEST_F(AssertTest, AssertNotEqual_Pass_String) {
    EXPECT_CALL(*mock_logger, log("expected != actual (hello)", "TRUE")).Times(1);

    test_assert.assert_not_equal(std::string("hello"), std::string("world"), mock_logger);

    EXPECT_EQ(test_assert.get_num_passed(), 1);
    EXPECT_EQ(test_assert.get_num_failed(), 0);
}

TEST_F(AssertTest, AssertNotEqual_Fail_String) {
    EXPECT_CALL(*mock_logger, log("expected: hello, actual: hello", "FALSE")).Times(1);

    test_assert.assert_not_equal(std::string("hello"), std::string("hello"), mock_logger);

    EXPECT_EQ(test_assert.get_num_passed(), 0);
    EXPECT_EQ(test_assert.get_num_failed(), 1);
}

TEST_F(AssertTest, AssertTrue_Pass) {
    EXPECT_CALL(*mock_logger, log("condition is TRUE", "TRUE")).Times(1);

    test_assert.assert_true(true, mock_logger);

    EXPECT_EQ(test_assert.get_num_passed(), 1);
    EXPECT_EQ(test_assert.get_num_failed(), 0);
}

TEST_F(AssertTest, AssertTrue_Fail) {
    EXPECT_CALL(*mock_logger, log("condition is FALSE", "FALSE")).Times(1);

    test_assert.assert_true(false, mock_logger);

    EXPECT_EQ(test_assert.get_num_passed(), 0);
    EXPECT_EQ(test_assert.get_num_failed(), 1);
}

TEST_F(AssertTest, AssertFalse_Pass) {
    EXPECT_CALL(*mock_logger, log("condition is FALSE", "TRUE")).Times(1);

    test_assert.assert_false(false, mock_logger);

    EXPECT_EQ(test_assert.get_num_passed(), 1);
    EXPECT_EQ(test_assert.get_num_failed(), 0);
}

TEST_F(AssertTest, AssertFalse_Fail) {
    EXPECT_CALL(*mock_logger, log("condition is TRUE", "FALSE")).Times(1);

    test_assert.assert_false(true, mock_logger);

    EXPECT_EQ(test_assert.get_num_passed(), 0);
    EXPECT_EQ(test_assert.get_num_failed(), 1);
}

TEST_F(AssertTest, ResetCounters) {
    EXPECT_CALL(*mock_logger, log(testing::_, testing::_)).Times(2);

    test_assert.assert_equal(1, 1, mock_logger);
    test_assert.assert_equal(1, 2, mock_logger);

    EXPECT_EQ(test_assert.get_num_passed(), 1);
    EXPECT_EQ(test_assert.get_num_failed(), 1);

    test_assert.reset();

    EXPECT_EQ(test_assert.get_num_passed(), 0);
    EXPECT_EQ(test_assert.get_num_failed(), 0);
}
