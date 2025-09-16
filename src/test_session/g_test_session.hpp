#ifndef FORTEST_GLOBAL_TEST_SESSION_HPP
#define FORTEST_GLOBAL_TEST_SESSION_HPP

#include "test_session.hpp"
#include "assert.hpp"
#include "g_assert.hpp"

class GlobalConsoleTestSession {
public:
    static TestSession<ConsoleLogger>& instance() {
        static TestSession<ConsoleLogger> session(*GlobalConsoleAssert::instance());
        return session;
    }

private:
    GlobalConsoleTestSession() = default;
    ~GlobalConsoleTestSession() = default;

    GlobalConsoleTestSession(const GlobalConsoleTestSession&) = delete;
    GlobalConsoleTestSession& operator=(const GlobalConsoleTestSession&) = delete;
};

#endif // FORTEST_GLOBAL_TEST_SESSION_HPP
