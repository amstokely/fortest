#ifndef TEST_SUITE_HPP
#define TEST_SUITE_HPP

#include "fixture.hpp"
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <utility>

#include "assert.hpp"
#include "logging.hpp"
#include "test.hpp"


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
