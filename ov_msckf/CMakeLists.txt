cmake_minimum_required(VERSION 3.16)

option(ILLIXR_INTEGRATION "Flag enabling integrated (non-standalone) mode" OFF)

# Project name
project(ov_msckf)

# Include our cmake files
#set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)


# If we are not building with ROS then we need to manually link to its headers
# This isn't that elegant of a way, but this at least allows for building without ROS
# See this stackoverflow answer: https://stackoverflow.com/a/11217008/7718197
if (NOT catkin_FOUND)
    message(WARNING "MANUALLY LINKING TO OV_CORE LIBRARY....")
    include_directories(${ov_core_SOURCE_DIR}/src/)
    list(APPEND thirdparty_libraries ov_core_lib)
endif()

##################################################
# Make the core library
##################################################
list(APPEND library_source_files
        src/sim/Simulator.cpp
        src/state/State.cpp
        src/state/StateHelper.cpp
        src/state/Propagator.cpp
        src/core/VioManager.cpp
        src/update/UpdaterHelper.cpp
        src/update/UpdaterMSCKF.cpp
        src/update/UpdaterSLAM.cpp
)
if (catkin_FOUND)
    list(APPEND library_source_files
        src/core/RosVisualizer.cpp
    )
endif()
add_library(ov_msckf_lib STATIC ${library_source_files})
target_link_libraries(ov_msckf_lib ${thirdparty_libraries})
target_include_directories(ov_msckf_lib PUBLIC src)
set_property(TARGET ov_msckf_lib PROPERTY CXX_STANDARD 17)


##################################################
# Make binary files!
##################################################

if (ILLIXR_INTEGRATION)
    find_package(spdlog REQUIRED)
    set(PLUGIN_NAME plugin.openvins${ILLIXR_BUILD_SUFFIX})
    add_library(${PLUGIN_NAME} SHARED src/slam2.cpp)
    target_include_directories(${PLUGIN_NAME} PUBLIC ${ILLIXR_ROOT})
    target_link_libraries(${PLUGIN_NAME} ov_msckf_lib ${thirdparty_libraries} spdlog::spdlog)
    set_property(TARGET ${PLUGIN_NAME} PROPERTY CXX_STANDARD 17)
    install(TARGETS ov_msckf_lib DESTINATION lib)
    install(TARGETS ${PLUGIN_NAME} DESTINATION lib)
else()
    add_executable(run_illixr_msckf src/run_illixr_msckf.cpp)
    target_link_libraries(run_illixr_msckf ov_msckf_lib ${thirdparty_libraries})
    set_property(TARGET run_illixr_msckf PROPERTY CXX_STANDARD 17)
endif()
