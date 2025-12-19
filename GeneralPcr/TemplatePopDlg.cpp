#include "StdAfx.h"
#include "TemplatePopDlg.h"
#include "GeneralPcr.h"

IMPLEMENT_DYNAMIC(CTemplatePopDlg,CDialogEx)

CTemplatePopDlg::CTemplatePopDlg(CWnd* pParent /*= NULL*/)
	: CDialogEx(CTemplatePopDlg::IDD, pParent)
{
	m_strTitle = _T("");
	m_iWidth = 330;
	m_iHeight = 230;
	m_frameWidth = 1;
	m_roundValue = 10;
	m_iHeaderH = 40;
}

CTemplatePopDlg::~CTemplatePopDlg(void)
{

}

void CTemplatePopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTemplatePopDlg,CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CTemplatePopDlg::OnEraseBkgnd(CDC* pDC)
{
#ifdef POPDLG_USESHADEBK
	CRect rect;
	GetClientRect(&rect);

	CDC memDc,memDc2;
	memDc.CreateCompatibleDC(pDC);
	memDc2.CreateCompatibleDC(pDC);

	CBitmap bitMapCB,bitMapCB2;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	bitMapCB2.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);
	CBitmap* pOldBitmapCB2 = memDc2.SelectObject(&bitMapCB2);

	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);
	memDc.SetBkMode(TRANSPARENT);

	memDc.BitBlt(0,0,rect.Width(),rect.Height(),pDC,0,0,SRCCOPY);
	memDc2.FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(0,0,0));

	BLENDFUNCTION blendFunc;
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.SourceConstantAlpha = CLRALPHAVALUE;
	blendFunc.AlphaFormat = AC_SRC_OVER;
	blendFunc.BlendFlags = 0;
	BOOL ret = memDc.AlphaBlend(0,0,rect.Width(),rect.Height(),&memDc2,0,0,rect.Width(),rect.Height(),blendFunc);

	CRect rcUseful;
	rcUseful.left = rect.left + (rect.Width() - m_iWidth) / 2;
	rcUseful.top = rect.top + (rect.Height() - m_iHeight) / 2;
	rcUseful.right = rcUseful.left + m_iWidth;
	rcUseful.bottom = rcUseful.top + m_iHeight;

	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top,rcUseful.Width(),rcUseful.Height(),m_roundValue,m_roundValue,m_roundValue,m_roundValue,TRUE,Color(197,197,197),m_frameWidth,TRUE,Color(255,255,255),1);
	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top,m_iWidth,m_iHeaderH,m_roundValue,m_roundValue,0,0,TRUE,Color(0XCC,0XCC,0XDC),2,TRUE,Color(0XCC,0XCC,0XDC),1);

	memDc.DrawText(m_strTitle,CRect(rcUseful.left + 10,rcUseful.top + 5,rcUseful.right - 10,rcUseful.top + m_iHeaderH - 5),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	memDc.SelectObject(pOldFont);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();

	memDc2.SelectObject(&pOldBitmapCB2);
	bitMapCB2.DeleteObject();
	memDc2.DeleteDC();
#else
	CRect rect;
	GetClientRect(&rect);

	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	memDc.FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(255,255,255));

	CPen pen;
	CPen* pOldPen = NULL;
	DrawRoundRectangleNew(&memDc,0 * m_dHoriRatio,0 * m_dVertRatio,rect.Width() - m_frameWidth * 2,rect.Height() - m_frameWidth * 2,m_roundValue,m_roundValue,m_roundValue,m_roundValue,TRUE,Color(197,197,197),m_frameWidth,FALSE,Color(0XCC,0XCC,0XDC),1);
	DrawRoundRectangleNew(&memDc,0 * m_dHoriRatio,0 * m_dVertRatio,330 * m_dHoriRatio,m_iHeaderH,m_roundValue,m_roundValue,0,0,TRUE,Color(0XCC,0XCC,0XDC),2,TRUE,Color(3,91,199),1);

	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);
	memDc.SetBkMode(TRANSPARENT);
	memDc.DrawText(m_strTitle,CRect(10,5,rect.Width() - 10,m_iHeaderH - 5),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	memDc.SelectObject(pOldFont);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();
#endif

	return TRUE;
}

void CTemplatePopDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
#ifdef POPDLG_USESHADEBK
	CDialogEx::OnLButtonDown(nFlags, point);
#else
	if(point.y < m_iHeaderH)
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	}
	else
	{
		CDialogEx::OnLButtonDown(nFlags, point);
	}
#endif

	return;
}

BOOL CTemplatePopDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

	InitDlg();

	CreateCtrl();
	InitCtrl();
	SetCtrlPos();
	ShowCtrl();

	return TRUE;
}

void CTemplatePopDlg::InitDlg()
{
	SetMyFont(m_stTxFont,16 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
#ifdef POPDLG_USESHADEBK
	SetWindowPos(GetParent(),theApp.m_rcWorkArea.left,theApp.m_rcWorkArea.top,theApp.m_rcWorkArea.Width(),theApp.m_rcWorkArea.Height(),SWP_NOZORDER);
#else
	SetWindowPos(GetParent(),0 * dHoriRatio,0 * dVertRatio,m_iWidth,m_iHeight,SWP_NOZORDER);
	CenterWindow(GetParent());
#endif
}

void CTemplatePopDlg::CreateCtrl()
{
	CreateBt();
	CreateSt();
	CreateCb();
	CreateEd();
	CreateLst();
}

void CTemplatePopDlg::CreateBt()
{

}

void CTemplatePopDlg::CreateSt()
{

}

void CTemplatePopDlg::CreateCb()
{

}

void CTemplatePopDlg::CreateEd()
{

}

void CTemplatePopDlg::CreateLst()
{

}

void CTemplatePopDlg::InitCtrl()
{
	InitBt();
	InitSt();
	InitCb();
	InitEd();
	InitLst();
}

void CTemplatePopDlg::InitBt()
{

}

void CTemplatePopDlg::InitSt()
{

}

void CTemplatePopDlg::InitCb()
{

}

void CTemplatePopDlg::InitEd()
{

}

void CTemplatePopDlg::InitLst()
{

}

void CTemplatePopDlg::SetCtrlPos()
{

}

void CTemplatePopDlg::ShowCtrl()
{

}

