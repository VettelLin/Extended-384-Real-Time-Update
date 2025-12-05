// SkipDlg.cpp : implementation file
//

#include "stdafx.h"

#include "Ps96CutOffDlg.h"
#include "GeneralPcr.h"
#include "PublicInfo.h"

// CPs96CutOffDlg dialog
IMPLEMENT_DYNAMIC(CPs96CutOffDlg, CDialogEx)

CPs96CutOffDlg::CPs96CutOffDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPs96CutOffDlg::IDD, pParent)
{
	m_iAngleRadius = 20;
	m_iFrameWidth = 1;
	m_iWndWidth = 700;
	m_iWndHeight = 700;
	m_iCapHeight = 60;

	m_pAnalysisParam = NULL;
	m_bChanged = FALSE;
}

CPs96CutOffDlg::~CPs96CutOffDlg()
{
}

void CPs96CutOffDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPs96CutOffDlg, CDialogEx)
	ON_WM_ERASEBKGND() 
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(ID_PS96SIMPLESETDLG_BT_SAVE,OnBnClickedSave)
	ON_BN_CLICKED(ID_PS96SIMPLESETDLG_BT_CANCEL,OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CPs96CutOffDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	CDC memDc,memDc2;
	memDc.CreateCompatibleDC(pDC);
	memDc2.CreateCompatibleDC(pDC);

	CBitmap bitMapCB,bitMapCB2;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);
	bitMapCB2.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB2 = memDc2.SelectObject(&bitMapCB2);

	memDc.BitBlt(0,0,rect.Width(),rect.Height(),pDC,0,0,SRCCOPY);
	memDc2.FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(0,0,0));

	BLENDFUNCTION blendFunc;
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.SourceConstantAlpha = CLRALPHAVALUE;
	blendFunc.AlphaFormat = AC_SRC_OVER;
	blendFunc.BlendFlags = 0;
	BOOL ret = memDc.AlphaBlend(0,0,rect.Width(),rect.Height(),&memDc2,0,0,rect.Width(),rect.Height(),blendFunc);

	CRect rcUseful;
	rcUseful.left = rect.left + (rect.Width() - m_iWndWidth) / 2;
	rcUseful.top = rect.top + (rect.Height() - m_iWndHeight) / 2;
	rcUseful.right = rcUseful.left + m_iWndWidth;
	rcUseful.bottom = rcUseful.top + m_iWndHeight;

	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top + m_iCapHeight,rcUseful.Width(), rcUseful.Height() - m_iCapHeight,0,0,m_iAngleRadius,m_iAngleRadius,FALSE,Color(197,197,197),m_iFrameWidth,TRUE,Color(255,255,255),1);
	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top,rcUseful.Width(),m_iCapHeight,m_iAngleRadius,m_iAngleRadius,0,0,FALSE,COLOR_SOFT_THEME,m_iFrameWidth,TRUE,COLOR_SOFT_THEME,1);

	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);
	memDc.SetBkMode(TRANSPARENT);

	DWORD dwTxStyle = DT_LEFT | DT_VCENTER | DT_SINGLELINE;

	memDc.SetTextColor(RGB(255,255,255));
	memDc.DrawText(theApp.m_pLangInfo->GetText(485),CRect(rcUseful.left + 10,rcUseful.top + 5,rcUseful.left + rcUseful.Width() - 10,rcUseful.top + m_iCapHeight - 5),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	memDc.SelectObject(pOldFont);

	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();

	memDc2.SelectObject(&pOldBitmapCB2);
	bitMapCB2.DeleteObject();
	memDc2.DeleteDC();

	return TRUE;
}

int CPs96CutOffDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_iWndWidth *= m_dHoriRatio;
	m_iWndHeight *= m_dVertRatio;
	m_iCapHeight *= m_dVertRatio;
	return CDialogEx::OnCreate(lpCreateStruct);
}

void CPs96CutOffDlg::OnLButtonDown(UINT nFlags, CPoint point)
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

BOOL CPs96CutOffDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

#ifdef POPDLG_USESHADEBK
	SetWindowPos(GetParent(),theApp.m_rcWorkArea.left,theApp.m_rcWorkArea.top,theApp.m_rcWorkArea.Width(),theApp.m_rcWorkArea.Height(),SWP_NOZORDER);
