# nlohmann/json — installed via mise from github:nlohmann/json

execute_process(COMMAND mise where "github:nlohmann/json"
                OUTPUT_VARIABLE NLOHMANN_ROOT
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET)
if(NOT NLOHMANN_ROOT)
    message(FATAL_ERROR "nlohmann/json not found. Install with: mise use github:nlohmann/json")
endif()

set(NLOHMANN_INCLUDES "${NLOHMANN_ROOT}/include")
message(STATUS "nlohmann/json: ${NLOHMANN_ROOT}")
