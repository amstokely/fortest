#ifndef FORTEST_C_TEST_SESSION_H
#define FORTEST_C_TEST_SESSION_H

#include <string>

#include "g_assert.hpp"
#include "g_logging.hpp"
#include "g_test_session.hpp"

extern "C" {
void c_register_test_suite(const char *name) {
    Fortest::GlobalTestSession::instance().add_test_suite(name);
}

void c_register_fixture(
    const char *suite_name, void *setup_ptr, void *teardown_ptr,
    void *args_ptr, const char *scope
) {
    auto setup = reinterpret_cast<void(*)(void *)>(setup_ptr);
    auto teardown = reinterpret_cast<void(*)(void *)>(teardown_ptr);
    try {
        Fortest::GlobalTestSession::instance().add_fixture(
            suite_name, Fortest::Fixture(
                setup, teardown, args_ptr, scope
            )
        );
    } catch (const std::out_of_range &) {
        Fortest::GlobalLogger::instance()->log(
            "Test suite not found: " + std::string(suite_name), "ERROR"
        );
    }
}

void c_register_test(
    const char *suite_name, const char *test_name, void *test_ptr
) {
    auto test = reinterpret_cast<void(*)(void *, void *, void *)>(
        test_ptr);
    try {
        Fortest::GlobalTestSession::instance().add_test(
            suite_name, test_name, test
        );
    } catch (const std::out_of_range &) {
        Fortest::GlobalLogger::instance()->log(
            "Test suite not found: " + std::string(suite_name), "ERROR"
        );
    }
}

void c_run_test_session() {
    const auto logger = Fortest::GlobalLogger::instance();
    Fortest::GlobalTestSession::instance().run(logger);
}

int c_get_test_suite_status(const char *name) {
    int status = 0;
    try {
        auto statuses = Fortest::GlobalTestSession::instance().
                get_test_suite_status(name);
        for (const auto &test_status: statuses | std::views::values) {
            if (test_status == Fortest::Test::Status::FAIL) {
                status = 1;
                break;
            }
        }
    } catch (const std::out_of_range &) {
        Fortest::GlobalLogger::instance()->log(
            "Test suite not found: " + std::string(name), "ERROR"
        );
        status = -1;
    }
    return status;
}
}

#endif //FORTEST_C_TEST_SESSION_H
