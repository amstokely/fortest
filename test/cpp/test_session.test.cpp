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

    std::string get_output() {
        return buffer.str();
    }

    void clear_output() {
        buffer.str("");
        buffer.clear();
    }
};

TEST_F(TestSessionFixture, AddTestSuite) {
    TestSession<OStreamLogger> test_session(assert_obj);
    auto &suite = test_session.add_test_suite("suite1");
    EXPECT_EQ(suite.get_name(), "suite1");
}

// ---------------------
// Fixture-related tests
// ---------------------

TEST_F(TestSessionFixture, AddSessionFixtureSuccess) {
    TestSession<OStreamLogger> test_session(assert_obj);
    Fixture session_fixture(nullptr, nullptr, nullptr, "session");
    EXPECT_NO_THROW(test_session.add_fixture(session_fixture));
}

TEST_F(TestSessionFixture, AddSessionFixtureWrongScopeThrows) {
    TestSession<OStreamLogger> test_session(assert_obj);
    Fixture bad_fixture(nullptr, nullptr, nullptr, "test");
    EXPECT_THROW(test_session.add_fixture(bad_fixture), std::runtime_error);
}

TEST_F(TestSessionFixture, AddSuiteFixtureSuccess) {
    TestSession<OStreamLogger> test_session(assert_obj);
    auto &suite = test_session.add_test_suite("suite1");
    Fixture test_fixture(nullptr, nullptr, nullptr, "test");

    EXPECT_NO_THROW(test_session.add_fixture("suite1", test_fixture));
}

TEST_F(TestSessionFixture, AddSuiteFixtureWrongScopeThrows) {
    TestSession<OStreamLogger> test_session(assert_obj);
    auto &suite = test_session.add_test_suite("suite1");
    Fixture bad_fixture(nullptr, nullptr, nullptr, "invalid");

    EXPECT_THROW(test_session.add_fixture("suite1", bad_fixture), std::runtime_error);
}

TEST_F(TestSessionFixture, AddSuiteFixtureSuiteDoesNotExistThrows) {
    TestSession<OStreamLogger> test_session(assert_obj);
    Fixture test_fixture(nullptr, nullptr, nullptr, "test");

    EXPECT_THROW(test_session.add_fixture("nosuite", test_fixture), std::runtime_error);
}

// ---------------------
// Test-related tests
// ---------------------

TEST_F(TestSessionFixture, AddTestSuccess) {
    TestSession<OStreamLogger> test_session(assert_obj);
    test_session.add_test_suite("suite1");

    bool called = false;
    test_session.add_test("suite1", "test1", [&](void*, void*, void*) {
        called = true;
    });

    // Run the suite to ensure the test actually executes
    auto &suite = test_session.add_test_suite("suite2"); // dummy to allow iteration
    test_session.run(logger);

    EXPECT_TRUE(called);
    EXPECT_THAT(get_output(), HasSubstr("Running test: test1"));
}

TEST_F(TestSessionFixture, AddTestSuiteDoesNotExistThrows) {
    TestSession test_session(assert_obj);

    EXPECT_THROW(
        test_session.add_test("nosuite", "test1", [](void*, void*, void*) {}),
        std::runtime_error
    );
}
