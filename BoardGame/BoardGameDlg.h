
// BoardGameDlg.h: 헤더 파일
//

#pragma once
#include "Blocks.h"
#include "Player.h"
#include "CMySocket.h"
#define BOARDSIZE 71


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
	BOOL userType;			//true = 방장, false = 도전자
	CMySocket MySocket, YourSoket;
	afx_msg void OnBnClickedCreatRoom();
	afx_msg void OnBnClickedEnterRoom();
	afx_msg void OnBnClickedExitGame();
	BOOL useItem1;	  //홀수 아이템 사용 여부
	BOOL useItem2;	  //짝수 아이템 사용 여부
	int numItem1 = 2; //홀수 아이템 개수
	int numItem2 = 2; //짝수 아이템 개수
	int diceNum = 0;  //주사위 나온 숫자
	Blocks *board = new Blocks[BOARDSIZE]; //게임판 생성
	//소켓 함수
	void OnAccept();
	void OnConnect();
	void OnClose();
	void OnReceive();
	void OnSend();
	afx_msg void ClickedItem1();
	afx_msg void ClickedItem2();
	afx_msg void OnBnClickedRollDice();
	Player *myPlayer;
	Player *yourPlayer;
	int animationFrame = 0;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStartGame();
	BOOL isConfirm = FALSE;
};
