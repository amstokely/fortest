!> @brief Tests for assert routines using fixtures.
!>
!> @details
!> This module defines a reusable fixture type (`assert_fixture_t`)
!> containing integers, reals, doubles, and strings. The fixture is
!> initialized in `setup_assert_fixture` before each test and can be
!> cleaned up in `teardown_assert_fixture`.
!>
!> Each test intentionally **mutates the fixture values** at the end
!> (e.g., setting them to `100` or `"a"`) to validate that the
!> framework's fixture mechanism resets state between test runs.
!> Without proper fixture reset, subsequent tests would fail due to
!> polluted state.
!>
!> The test suite includes:
!> - Equality checks for integers, reals, doubles, and strings
!> - Inequality checks for the same types
!> - Logical `assert_true` and `assert_false` tests
module test_assert_test_fixture_mod
   use iso_c_binding, only: c_ptr, c_f_pointer, c_loc
   use fortest_assert, only: assert_equal, assert_not_equal, assert_true, assert_false
   implicit none

   !> @brief Fixture holding integer, real, double, and character test values.
   type :: assert_fixture_t
      integer            :: int_a, int_b, int_c
      real               :: real_a, real_b, real_c
      double precision   :: double_a, double_b, double_c
      character(len = 20):: str_a, str_b, str_c
   end type assert_fixture_t

