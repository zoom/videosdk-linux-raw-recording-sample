
// Zoom Video SDK
#include "zoom_sdk_raw_data_def.h"
#include "zoom_video_sdk_def.h"
#include "helpers/zoom_video_sdk_audio_send_rawdata_interface.h"
using namespace ZOOMVIDEOSDK;

//needed for audio, applies to this entire .h file

class ZoomVideoSDKVirtualAudioSpeaker :
 public IZoomVideoSDKVirtualAudioSpeaker
{
public: 


    //IZoomVideoSDKVirtualAudioSpeaker

    virtual void onVirtualSpeakerMixedAudioReceived(AudioRawData* data_);
    virtual void onVirtualSpeakerOneWayAudioReceived(AudioRawData* data_, IZoomVideoSDKUser* pUser);
    virtual void onVirtualSpeakerSharedAudioReceived(AudioRawData* data_);

protected:


};



