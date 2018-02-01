
// EasyScreenLiveDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EasyScreenLive.h"
#include "EasyScreenLiveDlg.h"
#include "afxdialogex.h"
#include <string>
using namespace std;

#include <WinSock2.h>
#pragma comment(lib,"ws2_32")        //���ӵ�ws2_32��̬���ӿ�

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEasyScreenLiveDlg �Ի���




CEasyScreenLiveDlg::CEasyScreenLiveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEasyScreenLiveDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pusher = NULL;
	m_bCapture = FALSE;
	m_bPushing = FALSE;
	m_bPublishServer = FALSE;
}

void CEasyScreenLiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEasyScreenLiveDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PUSH, &CEasyScreenLiveDlg::OnBnClickedButtonPush)
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE, &CEasyScreenLiveDlg::OnBnClickedButtonCapture)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_PUSHSOURCE, &CEasyScreenLiveDlg::OnCbnSelchangeComboPushsource)
	ON_BN_CLICKED(IDC_BUTTON_PUBLISH_SERVER, &CEasyScreenLiveDlg::OnBnClickedButtonPublishServer)
END_MESSAGE_MAP()


// CEasyScreenLiveDlg ��Ϣ�������

BOOL CEasyScreenLiveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	GetDlgItem(IDC_EDIT_IP)->SetWindowText(_T("www.easydss.com"));
	GetDlgItem(IDC_EDIT_PORT)->SetWindowText(_T("10085"));	
	GetDlgItem(IDC_EDIT_STREAMNAME)->SetWindowText(_T("Sword"));	
	GetDlgItem(IDC_EDIT_LISTEN_PORT)->SetWindowText(_T("8554"));	
	GetDlgItem(IDC_EDIT_BITRATE)->SetWindowText(_T("2048"));

	
	
	CComboBox* pComboSource  = (CComboBox*)GetDlgItem( IDC_COMBO_PUSHSOURCE);
	if (pComboSource)
	{
		pComboSource->AddString(_T("����ͷ�ɼ�"));
		pComboSource->AddString(_T("��Ļ�ɼ�"));
		pComboSource->SetCurSel(1);
	}

	CComboBox* pComboEncoder  = (CComboBox*)GetDlgItem( IDC_COMBO_ENCODER_TYPE);
	if (pComboEncoder)
	{
		pComboEncoder->AddString(_T("Ĭ�ϱ�����"));
		pComboEncoder->AddString(_T("���������"));
		pComboEncoder->AddString(_T("����Ӳ����"));
		pComboEncoder->SetCurSel(1);
	}	

	CComboBox* pComboTransType  = (CComboBox*)GetDlgItem( IDC_COMBO_TRANSPORT_TYPE);
	if (pComboTransType)
	{
		pComboTransType->AddString(_T("����"));
		pComboTransType->AddString(_T("�鲥"));
		pComboTransType->SetCurSel(0);
	}	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CEasyScreenLiveDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CEasyScreenLiveDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CEasyScreenLiveDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CEasyScreenLiveDlg::OnBnClickedButtonCapture()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton* pCapture = (CButton*)GetDlgItem(IDC_BUTTON_CAPTURE);
	CButton* pBtnPush = (CButton*)GetDlgItem(IDC_BUTTON_PUSH);
	CButton* pBtnPublishServer = (CButton*)GetDlgItem(IDC_BUTTON_PUBLISH_SERVER);

	if (!m_bCapture)
	{
		HWND hShowVideo = GetDlgItem(IDC_STATIC_VIDEO)->GetSafeHwnd();
		
		if(!m_pusher )
			m_pusher =  EasyScreenLive_Create();

		if (m_pusher)
		{
			CString sSourceType;
			CString sLog;
			SOURCE_TYPE sourceType = SOURCE_LOCAL_CAMERA;
			int nEncoderType = 0;
			string sFormat = "RGB24";
			CComboBox* pComboxSourceMode = (CComboBox*)GetDlgItem(IDC_COMBO_PUSHSOURCE);
			if (pComboxSourceMode)
			{
				sourceType = (SOURCE_TYPE)pComboxSourceMode->GetCurSel();
			}
			CComboBox* pComboEncoder  = (CComboBox*)GetDlgItem( IDC_COMBO_ENCODER_TYPE);
			if (pComboEncoder)
			{
				nEncoderType = pComboEncoder->GetCurSel();
			}

			if (sourceType == SOURCE_LOCAL_CAMERA)
			{
				sFormat = "YUY2";
				sSourceType = _T("����������ɼ�");
			} 
			else 		if (sourceType == SOURCE_SCREEN_CAPTURE)
			{
				sFormat = "RGB24";
				sSourceType = _T("��Ļ�ɼ�");
			}

			wchar_t wszBitRate[128] = {0,};
			char szBitRate[128] = {0,};
			int nBitRate  = 2048;
			GetDlgItem(IDC_EDIT_BITRATE)->GetWindowText(wszBitRate, sizeof(wszBitRate));
			if (wcslen(wszBitRate)  > 0)//��ǰΪ��		
			{
				__WCharToMByte(wszBitRate, szBitRate, sizeof(szBitRate)/sizeof(szBitRate[0]));
				nBitRate = atoi( szBitRate );
			}

			int ret = EasyScreenLive_StartCapture(m_pusher, sourceType, 0, 0, hShowVideo, nEncoderType, 640,480,25,nBitRate, (char*)sFormat.c_str(),8000,1);
			if (ret)
			{
				m_bCapture = TRUE;
				pCapture->SetWindowText(_T("Stop"));
				sLog.Format(_T("����%s�ɹ���"), sSourceType);	
				OnLog( sLog );
			}else
			{
				sLog.Format(_T("����%sʧ�ܡ�"), sSourceType);
				OnLog( sLog );
			}
		}
	} 
	else
	{		
		if (m_pusher)
		{
			EasyScreenLive_StopPush(m_pusher, PUSH_RTMP);
			EasyScreenLive_StopServer(m_pusher);
			EasyScreenLive_StopCapture(m_pusher);
			EasyScreenLive_Release(m_pusher);
			m_pusher = NULL;
		}
		pCapture->SetWindowText(_T("Capture"));
		m_bCapture = FALSE;
		pBtnPush->SetWindowText(_T("Push"));
		m_bPushing = FALSE;
		pBtnPublishServer->SetWindowText(_T("Publish Server"));
		m_bPublishServer = FALSE;
		OnLog( _T("ֹͣ�ɼ�") );
	}
}


