#ifndef G_ASSERT_HPP
#define G_ASSERT_HPP

#include <memory>
#include "assert.hpp"      // your Assert class
#include "global_base.hpp" // your SingletonBase class

// GlobalAssert wraps Assert in a singleton
template <LoggerLike Logger>
class GlobalAssert : public Assert<Logger>,
                     public SingletonBase<GlobalAssert<Logger>> {
private:
    friend class SingletonBase<GlobalAssert<Logger>>;
    GlobalAssert() = default; // only SingletonBase can construct it
};

using GlobalConsoleAssert = GlobalAssert<ConsoleLogger>;

#endif // G_ASSERT_HPP
