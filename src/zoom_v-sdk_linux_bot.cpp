#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <map>
#include <algorithm>

#include "glib.h"
#include "helpers/zoom_video_sdk_user_helper_interface.h"
#include "zoom_video_sdk_api.h"
#include "zoom_video_sdk_def.h"
#include "zoom_video_sdk_delegate_interface.h"
#include "zoom_video_sdk_interface.h"
#include "zoom_video_sdk_platform.h"

//needed for audio
#include "ZoomVideoSDKVirtualAudioMic.h"
#include "ZoomVideoSDKVirtualAudioSpeaker.h"
#include "helpers/zoom_video_sdk_audio_send_rawdata_interface.h"

//needed for share screen
#include "ZoomVideoSDKShareSource.h"
#include "helpers/zoom_video_sdk_share_helper_interface.h"

//needed for get raw video
#include "ZoomVideoSDKRawDataPipeDelegate.h"
#include "ZoomVideoSDKVideoSource.h"
#include "helpers/zoom_video_sdk_video_helper_interface.h"

//needed for command channel
#include "helpers/zoom_video_sdk_cmd_channel_interface.h"

//needed for chat
#include "helpers/zoom_video_sdk_chat_helper_interface.h"
#include "zoom_video_sdk_chat_message_interface.h"

//needed for LTT
#include "zoom_video_sdk_session_info_interface.h"




USING_ZOOM_VIDEO_SDK_NAMESPACE
IZoomVideoSDK* video_sdk_obj;
GMainLoop* loop;

std::string session_name, session_psw, session_token;

//these are controls to demonstrate the flow
bool getRawAudio = false;
bool getRawVideo = false;
bool getRawShare = false;
bool sendRawVideo = false;
bool sendRawAudio = false;
bool sendRawShare = false;



std::string getSelfDirPath()
{
	char dest[PATH_MAX];
	memset(dest, 0, sizeof(dest)); // readlink does not null terminate!
	if (readlink("/proc/self/exe", dest, PATH_MAX) == -1)
	{
	}

	char* tmp = strrchr(dest, '/');
	if (tmp)
		*tmp = 0;
	printf("getpath\n");
	return std::string(dest);
}

class ZoomVideoSDKDelegate : public IZoomVideoSDKDelegate
{
public:
	/// \brief Triggered when user enter the session.
	virtual void onSessionJoin()
	{
		printf("Joined session successfully\n");

		if (sendRawAudio) {
			//needed for audio
			IZoomVideoSDKAudioHelper* m_pAudiohelper = video_sdk_obj->getAudioHelper();
			if (m_pAudiohelper) {
				// Connect User's audio.
				printf("Starting Audio\n");
				m_pAudiohelper->startAudio();
			}
		}

		if (sendRawShare) {
			//needed for share source
			//this needs to be done after joing session
			ZoomVideoSDKShareSource* virtual_share_source = new ZoomVideoSDKShareSource();
			ZoomVideoSDKErrors err2 = video_sdk_obj->getShareHelper()->startSharingExternalSource(virtual_share_source);
			if (err2 == ZoomVideoSDKErrors_Success) {
			}
			else {
				printf("Error setting external source %d\n", err2);
			}
		};

	
	
	
	}



	/// \brief Triggered when session leaveSession
	virtual void onSessionLeave()
	{
		g_main_loop_unref(loop);
		printf("Already left session.\n");
		exit(1);
	};


	virtual void onError(ZoomVideoSDKErrors errorCode, int detailErrorCode)
	{
		printf("join session errorCode : %d  detailErrorCode: %d\n", errorCode, detailErrorCode);
	};


	virtual void onUserJoin(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList)
	{
		if (getRawVideo) {
			if (userList) {
				int count = userList->GetCount();
				for (int index = 0; index < count; index++) {
					IZoomVideoSDKUser* user = userList->GetItem(index);
					if (user) {
						ZoomVideoSDKRawDataPipeDelegate* encoder = new ZoomVideoSDKRawDataPipeDelegate(user);
					}
				}
			}
		}
	};

	virtual void onUserLeave(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList)
	{
		if (getRawVideo) {
			if (userList) {
				int count = userList->GetCount();
				for (int index = 0; index < count; index++) {
					IZoomVideoSDKUser* user = userList->GetItem(index);
					if (user) {
						ZoomVideoSDKRawDataPipeDelegate::stop_encoding_for(user);
					}
				}
			}
		}
	};


