
// BoardGameDlg.h: 헤더 파일
//

#pragma once
#include "Blocks.h"
#define BOARDSIZE 47


// CBoardGameDlg 대화 상자
class CBoardGameDlg : public CDialogEx
{
// 생성입니다.
public:
	CBoardGameDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOARDGAME_DIALOG };
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
	CString userName;
	CString serverAddress;
	afx_msg void OnBnClickedCreatRoom();
	afx_msg void OnBnClickedEnterRoom();
	afx_msg void OnBnClickedExitGame();
	BOOL useItem1;
	BOOL useItem2;
	Blocks *board = new Blocks[BOARDSIZE];
};
