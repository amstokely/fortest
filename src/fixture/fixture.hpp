#ifndef FORTEST_FIXTURE_HPP
#define FORTEST_FIXTURE_HPP
#include <algorithm>
#include <functional>

namespace Fortest {
    using FixtureFunction = std::function<void(void *)>;
    using Args = void *;

    enum class Scope { Test, Suite, Session };

    template <typename T = void>
    class Fixture {
        using FixtureFunction = std::function<void(T*)>;

        FixtureFunction m_setup{};
        FixtureFunction m_teardown{};
        T* m_args{};
        Scope m_scope{};

    public:
        Fixture(FixtureFunction setup,
                FixtureFunction teardown,
                T* args,
                Scope scope)
            : m_setup(std::move(setup)),
              m_teardown(std::move(teardown)),
              m_args(args),
              m_scope(scope) {}

        void setup() const { if (m_setup) m_setup(m_args); }
        void teardown() const { if (m_teardown) m_teardown(m_args); }

        Fixture(const Fixture&) = default;
        Fixture(Fixture&&) noexcept = default;
        Fixture& operator=(const Fixture&) = default;
        Fixture& operator=(Fixture&&) noexcept = default;

        [[nodiscard]] Scope get_scope() const noexcept { return m_scope; }
        [[nodiscard]] T* get_args() const noexcept { return m_args; }
    };

}


#endif //FORTEST_FIXTURE_HPP
