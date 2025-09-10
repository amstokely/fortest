#include "logging.hpp"

ConsoleLogger::ConsoleLogger(
    std::ostream &out, std::string border, Color color
)
    : out_(out), border_(std::move(border)), color_(color) {
}

void ConsoleLogger::log(
    const std::string &msg, const std::string &tag
) {
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

void ConsoleLogger::log_with_format(
    const std::string &label, const std::string &msg, Color color
) {
    color_ = color;
    std::string color_code = color_to_code(color_);
    std::string reset_code = "\033[0m";

    if (!border_.empty()) {
        out_ << color_code << border_ << reset_code << '\n';
    }
    out_ << color_code << "[" << label << "] " << msg << reset_code <<
            '\n';
    if (!border_.empty()) {
        out_ << color_code << border_ << reset_code << '\n';
    }
}

std::string ConsoleLogger::color_to_code(Color c) {
    switch (c) {
        case Color::RED: return "\033[31m";
        case Color::GREEN: return "\033[32m";
        case Color::YELLOW: return "\033[33m";
        case Color::BLUE: return "\033[34m";
        case Color::MAGENTA: return "\033[35m";
        case Color::CYAN: return "\033[36m";
        case Color::WHITE: return "\033[37m";
        default: return "\033[0m";
    }
}

std::ostream &operator<<(
    std::ostream &os, const ConsoleLogger &logger
) {
    if (!logger.last_tag_.empty()) {
        os << "[" << logger.last_tag_ << "] " << logger.last_msg_;
    } else {
        os << "(no log yet)";
    }
    return os;
}
