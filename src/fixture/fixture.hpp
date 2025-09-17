#ifndef FORTEST_FIXTURE_HPP
#define FORTEST_FIXTURE_HPP
#include <algorithm>
#include <functional>


using FixtureFunction = std::function<void(void *)>;
using Args = void *;

class Fixture {
    FixtureFunction m_setup{};
    FixtureFunction m_teardown{};
    Args m_args{};
    std::string m_scope{};

public:

    Fixture(
        FixtureFunction setup,
        FixtureFunction teardown,
        Args args,
        std::string scope
    ) :
        m_setup(std::move(setup)),
        m_teardown(std::move(teardown)),
        m_args(args),
        m_scope(std::move(scope))
    {}

    void setup() const {
        if (m_setup) {
            m_setup(m_args);
        }
    }

    void teardown() const {
        if (m_teardown) {
            m_teardown(m_args);
        }
    }

    // Defaulted copy semantics
    Fixture(const Fixture &) = default;
    Fixture &operator=(const Fixture &) = default;
    [[nodiscard]] const std::string &get_scope() const { return m_scope; }
    [[nodiscard]] Args get_args() const { return m_args; }

};


#endif //FORTEST_FIXTURE_HPP