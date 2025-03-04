cmake_minimum_required(VERSION 3.16)

option(ILLIXR_INTEGRATION "Flag enabling integrated (non-standalone) mode" OFF)

# Project name
project(ov_core)

##################################################
# Make the core library
##################################################
add_library(ov_core_lib STATIC
        src/dummy.cpp
        src/init/InertialInitializer.cpp
        src/sim/BsplineSE3.cpp
        src/track/TrackBase.cpp
        src/track/TrackAruco.cpp
        src/track/TrackDescriptor.cpp
        src/track/TrackKLT.cpp
        src/track/TrackSIM.cpp
        src/types/Landmark.cpp
        src/feat/Feature.cpp
        src/feat/FeatureInitializer.cpp
)
target_link_libraries(ov_core_lib ${thirdparty_libraries})
target_include_directories(ov_core_lib PUBLIC src)
set_property(TARGET ov_core_lib PROPERTY CXX_STANDARD 17)

if(ILLIXR_INTEGRATION)
    install(TARGETS ov_core_lib DESTINATION lib)
endif()
##################################################
# Make binary files!
##################################################
#if (catkin_FOUND)
#    add_executable(test_tracking src/test_tracking.cpp)
#    target_link_libraries(test_tracking ov_core_lib ${thirdparty_libraries})
#endif()
#
#
#add_executable(test_webcam src/test_webcam.cpp)
#target_link_libraries(test_webcam ov_core_lib ${thirdparty_libraries})
