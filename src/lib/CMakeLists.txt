file(GLOB generated_files CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/../generated/*/*.c)

add_library(dedx SHARED
  libdedx.h
  libdedx.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../generated/erg.c
  ${generated_files} 
)

set_target_properties(dedx PROPERTIES 
  PUBLIC_HEADER libdedx.h)
  
target_compile_definitions(dedx PRIVATE -DLIBDEDX_EXPORTS)

target_include_directories(dedx
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# -----------------------
# Installation and config
# -----------------------
include(CMakePackageConfigHelpers)

# Install the library target and export targets file
install(TARGETS dedx
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# location where CMake config files will be installed (relocatable)
set(CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

# Install the exported targets so downstream projects can use target names
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CONFIG_INSTALL_DIR}
)

# Generate and install package config + version files
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/${PROJECT_NAME}Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION "${CONFIG_INSTALL_DIR}"
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION "${CONFIG_INSTALL_DIR}"
)