// 2019년 1학기 네트워크프로그래밍 숙제 2번
// 성명: 박상우 학번: 16013093
// 플랫폼: VS2017

#pragma once

struct info {
	WORD port;
	BYTE  ipAddr[4];
	CString name;
	CString ip;
};

// MulticastChat 대화 상자

#define BUFSIZE     1024

class MulticastChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MulticastChatDlg)

public:
	MulticastChatDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~MulticastChatDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void AddEventString(CString str);
	afx_msg void OnBnClickedButtonRename();

	virtual BOOL OnInitDialog();
	info user;
	CListBox m_chatList;
	CString m_ip;
	int m_port;
	CEdit m_renameContorl;
	CEdit m_name;
	CEdit m_message;
	CTime cTime;

	WSADATA wsa;
	SOCKET sock;
	int retval;
	char sendbuf[BUFSIZE + 1];
	int len;
	HANDLE hThread;
	SOCKADDR_IN remoteaddr;

	char m_MyHostName[256];
	CString m_MyHostIP;
	CString ID;

	void MySendTo(CString str);
	static DWORD WINAPI Receiver(LPVOID arg);
};
