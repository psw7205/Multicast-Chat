// MulticastChat.cpp: 구현 파일
//

#include "stdafx.h"
#include "HomeWork2.h"
#include "MulticastChat.h"
#include "afxdialogex.h"


// MulticastChat 대화 상자

IMPLEMENT_DYNAMIC(MulticastChat, CDialogEx)

MulticastChat::MulticastChat(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

MulticastChat::~MulticastChat()
{
}

void MulticastChat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MulticastChat, CDialogEx)
END_MESSAGE_MAP()


// MulticastChat 메시지 처리기
