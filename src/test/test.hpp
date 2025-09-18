#ifndef FORTEST_TEST_HPP
#define FORTEST_TEST_HPP

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include "fixture.hpp"

#include "assert.hpp"
#include "logging.hpp"

using TestFunction = std::function<void(void *, void *, void *)>;
using Args = void *;


namespace Fortest {
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

        [[nodiscard]] const std::string &get_name() const {
            return m_name;
        }

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
            const std::shared_ptr<Logger> &logger,
            Assert<Logger> &assert
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
}

#endif //FORTEST_TEST_HPP
