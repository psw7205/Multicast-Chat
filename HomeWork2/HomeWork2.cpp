// 2019년 1학기 네트워크프로그래밍 숙제 2번
// 성명: 박상우 학번: 16013093
// 플랫폼: VS2017

// HomeWork2.cpp: 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "HomeWork2.h"
#include "HomeWork2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHomeWork2App

BEGIN_MESSAGE_MAP(CHomeWork2App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHomeWork2App 생성

CHomeWork2App::CHomeWork2App()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CHomeWork2App 개체입니다.

CHomeWork2App theApp;

// CHomeWork2App 초기화

BOOL CHomeWork2App::InitInstance()
{
	CWinApp::InitInstance();

	CHomeWork2Dlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}

