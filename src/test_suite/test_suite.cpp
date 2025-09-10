#include "test_suite.hpp"

Fixture::Fixture(
    FixtureFunction setup, FixtureFunction teardown, Args args,
    std::string scope
): m_setup(std::move(setup)), m_teardown(std::move(teardown)),
   m_args(args) {
    m_scope = std::move(scope);
}

void Fixture::setup() const {
    if (m_setup) m_setup(m_args);
}

void Fixture::teardown() const {
    if (m_teardown) m_teardown(m_args);
}

std::string Fixture::get_scope() const {
    return m_scope;
}

Fixture::Fixture(const Fixture &) = default;

Fixture &Fixture::operator=(const Fixture &) = default;

TestSuite::TestSuite(
    std::string n, Assert<ConsoleLogger> &assert
): name(std::move(n)), m_assert(assert) {
}

void TestSuite::add_test(
    const std::string &testName, TestFunction func
) {
    tests.emplace(testName, std::move(func));
}
