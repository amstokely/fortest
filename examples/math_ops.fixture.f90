module math_ops_fixture
   use iso_c_binding, only: c_ptr, c_f_pointer
   implicit none
   public :: suite_fixture_t, test_fixture_t
   public :: setup_suite_fixture, teardown_suite_fixture
   public :: setup_test_fixture, teardown_test_fixture

   ! Suite-level fixture: constants for all tests
   type :: suite_fixture_t
      real, allocatable :: A(:, :), B(:, :), I(:, :), Z(:, :)
      real, allocatable :: U(:), V(:), W_expected(:)
   end type suite_fixture_t

   ! Test-level fixture: scratch data, reset before each test
   type :: test_fixture_t
      real, allocatable :: C(:, :)
      real, allocatable :: W(:)
   end type test_fixture_t

contains

   subroutine setup_test_fixture(args)
      type(c_ptr), value :: args
      type(test_fixture_t), pointer :: fix
      call c_f_pointer(args, fix)

      allocate(fix%C(2, 2))
      fix%C = 0.0

      allocate(fix%W(3))
      fix%W = 0.0
   end subroutine setup_test_fixture

   subroutine teardown_test_fixture(args)
      type(c_ptr), value :: args
      type(test_fixture_t), pointer :: fix
      call c_f_pointer(args, fix)

      if (allocated(fix%C)) deallocate(fix%C)
      if (allocated(fix%W)) deallocate(fix%W)
   end subroutine teardown_test_fixture

   subroutine setup_suite_fixture(args)
      type(c_ptr), value :: args
      type(suite_fixture_t), pointer :: fix
      call c_f_pointer(args, fix)

      ! Allocate matrices
      allocate(fix%A(2, 2), fix%B(2, 2), fix%I(2, 2), fix%Z(2, 2))
      fix%A = reshape([1.0, 2.0, 3.0, 4.0], [2, 2])
      fix%B = reshape([5.0, 6.0, 7.0, 8.0], [2, 2])

      fix%I = 0.0
      fix%I(1, 1) = 1.0
      fix%I(2, 2) = 1.0
      fix%Z = 0.0

      ! Allocate vectors
      allocate(fix%U(3), fix%V(3), fix%W_expected(3))
      fix%U = [1.0, 2.0, 3.0]
      fix%V = [4.0, 5.0, 6.0]
      fix%W_expected = [-3.0, 6.0, -3.0] ! U x V
   end subroutine setup_suite_fixture

   subroutine teardown_suite_fixture(args)
      type(c_ptr), value :: args
      type(suite_fixture_t), pointer :: fix
      call c_f_pointer(args, fix)

      if (allocated(fix%A)) deallocate(fix%A)
      if (allocated(fix%B)) deallocate(fix%B)
      if (allocated(fix%I)) deallocate(fix%I)
      if (allocated(fix%Z)) deallocate(fix%Z)

      if (allocated(fix%U)) deallocate(fix%U)
      if (allocated(fix%V)) deallocate(fix%V)
      if (allocated(fix%W_expected)) deallocate(fix%W_expected)
   end subroutine teardown_suite_fixture

end module math_ops_fixture
