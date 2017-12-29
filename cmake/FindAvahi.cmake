# FindAvahi
#
# Find Avahi
#
# ::
#   Avahi_INCLUDE_DIR
#   Avahi_LIBRARY
#   Avahi_FOUND
#
# Defines DNSSD::Avahi imported target

find_package(PkgConfig QUIET REQUIRED)
pkg_check_modules(Avahi QUIET avahi-client)

if (Avahi_FOUND)
  find_library(Avahi_COMMON_LIBRARY NAMES avahi-common PATHS ${Avahi_LIBDIR})
  find_library(Avahi_CLIENT_LIBRARY NAMES avahi-client PATHS ${Avahi_LIBDIR})

  if (NOT TARGET DNSSD::AvahiCommon)
    add_library(DNSSD::AvahiCommon SHARED IMPORTED GLOBAL)
    set_target_properties(DNSSD::AvahiCommon PROPERTIES
      IMPORTED_LOCATION "${Avahi_COMMON_LIBRARY}"
      INTERFACE_COMPILE_OPTIONS "${Avahi_CFLAGS}"
      INTERFACE_INCLUDE_DIRECTORIES "${Avahi_INCLUDE_DIR}"
      )
  endif()
  if (NOT TARGET DNSSD::Avahi)
    add_library(DNSSD::Avahi SHARED IMPORTED GLOBAL)
    set_target_properties(DNSSD::Avahi PROPERTIES
      IMPORTED_LOCATION "${Avahi_CLIENT_LIBRARY}"
      INTERFACE_COMPILE_OPTIONS "${Avahi_CFLAGS}"
      INTERFACE_INCLUDE_DIRECTORIES "${Avahi_INCLUDE_DIR}"
      INTERFACE_LINK_LIBRARIES DNSSD::AvahiCommon
      )
  endif()

elseif(Avahi_FIND_REQUIRED)
  if (NOT Avahi_FIND_QUIETLY)
    message(FATAL_ERROR "Avahi package not found")
  endif()
endif()
