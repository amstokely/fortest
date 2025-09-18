#ifndef FORTEST_C_TEST_SESSION_H
#define FORTEST_C_TEST_SESSION_H

#include <string>

#include "g_assert.hpp"
#include "g_logging.hpp"
#include "g_test_session.hpp"
#include "fixture.hpp"

extern "C" {

/**
 * @brief Register a new test suite with the global session.
 *
 * @param name Name of the test suite.
 */
void c_register_test_suite(const char *name) {
    Fortest::GlobalTestSession::instance().add_test_suite(name);
}

/**
 * @brief Register a fixture for a given test suite.
 *
 * @param suite_name  Name of the test suite.
 * @param setup_ptr   Pointer to the setup function (void(*)(void*)).
 * @param teardown_ptr Pointer to the teardown function (void(*)(void*)).
 * @param args_ptr    Pointer to user-provided arguments.
 * @param scope       Scope string ("test", "suite", "session").
 */
void c_register_fixture(
    const char *suite_name,
    void *setup_ptr,
    void *teardown_ptr,
    void *args_ptr,
    const char *scope
) {
    auto setup = reinterpret_cast<void(*)(void *)>(setup_ptr);
    auto teardown = reinterpret_cast<void(*)(void *)>(teardown_ptr);

    // Map string scope to enum
    Fortest::Scope scope_enum = Fortest::Scope::Test;
    if (std::string(scope) == "test") {
        scope_enum = Fortest::Scope::Test;
    }
    else if (std::string(scope) == "suite") {
        scope_enum = Fortest::Scope::Suite;
    } else if (std::string(scope) == "session") {
        scope_enum = Fortest::Scope::Session;
    }

    try {
        Fortest::GlobalTestSession::instance().add_fixture(
            suite_name,
            Fortest::Fixture<void>(setup, teardown, args_ptr, scope_enum)
        );
    } catch (const std::out_of_range &) {
        Fortest::GlobalLogger::instance()->log(
            "Test suite not found: " + std::string(suite_name),
            "ERROR"
        );
    }
}

/**
 * @brief Register a test case with the given suite.
 *
 * @param suite_name Name of the test suite.
 * @param test_name  Name of the test.
 * @param test_ptr   Pointer to the test function
 *                   (void(*)(void*, void*, void*)).
 */
void c_register_test(
    const char *suite_name,
    const char *test_name,
    void *test_ptr
) {
    auto test = reinterpret_cast<void(*)(void *, void *, void *)>(test_ptr);
    try {
        Fortest::GlobalTestSession::instance().add_test(
            suite_name, test_name, test
        );
    } catch (const std::out_of_range &) {
        Fortest::GlobalLogger::instance()->log(
            "Test suite not found: " + std::string(suite_name),
            "ERROR"
        );
    }
}

/**
 * @brief Run all registered tests in the global session.
 */
void c_run_test_session() {
    const auto logger = Fortest::GlobalLogger::instance();
    Fortest::GlobalTestSession::instance().run(logger);
}

/**
 * @brief Get the overall status of a test suite.
 *
 * @param name Name of the test suite.
 * @return 0 if all tests passed,
 *         1 if at least one test failed,
 *        -1 if suite was not found.
 */
int c_get_test_suite_status(const char *name) {
    int status = 0;
    try {
        auto statuses =
            Fortest::GlobalTestSession::instance().get_test_suite_status(name);

        for (const auto &test_status : statuses | std::views::values) {
            if (test_status == Fortest::Test::Status::FAIL) {
                status = 1;
                break;
            }
        }
    } catch (const std::out_of_range &) {
        Fortest::GlobalLogger::instance()->log(
            "Test suite not found: " + std::string(name),
            "ERROR"
        );
        status = -1;
    }
    return status;
}

} // extern "C"

#endif // FORTEST_C_TEST_SESSION_H
