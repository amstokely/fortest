add_executable(test_assert_fortran assert.test.f90)
set_target_properties(test_assert_fortran PROPERTIES
        LINKER_LANGUAGE Fortran
)
target_link_libraries(test_assert_fortran PUBLIC fortest)
add_test(NAME test_assert_fortran COMMAND test_assert_fortran)
set_tests_properties(
        test_assert_fortran
        PROPERTIES
        SUBTESTS TRUE)