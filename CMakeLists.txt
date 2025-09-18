cmake_minimum_required(VERSION 3.20)
enable_testing()
project(Fortest VERSION 1.0.0 LANGUAGES CXX C Fortran)

set(CMAKE_CXX_STANDARD 20)
# Ensure installed executables/libraries look in ${prefix}/lib for dependencies
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(examples)

include(CMakePackageConfigHelpers)

# Export targets
install(EXPORT FortestTargets
        FILE FortestTargets.cmake
        NAMESPACE Fortest::
        DESTINATION lib/cmake/Fortest
)

# Create the config file that includes the exported targets
configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/FortestConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/FortestConfig.cmake"
        INSTALL_DESTINATION lib/cmake/Fortest
)

# Version file (already fixed)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/FortestConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

# Install config + version files
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/FortestConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/FortestConfigVersion.cmake"
        DESTINATION lib/cmake/Fortest
)


# Generate version file
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/FortestConfigVersion.cmake"
        COMPATIBILITY SameMajorVersion
)

# Install version + config file
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/FortestConfigVersion.cmake"
        DESTINATION lib/cmake/Fortest
)
