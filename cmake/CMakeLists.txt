cmake_minimum_required(VERSION 3.7)
project(trial.aware CXX)

option(WITH_DNSSD "Build with DNS-SD" TRUE)

if (NOT DEFINED TRIAL_AWARE_BUILD_DIR)
  set(TRIAL_AWARE_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
endif()
set(LIBRARY_OUTPUT_PATH ${TRIAL_AWARE_BUILD_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${TRIAL_AWARE_BUILD_DIR}/bin)

get_filename_component(TRIAL_AWARE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../" ABSOLUTE)

list(APPEND CMAKE_MODULE_PATH "${TRIAL_AWARE_ROOT}/cmake")

if (${CMAKE_VERSION} VERSION_LESS 3.8)
  set(TRIAL_AWARE_FEATURES cxx_auto_type)
else()
  set(TRIAL_AWARE_FEATURES cxx_std_11)
endif()

if (MSVC)
  set(TRIAL_AWARE_WARNING_FLAGS /W3)
else()
  set(TRIAL_AWARE_WARNING_FLAGS -Wall -Wextra -pedantic -Werror=return-type)
endif()

find_package(Boost 1.55.0 REQUIRED QUIET COMPONENTS system)

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

add_subdirectory("${TRIAL_AWARE_ROOT}/src" "${TRIAL_AWARE_BUILD_DIR}/src")
