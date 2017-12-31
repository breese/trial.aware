if (TARGET DNSSD::Avahi)

  add_library(aware-dnssd OBJECT
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

  get_target_property(COMPILE_OPTIONS DNSSD::Avahi INTERFACE_COMPILE_OPTIONS)
  target_compile_options(aware-dnssd PRIVATE ${COMPILE_OPTIONS})
  get_target_property(INCLUDE_DIRECTORIES DNSSD::Avahi INTERFACE_INCLUDE_DIRECTORIES)
  target_include_directories(aware-dnssd PRIVATE ${INCLUDE_DIRECTORIES})
  set(TRIAL_AWARE_DNSSD_TARGET DNSSD::Avahi PARENT_SCOPE)

elseif (TARGET DNSSD::mDNSResponder)

  add_library(aware-dnssd OBJECT
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

  get_target_property(COMPILE_OPTIONS DNSSD::mDNSResponder INTERFACE_COMPILE_OPTIONS)
  target_compile_options(aware-dnssd PRIVATE ${COMPILE_OPTIONS})
  get_target_property(INCLUDE_DIRECTORIES DNSSD::mDNSResponder INTERFACE_INCLUDE_DIRECTORIES)
  target_include_directories(aware-dnssd PRIVATE ${INCLUDE_DIRECTORIES})
  set(TRIAL_AWARE_DNSSD_TARGET DNSSD::mDNSResponder PARENT_SCOPE)

else()

  add_library(aware-dnssd OBJECT)
  set(TRIAL_AWARE_TARGET_DNSSD "" PARENT_SCOPE)

endif()

target_compile_options(aware-dnssd PRIVATE ${TRIAL_AWARE_WARNING_FLAGS})
target_include_directories(aware-dnssd BEFORE PRIVATE .)
