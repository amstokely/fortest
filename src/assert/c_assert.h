#ifndef C_ASSERT_HPP
#define C_ASSERT_HPP

#include "assert.hpp"
#include "g_logging.hpp"
#include "g_assert.hpp"

/// @file c_assert.hpp
/// @brief C bindings for the Fortest assertion framework.
///
/// This header provides a C-compatible interface to the Fortest
/// assertion utilities. Each function delegates to the global
/// Fortest logger and assert instances, making it possible to use
/// the framework from C or Fortran code without directly depending
/// on C++ constructs.
///
/// Supported assertions:
/// - Boolean checks (`true` / `false`)
/// - Equality / inequality for integers, floats, doubles, and strings
///
/// All results are logged through the global logger instance.

extern "C" {

/// @brief Assert that a condition is true.
/// @param condition Integer treated as boolean (non-zero = true).
void c_assert_true(const int condition) {
    auto logger = Fortest::GlobalLogger::instance();
    auto assert = Fortest::GlobalAssert::instance();
    assert->assert_true(condition, logger);
}

/// @brief Assert that a condition is false.
/// @param condition Integer treated as boolean (zero = false).
void c_assert_false(const int condition) {
    auto logger = Fortest::GlobalLogger::instance();
    auto assert = Fortest::GlobalAssert::instance();
    assert->assert_false(condition, logger);
}

/// @brief Assert that two integers are equal.
/// @param expected Expected integer value.
/// @param actual Actual integer value.
void c_assert_equal_int(const int expected, const int actual) {
    auto logger = Fortest::GlobalLogger::instance();
    auto assert = Fortest::GlobalAssert::instance();
    assert->assert_equal(expected, actual, logger);
}

/// @brief Assert that two doubles are equal.
/// @param expected Expected double value.
/// @param actual Actual double value.
void c_assert_equal_double(const double expected, const double actual) {
    auto logger = Fortest::GlobalLogger::instance();
    auto assert = Fortest::GlobalAssert::instance();
    assert->assert_equal(expected, actual, logger);
}

/// @brief Assert that two C strings are equal.
/// @param expected Expected C string.
/// @param actual Actual C string.
void c_assert_equal_string(const char *expected, const char *actual) {
    auto logger = Fortest::GlobalLogger::instance();
    auto assert = Fortest::GlobalAssert::instance();
    assert->assert_equal(
        std::string(expected), std::string(actual), logger
    );
}

/// @brief Assert that two floats are equal.
/// @param expected Expected float value.
/// @param actual Actual float value.
void c_assert_equal_float(const float expected, const float actual) {
    auto logger = Fortest::GlobalLogger::instance();
    auto assert = Fortest::GlobalAssert::instance();
    assert->assert_equal(expected, actual, logger);
}

/// @brief Assert that two integers are not equal.
/// @param expected First integer value.
/// @param actual Second integer value.
void c_assert_not_equal_int(const int expected, const int actual) {
    auto logger = Fortest::GlobalLogger::instance();
    auto assert = Fortest::GlobalAssert::instance();
    assert->assert_not_equal(expected, actual, logger);
}

/// @brief Assert that two doubles are not equal.
/// @param expected First double value.
/// @param actual Second double value.
void c_assert_not_equal_double(
    const double expected, const double actual
) {
    auto logger = Fortest::GlobalLogger::instance();
    auto assert = Fortest::GlobalAssert::instance();
    assert->assert_not_equal(expected, actual, logger);
}

/// @brief Assert that two floats are not equal.
/// @param expected First float value.
/// @param actual Second float value.
void c_assert_not_equal_float(
    const float expected, const float actual
) {
    auto logger = Fortest::GlobalLogger::instance();
    auto assert = Fortest::GlobalAssert::instance();
    assert->assert_not_equal(expected, actual, logger);
}

/// @brief Assert that two C strings are not equal.
/// @param expected First C string.
/// @param actual Second C string.
void c_assert_not_equal_string(
    const char *expected, const char *actual
) {
    auto logger = Fortest::GlobalLogger::instance();
    auto assert = Fortest::GlobalAssert::instance();
    assert->assert_not_equal(
        std::string(expected), std::string(actual), logger
    );
}

} // extern "C"

#endif //C_ASSERT_HPP