void CEasyScreenLiveDlg::OnBnClickedButtonPush()
{

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// 
	CButton* pBtnPush = (CButton*)GetDlgItem(IDC_BUTTON_PUSH);
	if (!m_bPushing)
	{
		UpdateData(TRUE);
		wchar_t wszIP[128] = {0,};
		char szIP[128] = {0,};
		wchar_t wszPort[128] = {0,};
		char szPort[128] = {0,};
		wchar_t wszStreamName[128] = {0,};
		char szStreamName[128] = {0,};
		int nPort = 0;

		GetDlgItem(IDC_EDIT_IP)->GetWindowText(wszIP,  sizeof(wszIP));
		if (wcslen(wszIP)  > 0)//��ǰΪ��		
		{
			__WCharToMByte(wszIP, szIP, sizeof(szIP)/sizeof(szIP[0]));
			//nStartTime = atoi( szStartTime );
		}

		GetDlgItem(IDC_EDIT_PORT)->GetWindowText(wszPort,  sizeof(wszPort));	
		if (wcslen(wszPort)  > 0)//��ǰΪ��		
		{
			__WCharToMByte(wszPort, szPort, sizeof(szPort)/sizeof(szPort[0]));
			nPort = atoi( szPort );
		}

		GetDlgItem(IDC_EDIT_STREAMNAME)->GetWindowText(wszStreamName,  sizeof(wszStreamName));	
		if (wcslen(wszStreamName)  > 0)//��ǰΪ��		
		{
			__WCharToMByte(wszStreamName, szStreamName, sizeof(szStreamName)/sizeof(szStreamName[0]));
		}

		if (m_pusher)
		{
			EasyScreenLive_StartPush(m_pusher, PUSH_RTMP, szIP, nPort,  szStreamName );
			m_bPushing = TRUE;
			pBtnPush->SetWindowText(_T("Stop"));
			CString sLog = _T("");

			sLog.Format(_T("����RTMP����: rtmp://%s:%d/live/%s"), wszIP, nPort, wszStreamName);
			OnLog( sLog );
		}
	}
	else
	{
		EasyScreenLive_StopPush(m_pusher, PUSH_RTMP);

		pBtnPush->SetWindowText(_T("Push"));
		m_bPushing = FALSE;
		OnLog( _T("ֹͣ����") );

	}
}

void CEasyScreenLiveDlg::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EasyScreenLive_StopPush(m_pusher, PUSH_RTMP);
	EasyScreenLive_StopCapture(m_pusher);

}

void CEasyScreenLiveDlg::OnCbnSelchangeComboPushsource()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// 
	CComboBox* pComboxSourceMode = (CComboBox*)GetDlgItem(IDC_COMBO_PUSHSOURCE);
	if (pComboxSourceMode)
	{
		int nIdx = pComboxSourceMode->GetCurSel();
	}
}

int GetLocalIP( std::string &local_ip )  
{  
	WSADATA wsaData = {0};  
	if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)  
		return -1;  
	char szHostName[MAX_PATH] = {0};  
	int nRetCode;  
	nRetCode = gethostname(szHostName, sizeof(szHostName));  
	PHOSTENT hostinfo;  
	if (nRetCode != 0)  
		return WSAGetLastError();          
	hostinfo = gethostbyname(szHostName);  
	local_ip = inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list);  
	WSACleanup();  
	return 1;  
} 

typedef struct tagIPInfo
{
	char ip[30];
}IPInfo;

