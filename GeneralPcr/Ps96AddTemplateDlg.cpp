// SkipDlg.cpp : implementation file
//

#include "stdafx.h"

#include "Ps96AddTemplateDlg.h"
#include "GeneralPcr.h"

// CPs96AddTemplateDlg dialog
IMPLEMENT_DYNAMIC(CPs96AddTemplateDlg, CDialogEx)

CPs96AddTemplateDlg::CPs96AddTemplateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPs96AddTemplateDlg::IDD, pParent)
{
	m_iAngleRadius = 20;
	m_iFrameWidth = 1;
	m_iWndWidth = 900;
	m_iWndHeight = 900;
	m_iCapHeight = 90;

	m_strName.Empty();
	m_strLabName.Empty();
	m_strLabAdress.Empty();
	m_strContactUrl.Empty();
	m_strContactEmail.Empty();
	m_strContactPhone.Empty();
	m_strTestName.Empty();
	m_strDisclaimer.Empty();

	m_bAddTemplage = TRUE;
	m_bChanged = FALSE;

	m_pReportData = NULL;
}

CPs96AddTemplateDlg::~CPs96AddTemplateDlg()
{

}

void CPs96AddTemplateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DISCLAIMER, m_edDisclaimer);
}

BEGIN_MESSAGE_MAP(CPs96AddTemplateDlg, CDialogEx)
	ON_WM_ERASEBKGND() 
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(ID_PS96SIMPLESETDLG_BT_SAVE,OnBnClickedSave)
	ON_BN_CLICKED(ID_PS96SIMPLESETDLG_BT_CANCEL,OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CPs96AddTemplateDlg::OnEraseBkgnd(CDC* pDC)
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

	DrawRoundRectangleNew(&memDc,rcUseful.left, rcUseful.top + m_iCapHeight,rcUseful.Width(),rcUseful.Height() - m_iCapHeight,0,0,m_iAngleRadius,m_iAngleRadius,FALSE,Color(197,197,197),m_iFrameWidth,TRUE,Color(255,255,255),1);
	DrawRoundRectangleNew(&memDc,rcUseful.left, rcUseful.top,rcUseful.Width(),m_iCapHeight,m_iAngleRadius,m_iAngleRadius,0,0,FALSE,COLOR_SOFT_THEME,m_iFrameWidth,TRUE,COLOR_SOFT_THEME,1);

	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);
	memDc.SetBkMode(TRANSPARENT);

	DWORD dwTxStyle = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	
	int iStartX = rcUseful.left + int(20 * m_dHoriRatio);
	int iStartY = rcUseful.top + int(110 * m_dVertRatio);
	int iSpace = int(10 * m_dVertRatio);
	int iWidth = int(230 * m_dHoriRatio);
	int iHeight = int(50 * m_dHoriRatio);

	memDc.DrawText(theApp.m_pLangInfo->GetTextWithColon(504),CRect(iStartX, iStartY, iStartX+iWidth, iStartY+iHeight), dwTxStyle);
	iStartY += iHeight + iSpace;
	memDc.DrawText(theApp.m_pLangInfo->GetTextWithColon(505),CRect(iStartX, iStartY, iStartX+iWidth, iStartY+iHeight), dwTxStyle);
	iStartY += iHeight + iSpace;
	memDc.DrawText(theApp.m_pLangInfo->GetTextWithColon(506),CRect(iStartX, iStartY, iStartX+iWidth, iStartY+iHeight), dwTxStyle);

	iStartY += iHeight*2 + iSpace;
	memDc.DrawText(theApp.m_pLangInfo->GetTextWithColon(507),CRect(iStartX, iStartY, iStartX+iWidth, iStartY+iHeight), dwTxStyle);
	iStartY += iHeight + iSpace;
	memDc.DrawText(theApp.m_pLangInfo->GetTextWithColon(508),CRect(iStartX, iStartY, iStartX+iWidth, iStartY+iHeight), dwTxStyle);
	iStartY += iHeight + iSpace;
	memDc.DrawText(theApp.m_pLangInfo->GetTextWithColon(509),CRect(iStartX, iStartY, iStartX+iWidth, iStartY+iHeight), dwTxStyle);
	iStartY += iHeight + iSpace;
	memDc.DrawText(theApp.m_pLangInfo->GetTextWithColon(510),CRect(iStartX, iStartY, iStartX+iWidth, iStartY+iHeight), dwTxStyle);

	iStartY += iHeight + iSpace;
	memDc.DrawText(theApp.m_pLangInfo->GetTextWithColon(511),CRect(iStartX, iStartY, iStartX+iWidth, iStartY+iHeight), dwTxStyle);

	memDc.SetTextColor(RGB(255,255,255));
	if (m_bAddTemplage)
		memDc.DrawText(theApp.m_pLangInfo->GetText(501),CRect(rcUseful.left + 10,rcUseful.top + 5,rcUseful.left + rcUseful.Width() - 10,rcUseful.top + m_iCapHeight - 5),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	else
		memDc.DrawText(theApp.m_pLangInfo->GetText(502),CRect(rcUseful.left + 10,rcUseful.top + 5,rcUseful.left + rcUseful.Width() - 10,rcUseful.top + m_iCapHeight - 5),DT_CENTER | DT_VCENTER | DT_SINGLELINE);

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

int CPs96AddTemplateDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_iWndWidth *= m_dHoriRatio;
	m_iWndHeight *= m_dVertRatio;
	m_iCapHeight *= m_dVertRatio;
	return CDialogEx::OnCreate(lpCreateStruct);
}