	virtual void onUserVideoStatusChanged(IZoomVideoSDKVideoHelper* pVideoHelper,
		IVideoSDKVector<IZoomVideoSDKUser*>* userList) {};


	virtual void onUserAudioStatusChanged(IZoomVideoSDKAudioHelper* pAudioHelper,
		IVideoSDKVector<IZoomVideoSDKUser*>* userList) {
		if (getRawAudio) {
			IZoomVideoSDKAudioHelper* m_pAudiohelper = video_sdk_obj->getAudioHelper();
			if (m_pAudiohelper) {
				//needed for getting raw audio
				ZoomVideoSDKErrors err = m_pAudiohelper->subscribe();
				printf("subscribe status is %d\n", err);
			}
		}
	};
	virtual void onUserShareStatusChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, ZoomVideoSDKShareStatus status, ZoomVideoSDKShareType type) {
		if (getRawShare) {
			if (status == ZoomVideoSDKShareStatus_Start) {
				ZoomVideoSDKRawDataPipeDelegate* encoder = new ZoomVideoSDKRawDataPipeDelegate(pUser, true);
			}
			else if (status == ZoomVideoSDKShareStatus_Stop) {
				ZoomVideoSDKRawDataPipeDelegate::stop_encoding_for(pUser, true);
			}
		}
	}

	virtual void onUserRecordingConsent(IZoomVideoSDKUser* pUser) {};


	virtual void onLiveStreamStatusChanged(IZoomVideoSDKLiveStreamHelper* pLiveStreamHelper, ZoomVideoSDKLiveStreamStatus status) {};


	virtual void onChatNewMessageNotify(IZoomVideoSDKChatHelper* pChatHelper, IZoomVideoSDKChatMessage* messageItem) {
	
	};


	virtual void onUserHostChanged(IZoomVideoSDKUserHelper* pUserHelper, IZoomVideoSDKUser* pUser) {};


	virtual void onUserActiveAudioChanged(IZoomVideoSDKAudioHelper* pAudioHelper,
		IVideoSDKVector<IZoomVideoSDKUser*>* list) {};


	virtual void onSessionNeedPassword(IZoomVideoSDKPasswordHandler* handler) {};


	virtual void onSessionPasswordWrong(IZoomVideoSDKPasswordHandler* handler) {};

	//this is a helper method, and not part of the implementation
	void savePcmBufferToFile(const std::string& filename, char* pcmBuffer, std::size_t bufferSize) {
		std::ofstream outfile(filename, std::ios::out | std::ios::binary | std::ios::app);
		outfile.write(reinterpret_cast<char*>(pcmBuffer), bufferSize);
		outfile.close();
		if (!outfile) {
			std::cerr << "Error writing PCM data to file!" << std::endl;
		}
		else {
			std::cout << "PCM data saved to file: " << filename << std::endl;
		}
	}

	virtual void onMixedAudioRawDataReceived(AudioRawData* data_) {
		if (getRawAudio) {
			std::string filename = "output.pcm";
			printf("onMixedAudioRawDataReceived\n");
			if (data_) {
				savePcmBufferToFile(filename, data_->GetBuffer(), data_->GetBufferLen());
				printf("Data buffer: %s\n", data_->GetBuffer());
				printf("Length is : %d\n", data_->GetBufferLen());
				printf("Sample is : %d\n", data_->GetSampleRate());
				printf("Channel is : %d\n", data_->GetChannelNum());
			}
		}
	};

	virtual void onOneWayAudioRawDataReceived(AudioRawData* data_, IZoomVideoSDKUser* pUser) {
		if (getRawAudio) {
			std::string filename = pUser->getUserID();
			std::string extension = ".pcm";
			filename.append(extension);

			printf("onOneWayAudioRawDataReceived\n");
			if (data_) {
				savePcmBufferToFile(filename, data_->GetBuffer(), data_->GetBufferLen());
				printf("Data buffer: %s\n", data_->GetBuffer());
				printf("Length is : %d\n", data_->GetBufferLen());
				printf("Sample is : %d\n", data_->GetSampleRate());
				printf("Channel is : %d\n", data_->GetChannelNum());
			}
		}
	};

	virtual void onSharedAudioRawDataReceived(AudioRawData* data_) {};


	virtual void onUserManagerChanged(IZoomVideoSDKUser* pUser) {};


	virtual void onUserNameChanged(IZoomVideoSDKUser* pUser) {};


	virtual void onCameraControlRequestResult(IZoomVideoSDKUser* pUser, bool isApproved) {};

	virtual void onCameraControlRequestReceived(
		IZoomVideoSDKUser* pUser,
		ZoomVideoSDKCameraControlRequestType requestType,
		IZoomVideoSDKCameraControlRequestHandler* pCameraControlRequestHandler) {};


	virtual void onCommandReceived(IZoomVideoSDKUser* sender, const zchar_t* strCmd) {
		
	}
	virtual void onCommandChannelConnectResult(bool isSuccess) {
	
	};
	virtual void onInviteByPhoneStatus(PhoneStatus status, PhoneFailedReason reason) {};
	virtual void onCloudRecordingStatus(RecordingStatus status, IZoomVideoSDKRecordingConsentHandler* pHandler) {
	
	};
	virtual void onHostAskUnmute() {};


	virtual void onMultiCameraStreamStatusChanged(ZoomVideoSDKMultiCameraStreamStatus status, IZoomVideoSDKUser* pUser, IZoomVideoSDKRawDataPipe* pVideoPipe) {}
	virtual void onMicSpeakerVolumeChanged(unsigned int micVolume, unsigned int speakerVolume) {}
	virtual void onAudioDeviceStatusChanged(ZoomVideoSDKAudioDeviceType type, ZoomVideoSDKAudioDeviceStatus status) {}
	virtual void onTestMicStatusChanged(ZoomVideoSDK_TESTMIC_STATUS status) {}
	virtual void onSelectedAudioDeviceChanged() {}

	/// \brief Notify that the camera list has changed.
	virtual void onCameraListChanged() {};

	virtual void onLiveTranscriptionStatus(ZoomVideoSDKLiveTranscriptionStatus status) {
	

	};
	virtual void onLiveTranscriptionMsgReceived(const zchar_t* ltMsg, IZoomVideoSDKUser* pUser, ZoomVideoSDKLiveTranscriptionOperationType type) {
		
	};
	virtual void onLiveTranscriptionMsgInfoReceived(ILiveTranscriptionMessageInfo* messageInfo) {
		
	};
	virtual void onLiveTranscriptionMsgError(ILiveTranscriptionLanguage* spokenLanguage, ILiveTranscriptionLanguage* transcriptLanguage) {
		
	};
	virtual void onChatMsgDeleteNotification(IZoomVideoSDKChatHelper* pChatHelper, const zchar_t* msgID, ZoomVideoSDKChatMessageDeleteType deleteBy) {
		


	};

	virtual void onProxyDetectComplete() {};
	virtual void onProxySettingNotification(IZoomVideoSDKProxySettingHandler* handler) {};
	virtual void onSSLCertVerifiedFailNotification(IZoomVideoSDKSSLCertificateInfo* info) {};
	virtual void onUserVideoNetworkStatusChanged(ZoomVideoSDKNetworkStatus status, IZoomVideoSDKUser* pUser) {};
	virtual void onCallCRCDeviceStatusChanged(ZoomVideoSDKCRCCallStatus status) {};

	virtual void onVirtualSpeakerMixedAudioReceived(AudioRawData* data_) {

		printf("onVirtualSpeakerMixedAudioReceived() main \n");
		printf("data %s \n", data_->GetBuffer());

	};

	virtual void onVirtualSpeakerOneWayAudioReceived(AudioRawData* data_, IZoomVideoSDKUser* pUser) {

		printf("onVirtualSpeakerOneWayAudioReceived() main\n");
		printf("data %s \n", data_->GetBuffer());
	};

	virtual void onVirtualSpeakerSharedAudioReceived(AudioRawData* data_) {
		printf("onVirtualSpeakerSharedAudioReceived() main\n");
		printf("data %s \n", data_->GetBuffer());
	};

	virtual void onOriginalLanguageMsgReceived(ILiveTranscriptionMessageInfo* messageInfo) {};
	virtual void onChatPrivilegeChanged(IZoomVideoSDKChatHelper* pChatHelper, ZoomVideoSDKChatPrivilegeType privilege) {};
	virtual void onSendFileStatus(IZoomVideoSDKSendFile* file, const FileTransferStatus& status) {};
	virtual void onReceiveFileStatus(IZoomVideoSDKReceiveFile* file, const FileTransferStatus& status) {};

	virtual void onVideoCanvasSubscribeFail(ZoomVideoSDKSubscribeFailReason fail_reason, IZoomVideoSDKUser* pUser, void* handle) {};
	virtual void onShareCanvasSubscribeFail(ZoomVideoSDKSubscribeFailReason fail_reason, IZoomVideoSDKUser* pUser, void* handle) {};
	virtual void onAnnotationHelperCleanUp(IZoomVideoSDKAnnotationHelper* helper) {};
	virtual void onAnnotationPrivilegeChange(IZoomVideoSDKUser* pUser, bool enable) {};
	virtual void onAnnotationHelperActived(void* handle) {};
};

