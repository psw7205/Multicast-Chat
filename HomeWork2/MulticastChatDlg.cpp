// 2019년 1학기 네트워크프로그래밍 숙제 2번
// 성명: 박상우 학번: 16013093
// 플랫폼: VS2017

// MulticastChatDlg.cpp: 구현 파일

#include "stdafx.h"
#include "HomeWork2.h"
#include "MulticastChatDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(MulticastChatDlg, CDialogEx)

// 메세지 수신 함수(쓰레드에서 실행)
DWORD WINAPI MulticastChatDlg::Receiver(LPVOID arg)
{
	int rretval;

	MulticastChatDlg *pDlg = (MulticastChatDlg*)arg;

	// 윈속 초기화
	WSADATA rwsa;
	if (WSAStartup(MAKEWORD(2, 2), &rwsa) != 0)
		return 1;

	// socket()
	SOCKET rsock = socket(AF_INET, SOCK_DGRAM, 0);
	if (rsock == INVALID_SOCKET) AfxMessageBox("socket() Error");

	// SO_REUSEADDR 옵션 설정
	BOOL optval = TRUE;
	rretval = setsockopt(rsock, SOL_SOCKET,
		SO_REUSEADDR, (char*)& optval, sizeof(optval));
	if (rretval == SOCKET_ERROR) AfxMessageBox("setsockopt() Error");

	// bind()
	SOCKADDR_IN rlocaladdr;
	ZeroMemory(&rlocaladdr, sizeof(rlocaladdr));
	rlocaladdr.sin_family = AF_INET;
	rlocaladdr.sin_addr.s_addr = htonl(INADDR_ANY);
	rlocaladdr.sin_port = htons(pDlg->user.port);
	rretval = bind(rsock, (SOCKADDR*)& rlocaladdr, sizeof(rlocaladdr));
	if (rretval == SOCKET_ERROR) AfxMessageBox("bind() Error");

	// 멀티캐스트 그룹 가입
	struct ip_mreq rmreq;
	rmreq.imr_multiaddr.s_addr = inet_addr(pDlg->user.ip);
	rmreq.imr_interface.s_addr = htonl(INADDR_ANY);
	rretval = setsockopt(rsock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		(char*)& rmreq, sizeof(rmreq));
	if (rretval == SOCKET_ERROR) AfxMessageBox("setsockopt() Error");

	// 데이터 통신에 사용할 변수
	SOCKADDR_IN rpeeraddr;
	int raddrlen;
	char rbuf[BUFSIZE + 1] = { 0 };

	// 멀티캐스트 데이터 받기
	while (1) {
		// 데이터 받기
		memset(rbuf, 0, sizeof(rbuf));

		raddrlen = sizeof(rpeeraddr);

		rretval = recvfrom(rsock, rbuf, BUFSIZE, 0,
			(SOCKADDR*)& rpeeraddr, &raddrlen);
		if (rretval == SOCKET_ERROR) {
			AfxMessageBox("recvfrom() Error");
			continue;
		}

		rbuf[rretval] = '\0';
		CString recivedStr = rbuf;
		CString recivedID = recivedStr.Right(12);

		// 수신한 ID와 자신의 ID를 비교해서 다른 경우만 중복검사 실행
		if (recivedID.Compare(pDlg->ID))
		{
			// 문자열에서 닉네임을 찾아 비교
			int len = recivedStr.Find(':') - 1;
			CString recivedName = recivedStr.Left(len);

			// ID를 시간으로 설정하기 때문에 비교가능, 시간이 늦으면 늦게 생성한 닉네임
			if (!recivedName.Compare(pDlg->user.name) && recivedID.Compare(pDlg->ID) > 0)
			{
				CString errMsg = recivedID + "##아이디 중복 아이디를 변경하세요##";
				pDlg->MySendTo(errMsg);
			}
		}

		// 받은 데이터 출력
		pDlg->AddEventString(rbuf);
	}

	// 멀티캐스트 그룹 탈퇴
	rretval = setsockopt(rsock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
		(char*)& rmreq, sizeof(rmreq));
	if (rretval == SOCKET_ERROR) AfxMessageBox("setsockopt() Error");

	// closesocket()
	closesocket(rsock);

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
	//프로그램 종료시 퇴장메세지를 보내고 소켓닫기
	MySendTo("##############퇴장##############");

	closesocket(sock);
	WSACleanup();
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

	// 빈메세지가 아닌 경우
	if (str != "")
	{
		m_message.SetWindowText("");
		// 데이터 보내기
		MySendTo(str);
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

	// 공백이 아니고 현재 닉네임과 다른 경우
	if (!reName.IsEmpty() && reName.Compare(user.name))
	{
		// ID도 같이 변경해 메세지 보내기
		CString str = user.name + "에서 " + reName + "으로 아이디 변경";
		ID.Format("[%lld]", CTime::GetCurrentTime().GetTime());

		user.name = reName;
		m_name.SetWindowText(reName);
		m_renameContorl.SetWindowText("");

		MySendTo(str);
	}
}


BOOL MulticastChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_name.SetWindowText(user.name);
	memset(sendbuf, 0, sizeof(sendbuf));

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
	hThread = CreateThread(NULL, 0, Receiver, (LPVOID)this, 0, NULL);
	if (hThread == NULL)
		closesocket(sock);
	else
		CloseHandle(hThread);

	//현재 호스트 IP 저장
	gethostname(m_MyHostName, sizeof(m_MyHostName));
	HOSTENT *ptr = gethostbyname(m_MyHostName);
	if (ptr == NULL) {
		MessageBox("gethostbyname()");
		return FALSE;
	}

	IN_ADDR addr;
	memcpy(&addr, *(ptr->h_addr_list), ptr->h_length);
	m_MyHostIP = inet_ntoa(addr);
	
	// 아이디 생성
	ID.Format("[%lld]", CTime::GetCurrentTime().GetTime());

	// 입장 메세지 출력
	CString joinMSG = "##############입장##############";
	MySendTo(joinMSG);

	return TRUE;
}

// 메세지 보내기 함수
void MulticastChatDlg::MySendTo(CString str)
{
	// 현재 시간 설정
	CString time;
	cTime = CTime::GetCurrentTime();
	time.Format("%02dh:%02dm:%02ds", cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());

	// 메세지 구성
	str = user.name + " : " + str + " (" + time + "||" + m_MyHostIP + ")" + ID;

	retval = sendto(sock, str, str.GetLength(), 0,
		(SOCKADDR*)& remoteaddr, sizeof(remoteaddr));
	if (retval == SOCKET_ERROR) {
		AfxMessageBox("sendto() Error");
	}
}


void MulticastChatDlg::AddEventString(CString str)
{
	int idx = m_chatList.InsertString(-1, str);
	m_chatList.SetCurSel(idx);

	//화면보다 큰 메세지를 추가할 때 화면에 스크롤 바 생성
	CString    strTmp;
	CSize      sz;
	int        dx = 0;
	TEXTMETRIC tm;
	CDC*       pDC = m_chatList.GetDC();
	CFont*     pFont = m_chatList.GetFont();

	CFont* pOldFont = pDC->SelectObject(pFont);
	pDC->GetTextMetrics(&tm);

	for (int i = 0; i < m_chatList.GetCount(); i++)
	{
		m_chatList.GetText(i, strTmp);
		sz = pDC->GetTextExtent(strTmp);

		sz.cx += tm.tmAveCharWidth;

		if (sz.cx > dx)
			dx = sz.cx;
	}

	pDC->SelectObject(pOldFont);
	m_chatList.ReleaseDC(pDC);

	m_chatList.SetHorizontalExtent(dx);
}

