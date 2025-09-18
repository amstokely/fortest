#include "test_session.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>

using ::testing::HasSubstr;
using namespace Fortest;

// A simple logger for testing that writes into an ostringstream
class OStreamLogger : public Logger {
public:
    explicit OStreamLogger(std::ostream &out) : Logger(out) {}
};

class TestSessionFixture : public ::testing::Test {
protected:
    std::ostringstream buffer;
    std::shared_ptr<OStreamLogger> logger;
    Assert<OStreamLogger> assert_obj;  // for TestSuite

    void SetUp() override {
        logger = std::make_shared<OStreamLogger>(buffer);
        assert_obj.reset();
    }

    std::string get_output() { return buffer.str(); }

    void clear_output() {
        buffer.str("");
        buffer.clear();
    }
};

/**
 * @brief Behavior: Adding a test suite stores it and allows retrieval by name.
 */
TEST_F(TestSessionFixture, AddTestSuite) {
    TestSession<OStreamLogger> test_session(assert_obj);
    auto &suite = test_session.add_test_suite("suite1");
    EXPECT_EQ(suite.get_name(), "suite1");
}

// ---------------------
// Fixture-related tests
// ---------------------

/**
 * @brief Behavior: A session-level fixture can be added successfully.
 */
TEST_F(TestSessionFixture, AddSessionFixtureSuccess) {
    TestSession<OStreamLogger> test_session(assert_obj);

    Fixture<void> session_fixture(nullptr, nullptr, nullptr, Scope::Session);
    EXPECT_NO_THROW(test_session.add_fixture(session_fixture));
}

/**
 * @brief Behavior: Adding a non-session fixture at session scope throws.
 */
TEST_F(TestSessionFixture, AddSessionFixtureWrongScopeThrows) {
    TestSession<OStreamLogger> test_session(assert_obj);

    Fixture<void> bad_fixture(nullptr, nullptr, nullptr, Scope::Test);
    EXPECT_THROW(test_session.add_fixture(bad_fixture), std::runtime_error);
}

/**
 * @brief Behavior: Adding a test fixture to a valid suite succeeds.
 */
TEST_F(TestSessionFixture, AddSuiteFixtureSuccess) {
    TestSession<OStreamLogger> test_session(assert_obj);
    test_session.add_test_suite("suite1");

    Fixture<void> test_fixture(nullptr, nullptr, nullptr, Scope::Test);
    EXPECT_NO_THROW(test_session.add_fixture("suite1", test_fixture));
}

/**
 * @brief Behavior: Adding a fixture with wrong scope to a suite throws.
 */
TEST_F(TestSessionFixture, AddSuiteFixtureWrongScopeThrows) {
    TestSession<OStreamLogger> test_session(assert_obj);
    test_session.add_test_suite("suite1");

    // Session fixture is not valid at suite level
    Fixture<void> bad_fixture(nullptr, nullptr, nullptr, Scope::Session);
    EXPECT_THROW(test_session.add_fixture("suite1", bad_fixture), std::runtime_error);
}

/**
 * @brief Behavior: Adding a fixture to a non-existent suite throws.
 */
TEST_F(TestSessionFixture, AddSuiteFixtureSuiteDoesNotExistThrows) {
    TestSession<OStreamLogger> test_session(assert_obj);

    Fixture<void> test_fixture(nullptr, nullptr, nullptr, Scope::Test);
    EXPECT_THROW(test_session.add_fixture("nosuite", test_fixture), std::runtime_error);
}

// ---------------------
// Test-related tests
// ---------------------

/**
 * @brief Behavior: A test can be added to a suite and is executed when run.
 */
TEST_F(TestSessionFixture, AddTestSuccess) {
    TestSession<OStreamLogger> test_session(assert_obj);
    test_session.add_test_suite("suite1");

    bool called = false;
    test_session.add_test("suite1", "test1", [&](void*, void*, void*) {
        called = true;
    });

    // Run all suites
    test_session.run(logger);

    EXPECT_TRUE(called);
    EXPECT_THAT(get_output(), HasSubstr("Running test: test1"));
}

/**
 * @brief Behavior: Adding a test to a non-existent suite throws.
 */
TEST_F(TestSessionFixture, AddTestSuiteDoesNotExistThrows) {
    TestSession<OStreamLogger> test_session(assert_obj);

    EXPECT_THROW(
        test_session.add_test("nosuite", "test1", [](void*, void*, void*) {}),
        std::runtime_error
    );
}