bool GetLocalIPs(IPInfo* ips,int maxCnt,int* cnt)
{
	//1.��ʼ��wsa
	WSADATA wsaData;
	int ret=WSAStartup(MAKEWORD(2,2),&wsaData);
	if (ret!=0)
	{
		return false;
	}
	//2.��ȡ������
	char hostname[256];
	ret=gethostname(hostname,sizeof(hostname));
	if (ret==SOCKET_ERROR)
	{
		return false;
	}
	//3.��ȡ����ip
	HOSTENT* host=gethostbyname(hostname);
	if (host==NULL)
	{
		return false;
	}
	//4.���ת��Ϊchar*����������
	int n = *cnt=host->h_length<maxCnt?host->h_length:maxCnt;
	for (int i=0;i<n;i++)
	{
		in_addr* addr=(in_addr*)*host->h_addr_list;
		strcpy(ips[i].ip,inet_ntoa(addr[i]));
	}
	return true;
}

void CEasyScreenLiveDlg::OnBnClickedButtonPublishServer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// 
	//IDC_EDIT_LISTEN_PORT
	CButton* pBtnPublishServer = (CButton*)GetDlgItem(IDC_BUTTON_PUBLISH_SERVER);
	if (!m_bPublishServer)
	{
		UpdateData(TRUE);
		int nRet = 0;
		if (m_pusher)
		{
			int transportType = 0;
			CComboBox* pComboTransType  = (CComboBox*)GetDlgItem( IDC_COMBO_TRANSPORT_TYPE);
			if (pComboTransType)
			{

				transportType = pComboTransType->GetCurSel();
			}

			EASYLIVE_CHANNEL_INFO_T	liveChannel[MAX_CHANNEL_NUM];
			memset(&liveChannel[0], 0x00, sizeof(EASYLIVE_CHANNEL_INFO_T)*MAX_CHANNEL_NUM);
			for (int i=0; i<MAX_CHANNEL_NUM; i++)
			{
				liveChannel[i].id = i;
				//strcpy(liveChannel[i].name, channel[i].name);
				sprintf(liveChannel[i].name, "channel=%d", i);
#if 1
				if (i==0)
				{
					liveChannel[i].enable_multicast = transportType;
					strcpy(liveChannel[i].multicast_addr, "238.255.255.255");
					liveChannel[i].ttl = 255;
				}
#endif
			}

			wchar_t wszPort[128] = {0,};
			char szPort[128] = {0,};
			int nPort = 0;
			GetDlgItem(IDC_EDIT_LISTEN_PORT)->GetWindowText(wszPort,  sizeof(wszPort));	
			if (wcslen(wszPort)  > 0)//��ǰΪ��		
			{
				__WCharToMByte(wszPort, szPort, sizeof(szPort)/sizeof(szPort[0]));
				nPort = atoi( szPort );
			}

			string ip = "127.0.0.1";
#if 0
			int MaxCon = 10;
			int num = 0;
			IPInfo ips[10];
			GetLocalIPs(ips, MaxCon, &num);
#endif
			GetLocalIP(ip);
			//��ʼRTSP����
			nRet  = EasyScreenLive_StartServer(m_pusher, nPort, "", "",  liveChannel, MAX_CHANNEL_NUM );
			pBtnPublishServer->SetWindowText(_T("Stop"));
			m_bPublishServer = TRUE;
			CString sLog = _T("");
			for (int nI=0; nI<MAX_CHANNEL_NUM; nI++)
			{
				sLog.Format(_T("����RTSP����: rtsp://%s:%d/channel=%d"), CString(ip.c_str()), nPort, nI);
				OnLog( sLog );
			}
		}
	} 
	else
	{
		EasyScreenLive_StopServer(m_pusher);
		pBtnPublishServer->SetWindowText(_T("Publish Server"));
		m_bPublishServer = FALSE;
		OnLog(_T("ֹͣRTSP����"));
	}

}
void CEasyScreenLiveDlg::OnLog(CString sLog)
{
	CEdit* pLog = (CEdit*)GetDlgItem(IDC_EDIT_LOG);
	if (pLog)
	{
		CString strLog = sLog;
		CString strTime = _T("");
		CTime CurrentTime=CTime::GetCurrentTime(); 
		strTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d   "),CurrentTime.GetYear(),CurrentTime.GetMonth(),
			CurrentTime.GetDay(),CurrentTime.GetHour(),  CurrentTime.GetMinute(),
			CurrentTime.GetSecond());
		strLog = strTime + strLog + _T("\r\n");
		int nLength  =  pLog->SendMessage(WM_GETTEXTLENGTH);  
		pLog->SetSel(nLength,  nLength);  
		pLog->ReplaceSel(strLog); 
		pLog->SetFocus();
	}
}



void CEasyScreenLiveDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	if (m_pusher)
	{
		EasyScreenLive_StopPush(m_pusher, PUSH_RTMP);
		EasyScreenLive_StopServer(m_pusher);
		EasyScreenLive_StopCapture(m_pusher);
		EasyScreenLive_Release(m_pusher);
		m_pusher = NULL;
	}
}
