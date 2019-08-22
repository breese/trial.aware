set(TRIAL_AWARE_EXTERNAL_ROOT "${TRIAL_AWARE_ROOT}/external")
set(TRIAL_AWARE_EXTERNAL_BUILD_DIR "${TRIAL_AWARE_BUILD_DIR}/external")

###############################################################################
# Trial.Net
###############################################################################

if (NOT DEFINED TRIAL_NET_ROOT)
  set(TRIAL_NET_ROOT "${TRIAL_AWARE_EXTERNAL_ROOT}/trial.net/cmake")
endif()
add_subdirectory("${TRIAL_NET_ROOT}" "${TRIAL_AWARE_BUILD_DIR}/trial.net" EXCLUDE_FROM_ALL)
add_library(Trial::Net INTERFACE IMPORTED GLOBAL)
target_link_libraries(Trial::Net INTERFACE trial-net)