void joinVideoSDKSession(std::string& session_name, std::string& session_psw, std::string& session_token)
{
	ZoomVideoSDKRawDataMemoryMode heap = ZoomVideoSDKRawDataMemoryMode::ZoomVideoSDKRawDataMemoryModeHeap;
	video_sdk_obj = CreateZoomVideoSDKObj();
	ZoomVideoSDKInitParams init_params;
	init_params.domain = "https://go.zoom.us";
	init_params.enableLog = true;
	init_params.logFilePrefix = "zoom_videosdk_demo";
	init_params.videoRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
	init_params.shareRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
	init_params.audioRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
	init_params.enableIndirectRawdata = false;

	ZoomVideoSDKErrors err = video_sdk_obj->initialize(init_params);
	if (err != ZoomVideoSDKErrors_Success)
	{
		return;
	}
	IZoomVideoSDKDelegate* listener = new ZoomVideoSDKDelegate();
	video_sdk_obj->addListener(dynamic_cast<IZoomVideoSDKDelegate*>(listener));

	ZoomVideoSDKSessionContext session_context;
	session_context.sessionName = session_name.c_str();
	session_context.sessionPassword = session_psw.c_str();
	session_context.userName = "Linux Bot";
	session_context.token = session_token.c_str();
	session_context.videoOption.localVideoOn = true;
	session_context.audioOption.connect = false;
	session_context.audioOption.mute = true;


	if (getRawVideo) {
		//nothing much to do before joining session
	}
	if (getRawShare) {
		//nothing much to do before joining session
	}

	if (getRawAudio) {
		//this code to load virtualaudiospeaker is needed if you are using headless linux, or linux which does not come with soundcard.
		//if you do not wish to load virtualaudiospeaker, you can alternatively install `apt install pulseaudio` on your linux distro
		//ZoomVideoSDKVirtualAudioSpeaker* vSpeaker  =new ZoomVideoSDKVirtualAudioSpeaker();
		//session_context.virtualAudioSpeaker =vSpeaker;

		session_context.audioOption.connect = true;

		//dreamtcs check if this needed
		//ZoomVideoSDKVirtualAudioMic* vMic  =new ZoomVideoSDKVirtualAudioMic();
		//session_context.virtualAudioMic=vMic;
	}

	if (sendRawVideo) {
		//needed for send raw video
		//the sdk uses a Video Source to send raw video
		//this needs to be done before joining session
		ZoomVideoSDKVideoSource* virtual_video_source = new ZoomVideoSDKVideoSource();
		session_context.externalVideoSource = virtual_video_source;
	}
	if (sendRawShare) {
		//nothing much to do before joining session
	}

	if (sendRawAudio) {
		session_context.audioOption.connect = true; //needed for sending raw audio data
		session_context.audioOption.mute = false; //needed for sending raw audio data

		//ZoomVideoSDKVirtualAudioMic is the object used to send audio
		ZoomVideoSDKVirtualAudioMic* vMic = new ZoomVideoSDKVirtualAudioMic();
		session_context.virtualAudioMic = vMic;
	}

	//join the session
	IZoomVideoSDKSession* session = NULL;
	if (video_sdk_obj)
		session = video_sdk_obj->joinSession(session_context);

}

