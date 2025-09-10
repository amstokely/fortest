#include "test_suite.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>

using ::testing::HasSubstr;

// A simple logger for testing that writes into an ostringstream
class OStreamLogger : public ConsoleLogger {
public:
    explicit OStreamLogger(std::ostream &out) : ConsoleLogger(out) {}
};

class TestSuiteFixture : public ::testing::Test {
protected:
    std::ostringstream buffer;
    std::shared_ptr<OStreamLogger> logger;
    Assert<ConsoleLogger> assert_obj;  // for TestSuite

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

TEST_F(TestSuiteFixture, FixtureSetupAndTeardownAreCalled) {
    bool setup_called = false;
    bool teardown_called = false;
    auto setup = [&](void *) { setup_called = true; };
    auto teardown = [&](void *) { teardown_called = true; };

    Fixture f(setup, teardown, nullptr, "test");
    f.setup();
    f.teardown();

    EXPECT_TRUE(setup_called);
    EXPECT_TRUE(teardown_called);
    EXPECT_EQ(f.get_scope(), "test");
}

TEST_F(TestSuiteFixture, AddAndGetName) {
    TestSuite ts("MySuite", assert_obj);
    EXPECT_EQ(ts.get_name(), "MySuite");
}

TEST_F(TestSuiteFixture, AddAndRunSingleTestPasses) {
    TestSuite ts("SimpleSuite", assert_obj);

    // Add a test that does nothing (no failures)
    ts.add_test("empty_test", [](void*, void*, void*) {});

    ts.run(logger);

    std::string out = get_output();
    EXPECT_THAT(out, HasSubstr("Running test: empty_test"));
    EXPECT_THAT(out, HasSubstr("[PASS] Test passed: empty_test"));
    EXPECT_EQ(assert_obj.get_num_failed(), 0);
}

TEST_F(TestSuiteFixture, AddAndRunTestFailsWhenAssertFails) {
    TestSuite ts("FailSuite", assert_obj);

    ts.add_test("failing_test", [&](void*, void*, void*) {
        assert_obj.assert_equal(1, 2, logger);  // should fail
    });

    ts.run(logger);

    std::string out = get_output();
    EXPECT_THAT(out, HasSubstr("Running test: failing_test"));
    EXPECT_THAT(out, HasSubstr("[FAIL] Test failed: failing_test"));
    EXPECT_EQ(assert_obj.get_num_failed(), 1);
}

TEST_F(TestSuiteFixture, SuiteFixtureSetupAndTeardownAreCalled) {
    bool setup_called = false;
    bool teardown_called = false;

    Fixture suite_fixture(
        [&](void*) { setup_called = true; },
        [&](void*) { teardown_called = true; },
        nullptr, "suite"
    );

    TestSuite ts("WithSuiteFixture", assert_obj);
    ts.add_fixture(suite_fixture);

    ts.add_test("dummy", [](void*, void*, void*) {});

    ts.run(logger);

    EXPECT_TRUE(setup_called);
    EXPECT_TRUE(teardown_called);
}
