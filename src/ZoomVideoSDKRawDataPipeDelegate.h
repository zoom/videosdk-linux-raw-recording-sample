// ffmpeg
#define __STDC_CONSTANT_MACROS

#include <vector>
#include <chrono>
using namespace std::chrono;


// Zoom Video SDK

#include "helpers/zoom_video_sdk_user_helper_interface.h"
using namespace ZOOMVIDEOSDK;

class ZoomVideoSDKRawDataPipeDelegate :
    private IZoomVideoSDKRawDataPipeDelegate
{
	virtual void onRawDataFrameReceived(YUVRawDataI420* data);
	virtual void onRawDataStatusChanged(RawDataStatus status);
	static ZoomVideoSDKRawDataPipeDelegate* find_instance(IZoomVideoSDKUser* user);

	int instance_id_;
	static int instance_count;
	static std::vector<ZoomVideoSDKRawDataPipeDelegate*> list_;
	IZoomVideoSDKUser* user_;



	
	unsigned char* frame_buffer_in;
	unsigned char* frame_buffer_out;
	int in_width = 0;
	int in_height = 0;
	int out_width = 1280;
	int out_height = 720;

	

	//Output YUV
	FILE* fp_yuv;
	int isOutputYUV = 0;

	
	int picture_size;
	int y_size;
	int framecnt = 0;
	int current_sourceID = -1;
	// struct _timeb start_tstruct;
	steady_clock::time_point start_time;

	//Output video file name.
	char fn_out[120];

public: 
	ZoomVideoSDKRawDataPipeDelegate(IZoomVideoSDKUser* user);
	ZoomVideoSDKRawDataPipeDelegate(IZoomVideoSDKUser* user, bool isShareScreen);
	~ZoomVideoSDKRawDataPipeDelegate();

	static void stop_encoding_for(IZoomVideoSDKUser* user);
		static void stop_encoding_for(IZoomVideoSDKUser* user, bool isShareScreen);
	static void log(const wchar_t* format, ...);
	static void err_msg(int code);
};



