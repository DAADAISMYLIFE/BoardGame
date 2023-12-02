
// BoardGameDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "BoardGame.h"
#include "BoardGameDlg.h"
#include "afxdialogex.h"
#include <math.h>

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


// CBoardGameDlg 대화 상자



CBoardGameDlg::CBoardGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BOARDGAME_DIALOG, pParent)
	, userName(_T("Guest"))
	, serverAddress(_T("localhost"))
	, useItem1(FALSE)
	, useItem2(FALSE)
	, m_odd_count(0)
	, m_even_count(0)
	, m_under_count(0)
	, m_over_count(0)
	, useItem3(FALSE)
	, useItem4(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBoardGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SET_NAME, userName);
	DDX_Text(pDX, IDC_SET_SERVER_ADDR, serverAddress);
	DDX_Check(pDX, IDC_ITEM1, useItem1);
	DDX_Check(pDX, IDC_ITEM2, useItem2);
	DDX_Text(pDX, IDC_EDIT_ODD, m_odd_count);
	DDX_Text(pDX, IDC_EDIT_EVEN, m_even_count);
	DDX_Text(pDX, IDC_EDIT_UNDER, m_under_count);
	DDX_Text(pDX, IDC_EDIT_OVER, m_over_count);
	DDX_Check(pDX, IDC_ITEM3, useItem3);
	DDX_Check(pDX, IDC_ITEM4, useItem4);
}

BEGIN_MESSAGE_MAP(CBoardGameDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CREAT_ROOM, &CBoardGameDlg::OnBnClickedCreatRoom)
	ON_BN_CLICKED(IDC_ENTER_ROOM, &CBoardGameDlg::OnBnClickedEnterRoom)
	ON_BN_CLICKED(IDC_EXIT_GAME, &CBoardGameDlg::OnBnClickedExitGame)
	ON_BN_CLICKED(IDC_ITEM1, &CBoardGameDlg::ClickedItem1)
	ON_BN_CLICKED(IDC_ITEM2, &CBoardGameDlg::ClickedItem2)
	ON_BN_CLICKED(IDB_ROLL_DICE, &CBoardGameDlg::OnBnClickedRollDice)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_START_GAME, &CBoardGameDlg::OnBnClickedStartGame)
	ON_BN_CLICKED(IDC_ITEM3, &CBoardGameDlg::ClickedItem3)
	ON_BN_CLICKED(IDC_ITEM4, &CBoardGameDlg::ClickedItem4)
END_MESSAGE_MAP()


// CBoardGameDlg 메시지 처리기

