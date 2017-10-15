# -*- mode: cmake; coding: utf-8 -*-
# This file is part of Pathie.
#
# Pathie is a path management library.
# Copyright © 2015 Marvin Gülker
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8)
enable_language(CXX)
project(Pathie)

########################################
# Version number

set(PATHIE_VERSION_MAJOR 0)
set(PATHIE_VERSION_MINOR 1)
set(PATHIE_VERSION_PATCH 0)

execute_process(COMMAND git rev-parse --short HEAD
  WORKING_DIRECTORY ${Pathie_SOURCE_DIR}
  OUTPUT_VARIABLE PATHIE_VERSION_GIT
  OUTPUT_STRIP_TRAILING_WHITESPACE)

# If git was not available, unset empty variable
if (NOT(PATHIE_VERSION_GIT))
  unset(PATHIE_VERSION_GIT)
endif()

########################################
# Flags & Options

option(ASSUME_UTF8_ON_UNIX "Assume file pathes under UNIX to be always encoded in UTF-8." OFF)

########################################
# Extra flags

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 ")
endif()

# We only support Vista upwards. See http://msdn.microsoft.com/en-us/library/windows/desktop/aa383745(v=vs.85).aspx
# for the macro values.
if (WIN32)
  add_definitions("-D_WIN32_WINNT=0x0600") # Vista
  add_definitions("-D_WIN32_IE=0x0800") # IE 8.0+
  add_definitions("-DWINVER=0x0600") # Vista
elseif(UNIX)
  add_definitions("-D_PATHIE_UNIX=1")
endif()

add_definitions("-DPATHIE_VERSION_MAJOR=${PATHIE_VERSION_MAJOR}")
add_definitions("-DPATHIE_VERSION_MINOR=${PATHIE_VERSION_MINOR}")
add_definitions("-DPATHIE_VERSION_PATCH=${PATHIE_VERSION_PATCH}")
if (PATHIE_VERSION_GIT)
  add_definitions("-DPATHIE_VERSION_GIT=\"${PATHIE_VERSION_GIT}\"")
endif()

if (ASSUME_UTF8_ON_UNIX)
  add_definitions("-DASSUME_UTF8_ON_UNIX=1")
endif()

########################################
# Adjustments to cmake’s defaults

set(CMAKE_BUILD_SHARED_LIBS On CACHE BOOL "Additionally to the static lib, build the shared lib")

########################################
# Source files

file(GLOB_RECURSE pathie_sources
  "src/*.cpp"
  "include/*.hpp"
)
include_directories("${Pathie_SOURCE_DIR}/include")

file(GLOB_RECURSE test_sources
  "test/*.cpp")

########################################
# Targets

# Libraries
add_library(pathie STATIC ${pathie_sources})

if (CMAKE_BUILD_SHARED_LIBS)
  add_library(pathie-dynamic SHARED ${pathie_sources})
  set_target_properties(pathie-dynamic PROPERTIES OUTPUT_NAME pathie)
endif()

if(WIN32)
  target_link_libraries(pathie shlwapi)

  if (CMAKE_BUILD_SHARED_LIBS)
    target_link_libraries(pathie-dynamic shlwapi)
  endif()
endif()

# Tests
# TODO: How to only do not test with crosscompilation?
#if (NOT(WIN32))
#  foreach(testfile ${test_sources})
#    get_filename_component(testtargetname "${testfile}" NAME_WE)
#    set(testtargetname "${testtargetname}.test")
#    message(STATUS "TESTFILE: ${testtargetname}")
#    #string(REPLACE ".cpp" ".test" testtargetname ${testfile})
#    add_executable(${testtargetname} ${testfile} ${Pathie_SOURCE_DIR}/test/testhelpers.hpp)
#  endforeach()
#endif()

########################################
# Installation information

install(TARGETS pathie
  DESTINATION lib)

if (CMAKE_BUILD_SHARED_LIBS)
  install(TARGETS pathie-dynamic
    DESTINATION lib)
endif()

install(DIRECTORY "${Pathie_SOURCE_DIR}/include/"
  DESTINATION include/pathie)