#else
	SetWindowPos(GetParent(),0,0,m_iWndWidth,m_iWndHeight,SWP_NOZORDER);
	SetRoundRect(m_iAngleRadius,m_iAngleRadius);
	CenterWindow(GetParent());
#endif

	CreateCtrl();
	InitDlg();
	InitCtrl();
	SetCtrlPos();

	SetCtrlText();
	ShowCtrl();

	return TRUE;
}

void CPs96CutOffDlg::CreateCtrl()
{
	m_btnSave.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PS96SIMPLESETDLG_BT_SAVE);
	m_btnCancel.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PS96SIMPLESETDLG_BT_CANCEL);

	m_listCutOff.Create(IDD_MIDDLEDLG,this);
}

void CPs96CutOffDlg::InitDlg()
{
	SetTextClr1(RGB(0X39,0X45,0X65));
	SetMyFont(m_stTxFont,30 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
}

void CPs96CutOffDlg::InitCtrl()
{
	int iFontH = 26 * m_dTxRatio;

	m_btnSave.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btnSave.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);
	m_btnSave.SetBkColor(Color(0X38,0XE0,0XEF),Color(0X3A,0XEF,0XFF),COLOR_SOFT_THEME,Color(0XF1,0XF1,0XF1));
	m_btnSave.SetBkColorEnd(COLOR_SOFT_THEME,Color(0X38,0XE0,0XEF),Color(0X02,0X8D,0X99),Color(0XD8,0XD8,0XD8));
	m_btnSave.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XC6,0XC4,0XC4));

	m_btnCancel.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btnCancel.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);

	InitLst();
}

void CPs96CutOffDlg::InitLst()
{
	int iFontH = 26 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 27 * m_dTxRatio;
	}

	m_listCutOff.SetHeaderHeight(60 * m_dVertRatio);
	int iTempInt1 = 34 * m_dVertRatio;
	m_listCutOff.m_horiScroll.m_arrowWH = iTempInt1;
	m_listCutOff.m_horiScroll.m_scrollH = iTempInt1;
	m_listCutOff.m_vertScroll.m_arrowWH = iTempInt1;
	m_listCutOff.m_vertScroll.m_scrollW = iTempInt1;

	m_listCutOff.SetMyTitleFont(m_stTxFont,iFontH,FW_MEDIUM,TYPE_PIXEL);

	//m_listCutOff.SetLargeCheckIconUseState(TRUE);
	m_listCutOff.SetBeSingleSelect(TRUE);
	m_listCutOff.SetBeAllowCancelRowSelect(TRUE);
	m_listCutOff.SetBeTellParentRowSelChange(TRUE);
	m_listCutOff.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_listCutOff.SetTextClr1(RGB(0X39,0X45,0X65));
	m_listCutOff.SetMyHeaderFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_listCutOff.SetBeSingleSelect(TRUE);
	m_listCutOff.SetColumnCheckFunc(0,TRUE);
	m_listCutOff.SetFourAngleBkColor(RGB(0XFF,0XFF,0XFF));
	m_listCutOff.SetItemHeight(int(50 * m_dVertRatio));
	m_listCutOff.SetRowBkSelectedColor(RGB(0XF1,0XF1,0XF1));

	m_listCutOff.InsertColumn(0,theApp.m_pLangInfo->GetText(29),FALSE);
	m_listCutOff.InsertColumn(1,theApp.m_pLangInfo->GetText(112),FALSE);
	m_listCutOff.InsertColumn(2,theApp.m_pLangInfo->GetText(53),FALSE);
	m_listCutOff.InsertColumn(3,theApp.m_pLangInfo->GetText(486),FALSE);
	m_listCutOff.SetColumnEditable(3,TRUE);

	int tempInt1 = 0,tempInt2 = 668 * m_dHoriRatio;
	tempInt1 = 80 * m_dHoriRatio;
	m_listCutOff.SetColumnWidth(0,tempInt1);
	tempInt2 -= tempInt1;
	tempInt1 = 120 * m_dHoriRatio;
	m_listCutOff.SetColumnWidth(1,tempInt1);
	tempInt2 -= tempInt1;
	tempInt1 = 250 * m_dHoriRatio;
	m_listCutOff.SetColumnWidth(2,tempInt1);
	tempInt2 -= tempInt1;
	tempInt2 -= m_listCutOff.GetFrameLineWidth()*2;
	m_listCutOff.SetColumnWidth(3,tempInt2);

	SetListCutOff();

	return;
}


