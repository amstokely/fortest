#ifndef GLOBAL_BASE_HPP
#define GLOBAL_BASE_HPP
#include <memory>

namespace Fortest {
    template<typename Derived> class SingletonBase {
    public:
        static std::shared_ptr<Derived> &instance() {
            static std::shared_ptr<Derived> instance{new Derived()};
            return instance;
        }

        SingletonBase(const SingletonBase &) = delete;

        SingletonBase &operator=(const SingletonBase &) = delete;

    protected:
        SingletonBase() = default;

        virtual ~SingletonBase() = default;
    };
}


#endif //GLOBAL_BASE_HPP
