cmake_minimum_required(VERSION 3.10.0)

project(libdedx VERSION 0.1.0 LANGUAGES C CXX)

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
message (STATUS "Install prefix =  ${CMAKE_INSTALL_PREFIX}") 

# this should be called after setting the prefix !!
include (GNUInstallDirs)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH OFF)
message (STATUS "Install rpath =  ${CMAKE_INSTALL_RPATH}")

add_executable(generator EXCLUDE_FROM_ALL
    src/generator/generator.h
    src/generator/generator.cpp
    src/generator/gensrim96.cpp
    src/generator/print_cpp.cpp
)

file(GLOB generated_files CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/generated/*/*.c)

add_library(dedx SHARED
  src/lib/libdedx.h
  src/lib/libdedx.c
  ${PROJECT_SOURCE_DIR}/src/generated/erg.c
  ${generated_files} 
)

set_target_properties(dedx PROPERTIES PUBLIC_HEADER src/lib/libdedx.h)
target_compile_definitions(dedx PRIVATE -DLIBDEDX_EXPORTS)

add_executable(testdedx EXCLUDE_FROM_ALL
  src/test/main.cpp)

target_include_directories(testdedx PUBLIC src/lib)
target_link_libraries(testdedx dedx)

install(TARGETS dedx PUBLIC_HEADER)
