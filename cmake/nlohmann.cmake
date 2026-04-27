# nlohmann/json — installed via mise from conan:nlohmann_json

execute_process(COMMAND mise where "conan:nlohmann_json"
                OUTPUT_VARIABLE NLOHMANN_ROOT
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET)
if(NOT NLOHMANN_ROOT)
    message(FATAL_ERROR "nlohmann/json not found. Install with: mise use conan:nlohmann_json")
endif()

set(NLOHMANN_INCLUDES "${NLOHMANN_ROOT}/include")
message(STATUS "nlohmann/json: ${NLOHMANN_ROOT}")
