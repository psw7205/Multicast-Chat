// 2019년 1학기 네트워크프로그래밍 숙제 2번
// 성명: 박상우 학번: 16013093
// 플랫폼: VS2017

// HomeWork2Dlg.h: 헤더 파일
//

#pragma once


// CHomeWork2Dlg 대화 상자
class CHomeWork2Dlg : public CDialogEx
{
	// 생성입니다.
public:
	CHomeWork2Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOMEWORK2_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_portControl;
	CEdit m_ipAddrControl;
	BYTE  m_IpAddr[4];
	CEdit m_nameContorl;
	int m_port;

	afx_msg void OnBnClickedOk();
	afx_msg void OnEnUpdateEdit1();
	afx_msg bool CheckAddr();
	afx_msg bool CheckName();
};