void CPs96AddTemplateDlg::OnLButtonDown(UINT nFlags, CPoint point)
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

BOOL CPs96AddTemplateDlg::OnInitDialog()
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

void CPs96AddTemplateDlg::CreateCtrl()
{
	CreateBt();
	CreateEd();
}

void CPs96AddTemplateDlg::CreateBt()
{
	m_btSave.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PS96SIMPLESETDLG_BT_SAVE);
	m_btCancel.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PS96SIMPLESETDLG_BT_CANCEL);
}

void CPs96AddTemplateDlg::CreateEd()
{
	m_edTemplateName.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,CRect(0,0,0,0),this,ID_PS96ADDTEMPLATE_ED_NAME);
	m_edLabName.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,CRect(0,0,0,0),this,ID_PS96ADDTEMPLATE_ED_LAB_NAME);
	m_edLabAddress.Create(WS_CHILD | WS_BORDER | WS_TABSTOP | ES_MULTILINE | ES_AUTOVSCROLL,CRect(0,0,0,0),this,ID_PS96ADDTEMPLATE_ED_ADDRESS);
	m_edContactUrl.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,CRect(0,0,0,0),this,ID_PS96ADDTEMPLATE_ED_UML);
	m_edContactEmail.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,CRect(0,0,0,0),this,ID_PS96ADDTEMPLATE_ED_EMAIL);
	m_edContactPhone.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,CRect(0,0,0,0),this,ID_PS96ADDTEMPLATE_ED_PHONE);
	m_edTestName.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,CRect(0,0,0,0),this,ID_PS96ADDTEMPLATE_ED_TEST);
	//m_edDisclaimer.Create(WS_CHILD | WS_BORDER | WS_TABSTOP | ES_MULTILINE | ES_AUTOVSCROLL,CRect(0,0,0,0),this,ID_PS96ADDTEMPLATE_ED_DISCLAIMER);
	//m_edDisclaimer.Create(WS_CHILD | WS_BORDER | WS_TABSTOP | ES_MULTILINE | EN_VSCROLL,CRect(0,0,0,0),this,ID_PS96ADDTEMPLATE_ED_DISCLAIMER);
}

void CPs96AddTemplateDlg::InitDlg()
{
	SetTextClr1(RGB(0X39,0X45,0X65));
	SetMyFont(m_stTxFont,30 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
}

void CPs96AddTemplateDlg::InitCtrl()
{
	InitBt();
	InitEd();
}

void CPs96AddTemplateDlg::InitBt()
{
	int iFontH = 26 * m_dTxRatio;

	m_btSave.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btSave.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);
	m_btSave.SetBkColor(Color(0X38,0XE0,0XEF),Color(0X3A,0XEF,0XFF),COLOR_SOFT_THEME,Color(0XF1,0XF1,0XF1));
	m_btSave.SetBkColorEnd(COLOR_SOFT_THEME,Color(0X38,0XE0,0XEF),Color(0X02,0X8D,0X99),Color(0XD8,0XD8,0XD8));
	m_btSave.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XC6,0XC4,0XC4));

	m_btCancel.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btCancel.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);
}

