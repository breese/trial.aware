# FindAvahi
#
# Find Avahi
#
# ::
#
# Defines PkgConfig::Avahi imported target

find_package(PkgConfig QUIET REQUIRED)
pkg_check_modules(Avahi QUIET IMPORTED_TARGET avahi-client)

if(NOT TARGET PkgConfig::Avahi AND Avahi_FIND_REQUIRED)
  if (NOT Avahi_FIND_QUIETLY)
    message(FATAL_ERROR "Avahi package not found")
  endif()
endif()
