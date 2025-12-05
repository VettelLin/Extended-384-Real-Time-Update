// SkipDlg.cpp : implementation file
//

#include "stdafx.h"

#include "FloatDlg.h"
#include "GeneralPcr.h"

// CFloatDlg dialog
IMPLEMENT_DYNAMIC(CFloatDlg, CDialogEx)

CFloatDlg::CFloatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFloatDlg::IDD, pParent)
{
	m_frameWidth = 2;
	m_roundValue = 4;
	m_iWndWidth = 136;
	m_iWndHeight = 136;
	m_iCaptionHeight = 20;
	m_pParent = pParent;
	m_iHoriDotNum = 0;
	m_iVertDotNum = 2;
}

CFloatDlg::~CFloatDlg()
{

}

void CFloatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFloatDlg, CDialogEx)
	ON_WM_ERASEBKGND() 
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SHOWWINDOW()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

BOOL CFloatDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	memDc.BitBlt(0,0,rect.Width(),rect.Height(),pDC,0,0,SRCCOPY);

	CPen pen;
	CPen* pOldPen = NULL;
	DrawRoundRectangleNew(&memDc,2 + m_frameWidth,2 + m_frameWidth,rect.Width() - m_frameWidth * 2 - 2,rect.Height() - m_frameWidth * 2 - 2,m_roundValue,m_roundValue,m_roundValue,m_roundValue,TRUE,Color(233,233,233),m_frameWidth,TRUE,Color(0XFF,0XFF,0XFF),1);

	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);
	memDc.SetBkMode(TRANSPARENT);

	DrawTipText(&memDc);

	memDc.SelectObject(pOldFont);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);
	
	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();

	return TRUE;
}

int CFloatDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_iWndWidth *= m_dHoriRatio;
	m_iWndHeight *= m_dVertRatio;
	return CDialogEx::OnCreate(lpCreateStruct);
}

void CFloatDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonDown(nFlags,point); 
}

void CFloatDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonUp(nFlags,point); 
}

void CFloatDlg::OnSetFocus(CWnd* pOldWnd)
{
	return;
}

void CFloatDlg::OnKillFocus(CWnd* pNewWnd)
{
	ReleaseCapture();
	ShowWindow(SW_HIDE);

	return;
}

void CFloatDlg::OnShowWindow(BOOL bShow,UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow,nStatus);

	if(bShow)
	{
		SetCapture();
	}
	return;
}

void CFloatDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(&rect);
	if(!rect.PtInRect(point))
	{
		ReleaseCapture();
		ShowWindow(SW_HIDE);
	}
	return;
}

BOOL CFloatDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

	SetWindowPos(GetParent(),0,0,m_iWndWidth,m_iWndHeight,SWP_NOZORDER);

	CreateCtrl();
	InitDlg();
	InitCtrl();
	SetCtrlPos();

	ShowCtrl();

	return TRUE;
}

void CFloatDlg::CreateCtrl()
{
	CreateBt();
	CreateCkBt();
	CreateSt();
	CreateEd();
	CreateCb();
	CreateLst();
}

void CFloatDlg::CreateBt()
{
	
}

void CFloatDlg::CreateCkBt()
{
	
}

void CFloatDlg::CreateSt()
{
	
}

void CFloatDlg::CreateEd()
{
	
}

void CFloatDlg::CreateCb()
{
	
}

void CFloatDlg::CreateLst()
{
	
}

void CFloatDlg::InitDlg()
{
	SetTextClr1(RGB(0X39,0X45,0X65));
	SetMyFont(m_stTxFont,16 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
}

void CFloatDlg::InitCtrl()
{
	InitBt();
	InitCkBt();
	InitSt();
	InitEd();
	InitCb();
	InitLst();
}

void CFloatDlg::InitBt()
{
	
}

void CFloatDlg::InitCkBt()
{
	
}

void CFloatDlg::InitSt()
{
	
}

void CFloatDlg::InitEd()
{
	
}

void CFloatDlg::InitCb()
{
	
}

void CFloatDlg::InitLst()
{
	return;
}

void CFloatDlg::SetCtrlPos()
{
	
}

void CFloatDlg::ShowCtrl()
{
	return;
}

BOOL CFloatDlg::SetRoundRect(int x /*= 5*/,int y /*= 5*/)
{
	CRgn rgnWnd;
	CRect rcWnd;
	GetClientRect(&rcWnd);
	rgnWnd.CreateRoundRectRgn(rcWnd.left,rcWnd.top,rcWnd.right,rcWnd.bottom,x,y);
	if(!SetWindowRgn(rgnWnd,TRUE))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

int CFloatDlg::DrawTipText(CDC* pDc)
{
	if(NULL == pDc)
	{
		return -1;
	}

	CString strTemp(_T("")),strFormat(_T(""));

	if(m_lineInfo.GetBeXAxisMillisecondTime())
	{
		CString strTime(_T(""));
		int curSecond = int(m_ptDotInfo.m_X) / 1000;
		strTime.Format(_T("%01d:%02d:%02d"),curSecond / 3600,(curSecond % 3600) / 60,curSecond % 60);

		strFormat.Format(_T("(%%s,%%.%dlf)"),m_iVertDotNum);
		strTemp.Format(strFormat,strTime,m_ptDotInfo.m_Y);
	}
	else
	{
		strFormat.Format(_T("(%%.%dlf,%%.%dlf)"),m_iHoriDotNum,m_iVertDotNum);
		strTemp.Format(strFormat,m_ptDotInfo.m_X,m_ptDotInfo.m_Y);
	}

	CRect rect;
	GetClientRect(&rect);

	pDc->DrawText(m_lineInfo.m_strHoleTag,CRect(12 * m_dHoriRatio,12 * m_dVertRatio,rect.Width() - 12 * m_dHoriRatio,34 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	pDc->DrawText(m_lineInfo.m_strDyeName,CRect(12 * m_dHoriRatio,42 * m_dVertRatio,rect.Width() - 12 * m_dHoriRatio,64 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	pDc->DrawText(m_lineInfo.m_strTaget,CRect(12 * m_dHoriRatio,72 * m_dVertRatio,rect.Width() - 12 * m_dHoriRatio,94 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	pDc->DrawText(strTemp,CRect(12 * m_dHoriRatio,102 * m_dVertRatio,rect.Width() - 12 * m_dHoriRatio,124 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE );
}

void CFloatDlg::SetHoriDotNum(int iData)
{
	if(iData < 0)
	{
		return;
	}
	m_iHoriDotNum = iData;
}

int CFloatDlg::GetHoriDotNum()
{
	return m_iHoriDotNum;
}

void CFloatDlg::SetVertDotNum(int iData)
{
	if(iData < 0)
	{
		return;
	}
	m_iVertDotNum = iData;
}

int CFloatDlg::GetVertDotNum()
{
	return m_iVertDotNum;
}





