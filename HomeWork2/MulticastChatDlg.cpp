// MulticastChatDlg.cpp: 구현 파일

#include "stdafx.h"
#include "HomeWork2.h"
#include "MulticastChatDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(MulticastChatDlg, CDialogEx)

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
}


BEGIN_MESSAGE_MAP(MulticastChatDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &MulticastChatDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &MulticastChatDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_RENAME, &MulticastChatDlg::OnBnClickedButtonRename)
	ON_WM_CREATE()
END_MESSAGE_MAP()

void MulticastChatDlg::OnBnClickedOk()
{
	
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
	CString str;
	m_renameContorl.GetWindowText(str);

	if (!str.IsEmpty())
	{
		m_name.SetWindowText(str);
		m_renameContorl.SetWindowText("");
	}
}


BOOL MulticastChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	m_name.SetWindowText(user.name);

	return TRUE; 
}

void MulticastChatDlg::AddEventString(const CString str)
{
	int idx = m_chatList.InsertString(-1, str);
	m_chatList.SetCurSel(idx);
}