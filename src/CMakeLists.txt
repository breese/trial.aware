add_library(aware-common
  contact.cpp
  error.cpp
  native_socket.cpp
  )

target_compile_options(aware-common PRIVATE ${TRIAL_AWARE_WARNING_FLAGS})
target_link_libraries(aware-common Boost::system)

include_directories(aware-common BEFORE PRIVATE .)

if (WITH_DNSSD)
  add_subdirectory(dnssd)
  add_library(aware ALIAS aware-dnssd)
endif()
