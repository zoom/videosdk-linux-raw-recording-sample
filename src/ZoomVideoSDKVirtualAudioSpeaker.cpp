
#include "ZoomVideoSDKVirtualAudioSpeaker.h"
#include "helpers/zoom_video_sdk_user_helper_interface.h"

using namespace ZOOMVIDEOSDK;

void ZoomVideoSDKVirtualAudioSpeaker::onVirtualSpeakerMixedAudioReceived(AudioRawData* data_)
{
    // Handle mixed audio raw data here
    // data_->GetBuffer();
    // data_->GetBufferLen();
    // data_->GetChannelNum();
    // data_->GetSampleRate();
    	 //printf("onVirtualSpeakerMixedAudioReceived() \n");
         //	printf("data %s \n",  data_->GetBuffer());
        //     printf("length %n \n",  data_->GetBufferLen());
        //      printf("channel %n \n",  data_->GetChannelNum());
        //       printf("sample %n \n",  data_->GetSampleRate());
}

void ZoomVideoSDKVirtualAudioSpeaker::onVirtualSpeakerOneWayAudioReceived(AudioRawData* data_, IZoomVideoSDKUser* pUser)
{
    // Handle audio raw data from single user here
    // data_->GetBuffer();
    // data_->GetBufferLen();
    // data_->GetChannelNum();
    // data_->GetSampleRate();
    // pUser->getUserName();
    	 //printf("onVirtualSpeakerOneWayAudioReceived() \n");
     	//printf("data %s \n",  data_->GetBuffer());
}

void ZoomVideoSDKVirtualAudioSpeaker::onVirtualSpeakerSharedAudioReceived(AudioRawData* data_)
{
    // Handle audio raw data from share here
    // data_->GetBuffer();
    // data_->GetBufferLen();
    // data_->GetChannelNum();
    // data_->GetSampleRate();
    	//printf("onVirtualSpeakerSharedAudioReceived() \n");
}

