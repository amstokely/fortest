#ifndef G_ASSERT_HPP
#define G_ASSERT_HPP

#include <memory>
#include "assert.hpp"      // your Assert class
#include "global_base.hpp" // your SingletonBase class
namespace Fortest {
    // GlobalAssert wraps Assert in a singleton
    class GlobalAssert : public Assert<Logger>,
                         public SingletonBase<Assert<Logger>> {
    private:
        friend class SingletonBase<Assert<Logger>>;
        GlobalAssert() = default; // only SingletonBase can construct it
    };
}

#endif // G_ASSERT_HPP
