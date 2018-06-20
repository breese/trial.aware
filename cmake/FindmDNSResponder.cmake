# FindmDNSResponder
#
# Find Apple mDNSResponder
#
# ::
#   mDNSResponder_INCLUDE_DIR
#   mDNSResponder_LIBRARY
#   mDNSResponder_FOUND
#
#   mDNSResponder_LIBRARIES
#   mDNSResponder_INCLUDE_DIRS
#
# Defines DNSSD::mDNSResponder imported target

find_path(mDNSResponder_INCLUDE_DIR
  NAMES dns_sd.h
  PATHS "${mDNSResponder_BUILD_DIR}"
  PATH_SUFFIXES mDNSShared)

find_library(mDNSResponder_LIBRARY
  NAMES dns_sd
  PATHS "${mDNSResponder_BUILD_DIR}"
  PATH_SUFFIXES mDNSPosix/build/prod)

set(mDNSResponder_FOUND FALSE CACHE BOOL "" FORCE)
if (mDNSResponder_INCLUDE_DIR AND mDNSResponder_LIBRARY)
  if (EXISTS ${mDNSResponder_INCLUDE_DIR}/dns_sd.h)
    file(STRINGS ${mDNSResponder_INCLUDE_DIR}/dns_sd.h HEADER_FILE NEWLINE_CONSUME)
    string(REGEX MATCH "#define _DNS_SD_H[^\n]*" VERSION_LINE ${HEADER_FILE})
    string(REGEX REPLACE
      "#define _DNS_SD_H[ \t]*([0-9][0-9][0-9])([0-9][0-9])([0-9][0-9]).*"
      "\\1.\\2.\\3"
      mDNSResponder_VERSION
      ${VERSION_LINE})
    if ("${mDNSResponder_VERSION}" STREQUAL "${VERSION_LINE}")
      set(mDNSResponder_VERSION 0)
    else()
      set(mDNSResponder_FOUND TRUE CACHE BOOL "" FORCE)
    endif()
  endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(mDNSResponder
    FOUND_VAR mDNSResponder_FOUND
    REQUIRED_VARS mDNSResponder_LIBRARY mDNSResponder_INCLUDE_DIR
    VERSION_VAR mDNSResponder_VERSION
)

if(mDNSResponder_FOUND)
  set(mDNSResponder_LIBRARIES ${mDNSResponder_LIBRARY})
  set(mDNSResponder_INCLUDE_DIRS ${mDNSResponder_INCLUDE_DIR})
  set(mDNSResponder_DEFINITIONS "")

  if (NOT TARGET DNSSD::mDNSResponder)
    add_library(DNSSD::mDNSResponder SHARED IMPORTED GLOBAL)
    set_target_properties(DNSSD::mDNSResponder PROPERTIES
      IMPORTED_LOCATION "${mDNSResponder_LIBRARY}"
      INTERFACE_COMPILE_OPTIONS "${mDNSResponder_DEFINITIONS}"
      INTERFACE_INCLUDE_DIRECTORIES "${mDNSResponder_INCLUDE_DIR}"
      )
  endif()

elseif(mDNSResponder_FIND_REQUIRED)
  if (NOT mDNSResponder_FIND_QUIETLY)
    message(FATAL_ERROR "mDNSResponder package not found")
  endif()
endif()

mark_as_advanced(
  mDNSResponder_FOUND
  mDNSResponder_VERSION
  mDNSResponder_INCLUDE_DIR
  mDNSResponder_LIBRARY)
