// SkipDlg.cpp : implementation file
//

#include "stdafx.h"

#include "MyGeneTipDlg.h"
#include "GeneralPcrDlg.h"
#include "GeneralPcr.h"

// CMyGeneTipDlg dialog
IMPLEMENT_DYNAMIC(CMyGeneTipDlg, CDialogEx)

CMyGeneTipDlg::CMyGeneTipDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyGeneTipDlg::IDD, pParent)
{
	m_frameWidth = 1;
	m_roundValue = 20;
	m_strNoteInfo = _T("");
	m_noteFontSize = 20;
	m_dwNoteStyle = DT_LEFT | DT_VCENTER | DT_WORDBREAK | DT_NOFULLWIDTHCHARBREAK;
	m_eGeneTipType = EGTT_NORMAL;
	m_width = 480;
	m_height = 420;
	m_iCapHeight = 60;
	m_retCode = IDCANCEL;
}

CMyGeneTipDlg::~CMyGeneTipDlg()
{
}

void CMyGeneTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyGeneTipDlg, CDialogEx)
	ON_WM_ERASEBKGND() 
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(ID_GENERALTIPDLG_BT_OK,&CMyGeneTipDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_GENERALTIPDLG_BT_CANCEL,&CMyGeneTipDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CMyGeneTipDlg::OnEraseBkgnd(CDC* pDC)
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	
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
	rcUseful.left = rect.left + (rect.Width() - m_width) / 2;
	rcUseful.top = rect.top + (rect.Height() - m_height) / 2;
	rcUseful.right = rcUseful.left + m_width;
	rcUseful.bottom = rcUseful.top + m_height;

	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top + m_iCapHeight,rcUseful.Width(),rcUseful.Height() - m_iCapHeight,0,0,m_roundValue,m_roundValue,FALSE,Color(197,197,197),m_frameWidth,TRUE,Color(255,255,255),1);
	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top,m_width,m_iCapHeight,m_roundValue,m_roundValue,0,0,FALSE,Color(0X5F,0X6F,0X7F),m_frameWidth,TRUE,COLOR_SOFT_THEME,1);

	memDc.DrawText(pLangInfo->GetText(147),CRect(rcUseful.left + 10,rcUseful.top + 5,rcUseful.right - 10,rcUseful.top + m_iCapHeight - 5),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	memDc.SelectObject(pOldFont);

	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(0XD8,0XD8,0XD8));
	CPen* pOldPen = memDc.SelectObject(&pen);
	memDc.MoveTo(rcUseful.left,rcUseful.top + 331 * m_dVertRatio);
	memDc.LineTo(rcUseful.left + m_width,rcUseful.top + 331 * m_dVertRatio);
	memDc.SelectObject(pOldPen);

	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);
	clock_t ckStart = clock();	
	Node<CRect>* pNodeRst = m_rcManage.m_lnkRcRst.GetHead();
	while(NULL != pNodeRst)
	{
		pDC->BitBlt(pNodeRst->data.left,pNodeRst->data.top,pNodeRst->data.Width(),pNodeRst->data.Height(),&memDc,pNodeRst->data.left,pNodeRst->data.top,SRCCOPY);
		pNodeRst = pNodeRst->pNext;
	}
	ckStart = clock() - ckStart;

	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();

	memDc2.SelectObject(&pOldBitmapCB2);
	bitMapCB2.DeleteObject();
	memDc2.DeleteDC();
#else
	double dHoriRatio = theApp.m_dHoriRatio;
	double dVertRatio = theApp.m_dVertRatio;

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
	DrawRoundRectangleNew(&memDc,0 * dHoriRatio,0 * dVertRatio,rect.Width() - m_frameWidth * 2,rect.Height() - m_frameWidth * 2,m_roundValue,m_roundValue,m_roundValue,m_roundValue,TRUE,Color(197,197,197),m_frameWidth,FALSE,Color(3,91,199),1);
	DrawRoundRectangleNew(&memDc,0 * dHoriRatio,0 * dVertRatio,330 * dHoriRatio,40 * dVertRatio,4,4,0,0,TRUE,Color(3,91,199),2,TRUE,Color(3,91,199),1);

	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);
	memDc.SetBkMode(TRANSPARENT);
	memDc.DrawText(pLangInfo->GetText(147),CRect(10,5,rect.Width() - 10,40 * dVertRatio - 5),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	memDc.SelectObject(pOldFont);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();
#endif

	return TRUE;
}

int CMyGeneTipDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_width *= m_dHoriRatio;
	m_height *= m_dVertRatio;
	m_iCapHeight *= m_dVertRatio;
	m_noteFontSize *= m_dTxRatio;

	return CDialogEx::OnCreate(lpCreateStruct);
}

void CMyGeneTipDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
#ifdef POPDLG_USESHADEBK
	CDialogEx::OnLButtonDown(nFlags, point);
#else
	if(point.y < m_iCapHeight)
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

BOOL CMyGeneTipDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

#ifdef POPDLG_USESHADEBK
	SetWindowPos(GetParent(),theApp.m_rcWorkArea.left,theApp.m_rcWorkArea.top,theApp.m_rcWorkArea.Width(),theApp.m_rcWorkArea.Height(),SWP_NOZORDER);
#else
	SetWindowPos(GetParent(),0,0,m_width,m_height,SWP_NOZORDER);
	SetRoundRect(m_roundValue,m_roundValue);
	CenterWindow(GetParent());
#endif

	CreateCtrl();
	InitDlg();
	InitCtrl();
	SetCtrlPos();

	SetCtrlText();
	CheckNoteInfoSize();

	ShowCtrl();

	return TRUE;
}

void CMyGeneTipDlg::CreateCtrl()
{
	CreateBt();
	CreateCkBt();
	CreateSt();
	CreateEd();
}

void CMyGeneTipDlg::CreateBt()
{
	m_btOk.Create(_T("确定"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_GENERALTIPDLG_BT_OK);
	m_btCancel.Create(_T("取消"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_GENERALTIPDLG_BT_CANCEL);
}

void CMyGeneTipDlg::CreateCkBt()
{
	
}

void CMyGeneTipDlg::CreateSt()
{
	m_stNoteInfo.Create(_T(""),WS_CHILD,CRect(0,0,0,0),this,ID_GENERALTIPDLG_ST_NOTEINFO);
	m_stIcon.Create(_T(""),WS_CHILD,CRect(0,0,0,0),this,ID_GENERALTIPDLG_ST_ICON);
}

void CMyGeneTipDlg::CreateEd()
{
	
}

void CMyGeneTipDlg::InitDlg()
{
	int iFontH = 24 * m_dTxRatio;

	SetTextClr1(RGB(0XFF,0XFF,0XFF));
	SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
}

void CMyGeneTipDlg::InitCtrl()
{
	InitBt();
	InitCkBt();
	InitSt();
	InitEd();
}

void CMyGeneTipDlg::InitBt()
{
	int iFontH = 18 * m_dTxRatio;

	m_btOk.SetMyFont(m_stTxFont,iFontH);
#ifndef GRADIENT_SOME_BTBK
	m_btOk.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),FALSE);
	m_btOk.SetBkColor(COLOR_BTBK_NORMAL,COLOR_BTBK_HOVER,COLOR_BTBK_CLICKED,COLOR_BTBK_DISABLED);
#else
	m_btOk.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);
	m_btOk.SetBkColor(Color(0X38,0XE0,0XEF),Color(0X3A,0XEF,0XFF),COLOR_SOFT_THEME,Color(0XF1,0XF1,0XF1));
	m_btOk.SetBkColorEnd(COLOR_SOFT_THEME,Color(0X38,0XE0,0XEF),Color(0X02,0X8D,0X99),Color(0XD8,0XD8,0XD8));
#endif
	m_btOk.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XC6,0XC4,0XC4));

	m_btCancel.SetMyFont(m_stTxFont,iFontH);
	m_btCancel.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);
}

void CMyGeneTipDlg::InitCkBt()
{
	
}

