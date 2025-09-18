add_executable(test_math_ops math_ops.test.f90 math_ops.fixture.f90 math_ops_mod.f90)
set_target_properties(test_math_ops PROPERTIES
        LINKER_LANGUAGE Fortran
)
target_link_libraries(test_math_ops PRIVATE fortest)