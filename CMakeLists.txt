#-------------------------------------------------------------------------------
#
# CMakeLists.txt
#
# Top level CMakeLists.txt file for the roottest directory.
#
#-------------------------------------------------------------------------------

# Check the CMake version.
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Set the name of the project.
project(roottest)

# Check, if roottest is built out of source.
string(COMPARE EQUAL ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} insource)
if(insource)
  file(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/Testing)
  file(REMOVE ${CMAKE_SOURCE_DIR}/DartConfiguration.tcl)
  message(FATAL_ERROR "roottest should be installed as an out of source build, "
                      "to keep the source directory clean. Please create a "
                      "extra build directory and run the command 'cmake "
                      "<path_to_source_dir>' in this newly created directory. "
                      "You have also to delete the directory CMakeFiles and "
                      "the file CMakeCache.txt in the source directory. "
                      "Otherwise cmake will complain even if you run it from "
                      "an out-of-source directory.")
endif()

# Check, if ROOTSYS is set.
if(NOT DEFINED ROOTSYS)
  set(ROOTSYS $ENV{ROOTSYS})

  if(NOT DEFINED ROOTSYS)
    message(FATAL_ERROR "ROOTSYS is not set!")
  endif()
endif()

# If ROOT_SOURCE_DIR is set, roottest is built as part of the root build.
# If ROOT_SOURCE_DIR is not set, search for an installation.
if(DEFINED ROOT_SOURCE_DIR)
  find_package(ROOT REQUIRED)

  get_filename_component(ROOT_INCLUDE_DIR "${ROOTSYS}/include" ABSOLUTE)
  set(ROOT_INCLUDE_DIRS ${ROOT_INCLUDE_DIR})
  set(ROOT_LIBRARIES Core RIO Net Hist Gpad Tree Rint Matrix MathCore)
  set(ROOT_genreflex_CMD ${ROOTSYS}/bin/genreflex)
  set(ROOT_rootcint_CMD ${ROOTSYS}/bin/rootcint)
  set(ROOT_rootcling_CMD rootcling)

else()

  find_package(ROOT REQUIRED COMPONENTS Core RIO Net Hist Gpad Tree Rint Matrix MathCore
                             HINTS ${ROOTSYS}/etc/cmake)
endif()

get_filename_component(ROOT_BINARY_DIR  "${ROOTSYS}/bin" ABSOLUTE)
get_filename_component(ROOT_LIBRARY_DIR "${ROOTSYS}/lib" ABSOLUTE)

# Detect bitness.
if(CMAKE_SYSTEM_PROCESSOR MATCHES amd64.*|x86_64.*)
  set(X86_64 1)
  message("-- Check for bitness: Found 64 bit architecture.")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES i686.*|i386.*|x86.*)
  set(X86 1)
  message("-- Check for bitness: Found 32 bit architecture.")
endif()

# Setup environment.
set(ROOTTEST_ENV_PATH ${ROOT_BINARY_DIR})
set(ROOTTEST_ENV_PYTHONPATH ${ROOT_LIBRARY_DIR})
set(ROOTTEST_ENV_LIBRARYPATH ${ROOT_LIBRARY_DIR})

# Resolve symbolic links for the ROOTTEST_DIR variable.
get_filename_component(ROOTTEST_DIR ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)

# Set the CMake module path. Here are all the custom CMake modules.
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${ROOTTEST_DIR}/cmake/modules")

# Find GCCXML.
find_package(GCCXML REQUIRED)

# Find python.
find_package(PythonInterp)
find_program(python_cmd ${PYTHON_EXECUTABLE})

# Setup standard includes / links.
include_directories(${ROOT_INCLUDE_DIRS} ${ROOT_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${ROOT_LIBRARY_DIR})

# Enable and setup CTest.
include(RootNewMacros)
include(RoottestCTest)
include(RootCTestMacros)

message("-- Scanning subdirectories for tests...")
ROOTTEST_ADD_TESTDIRS()