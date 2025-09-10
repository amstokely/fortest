#ifndef G_LOGGING_HPP
#define G_LOGGING_HPP
#include "global_base.hpp"
#include "logging.hpp"

template <LoggerLike Logger>
class GlobalLogger : public Logger, public SingletonBase<GlobalLogger<Logger>> {
private:
    friend class SingletonBase<GlobalLogger>;
    GlobalLogger() = default;

};

// Alias for convenience
using GlobalConsoleLogger = GlobalLogger<ConsoleLogger>;

#endif //G_LOGGING_HPP
