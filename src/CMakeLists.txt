###############################################################################
# Core
###############################################################################

add_library(trial-aware
  contact.cpp
  error.cpp
  native_socket.cpp
  )

target_compile_options(trial-aware PRIVATE ${TRIAL_AWARE_WARNING_FLAGS})
target_compile_features(trial-aware PRIVATE ${TRIAL_AWARE_FEATURES})
target_include_directories(trial-aware PUBLIC "${TRIAL_AWARE_ROOT}/include")
target_include_directories(trial-aware PRIVATE .)
target_link_libraries(trial-aware Trial::Net)

###############################################################################
# DNS-SD
###############################################################################

if (TARGET PkgConfig::Avahi)

  find_package(Threads)

  target_sources(trial-aware PRIVATE
    dnssd/avahi/announce_socket.cpp
    dnssd/avahi/announcer.cpp
    dnssd/avahi/browser.cpp
    dnssd/avahi/client.cpp
    dnssd/avahi/error.cpp
    dnssd/avahi/factory.cpp
    dnssd/avahi/monitor_socket.cpp
    dnssd/avahi/poller.cpp
    dnssd/avahi/service.cpp
    dnssd/avahi/utility.cpp
    )

  target_link_libraries(trial-aware PkgConfig::Avahi Threads::Threads)

elseif (TARGET DNSSD::mDNSResponder)

  target_sources(trial-aware PRIVATE
    dnssd/mdns/announce_socket.cpp
    dnssd/mdns/announcer.cpp
    dnssd/mdns/browser.cpp
    dnssd/mdns/error.cpp
    dnssd/mdns/factory.cpp
    dnssd/mdns/handle.cpp
    dnssd/mdns/monitor.cpp
    dnssd/mdns/monitor_socket.cpp
    dnssd/mdns/properties.cpp
    dnssd/mdns/resolver.cpp
    dnssd/mdns/service.cpp
    dnssd/mdns/throw_on_error.cpp
    dnssd/mdns/utility.cpp
    )

  target_link_libraries(trial-aware DNSSD::mDNSResponder)

endif()
