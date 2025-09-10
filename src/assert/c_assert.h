#ifndef C_ASSERT_HPP
#define C_ASSERT_HPP

#include "assert.hpp"
#include "g_logging.hpp"
#include "g_assert.hpp"



extern "C" {
void c_assert_true(const int condition) {
   auto logger = GlobalConsoleLogger::instance();    // from your GlobalLogger
    auto assert = GlobalConsoleAssert::instance();
    assert->assert_true(condition, logger);
}

void c_assert_false(const int condition) {
   auto logger = GlobalConsoleLogger::instance();    // from your GlobalLogger
    auto assert = GlobalConsoleAssert::instance();
    assert->assert_false(condition, logger);
}

void c_assert_equal_int(const int expected, const int actual) {
   auto logger = GlobalConsoleLogger::instance();    // from your GlobalLogger
    auto assert = GlobalConsoleAssert::instance();
    assert->assert_equal(expected, actual, logger);
}

void c_assert_equal_double(const double expected, const double actual) {
   auto logger = GlobalConsoleLogger::instance();    // from your GlobalLogger
    auto assert = GlobalConsoleAssert::instance();
    assert->assert_equal(expected, actual, logger);
}

void c_assert_equal_string(const char *expected, const char *actual) {
   auto logger = GlobalConsoleLogger::instance();    // from your GlobalLogger
    auto assert = GlobalConsoleAssert::instance();
    assert->assert_equal(
        std::string(expected), std::string(actual),
        logger
    );
}


void c_assert_equal_float(const float expected, const float actual) {
   auto logger = GlobalConsoleLogger::instance();    // from your GlobalLogger
    auto assert = GlobalConsoleAssert::instance();
    assert->assert_equal(expected, actual, logger);
}

void c_assert_not_equal_int(const int expected, const int actual) {
   auto logger = GlobalConsoleLogger::instance();    // from your GlobalLogger
    auto assert = GlobalConsoleAssert::instance();
    assert->assert_not_equal(
        expected, actual, logger
    );
}

void c_assert_not_equal_double(
    const double expected, const double actual
) {
   auto logger = GlobalConsoleLogger::instance();    // from your GlobalLogger
    auto assert = GlobalConsoleAssert::instance();
    assert->assert_not_equal(
        expected, actual, logger
    );
}

void c_assert_not_equal_float(
    const float expected, const float actual
) {
   auto logger = GlobalConsoleLogger::instance();    // from your GlobalLogger
    auto assert = GlobalConsoleAssert::instance();
    assert->assert_not_equal(
        expected, actual, logger
    );
}

void c_assert_not_equal_string(
    const char *expected, const char *actual
) {
   auto logger = GlobalConsoleLogger::instance();    // from your GlobalLogger
    auto assert = GlobalConsoleAssert::instance();
    assert->assert_not_equal(
        std::string(expected), std::string(actual),
        logger
    );
}
}


#endif //C_ASSERT_HPP
