
#pragma once

#define LIB_EASYSLIVE_API __declspec(dllexport)

#define EASYSLIVE_HANDLE void*

// �ӿں������� [12/14/2017 Dingshuai]
typedef enum tagSOURCE_TYPE
{
	SOURCE_LOCAL_CAMERA = 0,//��������Ƶ
	SOURCE_SCREEN_CAPTURE =1,//��Ļ����
	SOURCE_FILE_STREAM = 2,       //�ļ�������(mp4,ts,flv???)
	SOURCE_RTSP_STREAM=3,//RTSP��
	SOURCE_RTMP_STREAM=4,//RTMP��
	// 	Any other Source to push

}SOURCE_TYPE;

typedef enum tagPUSH_TYPE
{
	PUSH_NONE = 0,
	PUSH_RTSP ,
	PUSH_RTMP ,
	//ADD ANY OTHER PUSH TYPE

}PUSH_TYPE;

typedef struct tagEASYLIVE_DEVICE_INFO_T
{
	char	friendlyName[64];

	tagEASYLIVE_DEVICE_INFO_T	*pNext;
}EASYLIVE_DEVICE_INFO_T;

typedef struct tagEASYLIVE_DEVICE_LIST_T
{
	int			count;
	EASYLIVE_DEVICE_INFO_T		*pDevice;
}EASYLIVE_DEVICE_LIST_T;

typedef struct __EASYLIVE_CHANNEL_INFO_T
{
	int		id;
	char	name[64];
	int		enable_multicast;		//�Ƿ������鲥
	char	multicast_addr[36];
	unsigned char	ttl;
}EASYLIVE_CHANNEL_INFO_T;

LIB_EASYSLIVE_API EASYSLIVE_HANDLE EasyScreenLive_Create();
LIB_EASYSLIVE_API void EasyScreenLive_Release(EASYSLIVE_HANDLE handler);

//nEncoderType �������ͣ� 
//		0=Ĭ�ϱ�������Ч����ͣ�ͨ����ǿ�� 
//		1=����루Ч�ʸߣ�ͨ���Բ�ǿ��
//		2=Ӳ�����루Ч����ߣ�ͨ������ͣ���ҪӢΰ������Կ�֧�֣�
LIB_EASYSLIVE_API int EasyScreenLive_StartCapture(EASYSLIVE_HANDLE handler, SOURCE_TYPE eSourceType, int nCamId, int nAudioId,  EASYSLIVE_HANDLE hCapWnd, int nEncoderType,
	int nVideoWidth=640, int nVideoHeight=480, int nFps=25, int nBitRate=2048, char* szDataType = "YUY2",  //VIDEO PARAM
	int nSampleRate=44100, int nChannel=2 );//AUDIO PARAM
//ֹͣ�ɼ�
LIB_EASYSLIVE_API void EasyScreenLive_StopCapture(EASYSLIVE_HANDLE handler);

//��ʼ����
LIB_EASYSLIVE_API int EasyScreenLive_StartPush(EASYSLIVE_HANDLE handler, PUSH_TYPE pushType, char* ServerIp, int nPushPort, char* sPushName, int nPushBufSize = 1024);
//ֹͣ����
LIB_EASYSLIVE_API void EasyScreenLive_StopPush(EASYSLIVE_HANDLE handler, PUSH_TYPE pushType);

//��ʼRTSP����
LIB_EASYSLIVE_API int EasyScreenLive_StartServer(EASYSLIVE_HANDLE handler, int listenport, char *username, char *password,  EASYLIVE_CHANNEL_INFO_T *channelInfo, int channelNum);
//ֹͣ����
LIB_EASYSLIVE_API void EasyScreenLive_StopServer(EASYSLIVE_HANDLE handler);

// DShow �ɼ�ö���豸
// ö����Ƶ�ɼ��豸
LIB_EASYSLIVE_API EASYLIVE_DEVICE_LIST_T* EasyScreenLive_GetAudioInputDevList(EASYSLIVE_HANDLE handler);
// ö����Ƶ�ɼ��豸
 LIB_EASYSLIVE_API EASYLIVE_DEVICE_LIST_T* EasyScreenLive_GetCameraList(EASYSLIVE_HANDLE handler);

