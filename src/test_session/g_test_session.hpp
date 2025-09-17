#ifndef FORTEST_GLOBAL_TEST_SESSION_HPP
#define FORTEST_GLOBAL_TEST_SESSION_HPP

#include <g_logging.hpp>

#include "test_session.hpp"
#include "assert.hpp"
#include "g_assert.hpp"

class GlobalTestSession {
public:
    static TestSession<Logger>& instance() {
        static TestSession session(*GlobalAssert::instance());
        return session;
    }

private:
    GlobalTestSession() = default;
    ~GlobalTestSession() = default;

    GlobalTestSession(const GlobalTestSession&) = delete;
    GlobalTestSession& operator=(const GlobalTestSession&) = delete;
};

#endif // FORTEST_GLOBAL_TEST_SESSION_HPP
