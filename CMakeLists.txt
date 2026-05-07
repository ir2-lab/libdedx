cmake_minimum_required(VERSION 3.10.0)

project(libdedx 
  VERSION 0.2.0 
  LANGUAGES C CXX
  DESCRIPTION "A compilation of different parametrizations for the electronic stopping of ions in matter"
  HOMEPAGE_URL https://github.com/ir2-lab/libdedx
)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# by default, install into $HOME/.local (not /usr/local), so that no root access (and sudo!!) is needed
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  if((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND (NOT CMAKE_CROSSCOMPILING))
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${PROJECT_NAME}" CACHE PATH "Default install path" FORCE)
  else()
    set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/.local" CACHE PATH "Default install path" FORCE)
  endif()
endif()

option (LIBDEDX_BUILD_GENERATOR "Build the generator executable" OFF)
option (LIBDEDX_BUILD_TESTS "Build tests" OFF)


# this should be called after setting the prefix !!
include (GNUInstallDirs)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_INSTALL_PREFIX STREQUAL "$ENV{HOME}/.local")
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH OFF)
  message (STATUS "Install rpath =  ${CMAKE_INSTALL_RPATH}")
endif()

add_subdirectory(src)

if(LIBDEDX_BUILD_TESTS)
  add_subdirectory(test)
endif()

# ------------------------------
# CMake configuration summary
# ------------------------------
message(STATUS "-------------------------------------------------")
message(STATUS "CMake configuration summary for ${PROJECT_NAME}")
message(STATUS "  Project:        ${PROJECT_NAME} ${PROJECT_VERSION}")
message(STATUS "  CMake:          ${CMAKE_VERSION}")
message(STATUS "  Generator:      ${CMAKE_GENERATOR}")
message(STATUS "  Build type:     ${CMAKE_BUILD_TYPE}")
message(STATUS "  C compiler:     ${CMAKE_C_COMPILER} (${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION})")
message(STATUS "  CXX compiler:   ${CMAKE_CXX_COMPILER} (${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION})")
message(STATUS "  C++ standard:   ${CMAKE_CXX_STANDARD}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Install libdir: ${CMAKE_INSTALL_FULL_LIBDIR}")
message(STATUS "  Install includedir: ${CMAKE_INSTALL_INCLUDEDIR}")
message(STATUS "  Install bindir: ${CMAKE_INSTALL_BINDIR}")
message(STATUS "  Build shared libs: ${BUILD_SHARED_LIBS}")
message(STATUS "-------------------------------------------------")

# -------------------------
# CPack packaging
# -------------------------
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "ir2-lab")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")

if(WIN32)
  set(CPACK_SYSTEM_NAME "win64")
endif()

# Components
set(CPACK_COMPONENTS_ALL Runtime Development)

set(CPACK_COMPONENT_RUNTIME_DISPLAY_NAME "Runtime Library")
set(CPACK_COMPONENT_RUNTIME_DESCRIPTION
  "The libdedx.dll shared library required to run applications that use libdedx.")

set(CPACK_COMPONENT_DEVELOPMENT_DISPLAY_NAME "Development Files")
set(CPACK_COMPONENT_DEVELOPMENT_DESCRIPTION
  "Import library (libdedx.dll.a), header (libdedx.h), and CMake package config files.")
set(CPACK_COMPONENT_DEVELOPMENT_DEPENDS Runtime)

# Inno Setup: enable per-component selection in the installer
set(CPACK_INNOSETUP_COMPONENT_INSTALL ON)
set(CPACK_INNOSETUP_INSTALL_ROOT "{autopf64}")

include(CPack)
