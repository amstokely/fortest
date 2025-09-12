#ifndef ASSERT_HPP
#define ASSERT_HPP
#include <memory>
#include <string>
#include "logging.hpp"

template<LoggerLike Logger>
class Assert {
    int m_num_passed{};
    int m_num_failed{};

    template<typename T>
    static std::string to_string_repr(const T& value) {
        if constexpr (std::is_same_v<T, std::string>) {
            return value;
        } else {
            return std::to_string(value);
        }
    }

public:
    template<typename T>
    void assert_equal(const T& expected, const T& actual,
                      const std::shared_ptr<Logger>& logger) {
        const auto expected_str = to_string_repr(expected);
        const auto actual_str   = to_string_repr(actual);

        if (expected == actual) {
//            logger->log("expected == actual (" + expected_str + ")", "TRUE");
            ++m_num_passed;
        } else {
//            logger->log("expected: " + expected_str + ", actual: " + actual_str, "FALSE");
            ++m_num_failed;
        }
    }

    template<typename T>
    void assert_not_equal(const T& expected, const T& actual,
                          const std::shared_ptr<Logger>& logger) {
        const auto expected_str = to_string_repr(expected);
        const auto actual_str   = to_string_repr(actual);

        if (expected != actual) {
//            logger->log("expected != actual (" + expected_str + ")", "TRUE");
            ++m_num_passed;
        } else {
//            logger->log("expected: " + expected_str + ", actual: " + actual_str, "FALSE");
            ++m_num_failed;
        }
    }

    void assert_true(bool condition, const std::shared_ptr<Logger>& logger) {
        if (condition) {
//            logger->log("condition is TRUE", "TRUE");
            ++m_num_passed;
        } else {
//            logger->log("condition is FALSE", "FALSE");
            ++m_num_failed;
        }
    }

    void assert_false(bool condition, const std::shared_ptr<Logger>& logger) {
        if (!condition) {
//            logger->log("condition is FALSE", "TRUE");
            ++m_num_passed;
        } else {
//            logger->log("condition is TRUE", "FALSE");
            ++m_num_failed;
        }
    }

    int get_num_passed() const { return m_num_passed; }
    int get_num_failed() const { return m_num_failed; }
    void reset() { m_num_passed = m_num_failed = 0; }
};
#endif // ASSERT_HPP
