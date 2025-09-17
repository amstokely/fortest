#ifndef C_TEST_SUITE_H
#define C_TEST_SUITE_H

#include <string>

#include "g_assert.hpp"
#include "g_logging.hpp"
#include "g_test_session.hpp"

extern "C" {
void c_register_test_suite(
    const char *name
) {
    GlobalTestSession::instance().add_test_suite(name);
}

void c_register_fixture(
    const char *suite_name, const char *fixture_name, void *setup_ptr, void *teardown_ptr,
    void *args_ptr, const char *scope
) {
    auto setup = reinterpret_cast<void(*)(void *)>(setup_ptr);
    auto teardown = reinterpret_cast<void(*)(void *)>(teardown_ptr);
    try {
        GlobalTestSession::instance().add_fixture(Fixture(setup, teardown, args_ptr, scope));
    } catch (const std::out_of_range &) {
        GlobalLogger::instance()->log(
            "Test suite not found: " + std::string(suite_name), "ERROR"
        );
    }
}

void c_register_test(
    const char *suite_name, const char *test_name, void *test_ptr
) {
    auto test = reinterpret_cast<void(*)(void *, void *, void *)>(test_ptr);
    try {
        GlobalTestSession::instance().add_test(suite_name, test_name, test);
    } catch (const std::out_of_range &) {
        GlobalLogger::instance()->log(
            "Test suite not found: " + std::string(suite_name), "ERROR"
        );
    }
}

void c_run_test_suite(const char *name) {
    try {
        GlobalLogger::instance()->log(
            "Running test suite: " + std::string(name), "INFO"
        );
        auto logger = GlobalLogger::instance();
        GlobalTestSession::instance().run(logger);
    } catch (const std::out_of_range &) {
        GlobalLogger::instance()->log(
            "Test suite not found: " + std::string(name), "ERROR"
        );
    }
}

int c_get_test_suite_status(const char *name) {
  int status = 0;
    try {
        auto statuses = GlobalTestSession::instance().get_test_suite_status(name);
        for (const auto& [test_name, test_status] : statuses) {
            if (test_status == Test::Status::FAIL) {
                status = 1; // At least one test failed
                break;
            }
        }
    } catch (const std::out_of_range &) {
        GlobalLogger::instance()->log(
            "Test suite not found: " + std::string(name), "ERROR"
        );
        status = -1; // Indicate error
    }
    return status;

}
}

#endif //C_TEST_SUITE_H
