#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <concepts>
#include <iostream>
#include <ostream>
#include <string>
#include <utility>

enum class Color {
    DEFAULT,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
};

template <typename T>
concept LoggerLike =
    requires(T logger, const std::string &msg, const std::string &tag) {
        { logger.log(msg, tag) } -> std::same_as<void>;
    };

/// Default implementation: logs to a given std::ostream
class Logger {
public:
    explicit Logger(std::ostream &out = std::cout,
                           std::string border = "",
                           Color color = Color::DEFAULT)
        : out_(out), border_(std::move(border)), color_(color) {}

    void log(const std::string &msg, const std::string &tag) {
        last_msg_ = msg;
        last_tag_ = tag;

        if (tag == "PASS") {
            log_with_format("PASS", msg, Color::GREEN);
        } else if (tag == "FAIL") {
            log_with_format("FAIL", msg, Color::RED);
        } else if (tag == "INFO") {
            log_with_format("INFO", msg, Color::DEFAULT);
        } else if (tag == "TRUE") {
            log_with_format("TRUE", msg, Color::GREEN);
        } else if (tag == "FALSE") {
            log_with_format("FALSE", msg, Color::RED);
        } else {
            out_ << msg << '\n';
        }
    }

    static std::string color_to_code(Color c) {
        switch (c) {
            case Color::RED:     return "\033[31m";
            case Color::GREEN:   return "\033[32m";
            case Color::YELLOW:  return "\033[33m";
            case Color::BLUE:    return "\033[34m";
            case Color::MAGENTA: return "\033[35m";
            case Color::CYAN:    return "\033[36m";
            case Color::WHITE:   return "\033[37m";
            default:             return "\033[0m";
        }
    }

    // stream insertion operator
    friend std::ostream& operator<<(std::ostream &os, const Logger &logger) {
        if (!logger.last_tag_.empty()) {
            os << "[" << logger.last_tag_ << "] " << logger.last_msg_;
        } else {
            os << "(no log yet)";
        }
        return os;
    }

private:
    std::ostream &out_;
    std::string border_;
    Color color_;

    // Remember last log for operator<<
    std::string last_msg_;
    std::string last_tag_;

    void log_with_format(const std::string &label,
                         const std::string &msg,
                         Color color) {
        color_ = color;
        std::string color_code = color_to_code(color_);
        std::string reset_code = "\033[0m";

        if (!border_.empty()) {
            out_ << color_code << border_ << reset_code << '\n';
        }
        out_ << color_code << "[" << label << "] " << msg << reset_code << '\n';
        if (!border_.empty()) {
            out_ << color_code << border_ << reset_code << '\n';
        }
    }
};

#endif // LOGGING_HPP
