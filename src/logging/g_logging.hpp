#ifndef G_LOGGING_HPP
#define G_LOGGING_HPP
#include "global_base.hpp"
#include "logging.hpp"

namespace Fortest {
    class GlobalLogger : public Logger, public SingletonBase<Logger> {
    private:
        friend class SingletonBase<GlobalLogger>;

        GlobalLogger() = default;
    };
}

#endif //G_LOGGING_HPP
