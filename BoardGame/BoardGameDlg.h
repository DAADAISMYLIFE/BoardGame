﻿
// BoardGameDlg.h: 헤더 파일
//

#pragma once
#include "Blocks.h"
#include "Player.h"
#include "CMySocket.h"
#include "ManualDialog.h"
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
	BOOL useItem3;	  //3이하 아이템 사용 여부
	BOOL useItem4;	  //4이상 아이템 사용 여부
	int numItem1; //홀수 아이템 개수
	int numItem2; //짝수 아이템 개수
	int numItem3; //3이하 아이템 개수
	int numItem4; //4이상 아이템 개수

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
	int animationFrame;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStartGame();
	BOOL isConfirm = FALSE;
	int receivedLocation;
	int moveBlocks;
	int m_odd_count;
	int m_even_count;
	int m_under_count;
	int m_over_count;
	afx_msg void ClickedItem3();
	afx_msg void ClickedItem4();
	afx_msg void OnBnClickedManual();
	BOOL isIsolated;
	int isolatedCount;
	BOOL isGameOn;
	void initAllSetting();
	ManualDialog* Manual;
};
