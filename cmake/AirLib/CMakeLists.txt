cmake_minimum_required(VERSION 3.5.0)
project(AirLib)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../cmake-modules") 
INCLUDE("${CMAKE_CURRENT_LIST_DIR}/../cmake-modules/CommonSetup.cmake")
CommonSetup()

IncludeEigen()

include_directories(
  ${AIRSIM_ROOT}/AirLib/
  ${AIRSIM_ROOT}/AirLib/include
  ${AIRSIM_ROOT}/MavLinkCom/include
  ${AIRSIM_ROOT}/AirLib/deps/msgpack/include
  ${ZENOH_INCLUDES}
  ${NLOHMANN_INCLUDES}
)

file(GLOB_RECURSE ${PROJECT_NAME}_sources 
  ${AIRSIM_ROOT}/${PROJECT_NAME}/src/api/*.cpp
  ${AIRSIM_ROOT}/${PROJECT_NAME}/src/common/common_utils/*.cpp
  ${AIRSIM_ROOT}/${PROJECT_NAME}/src/safety/*.cpp
  ${AIRSIM_ROOT}/${PROJECT_NAME}/src/vehicles/car/api/*.cpp
  ${AIRSIM_ROOT}/${PROJECT_NAME}/src/vehicles/multirotor/*.cpp
  ${AIRSIM_ROOT}/${PROJECT_NAME}/src/vehicles/multirotor/api/*.cpp
)

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_sources})
target_compile_definitions(${PROJECT_NAME} PRIVATE ZENOHCXX_ZENOHC)

CommonTargetLink()
target_link_libraries(${PROJECT_NAME} MavLinkCom)
target_link_libraries(${PROJECT_NAME} ${ZENOH_LIB})
target_link_libraries(${PROJECT_NAME} "-framework CoreFoundation" "-framework Security")