void CMyGeneTipDlg::InitSt()
{
	m_stNoteInfo.SetBDynamic(TRUE);
	m_stNoteInfo.SetBkColor(RGB(0XFF,0XFF,0XFF));
	m_stNoteInfo.SetMyFont(m_stTxFont,m_noteFontSize,FW_NORMAL,TYPE_PIXEL);
	m_stNoteInfo.SetTextColor(RGB(0X39,0X45,0X65));
	m_stNoteInfo.SetWordStyle(m_dwNoteStyle);
	m_stNoteInfo.SetWindowText(m_strNoteInfo);

	CString strTemp(_T(""));
	m_stIcon.SetBkColor(RGB(0XFF,0XFF,0XFF));
	if(EGTT_NORMAL == m_eGeneTipType)
	{
		strTemp = _T(".\\BkPic\\normaltip.png");
	}
	else if(EGTT_QUERY == m_eGeneTipType)
	{
		strTemp = _T(".\\BkPic\\querytip.png");
	}
	else if(EGTT_WARNING == m_eGeneTipType)
	{
		strTemp = _T(".\\BkPic\\warningtip.png");
	}
	m_stIcon.SetPngBkPic(strTemp);
}

void CMyGeneTipDlg::InitEd()
{
	
}

void CMyGeneTipDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	m_rcManage.m_rcInit = rect;
	m_rcManage.ClearRectToDelete();

	CRect rcUseful;
#ifdef POPDLG_USESHADEBK
	rcUseful.left = rect.left + (rect.Width() - m_width) / 2;
	rcUseful.top = rect.top + (rect.Height() - m_height) / 2;
	rcUseful.right = rcUseful.left + m_width;
	rcUseful.bottom = rcUseful.top + m_height;
#endif
	
	int width = 90 * m_dHoriRatio;
	int height = 40 * m_dVertRatio;

	m_stIcon.SetWindowPos(this,rcUseful.left + 56 * m_dHoriRatio,rcUseful.top + 180 * m_dVertRatio,60 * m_dHoriRatio,61 * m_dVertRatio,SWP_NOZORDER);
	
	m_stNoteInfo.SetWindowPos(this,rcUseful.left + 156 * m_dHoriRatio,rcUseful.top + 138 * m_dVertRatio,290 * m_dHoriRatio,150 * m_dVertRatio,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(rcUseful.left + 156 * m_dHoriRatio,rcUseful.top + 138 * m_dVertRatio,290 * m_dHoriRatio,150 * m_dVertRatio);

	if(EGTT_NORMAL == m_eGeneTipType || EGTT_WARNING == m_eGeneTipType)
	{
		m_btOk.SetWindowPos(this,rcUseful.left + 342 * m_dHoriRatio,rcUseful.top + 356 * m_dVertRatio,width,height,SWP_NOZORDER);
		m_rcManage.AddRectToDelete(rcUseful.left + 342 * m_dHoriRatio,rcUseful.top + 356 * m_dVertRatio,width,height);
	}
	else if(EGTT_QUERY == m_eGeneTipType)
	{
		m_btOk.SetWindowPos(this,rcUseful.left + 228 * m_dHoriRatio,rcUseful.top + 356 * m_dVertRatio,width,height,SWP_NOZORDER);
		m_rcManage.AddRectToDelete(rcUseful.left + 228 * m_dHoriRatio,rcUseful.top + 356 * m_dVertRatio,width,height);

		m_btCancel.SetWindowPos(this,rcUseful.left + 342 * m_dHoriRatio,rcUseful.top + 356 * m_dVertRatio,width,height,SWP_NOZORDER);
		m_rcManage.AddRectToDelete(rcUseful.left + 342 * m_dHoriRatio,rcUseful.top + 356 * m_dVertRatio,width,height);
	}
	
	m_rcManage.CalcRectResult();

	return;
}

void CMyGeneTipDlg::ShowCtrl()
{
	m_stNoteInfo.ShowWindow(SW_SHOW);
	m_stIcon.ShowWindow(SW_SHOW);

	if(EGTT_NORMAL == m_eGeneTipType || EGTT_WARNING == m_eGeneTipType)
	{
		m_btOk.ShowWindow(SW_SHOW);
	}
	else if(EGTT_QUERY == m_eGeneTipType)
	{
		m_btOk.ShowWindow(SW_SHOW);
		m_btCancel.ShowWindow(SW_SHOW);
	}
}

