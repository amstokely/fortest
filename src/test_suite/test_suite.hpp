#ifndef TEST_SUITE_HPP
#define TEST_SUITE_HPP

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "assert.hpp"
#include "logging.hpp"

using TestFunction = std::function<void(void *, void *, void *)>;
using FixtureFunction = std::function<void(void *)>;

using Args = void *;

class Fixture {
    FixtureFunction m_setup{};
    FixtureFunction m_teardown{};
    Args m_args{};
    std::string m_scope{};

public:
    Fixture(
        FixtureFunction setup, FixtureFunction teardown, Args args,
        std::string scope
    );

    void setup() const;

    void teardown() const;

    // Copy/clone support
    Fixture(const Fixture &);

    Fixture &operator=(const Fixture &);

    [[nodiscard]] std::string get_scope() const;

    [[nodiscard]] const Args &get_args() const { return m_args; }

    std::string get_scope() { return m_scope; }
};

class TestSuite {
    std::string name;
    std::map<std::string, TestFunction> tests;
    std::unique_ptr<Fixture> test_fixture = std::make_unique<Fixture>(
        nullptr, nullptr, nullptr, "test"
    );
    std::unique_ptr<Fixture> suite_fixture;
    std::unique_ptr<Fixture> session_fixture;
    Assert<ConsoleLogger> &m_assert;

public:
    TestSuite(std::string n, Assert<ConsoleLogger> &assert);

    void add_test(const std::string &testName, TestFunction func);

    void add_fixture(const Fixture &fixture) {
        if (fixture.get_scope() == "test") {
            test_fixture = std::make_unique<Fixture>(fixture);
        } else if (fixture.get_scope() == "suite") {
            suite_fixture = std::make_unique<Fixture>(fixture);
        } else {
            std::cerr << "Unknown fixture scope: " << fixture.
                    get_scope() << std::endl;
        }
    }

    [[nodiscard]] std::string get_name() const { return name; }

    template<LoggerLike Logger> void run(
        const std::shared_ptr<Logger> &logger
    ) const {
        void *test_args = nullptr;
        void *suite_args = nullptr;
        void *session_args = nullptr;
        if (test_fixture) {
            test_args = test_fixture->get_args();
        }
        if (suite_fixture) {
            suite_args = suite_fixture->get_args();
            suite_fixture->setup();
        }
        if (session_fixture) {
            session_args = session_fixture->get_args();
        }
        auto args = std::make_tuple(
            test_args, suite_args, session_args
        );
        for (const auto &[testName, func]: tests) {
            logger->log("Running test: " + testName, "INFO");
            m_assert.reset();
            test_fixture->setup();
            std::apply(func, args);
            test_fixture->teardown();
            if (m_assert.get_num_failed() == 0) {
                logger->log("Test passed: " + testName, "PASS");
            } else {
                logger->log("Test failed: " + testName, "FAIL");
            }
        }
        if (suite_fixture) {
            suite_fixture->teardown();
        }
    }
};


#endif // TEST_SUITE_HPP
