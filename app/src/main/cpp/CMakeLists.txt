
cmake_minimum_required(VERSION 3.4.1)

file(GLOB allCpp *.cpp)

add_library(
        my-player  #库的名字


        SHARED


        ${allCpp})


find_library(
        log-lib


        log)

#导入ffmpeg的头文件
include_directories(${CMAKE_SOURCE_DIR}/ffmpeg/include/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/ffmpeg/libs/${CMAKE_ANDROID_ARCH_ABI}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/rtmp/libs/${CMAKE_ANDROID_ARCH_ABI}")
target_link_libraries(
        my-player
        -Wl,--start-group
        avcodec avfilter avformat avutil swresample swscale
        -Wl,--end-group
        ${log-lib}
        z
        rtmp)