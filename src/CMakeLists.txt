add_library(aware-common OBJECT
  contact.cpp
  error.cpp
  native_socket.cpp
  )

target_compile_options(aware-common PRIVATE ${TRIAL_AWARE_WARNING_FLAGS})

include_directories(aware-common BEFORE PRIVATE .)

add_subdirectory(dnssd)
add_library(aware
  $<TARGET_OBJECTS:aware-common>
  $<TARGET_OBJECTS:aware-dnssd>
  )
target_link_libraries(aware ${TRIAL_AWARE_DNSSD_TARGET} Boost::system)