void CPs96CutOffDlg::SetListCutOff()
{
	if (m_pAnalysisParam == NULL)
		return;

	m_listCutOff.Clear();
	CString strTemp;
	int iCount = m_pAnalysisParam->GetAmplifyParaCount();
	for (int i=0; i<iCount; i++)
	{
		tagAnaAmplifyPara* pParam = m_pAnalysisParam->GetAmplifyParaBy(i);
		if (pParam->strDye == _T("MOT"))
			continue;

		m_listCutOff.InsertRowSetHeight(i, int(50 * m_dVertRatio));
		strTemp.Format(_T("%d"), i + 1);
		m_listCutOff.SetItemText(i, 0, strTemp);

		m_listCutOff.SetItemText(i, 1, pParam->strDye);
		m_listCutOff.SetItemText(i, 2, pParam->strTarget);

		strTemp.Format(_T("%.2f"), pParam->dUpperThreshold);
		m_listCutOff.SetItemText(i, 3, strTemp);
	}
}

void CPs96CutOffDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	CRect rcUseful;
#ifdef POPDLG_USESHADEBK
	rcUseful.left = rect.left + (rect.Width() - m_iWndWidth) / 2;
	rcUseful.top = rect.top + (rect.Height() - m_iWndHeight) / 2;
	rcUseful.right = rcUseful.left + m_iWndWidth;
	rcUseful.bottom = rcUseful.top + m_iWndHeight;
#endif

	int iStartY = int(80 * m_dVertRatio);
	m_listCutOff.SetWindowPos(this, int(rcUseful.left + 16*m_dHoriRatio), rcUseful.top + iStartY, int(668 * m_dHoriRatio), int(505 * m_dVertRatio),SWP_NOZORDER);

	int iBtnHeight = int(60 * m_dVertRatio);
	int iBtnWidth = int(160 * m_dVertRatio);
	iStartY += int(540*m_dVertRatio);
	m_btnSave.SetWindowPos(this, int(rcUseful.left + 300*m_dHoriRatio), rcUseful.top + iStartY,iBtnWidth, iBtnHeight,SWP_NOZORDER);
	m_btnCancel.SetWindowPos(this, int(rcUseful.left + 500 * m_dHoriRatio), rcUseful.top + iStartY, iBtnWidth, iBtnHeight,SWP_NOZORDER);

	return;
}

void CPs96CutOffDlg::ShowCtrl()
{
	m_btnSave.ShowWindow(SW_SHOW);
	m_btnCancel.ShowWindow(SW_SHOW);
	m_listCutOff.ShowWindow(SW_SHOW);
}

void CPs96CutOffDlg::SetCtrlText()
{
	m_btnSave.SetWindowText(theApp.m_pLangInfo->GetText(148));
	m_btnCancel.SetWindowText(theApp.m_pLangInfo->GetText(150));
}


void CPs96CutOffDlg::OnBnClickedSave()
{
	m_bChanged = FALSE;
	int iRowCount = m_listCutOff.GetRowNum();
	CString strChannel, strTarget, strValue;
	for (int i=0; i<iRowCount; i++)
	{
		CString strChannel = m_listCutOff.GetItemText(i, 1);
		CString strTarget = m_listCutOff.GetItemText(i, 2);
		tagAnaAmplifyPara* pParam = m_pAnalysisParam->GetAmplifyParaByTarget(strTarget, strChannel);
		if (pParam == NULL)
			pParam = m_pAnalysisParam->GetAmplifyParaByDye(strChannel);
		if (pParam == NULL)
			continue;

		double dValue = _ttof(m_listCutOff.GetItemText(i,3));
		if (int(dValue*100) != int(pParam->dUpperThreshold*100))
		{
			m_bChanged = TRUE;
			pParam->dUpperThreshold = dValue;
		}
	}

	OnOK();
}

void CPs96CutOffDlg::OnBnClickedCancel()
{
	OnCancel();
}

BOOL CPs96CutOffDlg::SetRoundRect(int x /*= 5*/,int y /*= 5*/)
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
