// MulticastChatDlg.cpp: 구현 파일

#include "stdafx.h"
#include "HomeWork2.h"
#include "MulticastChatDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(MulticastChatDlg, CDialogEx)

DWORD WINAPI Receiver(LPVOID arg)
{
	int retval;

	MulticastChatDlg *pDlg = (MulticastChatDlg*)arg;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) AfxMessageBox("socket() Error");

	// SO_REUSEADDR 옵션 설정
	BOOL optval = TRUE;
	retval = setsockopt(sock, SOL_SOCKET,
		SO_REUSEADDR, (char*)& optval, sizeof(optval));
	if (retval == SOCKET_ERROR) AfxMessageBox("setsockopt() Error");

	// bind()
	SOCKADDR_IN localaddr;
	ZeroMemory(&localaddr, sizeof(localaddr));
	localaddr.sin_family = AF_INET;
	localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localaddr.sin_port = htons(pDlg->user.port);
	retval = bind(sock, (SOCKADDR*)& localaddr, sizeof(localaddr));
	if (retval == SOCKET_ERROR) AfxMessageBox("bind() Error");

	// 멀티캐스트 그룹 가입
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(pDlg->user.ip);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	retval = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		(char*)& mreq, sizeof(mreq));
	if (retval == SOCKET_ERROR) AfxMessageBox("setsockopt() Error");

	// 데이터 통신에 사용할 변수
	SOCKADDR_IN peeraddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	char name[BUFSIZE];

	// 멀티캐스트 데이터 받기
	while (1) {
		// 데이터 받기
		addrlen = sizeof(peeraddr);

		retval = recvfrom(sock, name, BUFSIZE, 0,
			(SOCKADDR*)& peeraddr, &addrlen);
		if (retval == SOCKET_ERROR) {
			AfxMessageBox("recvfrom() Error");
			continue;
		}

		char *enter = strchr(name, '\n');
		
		if (enter != NULL)
		{
			int idx = enter - name;
			name[idx] = '\0';
		}
		else
		{
			name[retval] = '\0';
		}

		CString str = name;

		str.Append(" : ");

		retval = recvfrom(sock, buf, BUFSIZE, 0,
			(SOCKADDR*)& peeraddr, &addrlen);
		if (retval == SOCKET_ERROR) {
			AfxMessageBox("recvfrom() Error");
			continue;
		}

		// 받은 데이터 출력
		buf[retval] = '\0';
		str.Append(buf);
		pDlg->AddEventString(str);
	}

	// 멀티캐스트 그룹 탈퇴
	retval = setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
		(char*)& mreq, sizeof(mreq));
	if (retval == SOCKET_ERROR) AfxMessageBox("setsockopt() Error");

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}

MulticastChatDlg::MulticastChatDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CHAT, pParent)
{

}

MulticastChatDlg::~MulticastChatDlg()
{
}

void MulticastChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IPADDR, m_ip);
	DDX_Text(pDX, IDC_PORT, m_port);
	DDX_Control(pDX, IDC_EDIT_RENAME, m_renameContorl);
	DDX_Control(pDX, IDC_NAME, m_name);
	DDX_Control(pDX, IDC_LIST_CHAT, m_chatList);
	DDX_Control(pDX, IDC_EDIT_MSG, m_message);
}


BEGIN_MESSAGE_MAP(MulticastChatDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &MulticastChatDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &MulticastChatDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_RENAME, &MulticastChatDlg::OnBnClickedButtonRename)
	ON_WM_CREATE()
END_MESSAGE_MAP()

void MulticastChatDlg::OnBnClickedOk()
{
	CString str;
	m_message.GetWindowText(str);

	if (str != "")
	{
		m_message.SetWindowText("");

		// 데이터 보내기 (이름) 
		retval = sendto(sock, user.name, user.name.GetLength(), 0,
			(SOCKADDR*)& remoteaddr, sizeof(remoteaddr));
		if (retval == SOCKET_ERROR) {
			AfxMessageBox("sendto() Error");
		}

		// 데이터 보내기 (내용)
		retval = sendto(sock, str, str.GetLength(), 0,
			(SOCKADDR*)& remoteaddr, sizeof(remoteaddr));
		if (retval == SOCKET_ERROR) {
			AfxMessageBox("sendto() Error");
		}
	}
}


void MulticastChatDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


int MulticastChatDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_ip = user.ip;
	m_port = user.port;

	return 0;
}


void MulticastChatDlg::OnBnClickedButtonRename()
{
	CString reName;
	m_renameContorl.GetWindowText(reName);

	if (!reName.IsEmpty() && reName.Compare(user.name))
	{
		CString str = "닉네임 변경 : ";
		str += user.name + " -> " + reName;
		
		user.name = reName;
		m_name.SetWindowText(reName);
		m_renameContorl.SetWindowText("");
	
		CString time;
		cTime = CTime::GetCurrentTime();
		time.Format("%02d:%02d:%02d", cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());
		
		str = str + " (" + time + ")";

		AddEventString(str);
	}
}


BOOL MulticastChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_name.SetWindowText(user.name);
	userList.AddTail(user.name);
	
	// 윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) MessageBox("socket() Error");

	// 멀티캐스트 TTL 설정
	int ttl = 2;
	retval = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL,
		(char*)& ttl, sizeof(ttl));
	if (retval == SOCKET_ERROR) MessageBox("setsockopt() Error");

	// 소켓 주소 구조체 초기화
	ZeroMemory(&remoteaddr, sizeof(remoteaddr));
	remoteaddr.sin_family = AF_INET;
	remoteaddr.sin_addr.s_addr = inet_addr(user.ip);
	remoteaddr.sin_port = htons(user.port);

	//리시버 스레드 생성
	hThread = CreateThread(NULL, 0, Receiver,(LPVOID)this, 0, NULL);
	if (hThread == NULL)
		closesocket(sock);
	else
		CloseHandle(hThread);

	return TRUE; 
}


void MulticastChatDlg::AddEventString(CString str)
{
	int idx = m_chatList.InsertString(-1, str);
	m_chatList.SetCurSel(idx);
}

