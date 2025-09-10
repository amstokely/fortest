#ifndef TEST_SUITE_REGISTRY_HPP
#define TEST_SUITE_REGISTRY_HPP

#include <string>
#include <unordered_map>
#include "test_suite.hpp"

class TestSuiteRegistry {
public:
    static std::unordered_map<std::string, TestSuite>& instance();

    // delete copy/move
    TestSuiteRegistry(const TestSuiteRegistry&) = delete;
    TestSuiteRegistry& operator=(const TestSuiteRegistry&) = delete;

private:
    TestSuiteRegistry();
};

#endif // TEST_SUITE_REGISTRY_HPP
