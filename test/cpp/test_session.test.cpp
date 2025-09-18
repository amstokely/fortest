#include "test_suite.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>

using ::testing::HasSubstr;

// A simple logger for testing that writes into an ostringstream
class OStreamLogger : public Fortest::Logger {
public:
    explicit OStreamLogger(std::ostream &out) : Logger(out) {}
};

class TestSessionFixture : public ::testing::Test {
protected:
    std::ostringstream buffer;
    std::shared_ptr<OStreamLogger> logger;
    Fortest::Assert<OStreamLogger> assert_obj; // for TestSuite

    void SetUp() override {
        logger = std::make_shared<OStreamLogger>(buffer);
        assert_obj.reset();
    }

    std::string get_output() const { return buffer.str(); }

    void clear_output() {
        buffer.str("");
        buffer.clear();
    }
};

/**
 * @brief Behavior: A fixture should invoke setup and teardown callbacks.
 */
TEST_F(TestSessionFixture, FixtureSetupAndTeardownAreCalled) {
    bool setup_called = false;
    bool teardown_called = false;

    Fortest::Fixture<void> f(
        [&](void *) { setup_called = true; },
        [&](void *) { teardown_called = true; },
        nullptr,
        Fortest::Scope::Test
    );

    f.setup();
    f.teardown();

    EXPECT_TRUE(setup_called);
    EXPECT_TRUE(teardown_called);
}

/**
 * @brief Behavior: A test suite reports its name when queried.
 */
TEST_F(TestSessionFixture, AddAndGetName) {
    Fortest::TestSuite ts("MySuite", assert_obj);
    EXPECT_EQ(ts.get_name(), "MySuite");
}

/**
 * @brief Behavior: A test that performs no assertions passes.
 */
TEST_F(TestSessionFixture, AddAndRunSingleTestPasses) {
    Fortest::TestSuite ts("SimpleSuite", assert_obj);

    ts.add_test("empty_test", [](void *, void *, void *) {});

    ts.run(logger);

    std::string out = get_output();
    EXPECT_THAT(out, HasSubstr("Running test: empty_test"));
    EXPECT_THAT(out, HasSubstr("Test passed: empty_test"));
    EXPECT_EQ(assert_obj.get_num_failed(), 0);
}

/**
 * @brief Behavior: A suite reports statuses for both passing and failing tests.
 */
TEST_F(TestSessionFixture, GetStatuses) {
    Fortest::TestSuite ts("SimpleSuite", assert_obj);

    ts.add_test("passing_test", [&](void *, void *, void *) {
        assert_obj.assert_true(true, logger);
    });
    ts.add_test("failing_test", [&](void *, void *, void *) {
        assert_obj.assert_true(false, logger);
    });

    ts.run(logger);
    auto statuses = ts.get_statuses();

    EXPECT_EQ(statuses.size(), 2);
    EXPECT_EQ(statuses["passing_test"], Fortest::Test::Status::PASS);
    EXPECT_EQ(statuses["failing_test"], Fortest::Test::Status::FAIL);
}

/**
 * @brief Behavior: A test fails when an assertion fails.
 */
TEST_F(TestSessionFixture, AddAndRunTestFailsWhenAssertFails) {
    Fortest::TestSuite ts("FailSuite", assert_obj);

    ts.add_test("failing_test", [&](void *, void *, void *) {
        assert_obj.assert_equal(1, 2, logger); // should fail
    });

    ts.run(logger);

    const std::string out = get_output();
    EXPECT_THAT(out, HasSubstr("Running test: failing_test"));
    EXPECT_THAT(out, HasSubstr("Test failed: failing_test"));
    EXPECT_EQ(assert_obj.get_num_failed(), 1);
}

/**
 * @brief Behavior: Suite-level fixtures should invoke setup and teardown
 * around all contained tests.
 */
TEST_F(TestSessionFixture, SuiteFixtureSetupAndTeardownAreCalled) {
    bool setup_called = false;
    bool teardown_called = false;

    Fortest::Fixture<void> suite_fixture(
        [&](void *) { setup_called = true; },
        [&](void *) { teardown_called = true; },
        nullptr,
        Fortest::Scope::Suite
    );

    Fortest::TestSuite ts("WithSuiteFixture", assert_obj);
    ts.add_fixture(suite_fixture);

    ts.add_test("dummy", [](void *, void *, void *) {});

    ts.run(logger);

    EXPECT_TRUE(setup_called);
    EXPECT_TRUE(teardown_called);
}
