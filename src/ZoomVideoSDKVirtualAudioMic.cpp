#include "ZoomVideoSDKVirtualAudioMic.h"
#include "helpers/zoom_video_sdk_user_helper_interface.h"

using namespace ZOOMVIDEOSDK;

//needed for audio, applies to this entire .cpp file

void ZoomVideoSDKVirtualAudioMic::SendRawAudio(char* data, unsigned int data_length, int sample_rate)
{
    if (!virtual_audio_sender_)
    {
        return;
    }

    // See zoom_video_sdk_audio_send_rawdata_interface.h for raw audio data information
    virtual_audio_sender_->Send(data, data_length, sample_rate);
}

void ZoomVideoSDKVirtualAudioMic::onMicInitialize(IZoomVideoSDKAudioSender* rawdata_sender)
{
    printf("onMicInitialize\n");
    // Once the sender has been recieved from this callback, then Send can be called
    virtual_audio_sender_ = rawdata_sender;
}

void ZoomVideoSDKVirtualAudioMic::onMicStartSend()
{
    //sendRawAudio
    printf("onMicStartSend\n");
    if (!virtual_audio_sender_)
    {
        return;
    }

    // Virtual Mic began sending raw audio
}

void ZoomVideoSDKVirtualAudioMic::onMicStopSend()
{
    if (!virtual_audio_sender_)
    {
        return;
    }

    // Virtual Mic stopped sending raw audio
}

void ZoomVideoSDKVirtualAudioMic::onMicUninitialized()
{
    virtual_audio_sender_ = nullptr;
}
