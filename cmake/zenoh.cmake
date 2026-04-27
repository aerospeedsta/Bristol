# Zenoh — installed via mise
#   github:eclipse-zenoh/zenoh-c       → C library (headers + libzenohc.a)
#   github:eclipse-zenoh/zenoh-cpp     → C++ wrappers (headers only)

execute_process(COMMAND mise where "github:eclipse-zenoh/zenoh-c"
                OUTPUT_VARIABLE ZENOH_ROOT
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET)
if(NOT ZENOH_ROOT)
    message(FATAL_ERROR "zenoh-c not found. Install with: mise use github:eclipse-zenoh/zenoh-c")
endif()

execute_process(COMMAND mise where "github:eclipse-zenoh/zenoh-cpp"
                OUTPUT_VARIABLE ZENOH_CPP_ROOT
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET)
if(NOT ZENOH_CPP_ROOT)
    message(FATAL_ERROR "zenoh-cpp not found. Install with: mise use github:eclipse-zenoh/zenoh-cpp")
endif()

set(ZENOH_INCLUDES "${ZENOH_ROOT}/include" "${ZENOH_CPP_ROOT}/include")

if(APPLE AND CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
    set(ZENOH_LIB "${ZENOH_ROOT}/lib/libzenohc.a")
else()
    set(ZENOH_LIB "${ZENOH_ROOT}/lib/libzenohc.a")
endif()

message(STATUS "zenoh-c:   ${ZENOH_ROOT}")
message(STATUS "zenoh-cpp: ${ZENOH_CPP_ROOT}")