// Function to process a line containing a key-value pair
void processLine(const std::string& line, std::map<std::string, std::string>& config) {
	// Find the position of the ':' character
	size_t colonPos = line.find(':');

	if (colonPos != std::string::npos) {
		// Extract the key and value parts
		std::string key = line.substr(0, colonPos);
		std::string value = line.substr(colonPos + 1);

		// Remove leading/trailing whitespaces from the key and value
		key.erase(0, key.find_first_not_of(" \t"));
		key.erase(key.find_last_not_of(" \t") + 1);
		value.erase(0, value.find_first_not_of(" \t"));
		value.erase(value.find_last_not_of(" \t") + 1);

		// Remove double-quote characters and carriage return ('\r') from the value
		value.erase(std::remove_if(value.begin(), value.end(), [](char c) { return c == '"' || c == '\r'; }), value.end());
	
		// Store the key-value pair in the map
		config[key] = value;
	}
}

void ReadTEXTSettings()
{

	std::string self_dir = getSelfDirPath();
	printf("self path: %s\n", self_dir.c_str());
	self_dir.append("/config.txt");

	std::ifstream configFile(self_dir.c_str());
	if (!configFile) {
		std::cerr << "Error opening config file." << std::endl;
	}
	else {

		std::cerr << "Readfile success." << std::endl;
	}

	std::map<std::string, std::string> config;
	std::string line;

	while (std::getline(configFile, line)) {
		// Process each line to extract key-value pairs
		processLine(line, config);

		std::cerr << "Reading.." << line << std::endl;
	}

	// Example: Accessing values by key
	if (config.find("session_name") != config.end()) {

		session_name = config["session_name"];
		std::cout << "session_name: " << config["session_name"] << std::endl;
	}
	if (config.find("session_token") != config.end()) {
		session_token = config["session_token"];
		std::cout << "session_token: " << session_token << std::endl;
	}
	if (config.find("session_psw") != config.end()) {

		session_psw = config["session_psw"];
		std::cout << "session_psw: " << session_psw << std::endl;
	}
	
	if (config.find("GetVideoRawData") != config.end()) {
		std::cout << "GetVideoRawData before parsing is : " << config["GetVideoRawData"] << std::endl;

		if (config["GetVideoRawData"] == "true") {
			getRawVideo = true;
		}
		else {
			getRawVideo = false;
		}
		std::cout << "GetVideoRawData: " << getRawVideo << std::endl;
	}
	if (config.find("GetAudioRawData") != config.end()) {
		std::cout << "GetAudioRawData before parsing is : " << config["GetAudioRawData"] << std::endl;

		if (config["GetAudioRawData"] == "true") {
			getRawAudio = true;
		}
		else {
			getRawAudio = false;
		}
		std::cout << "GetAudioRawData: " << getRawAudio << std::endl;
	}

	if (config.find("SendVideoRawData") != config.end()) {
		std::cout << "SendVideoRawData before parsing is : " << config["SendVideoRawData"] << std::endl;

		if (config["SendVideoRawData"] == "true") {
			sendRawVideo = true;
		}
		else {
			sendRawVideo = false;
		}
		std::cout << "SendVideoRawData: " << sendRawVideo << std::endl;
	}
	if (config.find("SendAudioRawData") != config.end()) {
		std::cout << "SendAudioRawData before parsing is : " << config["SendAudioRawData"] << std::endl;

		if (config["SendAudioRawData"] == "true") {
			sendRawAudio = true;
		}
		else {
			sendRawAudio = false;
		}
		std::cout << "SendAudioRawData: " << sendRawAudio << std::endl;
	}

	// Additional processing or handling of parsed values can be done here

	printf("directory of config file: %s\n", self_dir.c_str());


}

gboolean timeout_callback(gpointer data)
{
	return TRUE;
}

void my_handler(int s)
{
	printf("\nCaught signal %d\n", s);
	video_sdk_obj->leaveSession(false);
	printf("Leaving session.\n");
}

int main(int argc, char* argv[])
{
	std::string self_dir = getSelfDirPath();
	printf("self path: %s\n", self_dir.c_str());
	self_dir.append("/config.txt");

	std::ifstream t(self_dir.c_str());
	t.seekg(0, std::ios::end);
	size_t size = t.tellg();
	std::string buffer(size, ' ');
	t.seekg(0);
	t.read(&buffer[0], size);

	

	ReadTEXTSettings();
	

	printf("begin to join: %s\n", self_dir.c_str());
	joinVideoSDKSession(session_name, session_psw, session_token);


	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = my_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

	loop = g_main_loop_new(NULL, FALSE);

	// add source to default context
	g_timeout_add(100, timeout_callback, loop);
	g_main_loop_run(loop);
	return 0;
}