#include "fixture.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <string>

/**
 * @brief Test fixture class for Fixture tests.
 * Provides shared state across individual test cases.
 */
class FixtureTest : public ::testing::Test {
protected:
    bool setup_called{};
    bool teardown_called{};
    std::string scope{"test"};

    void SetUp() override {
        setup_called = false;
        teardown_called = false;
    }
};

/**
 * @test Verify that setup() invokes the setup function.
 */
TEST_F(FixtureTest, setup_invokes_function) {
    Fixture f(
        [&](void*) { setup_called = true; },
        nullptr,
        nullptr,
        scope
    );

    f.setup();
    EXPECT_TRUE(setup_called);
}

/**
 * @test Verify that teardown() invokes the teardown function.
 */
TEST_F(FixtureTest, teardown_invokes_function) {
    Fixture f(
        nullptr,
        [&](void*) { teardown_called = true; },
        nullptr,
        scope
    );

    f.teardown();
    EXPECT_TRUE(teardown_called);
}

/**
 * @test Verify that both setup() and teardown() are called.
 */
TEST_F(FixtureTest, setup_and_teardown_both_invoked) {
    Fixture f(
        [&](void*) { setup_called = true; },
        [&](void*) { teardown_called = true; },
        nullptr,
        scope
    );

    f.setup();
    f.teardown();
    EXPECT_TRUE(setup_called);
    EXPECT_TRUE(teardown_called);
}

/**
 * @test Verify that arguments are passed to setup and teardown.
 */
TEST_F(FixtureTest, passes_arguments_to_functions) {
    int arg_value = 0;

    auto setup = [&](void* a) { *static_cast<int*>(a) = 10; };
    auto teardown = [&](void* a) { *static_cast<int*>(a) = 20; };

    Fixture f(setup, teardown, &arg_value, scope);

    f.setup();
    EXPECT_EQ(arg_value, 10);

    f.teardown();
    EXPECT_EQ(arg_value, 20);
}

/**
 * @test Verify that scope string is stored and retrievable.
 */
TEST_F(FixtureTest, stores_and_returns_scope) {
    Fixture f(nullptr, nullptr, nullptr, "custom_scope");
    EXPECT_EQ(f.get_scope(), "custom_scope");
}

/**
 * @test Verify that args pointer is stored and retrievable.
 */
TEST_F(FixtureTest, stores_and_returns_args) {
    int arg = 42;
    Fixture f(nullptr, nullptr, &arg, scope);

    EXPECT_EQ(f.get_args(), &arg);
}

/**
 * @test Verify that setup and teardown can be null without crashing.
 */
TEST_F(FixtureTest, null_setup_and_teardown_do_nothing) {
    Fixture f(nullptr, nullptr, nullptr, scope);

    EXPECT_NO_THROW({
        f.setup();
        f.teardown();
    });
}
