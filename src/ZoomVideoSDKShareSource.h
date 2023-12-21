// Zoom Video SDK
#include "helpers/zoom_video_sdk_share_helper_interface.h"
#include "zoom_video_sdk_def.h"
using namespace ZOOMVIDEOSDK;

//needed for audio, applies to this entire .h file

class ZoomVideoSDKShareSource :
  public IZoomVideoSDKShareSource 
{
public: 

	IZoomVideoSDKShareSender* share_sender_;

  virtual void onShareSendStarted(IZoomVideoSDKShareSender* pSender) ;
	virtual void onShareSendStopped() ;

protected:


};



