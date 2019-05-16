﻿#pragma once

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
	CStringList userList;
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


	bool flag;
};
