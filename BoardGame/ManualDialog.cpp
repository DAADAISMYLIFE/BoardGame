// ManualDialog.cpp: 구현 파일
//

#include "pch.h"
#include "BoardGame.h"
#include "ManualDialog.h"
#include "afxdialogex.h"


// ManualDialog 대화 상자

IMPLEMENT_DYNAMIC(ManualDialog, CDialog)

ManualDialog::ManualDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MANUAL_DLG, pParent)
{

}

ManualDialog::~ManualDialog()
{
}

void ManualDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ManualDialog, CDialog)
	ON_BN_CLICKED(IDOK, &ManualDialog::OnBnClickedOk)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// ManualDialog 메시지 처리기


void ManualDialog::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
}


void ManualDialog::OnPaint() {
	int setPosX = 150;
	int setPosY = 35;
	CPaintDC dc(this);
	CBrush brush;
	CBrush* oldBrush;
	//블럭 색깔별 설명
	for (int i = 0; i < 8; i++) {
		switch (i) {
		case 0: //시작칸
			brush.DeleteObject();
			brush.CreateSolidBrush(RGB(255, 0, 0));
			oldBrush = dc.SelectObject(&brush);
			break;
		case 1: //끝칸
			brush.DeleteObject();
			brush.CreateSolidBrush(RGB(0, 255, 0));
			oldBrush = dc.SelectObject(&brush);
			break;
		case 2: //함정칸
			brush.DeleteObject();
			brush.CreateSolidBrush(RGB(255, 255, 0));
			oldBrush = dc.SelectObject(&brush);
			break;
		case 3: //맨 마지막 전 칸 
			brush.DeleteObject();
			brush.CreateSolidBrush(RGB(200, 0, 0));
			oldBrush = dc.SelectObject(&brush);
			break;
		case 4: //아아템칸
			brush.DeleteObject();
			brush.CreateSolidBrush(RGB(255, 102, 0));
			oldBrush = dc.SelectObject(&brush);
			break;
		case 5: //무인도칸
			brush.DeleteObject();
			brush.CreateSolidBrush(RGB(80, 80, 80));
			oldBrush = dc.SelectObject(&brush);
			break;
		case 6: //일반칸
			brush.DeleteObject();
			brush.CreateSolidBrush(RGB(204, 201, 231));
			oldBrush = dc.SelectObject(&brush);
			break;
		case 7: 
			//플레이어 1
			brush.DeleteObject();
			brush.CreateSolidBrush(RGB(200, 255, 255));
			oldBrush = dc.SelectObject(&brush);
			break;
		}
		dc.Rectangle(setPosX, setPosY, 110, setPosY + 40);
		setPosY += 50;
	}
	//플레이어 2
	brush.DeleteObject();
	brush.CreateSolidBrush(RGB(255, 200, 255));
	oldBrush = dc.SelectObject(&brush);
	dc.Ellipse(110,setPosY,setPosX, setPosY+ 40);

	brush.DeleteObject();
}