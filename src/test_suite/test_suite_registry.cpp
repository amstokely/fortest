#include "test_suite_registry.hpp"

 std::unordered_map<std::string, TestSuite> & TestSuiteRegistry::
instance() {
    static std::unordered_map<std::string, TestSuite> registry;
    return registry;
}

 TestSuiteRegistry::TestSuiteRegistry() = default;