void CPs96AddTemplateDlg::InitEd()
{
	InitEditCtrlParam(&m_edTemplateName);
	InitEditCtrlParam(&m_edLabName);

	m_edLabAddress.SetFont(&m_edLabName.m_fontSet);
	//InitEditCtrlParam(&m_edLabAddress);
	InitEditCtrlParam(&m_edContactUrl);
	InitEditCtrlParam(&m_edContactEmail);
	InitEditCtrlParam(&m_edContactPhone);
	InitEditCtrlParam(&m_edTestName);
	//InitEditCtrlParam(&m_edDisclaimer);
	m_edDisclaimer.SetFont(&m_edLabName.m_fontSet);

	m_edTemplateName.SetText(m_strName,FALSE,TRUE);
	m_edLabName.SetText(m_strLabName,FALSE,TRUE);
	//m_edLabAddress.SetText(m_strLabAdress,FALSE,TRUE);
	m_edLabAddress.SetWindowText(m_strLabAdress);
	m_edContactUrl.SetText(m_strContactUrl,FALSE,TRUE);
	m_edContactEmail.SetText(m_strContactEmail,FALSE,TRUE);
	m_edContactPhone.SetText(m_strContactPhone,FALSE,TRUE);
	m_edTestName.SetText(m_strTestName,FALSE,TRUE);
	//m_edDisclaimer.SetText(m_strDisclaimer,FALSE,TRUE);
	m_edDisclaimer.SetWindowText(m_strDisclaimer);
}

void CPs96AddTemplateDlg::SetCtrlPos()
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

	int iStartX = rcUseful.left + int(320 * m_dHoriRatio);
	int iStartY = rcUseful.top + int(110 * m_dVertRatio);
	int iSpace = int(10 * m_dVertRatio);
	int iWidth = int(530 * m_dHoriRatio);
	int iHeight = int(50 * m_dHoriRatio);

	m_edTemplateName.SetWindowPos(this, iStartX, iStartY, iWidth, iHeight,SWP_NOZORDER);
	iStartY += iHeight + iSpace;
	m_edLabName.SetWindowPos(this, iStartX, iStartY, iWidth, iHeight,SWP_NOZORDER);
	iStartY += iHeight + iSpace;
	m_edLabAddress.SetWindowPos(this, iStartX, iStartY, iWidth, iHeight*2,SWP_NOZORDER);

	iStartY += iHeight*2 + iSpace;
	m_edContactUrl.SetWindowPos(this, iStartX, iStartY, iWidth, iHeight,SWP_NOZORDER);
	iStartY += iHeight + iSpace;
	m_edContactEmail.SetWindowPos(this, iStartX, iStartY, iWidth, iHeight,SWP_NOZORDER);
	iStartY += iHeight + iSpace;
	m_edContactPhone.SetWindowPos(this, iStartX, iStartY, iWidth, iHeight,SWP_NOZORDER);
	iStartY += iHeight + iSpace;
	m_edTestName.SetWindowPos(this, iStartX, iStartY, iWidth, iHeight,SWP_NOZORDER);
	iStartY += iHeight + iSpace;
	m_edDisclaimer.SetWindowPos(this, iStartX, iStartY, iWidth, iHeight*5,SWP_NOZORDER);
	
	iStartY += iHeight*5 + iSpace*2;

	int iBtnWidth = int(140 * m_dHoriRatio);
	int iBtnHeight = int(70 * m_dVertRatio);
	m_btSave.SetWindowPos(this, rcUseful.left + int(540 * m_dHoriRatio), iStartY, iBtnWidth, iBtnHeight,SWP_NOZORDER);
	m_btCancel.SetWindowPos(this, rcUseful.left + int(720 * m_dHoriRatio), iStartY, iBtnWidth, iBtnHeight,SWP_NOZORDER);

	return;
}