BOOL CBoardGameDlg::OnInitDialog()
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
	//플레이어 생성
	myPlayer = new Player;
	yourPlayer = new Player;
	MySocket.SetParent(this);			//서버 소켓
	YourSoket.SetParent(this);			//클라이언트
	isGameOn = FALSE;
	isIsolated = FALSE;
	isolatedCount = 0;
	m_even_count = numItem2;
	m_odd_count = numItem1;
	m_under_count = numItem3;
	m_over_count = numItem4;
	GetDlgItem(IDB_ROLL_DICE)-> EnableWindow(FALSE);
	GetDlgItem(IDC_START_GAME)-> EnableWindow(FALSE);
	srand((unsigned)time(NULL));
	int ver = 1;
	int hor = 1;
	int v = 1;
	for (int i = 0; i < BOARDSIZE; i++) {
		if (ver > 11 || ver <= 0) {
			v *= -1;
			ver += v;
			hor++;
			board[i].setCord(50 * (ver), 50 * hor);
			hor++;
		}
		else {
			board[i].setCord(50 * (ver), 50 * hor);
			ver += v;
		}
	}
	//특수칸 만들기 1 = 랜덤이동, 2 = 처음이로 이동, 3 = 아이템 획득, 4 = 무인도(3턴쉼)
	board[7].setBlockType(1);
	board[18].setBlockType(1);
	board[27].setBlockType(1);
	board[34].setBlockType(1);
	board[51].setBlockType(1);
	board[58].setBlockType(1);
	board[60].setBlockType(1);
	
	board[BOARDSIZE - 2].setBlockType(2);

	board[12].setBlockType(3);
	board[47].setBlockType(3);
	board[28].setBlockType(3);
	board[64].setBlockType(3);

	board[22].setBlockType(4);
	board[23].setBlockType(4);
	board[36].setBlockType(4);
	board[37].setBlockType(4);


	UpdateData(FALSE);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CBoardGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CBoardGameDlg::OnPaint()
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
		CClientDC dc(this);

		CBrush brush;
		CBrush* oldBrush;

		int r = 10;
		int pedding = 25;
		for (int i = 0; i < BOARDSIZE; i++) {
			int xPos = board[i].getX();
			int yPos = board[i].getY();
			if (i == 0) {
				brush.DeleteObject();
				brush.CreateSolidBrush(RGB(255, 0, 0));
			}
			else if (i == BOARDSIZE - 1) {
				brush.DeleteObject();
				brush.CreateSolidBrush(RGB(0, 255, 0));
			}
			
			else if (board[i].getBlockType() == 1) {
				brush.DeleteObject();
				brush.CreateSolidBrush(RGB(255, 255, 0));
			}
			else if (board[i].getBlockType() == 2) {
				brush.DeleteObject();
				brush.CreateSolidBrush(RGB(200, 0, 0));
			}
			else if (board[i].getBlockType() == 3) {
				brush.DeleteObject();
				brush.CreateSolidBrush(RGB(255, 102, 0));
			}
			else if (board[i].getBlockType() == 4) {
				brush.DeleteObject();
				brush.CreateSolidBrush(RGB(80, 80, 80));
			}
			else {
				brush.DeleteObject();
				brush.CreateSolidBrush(RGB(204, 201, 231));
			}
			oldBrush = dc.SelectObject(&brush);
			dc.Rectangle(xPos + pedding, yPos + pedding, xPos - pedding, yPos - pedding);

			if (i == yourPlayer->getI()) {
				brush.DeleteObject();
				if (userType) {
					brush.CreateSolidBrush(RGB(200, 255, 255));
					oldBrush = dc.SelectObject(&brush);
					dc.Rectangle(xPos + 18, yPos + 18, xPos - 18, yPos - 18);
				}
				else {
					brush.CreateSolidBrush(RGB(255, 200, 255));
					oldBrush = dc.SelectObject(&brush);
					dc.Ellipse(xPos + 15, yPos + 15, xPos - 15, yPos - 15);
				}
			}
			if (userType) {
				brush.DeleteObject();
				if (i == yourPlayer->getI()) {
					brush.CreateSolidBrush(RGB(200, 255, 255));
					oldBrush = dc.SelectObject(&brush);
					dc.Rectangle(xPos + 18, yPos + 18, xPos - 18, yPos - 18);
				}
				if (i == myPlayer->getI()) {
					brush.CreateSolidBrush(RGB(255, 200, 255));
					oldBrush = dc.SelectObject(&brush);
					dc.Ellipse(xPos + 15, yPos + 15, xPos - 15, yPos - 15);
				}
				
			}
			else {
				brush.DeleteObject();
				if (i == myPlayer->getI()) {
					brush.CreateSolidBrush(RGB(200, 255, 255));
					oldBrush = dc.SelectObject(&brush);
					dc.Rectangle(xPos + 18, yPos + 18, xPos - 18, yPos - 18);
				}
				if (i == yourPlayer->getI()) {
					brush.CreateSolidBrush(RGB(255, 200, 255));
					oldBrush = dc.SelectObject(&brush);
					dc.Ellipse(xPos + 15, yPos + 15, xPos - 15, yPos - 15);
				}

			}
			dc.SelectObject(oldBrush);
		}
		brush.DeleteObject();
		 
		// //////////
		//주사위 그림
		CDC MemDC;
		MemDC.CreateCompatibleDC(&dc);
		CBitmap diceBitMap;
		if (diceNum != 0) {
			diceBitMap.LoadBitmap(diceNum);
			CBitmap* oldbitmap = MemDC.SelectObject(&diceBitMap);
			//출력 좌표x, y, 폭, 넓이, 넣을 BITMAP DC, 저장한 것이 어디서 시작하는지 좌표
			dc.BitBlt(750, 400, 100, 100, &MemDC, 0, 0, SRCCOPY);
			dc.SelectObject(oldbitmap);
			diceBitMap.DeleteObject();
		}
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CBoardGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//방 만들기 버튼
void CBoardGameDlg::OnBnClickedCreatRoom(){
	UpdateData(TRUE);
	if (isGameOn) {
		MySocket.Close();
		GetDlgItem(IDC_ENTER_ROOM)->EnableWindow(TRUE);
		isGameOn = FALSE;
	}
	else {
		userType = TRUE;
		isGameOn = TRUE;
		//소켓 생성
		MySocket.Create(4000);				//포트 생성
		MySocket.Listen();
		isGameOn = TRUE;
		GetDlgItem(IDC_ENTER_ROOM)->EnableWindow(FALSE);
	}
}

