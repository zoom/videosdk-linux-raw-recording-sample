
// Zoom Video SDK

#include "zoom_sdk_raw_data_def.h"
#include "zoom_video_sdk_def.h"
#include "helpers/zoom_video_sdk_audio_send_rawdata_interface.h"
using namespace ZOOMVIDEOSDK;

//needed for audio, applies to this entire .h file

class ZoomVideoSDKVirtualAudioMic :
  public IZoomVideoSDKVirtualAudioMic 
  {
public: 


   void SendRawAudio(char* data, unsigned int data_length, int sample_rate);


    //IZoomVideoSDKVirtualAudioMic
    virtual void onMicInitialize(ZOOM_VIDEO_SDK_NAMESPACE::IZoomVideoSDKAudioSender* rawdata_sender);
    virtual void onMicStartSend();
    virtual void onMicStopSend();
    virtual void onMicUninitialized();


protected:
    ZOOM_VIDEO_SDK_NAMESPACE::IZoomVideoSDKAudioSender* virtual_audio_sender_;

};