void CPs96AddTemplateDlg::ShowCtrl()
{
	m_btSave.ShowWindow(SW_SHOW);
	m_btCancel.ShowWindow(SW_SHOW);

	m_edTemplateName.ShowWindow(SW_SHOW);
	m_edLabName.ShowWindow(SW_SHOW);
	m_edLabAddress.ShowWindow(SW_SHOW);
	m_edContactUrl.ShowWindow(SW_SHOW);
	m_edContactEmail.ShowWindow(SW_SHOW);
	m_edContactPhone.ShowWindow(SW_SHOW);
	m_edTestName.ShowWindow(SW_SHOW);
	m_edDisclaimer.ShowWindow(SW_SHOW);

	return;
}

void CPs96AddTemplateDlg::SetCtrlText()
{
	m_btSave.SetWindowText(theApp.m_pLangInfo->GetText(148));
	m_btCancel.SetWindowText(theApp.m_pLangInfo->GetText(150));

	return;
}

void CPs96AddTemplateDlg::OnBnClickedSave()
{
	CString strTemp;
	strTemp = m_edTemplateName.GetText();
	if (strTemp.IsEmpty())
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(512));
		return;
	}
	if (m_bAddTemplage)
	{
		if (m_pReportData->FindSameName(strTemp))
		{
			PopNormalTipInfo(theApp.m_pLangInfo->GetText(513));
			return;
		}
	}
	else
	{
		if (strTemp != m_strName)
		{
			if (m_pReportData->FindSameName(strTemp))
			{
				PopNormalTipInfo(theApp.m_pLangInfo->GetText(513));
				return;
			}
		}
	}

	if (strTemp != m_strName)
	{
		m_strName = strTemp;
		m_bChanged = TRUE;
	}

	strTemp = m_edLabName.GetText();
	if (strTemp != m_strLabName)
	{
		m_strLabName = strTemp;
		m_bChanged = TRUE;
	}

	//m_strLabAdress = m_edLabAddress.GetText();
	m_edLabAddress.GetWindowText(strTemp);
	if (strTemp != m_strLabAdress)
	{
		m_strLabAdress = strTemp;
		m_bChanged = TRUE;
	}

	strTemp = m_edContactUrl.GetText();
	if (strTemp != m_strContactUrl)
	{
		m_strContactUrl = strTemp;
		m_bChanged = TRUE;
	}


	strTemp = m_edContactEmail.GetText();
	if (strTemp != m_strContactEmail)
	{
		m_strContactEmail = strTemp;
		m_bChanged = TRUE;
	}


	strTemp = m_edContactPhone.GetText();
	if (strTemp != m_strContactPhone)
	{
		m_strContactPhone = strTemp;
		m_bChanged = TRUE;
	}

	strTemp = m_edTestName.GetText();
	if (strTemp != m_strTestName)
	{
		m_strTestName = strTemp;
		m_bChanged = TRUE;
	}

	m_edDisclaimer.GetWindowText(strTemp);
	if (strTemp != m_strDisclaimer)
	{
		m_strDisclaimer = strTemp;
		m_bChanged = TRUE;
	}

	OnOK();
	return;
}

void CPs96AddTemplateDlg::OnBnClickedCancel()
{
	OnCancel();
	return;
}

BOOL CPs96AddTemplateDlg::SetRoundRect(int x /*= 5*/,int y /*= 5*/)
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

void CPs96AddTemplateDlg::InitEditCtrlParam(CMyNewEdit* pEdit)
{
	if(NULL == pEdit)
	{
		return;
	}

	pEdit->SetFrameColor(Color(0,0,0),Color(0,0,0),Color(0,0,0),Color(0,0,0));
	//pEdit->SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	pEdit->SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	pEdit->SetSelectedBkColor(RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB));
	pEdit->SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XC6,0XC6,0XC6));
	pEdit->SetMyFont(m_stTxFont,26 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
	pEdit->SetTextClr1(RGB(0X56,0X56,0X56));

	return;
}



