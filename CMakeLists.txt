cmake_minimum_required(VERSION 3.16)

project(open_vins)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE DEBUG)

add_definitions(-DTIMING_DEBUG -DSMP_DEBUG)

if (CMAKE_CROSSCOMPILING)
    add_definitions(-DCMAKE_CROSSCOMPILING)
endif()

if (NOT CMAKE_CROSSCOMPILING)
    find_package(Tracy REQUIRED)
endif()

if(${Tracy_FOUND})
    add_definitions(-DTRACY_ENABLE)
    get_target_property(Tracy_INCLUDE_DIR Tracy::TracyClient INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(Tracy_LIBRARIES Tracy::TracyClient LOCATION)
    message(STATUS "Tracy path: " ${Tracy_INCLUDE_DIR})
    message(STATUS "Tracy libs: " ${Tracy_LIBRARIES})
    list(APPEND thirdparty_libraries
        ${Tracy_LIBRARIES}
    )
    include_directories(BEFORE
        ${Tracy_INCLUDE_DIR}
    )
endif()

# TODO: fetch and build directly from github
if (CMAKE_CROSSCOMPILING)
    find_package(OpenCV REQUIRED PATHS "/home/zekailin00/Desktop/opencv/build-debug-rvv/drops-debug-rvv")
else()
    find_package(OpenCV REQUIRED)
endif()

if (CMAKE_CROSSCOMPILING)
    # OpenBLAS CrossCompiling Settings
    set(NOFORTRAN "1")
    set(DYNAMIC_ARCH OFF)
    set(BUILD_WITHOUT_LAPACK OFF)
    set(BUILD_TESTING OFF)
    set(BUILD_BENCHMARKS OFF)
    set(TARGET "RISCV64_GENERIC")
    add_definitions(-DGEMMINI_BACKEND)

    #OpenVINS CrosCompiling Settings
    add_definitions(-DCYCLE_COUNT_BENCH)
endif()

add_definitions(-DEIGEN_USE_BLAS=1)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/OpenBLAS)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/eigen)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/boost)


if(${OpenCV_VERSION_MAJOR} EQUAL 4)
    add_definitions(-DUSING_OPENCV4)
endif()

include_directories(.)
list(APPEND thirdparty_libraries
    openblas_static
    Eigen3::Eigen

    Boost::date_time
    Boost::filesystem
    Boost::system
    Boost::thread
    Boost::math

    opencv_calib3d
    opencv_core
    opencv_dnn
    opencv_features2d
    opencv_flann
    # opencv_gapi
    opencv_highgui
    opencv_imgcodecs
    opencv_imgproc
    opencv_ml
    opencv_objdetect
    opencv_photo
    opencv_stitching
    opencv_video
    opencv_videoio
    opencv_aruco
    opencv_bgsegm
    opencv_bioinspired
    opencv_ccalib
    opencv_datasets
    opencv_dnn_objdetect
    opencv_dnn_superres
    opencv_dpm
    opencv_face
    opencv_fuzzy
    opencv_hfs
    opencv_img_hash
    opencv_intensity_transform
    opencv_line_descriptor
    opencv_mcc
    opencv_optflow
    opencv_phase_unwrapping
    opencv_plot
    opencv_quality
    opencv_rapid
    opencv_reg
    opencv_rgbd
    opencv_saliency
    opencv_shape
    # opencv_signal
    opencv_stereo
    opencv_structured_light
    opencv_superres
    opencv_surface_matching
    opencv_text
    opencv_tracking
    opencv_videostab
    # opencv_wechat_qrcode
    # opencv_xfeatures2d
    opencv_ximgproc
    opencv_xobjdetect
    opencv_xphoto
)

message(("-- OpenCV: ${OpenCV_LIBS}"))

# Find catkin (the ROS build system)
#find_package(catkin QUIET COMPONENTS roscpp rosbag tf std_msgs geometry_msgs sensor_msgs nav_msgs visualization_msgs cv_bridge ov_core)
if (catkin_FOUND)
    add_definitions(-DROS_AVAILABLE=1)
    catkin_package(
            CATKIN_DEPENDS roscpp rosbag tf std_msgs geometry_msgs sensor_msgs nav_msgs visualization_msgs cv_bridge ov_core
            INCLUDE_DIRS src
            LIBRARIES ov_msckf_lib
    )
    include_directories(${catkin_INCLUDE_DIRS})
    list(APPEND thirdparty_libraries ${catkin_LIBRARIES})
else()
    message(WARNING "CATKIN NOT FOUND BUILDING WITHOUT ROS!")
endif()

if (ILLIXR_INTEGRATION)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DILLIXR_INTEGRATION")
endif()



# # Enable compile optimizations
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fsee -fomit-frame-pointer -fno-signed-zeros -fno-math-errno -funroll-loops")
# # Enable debug flags (use if you want to debug in gdb)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -Wall -Wuninitialized")


# add_subdirectory(ov_eval)
add_subdirectory(ov_core)
add_subdirectory(ov_msckf)
