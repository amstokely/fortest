add_executable(test_assert_no_fixture_fortran assert_no_fixture.test.f90)
set_target_properties(
        test_assert_no_fixture_fortran PROPERTIES
        LINKER_LANGUAGE Fortran
)
target_link_libraries(test_assert_no_fixture_fortran PRIVATE fortest)
add_test(NAME test_assert_no_fixture_fortran COMMAND test_assert_no_fixture_fortran)

add_executable(test_assert_test_fixture_fortran assert_test_fixture.test.f90)
set_target_properties(
        test_assert_test_fixture_fortran PROPERTIES
        LINKER_LANGUAGE Fortran
)
target_link_libraries(test_assert_test_fixture_fortran PRIVATE fortest)
add_test(NAME test_assert_test_fixture_fortran COMMAND test_assert_test_fixture_fortran)


