#ifndef FORTEST_TEST_SESSION_HPP
#define FORTEST_TEST_SESSION_HPP
#include <string>
#include <unordered_map>
#include <utility>
#include "test_suite.hpp"

namespace Fortest {
    template<LoggerLike Logger> class TestSession {
        Assert<Logger> &m_assert;
        std::unordered_map<std::string, std::unique_ptr<TestSuite<
            Logger> > > m_suites;
        std::shared_ptr<Fixture> m_session_fixture;

    public:
        explicit
        TestSession(Assert<Logger> &assert) : m_assert(assert) {
        }

        TestSuite<Logger> &add_test_suite(const std::string &name) {
            if (m_suites.find(name) != m_suites.end()) {
                throw std::runtime_error(
                    "Test suite with name '" + name +
                    "' already exists in session '"
                );
            }
            auto suite = std::make_unique<TestSuite<Logger> >(
                name, m_assert
            );
            auto &ref = *suite;
            m_suites[name] = std::move(suite);
            return ref;
        }

        void add_fixture(const Fixture &fixture) {
            if (fixture.get_scope() != "session") {
                throw std::runtime_error(
                    "Fixture without suite name must have session scope"
                );
            }
            m_session_fixture = std::make_shared<Fixture>(fixture);
        }

        void add_fixture(
            const std::string &suite_name, const Fixture &fixture
        ) {
            if (fixture.get_scope() != "test" && fixture.get_scope() !=
                "suite") {
                throw std::runtime_error(
                    "Fixture for suite '" + suite_name +
                    "' must have test or suite scope"
                );
            }
            auto it = m_suites.find(suite_name);
            if (it == m_suites.end()) {
                throw std::runtime_error(
                    "Suite '" + suite_name +
                    "' does not exist in session '"
                );
            }
            it->second->add_fixture(fixture);
        }

        void add_test(
            const std::string &suite_name, const std::string &test_name,
            TestFunction func
        ) {
            auto it = m_suites.find(suite_name);
            if (it == m_suites.end()) {
                throw std::runtime_error(
                    "Suite '" + suite_name +
                    "' does not exist in session '"
                );
            }
            it->second->add_test(test_name, std::move(func));
        }

        // 🔹 Run all test suites
        void run(const std::shared_ptr<Logger> &logger) {
            logger->log("Starting test session: ", "INFO");

            if (m_session_fixture) {
                m_session_fixture->setup();
            }

            for (auto &[name, suite]: m_suites) {
                logger->log("Running test suite: " + name, "INFO");
                suite->add_fixture(
                    Fixture(
                        nullptr, nullptr,
                        m_session_fixture
                            ? m_session_fixture->get_args() : nullptr,
                        "session"
                    )
                );
                suite->run(logger);
            }

            if (m_session_fixture) {
                m_session_fixture->teardown();
            }

            logger->log("Finished test session: ", "INFO");
        }

        [[nodiscard]] std::unordered_map<std::string, Test::Status>
        get_test_suite_status(const std::string &suite_name) const {
            auto it = m_suites.find(suite_name);
            if (it == m_suites.end()) {
                throw std::runtime_error(
                    "Suite '" + suite_name +
                    "' does not exist in session '"
                );
            }
            return it->second->get_statuses();
        }
    };
}

#endif //FORTEST_TEST_SESSION_HPP