contains

   !> @brief Setup routine to initialize fixture values.
   subroutine setup_assert_fixture(args)
      type(c_ptr), value :: args
      type(assert_fixture_t), pointer :: fixture

      call c_f_pointer(args, fixture)

      fixture%int_a = 2
      fixture%int_b = 3
      fixture%int_c = 5

      fixture%real_a = 1.5
      fixture%real_b = 2.5
      fixture%real_c = 3.75

      fixture%double_a = 1.5d0
      fixture%double_b = 2.5d0
      fixture%double_c = 3.75d0

      fixture%str_a = "Hello,"
      fixture%str_b = "World!"
      fixture%str_c = "Hello, World!"
   end subroutine setup_assert_fixture

   !> @brief Teardown routine for fixture (no-op).
   subroutine teardown_assert_fixture(args)
      type(c_ptr), value :: args
   end subroutine teardown_assert_fixture

   !> @test Verify integer addition equality using fixture values.
   subroutine test_assert_equal_int(t_ptr, ts_ptr, s_ptr)
      use iso_c_binding, only: c_ptr, c_f_pointer
      type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
      type(assert_fixture_t), pointer :: t
      call c_f_pointer(t_ptr, t)
      call assert_equal(t%int_a + t%int_b, t%int_c)
      ! Mutate fixture to test isolation.
      t%int_a = 100; t%int_b = 100; t%int_c = 100
   end subroutine test_assert_equal_int

   !> @test Verify real multiplication equality using fixture values.
   subroutine test_assert_equal_float(t_ptr, ts_ptr, s_ptr)
      use iso_c_binding, only: c_ptr, c_f_pointer
      type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
      type(assert_fixture_t), pointer :: t
      call c_f_pointer(t_ptr, t)
      call assert_equal(t%real_a * t%real_b, t%real_c)
      t%real_a = 100.0; t%real_b = 100.0; t%real_c = 100.0
   end subroutine test_assert_equal_float

   !> @test Verify double multiplication equality using fixture values.
   subroutine test_assert_equal_double(t_ptr, ts_ptr, s_ptr)
      use iso_c_binding, only: c_ptr, c_f_pointer
      type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
      type(assert_fixture_t), pointer :: t
      call c_f_pointer(t_ptr, t)
      call assert_equal(t%double_a * t%double_b, t%double_c)
      t%double_a = 100.0d0; t%double_b = 100.0d0; t%double_c = 100.0d0
   end subroutine test_assert_equal_double

   !> @test Verify string concatenation equality using fixture values.
   subroutine test_assert_equal_string(t_ptr, ts_ptr, s_ptr)
      use iso_c_binding, only: c_ptr, c_f_pointer
      type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
      type(assert_fixture_t), pointer :: t
      call c_f_pointer(t_ptr, t)
      call assert_equal(trim(t%str_a)//" "//trim(t%str_b), trim(t%str_c))
      t%str_a = "a"; t%str_b = "a"; t%str_c = "a"
   end subroutine test_assert_equal_string

   !> @test Verify integer inequality using fixture values.
   subroutine test_assert_not_equal_int(t_ptr, ts_ptr, s_ptr)
      use iso_c_binding, only: c_ptr, c_f_pointer
      type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
      type(assert_fixture_t), pointer :: t
      call c_f_pointer(t_ptr, t)
      call assert_not_equal(t%int_a, t%int_c)
      t%int_a = 100; t%int_b = 100; t%int_c = 100
   end subroutine test_assert_not_equal_int

   !> @test Verify real inequality using fixture values.
   subroutine test_assert_not_equal_float(t_ptr, ts_ptr, s_ptr)
      use iso_c_binding, only: c_ptr, c_f_pointer
      type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
      type(assert_fixture_t), pointer :: t
      call c_f_pointer(t_ptr, t)
      call assert_not_equal(t%real_a, t%real_c)
      t%real_a = 100.0; t%real_b = 100.0; t%real_c = 100.0
   end subroutine test_assert_not_equal_float

   !> @test Verify double inequality using fixture values.
   subroutine test_assert_not_equal_double(t_ptr, ts_ptr, s_ptr)
      use iso_c_binding, only: c_ptr, c_f_pointer
      type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
      type(assert_fixture_t), pointer :: t
      call c_f_pointer(t_ptr, t)
      call assert_not_equal(t%double_b, t%double_c)
      t%double_a = 100.0d0; t%double_b = 100.0d0; t%double_c = 100.0d0
   end subroutine test_assert_not_equal_double

   !> @test Verify string inequality using fixture values.
   subroutine test_assert_not_equal_string(t_ptr, ts_ptr, s_ptr)
      use iso_c_binding, only: c_ptr, c_f_pointer
      type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
      type(assert_fixture_t), pointer :: t
      call c_f_pointer(t_ptr, t)
      call assert_not_equal(trim(t%str_a), trim(t%str_c))
      t%str_a = "a"; t%str_b = "a"; t%str_c = "a"
   end subroutine test_assert_not_equal_string

   !> @test Verify a true condition (int_a < int_c).
   subroutine test_assert_true(t_ptr, ts_ptr, s_ptr)
      use iso_c_binding, only: c_ptr, c_f_pointer
      type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
      type(assert_fixture_t), pointer :: t
      call c_f_pointer(t_ptr, t)
      call assert_true(t%int_a < t%int_c)
   end subroutine test_assert_true

   !> @test Verify a false condition (int_a > int_c).
   subroutine test_assert_false(t_ptr, ts_ptr, s_ptr)
      use iso_c_binding, only: c_ptr, c_f_pointer
      type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
      type(assert_fixture_t), pointer :: t
      call c_f_pointer(t_ptr, t)
      call assert_false(t%int_a > t%int_c)
   end subroutine test_assert_false

end module test_assert_test_fixture_mod


!> @brief Driver program to run all fixture-based assert tests.
!>
!> Registers the test suite, attaches the fixture setup/teardown, and
!> executes all equality, inequality, and logical tests.
program test_assert_with_test_fixture
   use iso_c_binding, only: c_ptr, c_loc
   use fortest_test_session, only: test_session_t, test_suite_t
   use test_assert_test_fixture_mod, only: assert_fixture_t, &
         setup_assert_fixture, teardown_assert_fixture, &
         test_assert_equal_int, test_assert_equal_float, &
         test_assert_equal_double, test_assert_equal_string, &
         test_assert_not_equal_int, test_assert_not_equal_float, &
         test_assert_not_equal_double, test_assert_not_equal_string, &
         test_assert_true, test_assert_false
   implicit none

   type(test_session_t) :: test_session
   type(assert_fixture_t), target :: assert_fixture
   type(c_ptr) :: assert_fixture_ptr

   assert_fixture_ptr = c_loc(assert_fixture)

   call test_session%register_test_suite(test_suite_name = "test_suite")
   call test_session%register_fixture(&
         setup = setup_assert_fixture, &
         teardown = teardown_assert_fixture, &
         args = assert_fixture_ptr, &
         scope = "test", &
         test_suite_name = "test_suite" )

   call test_session%register_test(&
         test_suite_name = "test_suite", &
         test_name = "test_assert_equal_int", &
         test = test_assert_equal_int)

   call test_session%register_test(&
         test_suite_name = "test_suite", &
         test_name = "test_assert_equal_float", &
         test = test_assert_equal_float)

   call test_session%register_test(&
         test_suite_name = "test_suite", &
         test_name = "test_assert_equal_double", &
         test = test_assert_equal_double)

   call test_session%register_test(&
         test_suite_name = "test_suite", &
         test_name = "test_assert_equal_string", &
         test = test_assert_equal_string)

   call test_session%register_test(&
         test_suite_name = "test_suite", &
         test_name = "test_assert_not_equal_int", &
         test = test_assert_not_equal_int)

   call test_session%register_test(&
         test_suite_name = "test_suite", &
         test_name = "test_assert_not_equal_float", &
         test = test_assert_not_equal_float)

   call test_session%register_test(&
         test_suite_name = "test_suite", &
         test_name = "test_assert_not_equal_double", &
         test = test_assert_not_equal_double)

   call test_session%register_test(&
         test_suite_name = "test_suite", &
         test_name = "test_assert_not_equal_string", &
         test = test_assert_not_equal_string)

   call test_session%register_test(&
         test_suite_name = "test_suite", &
         test_name = "test_assert_true", &
         test = test_assert_true)

   call test_session%register_test(&
         test_suite_name = "test_suite", &
         test_name = "test_assert_false", &
         test = test_assert_false)

   call test_session%run()
   call test_session%finalize()
end program test_assert_with_test_fixture
