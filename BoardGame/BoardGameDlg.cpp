
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
}

BEGIN_MESSAGE_MAP(CBoardGameDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CREAT_ROOM, &CBoardGameDlg::OnBnClickedCreatRoom)
	ON_BN_CLICKED(IDC_ENTER_ROOM, &CBoardGameDlg::OnBnClickedEnterRoom)
	ON_BN_CLICKED(IDC_EXIT_GAME, &CBoardGameDlg::OnBnClickedExitGame)
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

		CBrush brush, * oldBrush;

		int r = 10;
		int pedding = 25;
		for (int i = 0; i < BOARDSIZE; i++) {
			int xPos = board[i].getX();
			int yPos = board[i].getY();
			if (i == 0) {
				brush.CreateSolidBrush(RGB(255, 0, 0));
				oldBrush = dc.SelectObject(&brush);
			}
			else if (i == BOARDSIZE - 1) {
				brush.CreateSolidBrush(RGB(0, 255, 0));
				oldBrush = dc.SelectObject(&brush);
			}
			else if (i == BOARDSIZE - 2) {
				brush.CreateSolidBrush(RGB(200, 0, 0));
				oldBrush = dc.SelectObject(&brush);
			}
			else if (board[i].getBlockType() == 1) {
				brush.CreateSolidBrush(RGB(255, 255, 0));
				oldBrush = dc.SelectObject(&brush);
			}
			else {
				brush.CreateSolidBrush(RGB(204, 201, 231));
				oldBrush = dc.SelectObject(&brush);
			}

			dc.Rectangle(xPos + pedding, yPos + pedding, xPos - pedding, yPos - pedding);
		}
		dc.SelectObject(oldBrush);
		brush.DeleteObject();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CBoardGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//방 만들기 버튼
void CBoardGameDlg::OnBnClickedCreatRoom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

//방 접속 버튼
void CBoardGameDlg::OnBnClickedEnterRoom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

//게임 종료 버튼
void CBoardGameDlg::OnBnClickedExitGame()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
	//MessageBox(_T(""));
}
