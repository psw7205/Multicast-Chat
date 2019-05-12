
// HomeWork2Dlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "HomeWork2.h"
#include "HomeWork2Dlg.h"
#include "afxdialogex.h"
#include "MulticastChat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHomeWork2Dlg 대화 상자



CHomeWork2Dlg::CHomeWork2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HOMEWORK2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHomeWork2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_portControl);
	DDX_Control(pDX, IDC_EDIT3, m_nameContorl);
	DDX_Control(pDX, IDC_EDIT2, m_ipAddrControl);
}

BEGIN_MESSAGE_MAP(CHomeWork2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_UPDATE(IDC_EDIT1, &CHomeWork2Dlg::OnEnUpdateEdit1)
	ON_BN_CLICKED(IDOK, &CHomeWork2Dlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CHomeWork2Dlg 메시지 처리기

BOOL CHomeWork2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_port = 9000;
	m_portControl.SetSel(0, -1);
	m_portControl.SetWindowText("9000");
	m_ipAddrControl.SetSel(0, -1);
	m_ipAddrControl.SetWindowText("235.7.8.1");
	
	m_nameContorl.SetLimitText(20);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CHomeWork2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CHomeWork2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CHomeWork2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHomeWork2Dlg::OnBnClickedOk()
{
	if (!checkAddr())
	{
		MessageBox("IP Address를 다시 입력하세요!", "ERROR");
		m_ipAddrControl.SetSel(0, -1);
		m_ipAddrControl.SetWindowText("235.7.8.1");
	}
	else if (!checkName())
	{
		MessageBox("닉네임을 입력하세요", "ERROR");
	}
	else
	{
		CDialogEx::OnOK();
		MulticastChat chat;
		chat.DoModal();
	}
}

// port에 입력 될 때마다 검사
void CHomeWork2Dlg::OnEnUpdateEdit1()
{
	CString portStr;
	m_portControl.GetWindowText(portStr);
	int len = portStr.GetLength();

	// 모든 문자열에서 숫자가 아닌 문자가 나오면 return
	for (int i = 0; i < len; ++i)
	{
		if (!(portStr.GetAt(i) >= '0' && portStr.GetAt(i) <= '9'))
		{
			MessageBox("숫자가 아닙니다!", "ERROR");
			m_portControl.SetSel(len - 1, len);
			m_portControl.Cut();
			return;
		}
	}

	// 입력된 값이 범위에 벗어나지 않으면 m_port에 저장
	int tmp = atoi(portStr);
	if (tmp > USHORT_MAX)
	{
		MessageBox("범위를 벗어났습니다!\n(0~65535)", "ERROR");
		m_portControl.SetSel(len - 1, len);
		m_portControl.Cut();
	}
	else
	{
		m_port = tmp;
	}
}

bool CHomeWork2Dlg::checkAddr()
{
	CString str;
	m_ipAddrControl.GetWindowText(str);
	str.Remove(' ');

	int len = str.GetLength();

	// 입력이 안된 경우 X
	if (len == 0)
		return false;

	int cnt = 0;
	for (int i = 0; i < str.GetLength(); ++i)
	{
		char curChar = str.GetAt(i);
		// '.'이나 숫자가 아닌 경우 X
		if (curChar != '.' && !(curChar >= '0' && curChar <= '9'))
			return false;

		if (curChar == '.')
			cnt++;
	}

	// ip가 모두 입력되었는지 확인('.'이 3개 있어야 함)
	if (cnt != 3)
		return false;

	// 문자열을 '.' 기준으로 분리
	int pos = 0;
	for (int i = 0; i < 4; ++i)
	{
		int ip = atoi(str.Tokenize(".", pos));

		// 255 이상이면 잘못된 ip주소
		if (ip > 255)
			return false;

		m_IpAddr[i] = ip;
	}

	// class D인지 확인 (최상위 비트 1110XXXX)
	if (!(m_IpAddr[0] >= 0b11100000 && m_IpAddr[0] <= 0b11101111))
		return false;

	return true;
}

bool CHomeWork2Dlg::checkName()
{
	CString str;
	m_nameContorl.GetWindowText(str);

	if (str.GetLength() == 0)
		return false;

	return true;
}