add_executable(test_assert_without_fixture_fortran assert_without_fixture.test.f90)
set_target_properties(
        test_assert_without_fixture_fortran PROPERTIES
        LINKER_LANGUAGE Fortran
)
target_link_libraries(test_assert_without_fixture_fortran PRIVATE fortest)
add_test(NAME test_assert_without_fixture_fortran COMMAND test_assert_without_fixture_fortran)

add_executable(test_assert_with_test_fixture_fortran assert_with_test_fixture.test.f90)
set_target_properties(
        test_assert_with_test_fixture_fortran PROPERTIES
        LINKER_LANGUAGE Fortran
)
target_link_libraries(test_assert_with_test_fixture_fortran PRIVATE fortest)
add_test(NAME test_assert_with_test_fixture_fortran COMMAND test_assert_with_test_fixture_fortran)

add_executable(test_session_finalize_fortran test_session_finalize.test.f90)
set_target_properties(
        test_session_finalize_fortran PROPERTIES
        LINKER_LANGUAGE Fortran
)
target_link_libraries(test_session_finalize_fortran PRIVATE fortest)
add_test(NAME test_session_finalize_fortran COMMAND test_session_finalize_fortran)


