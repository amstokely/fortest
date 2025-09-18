#include "test_suite.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>

using ::testing::HasSubstr;

// A simple logger for testing that writes into an ostringstream
class OStreamLogger : public Fortest::Logger {
public:
    explicit OStreamLogger(std::ostream &out) : Logger(out) {
    }
};

class TestFixture : public ::testing::Test {
protected:
    std::ostringstream buffer;
    std::shared_ptr<OStreamLogger> logger;
    Fortest::Assert<OStreamLogger> assert_obj; // for TestSuite

    void SetUp() override {
        logger = std::make_shared<OStreamLogger>(buffer);
        assert_obj.reset();
    }

    std::string get_output() const {
        return buffer.str();
    }

    void clear_output() {
        buffer.str("");
        buffer.clear();
    }
};

TEST_F(TestFixture, ConstructorSetsName) {
    const std::string test_name = "test";
    int a{}, b{}, c{};
    const Fortest::Test test(test_name, TestFunction());
    EXPECT_EQ(test_name, test.get_name());
}

TEST_F(TestFixture, RunTestFunction) {
    const std::string test_name = "test";
    auto setup = [](void *a) {
        const auto int_a = static_cast<int *>(a);
        *int_a = 0;
    };
    auto teardown = [](void *a) {
    };
    const auto test_args = new int(1);
    const auto test_fixture = std::make_shared<Fortest::Fixture>(
        Fortest::Fixture(setup, teardown, (void *) test_args, "test")
    );
    const auto suite_args = new int(1);
    suite_args[0] = 1;
    const auto suite_fixture = std::make_shared<Fortest::Fixture>(
        Fortest::Fixture(setup, teardown, (void *) suite_args, "suite")
    );
    const auto session_args = new int(1);
    session_args[0] = 1;
    const auto session_fixture = std::make_shared<Fortest::Fixture>(
        Fortest::Fixture(
            setup, teardown, (void *) session_args, "session"
        )
    );
    auto fn = [](void *a, void *b, void *c) {
        const auto int_a = static_cast<int *>(a);
        const auto int_b = static_cast<int *>(b);
        const auto int_c = static_cast<int *>(c);
        *int_a = *int_b + *int_c;
    };
    Fortest::Test test(test_name, fn);
    test.add_fixture(test_fixture);
    test.add_fixture(suite_fixture);
    test.add_fixture(session_fixture);
    test.run(logger, assert_obj);
    ASSERT_EQ(*test_args, 2);
};

TEST_F(TestFixture, ConstructorWithEmptyName) {
    Fortest::Test test("", TestFunction());
    EXPECT_TRUE(test.get_name().empty());
}

TEST_F(TestFixture, AddFixtureStoresCorrectScope) {
    auto f_test = std::make_shared<Fortest::Fixture>(
        nullptr, nullptr, nullptr, "test"
    );
    auto f_suite = std::make_shared<Fortest::Fixture>(
        nullptr, nullptr, nullptr, "suite"
    );
    auto f_session = std::make_shared<Fortest::Fixture>(
        nullptr, nullptr, nullptr, "session"
    );

    Fortest::Test test("name", TestFunction());
    test.add_fixture(f_test);
    test.add_fixture(f_suite);
    test.add_fixture(f_session);

    // Run with dummy function to make sure they are applied
    auto fn = [](void *a, void *b, void *c) {
        EXPECT_EQ(a, nullptr);
        EXPECT_EQ(b, nullptr);
        EXPECT_EQ(c, nullptr);
    };
    Fortest::Test t2("with fixtures", fn);
    t2.add_fixture(f_test);
    t2.add_fixture(f_suite);
    t2.add_fixture(f_session);
    t2.run(logger, assert_obj);
}

TEST_F(TestFixture, AddFixtureWithUnknownScopeDoesNothing) {
    auto f_unknown = std::make_shared<Fortest::Fixture>(
        nullptr, nullptr, nullptr, "unknown"
    );
    Fortest::Test test(
        "name", [](void *, void *, void *) {
        }
    );
    test.add_fixture(f_unknown);

    // Should not crash or use the unknown fixture
    EXPECT_NO_THROW(test.run(logger, assert_obj));
}

TEST_F(TestFixture, RunWithoutFixtures) {
    bool called = false;
    Fortest::Test test(
        "simple", [&](void *, void *, void *) { called = true; }
    );
    test.run(logger, assert_obj);
    EXPECT_TRUE(called);
}

