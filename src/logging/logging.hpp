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
class ConsoleLogger {
public:
    explicit ConsoleLogger(std::ostream &out = std::cout,
                           std::string border = "",
                           Color color = Color::DEFAULT);

    void log(const std::string &msg, const std::string &tag);

    static std::string color_to_code(Color c);

    // stream insertion operator
    friend std::ostream& operator<<(std::ostream &os, const ConsoleLogger &logger);

private:
    std::ostream &out_;
    std::string border_;
    Color color_;

    // Remember last log for operator<<
    std::string last_msg_;
    std::string last_tag_;

    void log_with_format(const std::string &label,
                         const std::string &msg,
                         Color color);
};

#endif // LOGGING_HPP
