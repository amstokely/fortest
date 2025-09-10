#ifndef C_TEST_SUITE_H
#define C_TEST_SUITE_H

#include <string>

#include "g_assert.hpp"
#include "g_logging.hpp"
#include "test_suite_registry.hpp"

extern "C" {
void c_register_test_suite(
    const char *name
) {
    auto assert = GlobalConsoleAssert::instance();
    TestSuiteRegistry::instance().try_emplace(
        name, TestSuite(
            name, *assert
        )
    );
}

void c_register_fixture(
    const char *suite_name, const char *fixture_name, void *setup_ptr, void *teardown_ptr,
    void *args_ptr, const char *scope
) {
    auto setup = reinterpret_cast<void(*)(void *)>(setup_ptr);
    auto teardown = reinterpret_cast<void(*)(void *)>(teardown_ptr);
    try {
        auto &suite = TestSuiteRegistry::instance().at(suite_name);
        suite.add_fixture(Fixture(setup, teardown, args_ptr, scope));
    } catch (const std::out_of_range &) {
        GlobalConsoleLogger::instance()->log(
            "Test suite not found: " + std::string(suite_name), "ERROR"
        );
    }
}

void c_register_test(
    const char *suite_name, const char *test_name, void *test_ptr
) {
    auto test = reinterpret_cast<void(*)(void *, void *, void *)>(test_ptr);
    try {
        auto &suite = TestSuiteRegistry::instance().at(suite_name);
        suite.add_test(test_name, test);
    } catch (const std::out_of_range &) {
        GlobalConsoleLogger::instance()->log(
            "Test suite not found: " + std::string(suite_name), "ERROR"
        );
    }
}

void c_run_test_suite(const char *name) {
    try {
        auto &suite = TestSuiteRegistry::instance().at(name);
        GlobalConsoleLogger::instance()->log(
            "Running test suite: " + std::string(name), "INFO"
        );
        suite.run(GlobalConsoleLogger::instance());
    } catch (const std::out_of_range &) {
        GlobalConsoleLogger::instance()->log(
            "Test suite not found: " + std::string(name), "ERROR"
        );
    }
}
}

#endif //C_TEST_SUITE_H
