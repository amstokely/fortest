module test_finalize_test_session_mod
   use iso_c_binding, only: c_ptr
   use fortest_assert, only: assert_equal, assert_not_equal, assert_true, assert_false
   implicit none
contains

   subroutine test_add_passes(t_ptr, ts_ptr, s_ptr)
      type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
      integer :: a, b, result
      a = 2
      b = 3
      result = a + b
      call assert_equal(result, 5)
   end subroutine

   subroutine test_subtract_passes(t_ptr, ts_ptr, s_ptr)
      type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
      integer :: a, b, result
      a = 2
      b = 3
      result = b - a
      call assert_equal(result, 1)
   end subroutine

   subroutine test_add_fails(t_ptr, ts_ptr, s_ptr)
      type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
      integer :: a, b, result
      a = 2
      b = 3
      result = a + b
      call assert_equal(result, 6)  ! This will fail
   end subroutine

   subroutine test_subtract_fails(t_ptr, ts_ptr, s_ptr)
      type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
      integer :: a, b, result
      a = 2
      b = 3
      result = b - a
      call assert_equal(result, 0)  ! This will fail
   end subroutine


end module test_finalize_test_session_mod


program test_assert_no_fixture
   use test_finalize_test_session_mod, only: test_add_passes, test_subtract_passes, test_add_fails, test_subtract_fails
   use fortest_test_session
   implicit none
   type(test_suite_t) :: test_suite
   type (test_session_t) :: test_session

   call test_session%register_test_suite("test_finalize_test_session")
   call test_session%register_test("test_finalize_test_session", "test_add_passes", test_add_passes)
   call test_session%run()
   if (test_session%get_status() /= 0) then
      call exit(1)
   end if
   call test_session%register_test("test_finalize_test_session", "test_subtract_passes", test_subtract_passes)
   call test_session%run()
   if (test_session%get_status() /= 0) then
      call exit(1)
   end if
   call test_session%register_test("test_finalize_test_session", "test_add_fails", test_add_fails)
   call test_session%run()
   if (test_session%get_status() == 0) then
      call exit(1)
   end if
   call test_session%register_test("test_finalize_test_session", "test_subtract_fails", test_subtract_fails)
   call test_session%run()
   if (test_session%get_status() == 0) then
      call exit(1)
   end if

end program test_assert_no_fixture
