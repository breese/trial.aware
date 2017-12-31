###############################################################################
#
# Copyright (C) 2017 Bjorn Reese <breese@users.sourceforge.net>
#
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)
#
###############################################################################

cmake_minimum_required(VERSION 3.0)
project(trial.aware)

option(WITH_DNSSD "Build with DNS-SD" TRUE)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

set(TRIAL_AWARE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(TRIAL_AWARE_BUILD_DIR ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${TRIAL_AWARE_BUILD_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${TRIAL_AWARE_BUILD_DIR}/bin)

set(CMAKE_CXX_STANDARD 11)

if (MSVC)
  set(TRIAL_AWARE_WARNING_FLAGS /W4)
else()
  set(TRIAL_AWARE_WARNING_FLAGS -Wall -Wextra -pedantic)
endif()

###############################################################################
# Boost package
###############################################################################

find_package(Boost 1.55.0 REQUIRED QUIET COMPONENTS system)

###############################################################################
# DNS-SD package
###############################################################################

if (WITH_DNSSD)
  find_package(DNSSD REQUIRED)
  if (TARGET PkgConfig::Avahi)
    message(STATUS "DNS-SD backend: Avahi")
  elseif (TARGET DNSSD::mDNSResponder)
    message(STATUS "DNS-SD backend: mDNSResponder")
  else()
    message(FATAL_ERROR "DNS-SD backend: Not found")
  endif()
endif()

###############################################################################
# Trial.Aware package
###############################################################################

include_directories(${TRIAL_AWARE_ROOT}/include)
add_subdirectory(src)

###############################################################################
# Examples
###############################################################################

add_subdirectory(example EXCLUDE_FROM_ALL)
