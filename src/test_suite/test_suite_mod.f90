module fortest_test_suite
   use iso_c_binding
   use procedure_interfaces_mod, only: test_proc, fixture_proc
   implicit none

   public :: test_suite_t

   !> @brief Represents a test suite in Fortest.
   !!
   !! A test suite groups related tests under a common name.
   !! It allows registering fixtures with a specific scope,
   !! querying suite status, and finalizing (exiting with
   !! the suite result code).
   type test_suite_t
      !> Name of the test suite.
      character(len = :), allocatable :: name
   contains
      !> Register a fixture for this suite.
      procedure :: register_fixture => register_fixture
      !> Retrieve the current status code of the suite.
      procedure :: get_status => get_status
      !> Finalize the suite and exit with its status code.
      procedure, public :: finalize
   end type test_suite_t

contains

   !> @brief Register a fixture with setup/teardown for this suite.
   !!
   !! @param[in] this    The test suite.
   !! @param[in] setup   Setup procedure pointer.
   !! @param[in] teardown Teardown procedure pointer.
   !! @param[in] args    Opaque pointer to fixture arguments.
   !! @param[in] scope   Scope string: "test", "suite", or "session".
   subroutine register_fixture(this, setup, teardown, args, scope)
      use iso_c_binding
      use f_c_string_t_mod, only: f_c_string_t
      class(test_suite_t), intent(in) :: this
      procedure(fixture_proc) :: setup
      procedure(fixture_proc) :: teardown
      type(c_ptr), value :: args
      character(len = *), intent(in) :: scope
      type(f_c_string_t) :: f_c_string_test_suite_name
      type(f_c_string_t) :: f_c_string_scope
      integer :: status
      interface
         subroutine c_register_fixture(test_suite_name, &
               setup, teardown, args, scope) bind(C, name = "c_register_fixture")
            import :: c_ptr, c_funptr
            type(c_ptr), value :: test_suite_name
            type(c_funptr), value :: setup
            type(c_funptr), value :: teardown
            type(c_ptr), value :: args
            type(c_ptr), value :: scope
         end subroutine c_register_fixture
      end interface

      f_c_string_test_suite_name = f_c_string_t(this%name)
      status = f_c_string_test_suite_name%to_c()
      f_c_string_scope = f_c_string_t(scope)
      status = f_c_string_scope%to_c()
      call c_register_fixture(&
            f_c_string_test_suite_name%get_c_string(), &
            c_funloc(setup), &
            c_funloc(teardown), &
            args, &
            f_c_string_scope%get_c_string())
   end subroutine register_fixture

   !> @brief Get the status code of this test suite.
   !!
   !! Calls into the C backend to fetch the most recent
   !! pass/fail status for the suite.
   !!
   !! @param[in] this The test suite.
   !! @return Status code (0 = success, nonzero = failure).
   function get_status(this) result(status)
      use iso_c_binding
      use f_c_string_t_mod, only: f_c_string_t
      implicit none
      class(test_suite_t), intent(in) :: this
      integer :: status, ierr
      type (f_c_string_t) :: f_c_string_name
      interface
         function c_get_test_suite_status(name) bind(C, name = "c_get_test_suite_status")
            import :: c_ptr
            type(c_ptr), value :: name
            integer :: c_get_test_suite_status
         end function c_get_test_suite_status
      end interface
      f_c_string_name = f_c_string_t(this%name)
      ierr = f_c_string_name%to_c()
      status = c_get_test_suite_status(f_c_string_name%get_c_string())
   end function get_status

   !> @brief Finalize this test suite and exit with its status code.
   !!
   !! This will terminate the program using the exit code
   !! returned by get_status(). A nonzero value indicates failure.
   subroutine finalize(this)
      implicit none
      class(test_suite_t), intent(in) :: this
      integer :: status

      status = this%get_status()
      call exit(status)
   end subroutine finalize

end module fortest_test_suite
