# --------------------
# C++ side
# --------------------
add_library(cpp_fortest SHARED
        test_suite/test_suite.cpp
        assert/assert.cpp
        assert/c_assert.cpp
        logging/logging.cpp
        test_suite/test_suite_registry.cpp
        test_suite/c_test_suite.cpp
)

target_include_directories(cpp_fortest
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/assert>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/logging>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/test_suite>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/utils>
        $<INSTALL_INTERFACE:include/fortest>)


# --------------------
# Fortran side
# --------------------
add_library(fortest SHARED
        test_suite/test_suite_mod.f90
        assert/assert_mod.f90
        utils/f_c_string_t_mod.f90
)
set_target_properties(fortest PROPERTIES
        LINKER_LANGUAGE Fortran
        Fortran_MODULE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src/mod
)
target_link_libraries(fortest
        PUBLIC cpp_fortest           # and on cpp_fortest directly
)
target_include_directories(fortest
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src/mod>
        $<INSTALL_INTERFACE:include/fortest/mod>
)

# --------------------
# Install libraries and export targets
# --------------------
install(TARGETS fortest cpp_fortest
        EXPORT FortestTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

# --------------------
# Install headers
# --------------------
install(FILES
        assert/assert.hpp
        assert/c_assert.h
        assert/g_assert.hpp
        logging/logging.hpp
        logging/g_logging.hpp
        test_suite/test_suite.hpp
        test_suite/test_suite_registry.hpp
        test_suite/c_test_suite.h
        utils/global_base.hpp
        DESTINATION include/fortest
)

# --------------------
# Install Fortran modules (.mod files)
# --------------------
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/mod/
        DESTINATION include/fortest/mod
        FILES_MATCHING PATTERN "*.mod"
)

# --------------------
# Export CMake package
# --------------------
install(EXPORT FortestTargets
        FILE FortestTargets.cmake
        NAMESPACE Fortest::
        DESTINATION lib/cmake/Fortest
)

add_executable(main main.f90)
set_target_properties(main PROPERTIES
        LINKER_LANGUAGE Fortran
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
target_link_libraries(main PUBLIC fortest)