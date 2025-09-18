module test_assert_no_fixture_mod
    use iso_c_binding, only : c_ptr
    use fortest_assert, only : assert_equal, assert_not_equal, assert_true, assert_false
    implicit none
contains
    !***********************************************************************
    !  Subroutine test_assert_equal_int
    !
    !> \brief   Tests assert_equal with integer values.
    !>
    !> \details Verifies that the sum of two integers (2 + 3) equals 5.
    !>
    !> \param t_ptr   C pointer to the test fixture derived type.
    !> \param ts_ptr  C pointer to the test suite fixture derived type.
    !> \param s_ptr   C pointer to the session fixture derived type.
    !>                These arguments are unused in this test and only
    !>                satisfy the test_proc procedure interface.
    !***********************************************************************
    subroutine test_assert_equal_int(t_ptr, ts_ptr, s_ptr)
        type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
        integer :: a, b, result
        a = 2
        b = 3
        result = a + b
        call assert_equal(result, 5)
    end subroutine

    !***********************************************************************
    !  Subroutine test_assert_equal_float
    !
    !> \brief   Tests assert_equal with real (single precision) values.
    !>
    !> \details Verifies that multiplying 1.5 by 2.5 yields 3.75.
    !***********************************************************************
    subroutine test_assert_equal_float(t_ptr, ts_ptr, s_ptr)
        type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
        real :: x, y, result
        x = 1.5
        y = 2.5
        result = x * y
        call assert_equal(result, 3.75)
    end subroutine

    !***********************************************************************
    !  Subroutine test_assert_equal_double
    !
    !> \brief   Tests assert_equal with double precision values.
    !>
    !> \details Verifies that multiplying 1.5d0 by 2.5d0 yields 3.75d0.
    !***********************************************************************
    subroutine test_assert_equal_double(t_ptr, ts_ptr, s_ptr)
        type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
        double precision :: x, y, result
        x = 1.5d0
        y = 2.5d0
        result = x * y
        call assert_equal(result, 3.75d0)
    end subroutine

    !***********************************************************************
    !  Subroutine test_assert_equal_string
    !
    !> \brief   Tests assert_equal with character strings.
    !>
    !> \details Verifies that concatenating "Hello" and "World" yields
    !>          "Hello World".
    !***********************************************************************
    subroutine test_assert_equal_string(t_ptr, ts_ptr, s_ptr)
        type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
        character(len = 20) :: str1, str2
        str1 = "Hello"
        str2 = "World"
        call assert_equal(trim(str1) // " " // trim(str2), "Hello World")
    end subroutine

    !***********************************************************************
    !  Subroutine test_assert_not_equal_int
    !
    !> \brief   Tests assert_not_equal with integer values.
    !>
    !> \details Verifies that the sum of two integers (2 + 3) is not equal
    !>          to 4.
    !***********************************************************************
    subroutine test_assert_not_equal_int(t_ptr, ts_ptr, s_ptr)
        type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
        integer :: a, b, result
        a = 2
        b = 3
        result = a + b
        call assert_not_equal(result, 4)
    end subroutine

    !***********************************************************************
    !  Subroutine test_assert_not_equal_float
    !
    !> \brief   Tests assert_not_equal with real (single precision) values.
    !>
    !> \details Verifies that multiplying 1.5 by 2.5 yields 3.75,
    !>          which is not equal to 4.0.
    !***********************************************************************
    subroutine test_assert_not_equal_float(t_ptr, ts_ptr, s_ptr)
        type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
        real :: x, y, result
        x = 1.5
        y = 2.5
        result = x * y
        call assert_not_equal(result, 4.0)
    end subroutine

    !***********************************************************************
    !  Subroutine test_assert_not_equal_double
    !
    !> \brief   Tests assert_not_equal with double precision values.
    !>
    !> \details Verifies that multiplying 1.5d0 by 2.5d0 yields 3.75d0,
    !>          which is not equal to 4.0d0.
    !***********************************************************************
    subroutine test_assert_not_equal_double(t_ptr, ts_ptr, s_ptr)
        type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
        double precision :: x, y, result
        x = 1.5d0
        y = 2.5d0
        result = x * y
        call assert_not_equal(result, 4.0d0)
    end subroutine

    !***********************************************************************
    !  Subroutine test_assert_not_equal_string
    !
    !> \brief   Tests assert_not_equal with character strings.
    !>
    !> \details Verifies that concatenating "Hello" and "World" yields
    !>          "Hello World", which is not equal to "Goodbye World".
    !***********************************************************************
    subroutine test_assert_not_equal_string(t_ptr, ts_ptr, s_ptr)
        type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
        character(len = 20) :: str1, str2
        str1 = "Hello"
        str2 = "World"
        call assert_not_equal(trim(str1) // " " // trim(str2), "Goodbye World")
    end subroutine

    !***********************************************************************
    !  Subroutine test_assert_true
    !
    !> \brief   Tests assert_true with a logical condition.
    !>
    !> \details Verifies that the condition (2 + 2 == 4) evaluates to .true.
    !***********************************************************************
    subroutine test_assert_true(t_ptr, ts_ptr, s_ptr)
        type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
        logical :: condition
        condition = (2 + 2 == 4)
        call assert_true(condition)
    end subroutine

    !***********************************************************************
    !  Subroutine test_assert_false
    !
    !> \brief   Tests assert_true with the negation of a logical condition.
    !>
    !> \details Verifies that the condition (2 + 2 == 5) evaluates to .false.,
    !>          and that its negation evaluates to .true.
    !***********************************************************************
    subroutine test_assert_false(t_ptr, ts_ptr, s_ptr)
        type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
        logical :: condition
        condition = (2 + 2 == 5)
        call assert_false(condition)
    end subroutine

end module test_assert_no_fixture_mod


program test_assert_no_fixture
    use test_assert_no_fixture_mod
    use fortest_test_session
    implicit none
    type(test_suite_t) :: test_suite
    type (test_session_t) :: test_session

    call test_session%register_test_suite("test_suite")
    call test_session%register_test("test_suite", "test_assert_equal_int", test_assert_equal_int)
    call test_session%register_test("test_suite", "test_assert_equal_float", test_assert_equal_float)
    call test_session%register_test("test_suite", "test_assert_equal_double", test_assert_equal_double)
    call test_session%register_test("test_suite", "test_assert_equal_string", test_assert_equal_string)
    call test_session%register_test("test_suite", "test_assert_not_equal_int", test_assert_not_equal_int)
    call test_session%register_test("test_suite", "test_assert_not_equal_float", test_assert_not_equal_float)
    call test_session%register_test("test_suite", "test_assert_not_equal_double", test_assert_not_equal_double)
    call test_session%register_test("test_suite", "test_assert_not_equal_string", test_assert_not_equal_string)
    call test_session%register_test("test_suite", "test_assert_true", test_assert_true)
    call test_session%register_test("test_suite", "test_assert_false", test_assert_false)
    call test_session%run()
    call test_session%finalize()
end program test_assert_no_fixture
