# Eigen — installed via mise from conan:eigen

execute_process(COMMAND mise where "conan:eigen"
                OUTPUT_VARIABLE EIGEN_ROOT
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET)
if(NOT EIGEN_ROOT)
    message(FATAL_ERROR "Eigen not found. Install with: mise use conan:eigen")
endif()

set(EIGEN_INCLUDES "${EIGEN_ROOT}/include/eigen3")
message(STATUS "Eigen: ${EIGEN_ROOT}")
