add_executable(zannounce
  zannounce.cpp
  )
target_link_libraries(zannounce aware)

add_executable(zmonitor
  zmonitor.cpp
  )
target_link_libraries(zmonitor aware)

add_custom_target(dnssd-example DEPENDS zannounce zmonitor)
