# Zoom Video SDK Linux Sample App

The is a sample app for Zoom Video SDK Linux. It demonstrates access to raw audio and raw video in a headless docker environment.


## Prerequistes
1. [Zoom Video SDK Account](https://zoom.us/buy/videosdk)
2. [Zoom Video SDK App Type](https://marketplace.zoom.us)
2. Authentication token signed by 
    1. Video SDK Key
    2. Video SDK Secret
3. Docker environment
4. Linux or Windows with WSL environment.
5. VS Code or Visual Studio
 ======================================================================


## Getting Started

#### Install necessary dependencies (on your local or remote linux machine)

```
sudo apt update
sudo apt install -y build-essential gcc cmake
sudo apt install -y libglib2.0-dev liblzma-dev libxcb-image0 libxcb-keysyms1 libxcb-xfixes0 libxcb-xkb1 libxcb-shape0 libxcb-shm0 libxcb-randr0 libxcb-xtest0 libgbm1 libxtst6 libgl1 libnss3  libasound2 libpulse0
```


### Download Linux Video SDK from marketplace.zoom.us

1 Please decompress the downloaded `zoom-video-sdk-linux_x86_64-1.10.x.tar` and copy the SDK files from the decompressed folder to these sample code folders

 - copy `/qt_libs/*.*` to `/lib/zoom_video_sdk/qt_libs/`
 - 4 of the `lib******.so` files to `/lib/zoom_video_sdk/lib******.so`
 - copy `/h/*.*` folder to `/include/zoom_video_sdk/`
 - softlink  `libvideosdk.so` to `libvideosdk.so.1` within `/lib/zoom_video_sdk/` . You can use the command `ln -s libvideosdk.so libvideosdk.so.1` to do so.



2 Add `config.txt` in your root directory
its content will look something like this
```
session_name: "yoursessionname"
session_token: "xxxx.yyyy.zzzz"
session_psw: "12345678"
GetVideoRawData: "true"
GetAudioRawData: "true"
SendVideoRawData: "false"
SendAudioRawData: "false"
```
#### Disclaimer
Please be aware that all hard-coded variables and constants shown in the documentation and in the demo, such as Zoom Token, Zoom Access, Token, etc., are ONLY FOR DEMO AND TESTING PURPOSES. We STRONGLY DISCOURAGE the way of HARDCODING any Zoom Credentials (username, password, API Keys & secrets, SDK keys & secrets, etc.) or any Personal Identifiable Information (PII) inside your application. WE DON’T MAKE ANY COMMITMENTS ABOUT ANY LOSS CAUSED BY HARD-CODING CREDENTIALS OR SENSITIVE INFORMATION INSIDE YOUR APP WHEN DEVELOPING WITH OUR SDK.



3 Execute `cmake -B build` in root folder . After successful `cmake`, the `/build/` folder will be generated.

4 Execute `make` in the `/build/` directory to compile. This will generate a `bin` folder in the root directory

5 Run the app `./zoom_v-sdk_linux_bot` from the generated `bin` folder:






## CMakeLists.txt
If you have additional cpp and/or .h files, you might need to be included in this CMakeLists.txt

```
add_executable(zoom_v-sdk_linux_bot ${CMAKE_SOURCE_DIR}/src/ZoomVideoSDKRawDataPipeDelegate.cpp  
                                ${CMAKE_SOURCE_DIR}/src/ZoomVideoSDKVirtualAudioMic.cpp 
                                ${CMAKE_SOURCE_DIR}/src/ZoomVideoSDKVirtualAudioSpeaker.cpp 
                                 ${CMAKE_SOURCE_DIR}/src/ZoomVideoSDKShareSource.cpp 
                                 ${CMAKE_SOURCE_DIR}/src/ZoomVideoSDKVideoSource.cpp 
                                 ${CMAKE_SOURCE_DIR}/src/zoom_v-sdk_linux_bot.cpp  
                                 )
```

Similarly for libraries

```
target_link_libraries(zoom_v-sdk_linux_bot PkgConfig::deps)
target_link_libraries(zoom_v-sdk_linux_bot videosdk)
target_link_libraries(zoom_v-sdk_linux_bot z pthread avformat)
target_link_libraries(zoom_v-sdk_linux_bot z lzma swresample avcodec)
target_link_libraries(zoom_v-sdk_linux_bot avutil)
target_link_libraries(zoom_v-sdk_linux_bot swscale avfilter)
target_link_libraries(zoom_v-sdk_linux_bot curl)
```

## Walkthru on samples

Here are some boolean variables which you can use to trace the code.
These wilL demonstrate some of the requirements needed to get / send raw data.

```
  //these are controls to demonstrate the flow
    bool getRawAudio = false;
    bool getRawVideo = false;
    bool getRawShare = false;
    bool sendRawVideo = false;
    bool sendRawAudio = false;
    bool sendRawShare = true;
```

### Additional Raw Audio requirements
For raw audio access to work, here are some high level requirements
- if you are using a normal desktop linux, there is no need for virtualspeaker or PulseAudio
- if you are using a headless linux (wsl, docker, ubuntu server) without soundcard, you will need to use virtualspeaker or PulseAudio




#### Pulseaudio Setup

The configuration for pulseaudio has been provided for you as shell scripts.

The setup is done via `setup-pulseaudio.sh`, this need to be run prior to running this project in a docker environment.

The script 
- starts the pulseaudio service, 
- creates a virtual speaker, 
- a virtual microphone, and 
- add a ~/.config/zoomus.conf file in the docker environment with the content 
```
[General]
system.audio.type=default
```

## Docker

The sample app has been tested on Ubuntu 22 and Ubuntu 23.
Refer to the `Dockerfile` in `Dockerfile-Ubuntu` folder



## Need help?

If you're looking for help, try [Developer Support](https://devsupport.zoom.us) or
our [Developer Forum](https://devforum.zoom.us). Priority support is also available
with [Premier Developer Support](https://zoom.us/docs/en-us/developer-support-plans.html) plans.

### Documentation
Make sure to review [our documentation](https://marketplace.zoom.us/docs/zoom-apps/introduction/) as a reference when building your Zoom Apps.