void CMyGeneTipDlg::SetCtrlText()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	if(EGTT_QUERY == m_eGeneTipType)
	{
		m_btOk.SetWindowText(pLangInfo->GetText(151));
		m_btCancel.SetWindowText(pLangInfo->GetText(152));
	}
	else
	{
		m_btOk.SetWindowText(pLangInfo->GetText(149));
		m_btCancel.SetWindowText(pLangInfo->GetText(150));
	}

	return;
}

void CMyGeneTipDlg::OnBnClickedOk()
{
	if(EGTT_NORMAL == m_eGeneTipType || EGTT_WARNING == m_eGeneTipType)
	{
		m_retCode = IDOK;
	}
	else if(EGTT_QUERY == m_eGeneTipType)
	{
		m_retCode = IDYES;
	}
	else
	{
		m_retCode = IDOK;
	}
	OnOK();
	return;
}

void CMyGeneTipDlg::OnBnClickedCancel()
{
	if(EGTT_NORMAL == m_eGeneTipType || EGTT_WARNING == m_eGeneTipType)
	{
		m_retCode = IDCANCEL;
	}
	else if(EGTT_QUERY == m_eGeneTipType)
	{
		m_retCode = IDCANCEL;
	}
	else
	{
		m_retCode = IDCANCEL;
	}
	OnCancel();
	return;
}

void CMyGeneTipDlg::SetNoteInfo(CString strNoteInfo)
{
	m_strNoteInfo = strNoteInfo;
}

CString CMyGeneTipDlg::GetNoteInfo()
{
	return m_strNoteInfo;
}

void CMyGeneTipDlg::SetGeneTipType(EGENETIPTYPE eGeneType)
{
	m_eGeneTipType = eGeneType;
}

EGENETIPTYPE CMyGeneTipDlg::GetGeneTipType()
{
	return m_eGeneTipType;
}

void CMyGeneTipDlg::SetNoteInfoStyle(DWORD style)
{
	m_dwNoteStyle = style;
}

DWORD CMyGeneTipDlg::GetNoteInfoStyle()
{
	return m_dwNoteStyle;
}

BOOL CMyGeneTipDlg::SetRoundRect(int x /*= 5*/,int y /*= 5*/)
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

void CMyGeneTipDlg::CheckNoteInfoSize()
{
	clock_t startMs = clock();

	if(NULL == m_stNoteInfo.GetSafeHwnd())
	{
		return;
	}
	CDC* pDc = m_stNoteInfo.GetDC();
	CFont* pOldFont = pDc->SelectObject(&(m_stNoteInfo.m_fontSet));
	CSize sizeStr = pDc->GetTextExtent(m_strNoteInfo);

	CRect rcStatic;
	m_stNoteInfo.GetClientRect(&rcStatic);
	int iStWidth = rcStatic.Width();

	if(sizeStr.cx > iStWidth)
	{
		m_noteFontSize = 19 * m_dTxRatio;
		pDc->SelectObject(pOldFont);
		m_stNoteInfo.SetMyFont(m_stTxFont,m_noteFontSize,FW_NORMAL,TYPE_PIXEL);
		pDc->SelectObject(&(m_stNoteInfo.m_fontSet));
		sizeStr = pDc->GetTextExtent(m_strNoteInfo);

		if(sizeStr.cx > iStWidth)
		{
			m_dwNoteStyle = DT_LEFT | DT_VCENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_NOFULLWIDTHCHARBREAK; 
			m_stNoteInfo.SetSingleLineState(FALSE);
			m_stNoteInfo.SetWordStyle(m_dwNoteStyle); 
		}
		else
		{
			m_stNoteInfo.SetSingleLineState(TRUE);
		}
	}

	pDc->SelectObject(pOldFont);
	ReleaseDC(pDc);

	return;
}



