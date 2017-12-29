if (TARGET DNSSD::Avahi)

  add_library(aware-dnssd
    avahi/announce_socket.cpp
    avahi/announcer.cpp
    avahi/browser.cpp
    avahi/client.cpp
    avahi/error.cpp
    avahi/factory.cpp
    avahi/monitor_socket.cpp
    avahi/poller.cpp
    avahi/service.cpp
    avahi/utility.cpp
    )

  target_link_libraries(aware-dnssd aware-common DNSSD::Avahi)

elseif (TARGET DNSSD::mDNSResponder)

  add_library(aware-dnssd
    mdns/announce_socket.cpp
    mdns/announcer.cpp
    mdns/browser.cpp
    mdns/error.cpp
    mdns/factory.cpp
    mdns/handle.cpp
    mdns/monitor.cpp
    mdns/monitor_socket.cpp
    mdns/properties.cpp
    mdns/resolver.cpp
    mdns/service.cpp
    mdns/throw_on_error.cpp
    mdns/utility.cpp
    )

  target_link_libraries(aware-dnssd aware-common DNSSD::mDNSResponder)

else()
  message(FATAL_ERROR "No DNS-SD backend is found")
endif()

target_compile_options(aware-dnssd PRIVATE ${TRIAL_AWARE_WARNING_FLAGS})
target_include_directories(aware-dnssd BEFORE PRIVATE .)