TEST_F(TestFixture, GetTestStatusPassed) {
    Fortest::Test passing_test(
        "simple", [&](void *, void *, void *) {
            assert_obj.assert_true(true, logger);
        }
    );
    Fortest::Test failing_test(
        "simple", [&](void *, void *, void *) {
            assert_obj.assert_true(false, logger);
        }
    );
    passing_test.run(logger, assert_obj);
    failing_test.run(logger, assert_obj);
    EXPECT_EQ(passing_test.get_status(), Fortest::Test::Status::PASS);
    EXPECT_EQ(failing_test.get_status(), Fortest::Test::Status::FAIL);
}

TEST_F(TestFixture, RunCallsSetupAndTeardown) {
    bool setup_called = false;
    bool teardown_called = false;
    int *arg = new int(5);

    auto fixture = std::make_shared<Fortest::Fixture>(
        [&](void *a) { setup_called = true; },
        [&](void *a) { teardown_called = true; }, (void *) arg, "test"
    );

    Fortest::Test test(
        "with fixture", [&](void *a, void *, void *) {
            EXPECT_EQ(a, arg);
        }
    );
    test.add_fixture(fixture);
    test.run(logger, assert_obj);

    EXPECT_TRUE(setup_called);
    EXPECT_TRUE(teardown_called);
    delete arg;
}

TEST_F(TestFixture, RunTestFunctionThrowsStillCallsTeardown) {
    bool teardown_called = false;
    int *arg = new int(42);
    auto fixture = std::make_shared<Fortest::Fixture>(
        nullptr, [&](void *a) { teardown_called = true; }, (void *) arg,
        "test"
    );

    Fortest::Test test(
        "throws", [&](void *, void *, void *) {
            throw std::runtime_error("boom");
        }
    );
    test.add_fixture(fixture);

    EXPECT_THROW(test.run(logger, assert_obj), std::runtime_error);
    EXPECT_TRUE(teardown_called);
    delete arg;
}


TEST_F(TestFixture, RunWithOnlyTestFixture) {
    int *arg = new int(42);
    bool setup_called = false;
    bool teardown_called = false;

    auto fixture = std::make_shared<Fortest::Fixture>(
        [&](void *a) { setup_called = true; },
        [&](void *a) { teardown_called = true; }, (void *) arg, "test"
    );

    Fortest::Test test(
        "only-test", [&](void *a, void *, void *) {
            EXPECT_EQ(a, arg);
        }
    );
    test.add_fixture(fixture);
    test.run(logger, assert_obj);

    EXPECT_TRUE(setup_called);
    EXPECT_TRUE(teardown_called);
    delete arg;
}

TEST_F(TestFixture, RunWithOnlySuiteFixture) {
    int *arg = new int(7);
    Fortest::Test test(
        "only-suite", [&](void *, void *b, void *) {
            EXPECT_EQ(b, arg);
        }
    );

    auto f_suite = std::make_shared<Fortest::Fixture>(
        nullptr, nullptr, (void *) arg, "suite"
    );
    test.add_fixture(f_suite);
    test.run(logger, assert_obj);
    delete arg;
}

TEST_F(TestFixture, SetupRunsBeforeTestAndTeardownAfter) {
    bool order_ok = false;
    int *arg = new int(1);

    auto fixture = std::make_shared<Fortest::Fixture>(
        [&](void *a) { *static_cast<int *>(a) = 2; }, [&](void *a) {
            if (*static_cast<int *>(a) == 3) order_ok = true;
        }, (void *) arg, "test"
    );

    Fortest::Test test(
        "order", [&](void *a, void *, void *) {
            EXPECT_EQ(*static_cast<int*>(a), 2); // setup ran
            *static_cast<int *>(a) = 3;
        }
    );
    test.add_fixture(fixture);
    test.run(logger, assert_obj);

    EXPECT_TRUE(order_ok);
    delete arg;
}

TEST_F(TestFixture, MultipleAddFixtureOverridesPrevious) {
    int *arg1 = new int(5);
    int *arg2 = new int(10);

    auto f1 = std::make_shared<Fortest::Fixture>(
        nullptr, nullptr, (void *) arg1, "suite"
    );
    auto f2 = std::make_shared<Fortest::Fixture>(
        nullptr, nullptr, (void *) arg2, "suite"
    );

    Fortest::Test test(
        "override", [&](void *, void *b, void *) {
            EXPECT_EQ(b, arg2); // last one should win
        }
    );
    test.add_fixture(f1);
    test.add_fixture(f2);
    test.run(logger, assert_obj);

    delete arg1;
    delete arg2;
}
