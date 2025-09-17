#ifndef TEST_SUITE_HPP
#define TEST_SUITE_HPP

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <tuple>
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

    // Lifecycle
    void setup() const;

    void teardown() const;

    // Copy semantics
    Fixture(const Fixture &);

    Fixture &operator=(const Fixture &);

    [[nodiscard]] const std::string &get_scope() const {
        return m_scope;
    }

    [[nodiscard]] Args get_args() const { return m_args; }
};

class Test {
public:
    enum class Status {
        PASS,
        FAIL,
        NONE
    };

private:
    TestFunction m_test;
    std::shared_ptr<Fixture> m_test_fixture;
    std::shared_ptr<Fixture> m_suite_fixture;
    std::shared_ptr<Fixture> m_session_fixture;
    std::string m_name;
    Status m_status = Status::NONE;

public:
    explicit Test(std::string name, TestFunction test)
        : m_test(std::move(test)), m_name(std::move(name)) {
    }

    [[nodiscard]] const std::string &get_name() const { return m_name; }

    void add_fixture(const std::shared_ptr<Fixture> &fixture) {
        const auto &scope = fixture->get_scope();
        if (scope == "session") {
            m_session_fixture = fixture;
        } else if (scope == "suite") {
            m_suite_fixture = fixture;
        } else if (scope == "test") {
            m_test_fixture = fixture;
        }
    }

    template<LoggerLike Logger> void run(
        const std::shared_ptr<Logger> &logger, Assert<Logger> &assert
    ) {
        void *test_args = nullptr;
        void *suite_args = nullptr;
        void *session_args = nullptr;

        if (m_test_fixture) {
            test_args = m_test_fixture->get_args();
            m_test_fixture->setup();
        }
        if (m_suite_fixture) {
            suite_args = m_suite_fixture->get_args();
        }
        if (m_session_fixture) {
            session_args = m_session_fixture->get_args();
        }
        assert.reset();

        auto args = std::make_tuple(
            test_args, suite_args, session_args
        );
        try {
            std::apply(m_test, args);
            if (assert.get_num_failed() == 0) {
                m_status = Status::PASS;
            } else {
                m_status = Status::FAIL;
            }
            if (m_test_fixture) {
                m_test_fixture->teardown();
            }
        } catch (...) {
            if (m_test_fixture) {
                m_test_fixture->teardown();
            }
            m_status = Status::FAIL;
            throw;
        }
    }

    [[nodiscard]] Status get_status() const { return m_status; }
};

template<typename Logger> class TestSuite {
    std::string m_name;
    std::map<std::string, Test> m_tests;
    std::shared_ptr<Fixture> m_test_fixture = std::make_shared<Fixture>(
        nullptr, nullptr, nullptr, "test"
    );
    std::shared_ptr<Fixture> m_suite_fixture;
    std::shared_ptr<Fixture> m_session_fixture;
    Assert<Logger> &m_assert;
    std::unordered_map<std::string, Test::Status> m_statuses;

public:
    TestSuite(std::string name, Assert<Logger> &assert) : m_name(
        std::move(name)
    ), m_assert(assert) {
    }

    void add_fixture(const Fixture &fixture) {
        const auto &scope = fixture.get_scope();
        if (scope == "session") {
            m_session_fixture = std::make_shared<Fixture>(fixture);
        } else if (scope == "suite") {
            m_suite_fixture = std::make_shared<Fixture>(fixture);
        } else if (scope == "test") {
            m_test_fixture = std::make_shared<Fixture>(fixture);
        }
    }

    void add_test(const std::string &test_name, TestFunction func) {
        Test test(test_name, std::move(func));

        if (m_test_fixture) {
            test.add_fixture(m_test_fixture);
        }
        if (m_suite_fixture) {
            test.add_fixture(m_suite_fixture);
        }
        if (m_session_fixture) {
            test.add_fixture(m_session_fixture);
        }
        m_tests.emplace(test_name, std::move(test));
        m_statuses[test_name] = test.get_status();
    }

    [[nodiscard]] const std::string &get_name() const { return m_name; }

    [[nodiscard]] const std::unordered_map<std::string, Test::Status> &
    get_statuses() const {
        return m_statuses;
    }

    void run(const std::shared_ptr<Logger> &logger) {
        if (m_suite_fixture) {
            m_suite_fixture->setup();
        }

        for (auto &[test_name, test]: m_tests) {
            logger->log("Running test: " + test_name, "INFO");
            test.run(logger, m_assert);
            m_statuses[test_name] = test.get_status();
            if (test.get_status() == Test::Status::PASS) {
                logger->log("Test passed: " + test_name, "PASS");
            } else {
                logger->log("Test failed: " + test_name, "FAIL");
            }
        }

        if (m_suite_fixture) {
            m_suite_fixture->teardown();
        }
    }
};

#endif // TEST_SUITE_HPP
