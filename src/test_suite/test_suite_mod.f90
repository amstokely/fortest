module test_suite_mod
   use iso_c_binding
   implicit none

   public :: test_suite_t

   ! Abstract interfaces
   abstract interface
      subroutine test_proc(test_fixture_args, suite_fixture_args, session_fixture_args)
         import :: c_ptr
         type(c_ptr), value :: test_fixture_args
         type(c_ptr), value :: suite_fixture_args
         type(c_ptr), value :: session_fixture_args
      end subroutine test_proc

      subroutine fixture_proc(args)
         import :: c_ptr
         type(c_ptr), value :: args
      end subroutine fixture_proc
   end interface

   interface test_suite_t
      module procedure register_test_suite
   end interface test_suite_t

   type test_suite_t
      character(len = :), allocatable :: name
   contains
      procedure :: register_fixture => register_fixture
      procedure :: register_test => register_test
      procedure :: run => run
   end type test_suite_t

contains

   function register_test_suite(name) result(test_suite)
      use iso_c_binding
      use f_c_string_t_mod, only: f_c_string_t
      character(len = *),  intent(in) :: name
      type(test_suite_t) :: test_suite
      type(f_c_string_t) :: f_c_string_name
      integer :: status
      interface
         subroutine c_register_test_suite(name) bind(C, name = "c_register_test_suite")
            import :: c_ptr, c_funptr
            type(c_ptr), value :: name
         end subroutine c_register_test_suite
      end interface
      test_suite%name = name
      f_c_string_name = f_c_string_t(name)
      status = f_c_string_name%to_c()
      call c_register_test_suite(&
            f_c_string_name%get_c_string())
   end function register_test_suite

   subroutine register_fixture(this, name, setup, teardown, args, scope)
      use iso_c_binding
      use f_c_string_t_mod, only: f_c_string_t
      class(test_suite_t), intent(in) :: this
      character(len = *), intent(in) :: name
      procedure(fixture_proc) :: setup
      procedure(fixture_proc) :: teardown
      type(c_ptr), value :: args
      character(len = *), intent(in) :: scope
      type(f_c_string_t) :: f_c_string_test_suite_name
      type(f_c_string_t) :: f_c_string_fixture_name
      type(f_c_string_t) :: f_c_string_scope
      integer :: status
      interface
         subroutine c_register_fixture(test_suite_name, fixture_name, &
               setup, teardown, args, scope) bind(C, name = "c_register_fixture")
            import :: c_ptr, c_funptr
            type(c_ptr), value :: test_suite_name
            type(c_ptr), value :: fixture_name
            type(c_funptr), value :: setup
            type(c_funptr), value :: teardown
            type(c_ptr), value :: args
            type(c_ptr), value :: scope
         end subroutine c_register_fixture
      end interface

      f_c_string_test_suite_name = f_c_string_t(this%name)
      status = f_c_string_test_suite_name%to_c()
      f_c_string_fixture_name = f_c_string_t(name)
      status = f_c_string_fixture_name%to_c()
      f_c_string_scope = f_c_string_t(scope)
      status = f_c_string_scope%to_c()
      call c_register_fixture(&
            f_c_string_test_suite_name%get_c_string(), &
            f_c_string_fixture_name%get_c_string(), &
            c_funloc(setup), &
            c_funloc(teardown), &
            args&
            , f_c_string_scope%get_c_string())
   end subroutine register_fixture

   subroutine register_test(this, name, test)
      use iso_c_binding
      use f_c_string_t_mod, only: f_c_string_t
      class(test_suite_t), intent(in) :: this
      character(len = *), intent(in) :: name
      procedure(test_proc) :: test
      type(f_c_string_t) :: f_c_string_test_suite_name
      type(f_c_string_t) :: f_c_string_test_name
      integer :: status
      interface
         subroutine c_register_test(test_suite_name, test_name, test) bind(C, name = "c_register_test")
            import :: c_ptr, c_funptr
            type(c_ptr), value :: test_suite_name
            type(c_ptr), value :: test_name
            type(c_funptr), value :: test
         end subroutine c_register_test
      end interface

      f_c_string_test_suite_name = f_c_string_t(this%name)
      status = f_c_string_test_suite_name%to_c()
      f_c_string_test_name = f_c_string_t(name)
      status = f_c_string_test_name%to_c()
      call c_register_test(&
            f_c_string_test_suite_name%get_c_string(), &
            f_c_string_test_name%get_c_string(), &
            c_funloc(test))
   end subroutine register_test

   subroutine run(this)
      use iso_c_binding
      use f_c_string_t_mod, only: f_c_string_t
      class(test_suite_t), intent(in) :: this
      type(f_c_string_t) :: f_c_string_name
      integer :: status
      interface
         subroutine c_run_test_suite(name) bind(C, name = "c_run_test_suite")
            import :: c_ptr
            type(c_ptr), value :: name
         end subroutine c_run_test_suite
      end interface

      f_c_string_name = f_c_string_t(this%name)
      status = f_c_string_name%to_c()
      call c_run_test_suite(f_c_string_name%get_c_string())
   end subroutine run

end module test_suite_mod
