# Common setup instructions shared by all AirSim CMakeLists.

macro(CommonTargetLink)
    target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
    #target_link_libraries(c++abi)
endmacro(CommonTargetLink)

macro(IncludeEigen)
    include_directories(${EIGEN_INCLUDES})
endmacro(IncludeEigen)

macro(AddExecutableSource)
    set(PROJECT_CPP ${PROJECT_NAME}_sources)
    file(GLOB_RECURSE PROJECT_CPP "${AIRSIM_ROOT}/${PROJECT_NAME}/*.cpp")
    add_executable(${PROJECT_NAME} ${PROJECT_CPP})
endmacro(AddExecutableSource)

macro(SetupConsoleBuild)
    IF(UNIX)
    ELSE()
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_CONSOLE ")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
    ENDIF()
endmacro(SetupConsoleBuild)

macro(CommonSetup)
    find_package(Threads REQUIRED)
    find_path(AIRSIM_ROOT NAMES build.sh PATHS ".." "../.." "../../.." "../../../.." "../../../../.." "../../../../../.." REQUIRED)

    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
    SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/output/bin)
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

    include("${AIRSIM_ROOT}/cmake/zenoh.cmake")
    include("${AIRSIM_ROOT}/cmake/nlohmann.cmake")
    include("${AIRSIM_ROOT}/cmake/eigen.cmake")

    string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE)

    IF(UNIX)
        set(RPC_LIB_DEFINES "-D MSGPACK_PP_VARIADICS_MSVC=0")
        set(CMAKE_CXX_STANDARD 17)

        if (APPLE)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wstrict-aliasing -D__CLANG__ -DMSGPACK_DISABLE_LEGACY_NIL -DZENOHCXX_ZENOHC")
        else ()
            set(CMAKE_CXX_FLAGS "\
                -Wall -Wextra \
                -Wnon-virtual-dtor -Woverloaded-virtual \
                -Wno-variadic-macros -Wno-unused-function -Wno-unused \
                -pthread \
                ${RPC_LIB_DEFINES} ${CMAKE_CXX_FLAGS}")

            if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
                set(CMAKE_CXX_FLAGS "-stdlib=libc++ -Wno-documentation -Wno-unknown-warning-option ${CMAKE_CXX_FLAGS}")
                find_package(LLVM QUIET CONFIG)
            endif ()
        endif ()

        if(APPLE AND CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
            set(BUILD_PLATFORM "arm64")
        else()
            set(BUILD_PLATFORM "x64")
        endif()

        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
        if (CMAKE_BUILD_TYPE MATCHES Release)
            set(CMAKE_CXX_FLAGS "-O3 ${CMAKE_CXX_FLAGS}")
        endif ()

    ELSE()
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0600 /GS /W4 /wd4100 /wd4505 /wd4820 /wd4464 /wd4514 /wd4710 /wd4571 /Zc:wchar_t /ZI /Zc:inline /fp:precise /D_SCL_SECURE_NO_WARNINGS /D_CRT_SECURE_NO_WARNINGS /D_UNICODE /DUNICODE /WX- /Zc:forScope /Gd /EHsc ")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NXCOMPAT /DYNAMICBASE /INCREMENTAL:NO ")

        if("${BUILD_TYPE}" STREQUAL "debug")
          set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_DEBUG /MDd /RTC1 /Gm /Od ")
        elseif("${BUILD_TYPE}" STREQUAL "release")
          set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MD /O2 /Oi /GL /Gm- /Gy /TP ")
        else()
          message(FATAL_ERROR "Please specify '-D CMAKE_BUILD_TYPE=Debug' or Release on the cmake command line")
        endif()
    ENDIF()

endmacro(CommonSetup)
