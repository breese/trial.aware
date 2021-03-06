# FindDNSSD
#
# Find DNS Service Discovery backend

option(DNSSD_WITH_AVAHI "Check for Avahi" ON)
set(DNSSD_WITH_MDNSRESPONDER "" CACHE PATH "Check for mDNSResponder using build directory")

if (APPLE)
  find_package(mDNSResponder)

elseif(WIN32)
  # FIXME

else() # Posix
  if (DNSSD_WITH_AVAHI)
    find_package(Avahi QUIET)
  endif()

  if (NOT DNSSD_FOUND)
    if (NOT DNSSD_WITH_MDNSRESPONDER STREQUAL "")
      set(mDNSResponder_BUILD_DIR ${DNSSD_WITH_MDNSRESPONDER})
    endif()
    find_package(mDNSResponder QUIET)
  endif()
endif()
