cmake_minimum_required(VERSION 3.14)

project(VideoSDKSessionRecorder CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
add_definitions(-std=c++11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/zoom_video_sdk)
include_directories(${CMAKE_SOURCE_DIR}/include/ffmpeg)

link_directories(${CMAKE_SOURCE_DIR}/lib/zoom_video_sdk)
link_directories(${CMAKE_SOURCE_DIR}/lib/ffmpeg)

add_executable(zoom_v-sdk_linux_bot ${CMAKE_SOURCE_DIR}/src/ZoomVideoSDKRawDataPipeDelegate.cpp  
                                ${CMAKE_SOURCE_DIR}/src/ZoomVideoSDKVirtualAudioMic.cpp 
                                ${CMAKE_SOURCE_DIR}/src/ZoomVideoSDKVirtualAudioSpeaker.cpp 
                                 ${CMAKE_SOURCE_DIR}/src/ZoomVideoSDKShareSource.cpp 
                                 ${CMAKE_SOURCE_DIR}/src/ZoomVideoSDKVideoSource.cpp 
                                 ${CMAKE_SOURCE_DIR}/src/zoom_v-sdk_linux_bot.cpp  
                                 )

target_link_libraries(zoom_v-sdk_linux_bot PkgConfig::deps)
target_link_libraries(zoom_v-sdk_linux_bot videosdk)
target_link_libraries(zoom_v-sdk_linux_bot z pthread avformat)
target_link_libraries(zoom_v-sdk_linux_bot z lzma swresample avcodec)
target_link_libraries(zoom_v-sdk_linux_bot avutil)
target_link_libraries(zoom_v-sdk_linux_bot swscale avfilter)
target_link_libraries(zoom_v-sdk_linux_bot curl)

configure_file(${CMAKE_SOURCE_DIR}/config.txt ${CMAKE_SOURCE_DIR}/bin/config.txt COPYONLY)
file(COPY ${CMAKE_SOURCE_DIR}/lib/zoom_video_sdk/ DESTINATION ${CMAKE_SOURCE_DIR}/bin)

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/ffmpeg")
    execute_process(COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/lib/ffmpeg COMMAND tar -xf ${CMAKE_SOURCE_DIR}/lib/ffmpeg.tar.gz -C lib/ffmpeg WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/zoom_video_sdk/qt_libs")
    execute_process(COMMAND tar -xf ${CMAKE_SOURCE_DIR}/lib/qt_libs.tar.gz -C lib/zoom_video_sdk WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

execute_process(COMMAND mkdir -p .zoom/logs WORKING_DIRECTORY $ENV{HOME})
