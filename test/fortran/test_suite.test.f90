module test_test_suite_mod
    use iso_c_binding, only: c_ptr
    implicit none
contains
    subroutine test_assesr_equal_int(t_ptr, ts_ptr, s_ptr)
        use assert_mod
        implicit none
        type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
        integer :: a, b, result
        a = 2
        b = 3
        result = a + b
        call assert_equal(result, 5)
    end subroutine

    subroutine test_assert_equal_float(t_ptr, ts_ptr, s_ptr)
        use assert_mod
        implicit none
        type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
        real :: x, y, result
        x = 1.5
        y = 2.5
        result = x * y
        call assert_equal(result, 3.75)
    end subroutine

    subroutine test_assert_equal_double(t_ptr, ts_ptr, s_ptr)
        use assert_mod
        implicit none
        type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
        double precision :: x, y, result
        x = 1.5d0
        y = 2.5d0
        result = x * y
        call assert_equal(result, 3.75d0)
    end subroutine

    subroutine test_assert_equal_string(t_ptr, ts_ptr, s_ptr)
        use assert_mod
        implicit none
        type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
        character(len=20) :: str1, str2
        str1 = "Hello"
        str2 = "World"
        call assert_equal(trim(str1)//" "//trim(str2), "Hello World")
    end subroutine

end module

program main
    use test_test_suite_mod
    use test_suite_mod
    implicit none
    type(test_suite_t) :: test_suite

    test_suite = test_suite_t("test_suite")
    call test_suite%register_test("test_assert_equal_int", test_assesr_equal_int)
    call test_suite%register_test("test_assert_equal_float", test_assert_equal_float)
    call test_suite%register_test("test_assert_equal_double", test_assert_equal_double)
    call test_suite%register_test("test_assert_equal_string", test_assert_equal_string)
    call test_suite%run()

end program main