//방 접속 버튼
void CBoardGameDlg::OnBnClickedEnterRoom(){
	UpdateData(TRUE);
	if (isGameOn) {
		YourSoket.Close();
		GetDlgItem(IDC_CREAT_ROOM)->EnableWindow(TRUE);
		isGameOn = FALSE;
	}
	else {
		userType = FALSE;
		isGameOn = TRUE;
		//소켓 생성
		YourSoket.Create();					//포트 생성
		YourSoket.Connect(serverAddress, 4000);
		GetDlgItem(IDC_CREAT_ROOM)->EnableWindow(FALSE);
	}
}

//게임 종료 버튼
void CBoardGameDlg::OnBnClickedExitGame()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
	//MessageBox(_T(""));
}
//소켓 관련 함수------------------------------------
void CBoardGameDlg::OnAccept(){
	MySocket.Accept(YourSoket);
	MessageBox(_T("상대가 접속하였습니다!")); //접속 확인용 코드
	GetDlgItem(IDC_START_GAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_CREAT_ROOM)->EnableWindow(FALSE);
}

void CBoardGameDlg::OnConnect(){
	MessageBox(_T("접속 성공!")); //접속 확인용 코드
	GetDlgItem(IDC_ENTER_ROOM)->EnableWindow(FALSE);
}

void CBoardGameDlg::OnClose(){
}
//데이터 받으면 실행
void CBoardGameDlg::OnReceive(){
	char* pBuf = new char[1025];
	int iBufSize = 1024;
	int iRcvd;
	iRcvd = YourSoket.Receive((char*)pBuf, iBufSize);

	if (iRcvd == SOCKET_ERROR) {
		MessageBox(_T("오류"));
	}
	else {
		pBuf[iRcvd-1] = NULL;
		receivedLocation = _tstoi((const wchar_t*)pBuf);
		SetTimer(DICE_YOUR_TIMER, 50, 0);
	}
	delete[] pBuf;
}

void CBoardGameDlg::OnSend(){}
//--------------------------------------------------------
void CBoardGameDlg::ClickedItem1(){
	if (numItem1> 0) {
		if (useItem1) {
			useItem1 = FALSE;
		}
		else {
			useItem1 = TRUE;
			useItem2 = FALSE;
		}
	}
	else{
		MessageBox(_T("아이템이 부족합니다."));
	}
	UpdateData(FALSE);
}
void CBoardGameDlg::ClickedItem2(){
	if (numItem2 > 0) {
		if (useItem2) {
			useItem2 = FALSE;
		}
		else {
			useItem2 = TRUE;
			useItem1 = FALSE;
		}
	}
	else {
		MessageBox(_T("아이템이 부족합니다."));
	}
	UpdateData(FALSE);
}

void CBoardGameDlg::ClickedItem3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (numItem3 > 0) {
		if (useItem3) {
			useItem3 = FALSE;
		}
		else {
			useItem3 = TRUE;
			useItem4 = FALSE;
		}
	}
	else {
		MessageBox(_T("아이템이 부족합니다."));
	}
	UpdateData(FALSE);
}


void CBoardGameDlg::ClickedItem4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (numItem4 > 0) {
		if (useItem4) {
			useItem4 = FALSE;
		}
		else {
			useItem3 = FALSE;
			useItem4 = TRUE;
		}
	}
	else {
		MessageBox(_T("아이템이 부족합니다."));
	}
	UpdateData(FALSE);
}

//주사위 굴리는 함수
void CBoardGameDlg::OnBnClickedRollDice(){
	GetDlgItem(IDB_ROLL_DICE)->EnableWindow(FALSE);
	SetTimer(DICE_TIMER, 50, 0);
	UpdateData(FALSE);
}

void CBoardGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);
	if (nIDEvent == DICE_TIMER) {
		if(isolatedCount) {
			isolatedCount--;
			CString sendLocation = _T("0");
			sendLocation.Format(_T("%d"), myPlayer->getI());
			//상대방에게 나의 위치를 보낸다.
			YourSoket.Send((LPCTSTR)sendLocation, (sendLocation.GetLength() + 1) * sizeof(TCHAR));
			KillTimer(DICE_TIMER);
			return;
		}

		diceNum = (rand() % 6) + 1;
		Invalidate();
		animationFrame++;

		if (animationFrame >= 20) {
			KillTimer(DICE_TIMER);
			animationFrame = 0;

			diceNum = rand() % 6 + 1;	//주사위 숫자
			if (useItem1) { //홀수 아이템 사용
				numItem1--;
				if (numItem1 <= 0) {
					useItem1 = FALSE;
				}
				while (diceNum % 2 == 0){
					diceNum = rand() % 6 + 1;
				}
			}
			if (useItem2) { //짝수 아이템 사용
				numItem2--;
				if (numItem2 <= 0) {
					useItem2 = FALSE;
				}
				while (diceNum % 2 != 0) {
					diceNum = rand() % 6 + 1;
				}
			}
			if (useItem3) { //3이하 아이템 사용
				numItem3--;
				if (numItem3 <= 0) {
					useItem3 = FALSE;
				}
				while (diceNum >= 3) {
					diceNum = rand() % 6 + 1;
				}
			}
			if (useItem4) { //4이상 아이템 사용
				numItem4--;
				if (numItem4 <= 0) {
					useItem4 = FALSE;
				}
				while (diceNum <= 4) {
					diceNum = rand() % 6 + 1;
				}
			}
			//-------------------여기까지 오면 주사위 숫자 구해짐
			int moveBlocks = myPlayer->getI() + diceNum;
			CString sendLocation;
			sendLocation.Format(_T("%d"), moveBlocks);
			myPlayer->SetI(moveBlocks);
			//상대방에게 나의 위치를 보낸다.
			YourSoket.Send((LPCTSTR)sendLocation, (sendLocation.GetLength() + 1) * sizeof(TCHAR));

			if (moveBlocks >= BOARDSIZE - 1) {
				moveBlocks = BOARDSIZE - 1;
				MessageBox(_T("내가 이김"));
			}
			else {
				//특수칸 이벤트 처리
				//플레이어 이동 후 이동한 만큼 상대에게 전달할 예정
				myPlayer->SetI(moveBlocks);
				if (board[moveBlocks].getBlockType() == 1) {
					MessageBox(_T("랜덤 이동!"));
					do {
						moveBlocks = rand() % (BOARDSIZE-7);
					} while (board[moveBlocks].getBlockType() != 0);
				}
				else if (board[moveBlocks].getBlockType() == 2) {
					MessageBox(_T("앗! 처음으로 이동!!"));
					moveBlocks = 0;
				}
				else if (board[moveBlocks].getBlockType() == 3) {
					MessageBox(_T("랜덤 아이템 획득!"));
					int itemType = rand() % 4;
					switch (itemType)
					{
					case 0:
						numItem1++;
						break;
					case 1:
						numItem2++;
						break;
					case 2:
						numItem3++;
						break;
					case 3:
						numItem4++;
						break;
					default:
						break;
					}
				}
				else if (board[moveBlocks].getBlockType() == 4) {
					MessageBox(_T("앗! 무인도에 갇혀버렸다!!"));
					isIsolated = TRUE;
					isolatedCount = 3;
				}
			}
			sendLocation.Format(_T("%d"), moveBlocks);
			myPlayer->SetI(moveBlocks);

			//상대방에게 나의 위치를 보낸다.
			YourSoket.Send((LPCTSTR)sendLocation, (sendLocation.GetLength() + 1) * sizeof(TCHAR));
			m_odd_count = numItem1;
			m_even_count = numItem2;
			m_under_count = numItem3;
			m_over_count = numItem4;
			useItem1 = FALSE;
			useItem2 = FALSE;
			useItem3 = FALSE;
			useItem4 = FALSE;
			UpdateData(FALSE);
			Invalidate();
		}
	}
	else if (nIDEvent == DICE_YOUR_TIMER) {
		if (board[yourPlayer->getI()].getBlockType() != 0) {
			animationFrame = 20;
		}

		if (animationFrame >= 20) {
			KillTimer(DICE_YOUR_TIMER);
			animationFrame = 0;
			
			moveBlocks = receivedLocation - yourPlayer->getI();
			diceNum = moveBlocks;
			yourPlayer->SetI(receivedLocation);
			if (board[yourPlayer->getI()].getBlockType() == 0 || board[yourPlayer->getI()].getBlockType() == 3 || board[yourPlayer->getI()].getBlockType() == 4) {
				GetDlgItem(IDB_ROLL_DICE)->EnableWindow(TRUE);
			}
			UpdateData(FALSE);
			Invalidate();
		}
		else {
			diceNum = (rand() % 6) + 1;
			Invalidate();
			animationFrame++;
		}


	}
	CDialogEx::OnTimer(nIDEvent);
}


void CBoardGameDlg::OnBnClickedStartGame()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_START_GAME)->EnableWindow(FALSE);
	GetDlgItem(IDB_ROLL_DICE)->EnableWindow(TRUE);

}