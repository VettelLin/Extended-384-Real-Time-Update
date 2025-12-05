// SkipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralPcr.h"
#include "GeneralPcrDlg.h"
#include "MonitorBaseInfoDlg.h"

// CMonitorBaseInfoDlg dialog
IMPLEMENT_DYNAMIC(CMonitorBaseInfoDlg, CDialogEx)

CMonitorBaseInfoDlg::CMonitorBaseInfoDlg(BOOL bPrjManage /*= FALSE*/,CWnd* pParent /*=NULL*/)
: CDialogEx(CMonitorBaseInfoDlg::IDD, pParent)
{
	m_bCtrlCreated = FALSE;
}

CMonitorBaseInfoDlg::~CMonitorBaseInfoDlg()
{
}

void CMonitorBaseInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMonitorBaseInfoDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CMonitorBaseInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	
	CRect rect; 
	GetClientRect(&rect);
	
	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	memDc.FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(255,255,255));

	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);
	memDc.SetBkMode(TRANSPARENT);

	memDc.FillSolidRect(0,0,rect.Width(),rect.Height(),CLR_SOFT_LIGHT);
	DrawRoundRectangleNew(&memDc,0,0,rect.Width() - 1,rect.Height() - 1,8,8,8,8,TRUE,Color(0XD8,0XD8,0XD8),1,TRUE,Color(0XFF,0XFF,0XFF),1);

	//»æÖÆÍ¼±ê
	Graphics graphics(memDc.m_hDC);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
	
	ImageAttributes ImgAtt;
	ImgAtt.SetWrapMode(WrapModeTileFlipXY);

	Image image(_T(".\\BkPic\\timeicon.png"));

	Rect destRect;
	destRect.X = 36 * m_dHoriRatio;
	destRect.Y = 59 * m_dVertRatio;
	destRect.Width = 16 * m_dHoriRatio;
	destRect.Height = destRect.Width;
	graphics.DrawImage(&image,destRect,0,0,image.GetWidth(),image.GetHeight(),UnitPixel,&ImgAtt);

	destRect.Y = 90 * m_dVertRatio;
	graphics.DrawImage(&image,destRect,0,0,image.GetWidth(),image.GetHeight(),UnitPixel,&ImgAtt);

	destRect.Y = 254 * m_dVertRatio;
	graphics.DrawImage(&image,destRect,0,0,image.GetWidth(),image.GetHeight(),UnitPixel,&ImgAtt);

	destRect.Y = 284 * m_dVertRatio;
	graphics.DrawImage(&image,destRect,0,0,image.GetWidth(),image.GetHeight(),UnitPixel,&ImgAtt);

	Image image2(_T(".\\BkPic\\temperatureicon.png"));
	destRect.X = 40 * m_dHoriRatio;
	destRect.Y = 156 * m_dVertRatio;
	destRect.Width = 8 * m_dHoriRatio;
	destRect.Height = 16 * m_dVertRatio;
	graphics.DrawImage(&image2,destRect,0,0,image2.GetWidth(),image2.GetHeight(),UnitPixel,&ImgAtt);

	destRect.Y = 187 * m_dVertRatio;
	graphics.DrawImage(&image2,destRect,0,0,image2.GetWidth(),image2.GetHeight(),UnitPixel,&ImgAtt);

	graphics.ReleaseHDC(memDc.m_hDC);
	
	int iTempInt1 = 66 * m_dHoriRatio,iTempInt2 = 163 * m_dHoriRatio,iTempInt3 = 176 * m_dHoriRatio,iTempInt4 = 176 * m_dHoriRatio,iTempInt5 = 163 * m_dHoriRatio;
	if(theApp.GetBeEnglish())
	{
		iTempInt2 = 192 * m_dHoriRatio;
		iTempInt3 = 280 * m_dHoriRatio;
		iTempInt4 = 182 * m_dHoriRatio;
		iTempInt5 = 240 * m_dHoriRatio;
	}

	memDc.DrawText(pLangInfo->GetText(83),CRect(iTempInt1,55 * m_dVertRatio,iTempInt2,79 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	memDc.DrawText(pLangInfo->GetText(84),CRect(iTempInt1,86 * m_dVertRatio,iTempInt2,110 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	memDc.DrawText(pLangInfo->GetText(85),CRect(iTempInt1,152 * m_dVertRatio,iTempInt5,176 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	memDc.DrawText(pLangInfo->GetText(86),CRect(iTempInt1,183 * m_dVertRatio,iTempInt3,207 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	memDc.DrawText(pLangInfo->GetText(87),CRect(iTempInt1,249 * m_dVertRatio,iTempInt4,273 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	memDc.DrawText(pLangInfo->GetText(88),CRect(iTempInt1,280 * m_dVertRatio,iTempInt4,304 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);

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

	return TRUE;
}

void CMonitorBaseInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}
	return;
}

BOOL CMonitorBaseInfoDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

	InitDlg();
	CreateCtrl();
	m_bCtrlCreated = TRUE;
	InitCtrl();
	SetCtrlPos();

	SetCtrlText();

	ShowCtrl();

	return TRUE;
}

void CMonitorBaseInfoDlg::CreateCtrl()
{
	CreateBt();
	CreateSt();
	CreateCb();
	CreateEd();

	//m_progressBar.Create(_T(""),WS_CHILD,CRect(0,0,0,0),this,ID_GENERALPCRDLG_PROGRESS_EXPER);
}

void CMonitorBaseInfoDlg::CreateBt()
{
	
}

void CMonitorBaseInfoDlg::CreateSt()
{
	CRect rcCtrl(0,0,0,0);

	m_stStartTm.Create(_T(""),WS_CHILD,rcCtrl,this,ID_MONITORBASEINFODLG_ST_STARTTM);
	m_stEndTm.Create(_T(""),WS_CHILD,rcCtrl,this,ID_MONITORBASEINFODLG_ST_ENDTM);
	m_stTemLid.Create(_T("--¡æ"),WS_CHILD,rcCtrl,this,ID_MONITORBASEINFODLG_ST_TEMLID);
	m_stTemTube.Create(_T("--¡æ"),WS_CHILD,rcCtrl,this,ID_MONITORBASEINFODLG_ST_TEMTUBE);
	m_stTotalSec.Create(_T(""),WS_CHILD,rcCtrl,this,ID_MONITORBASEINFODLG_ST_TOTALSEC);
	m_stRemainSec.Create(_T(""),WS_CHILD,rcCtrl,this,ID_MONITORBASEINFODLG_ST_REMAINSEC);
}

void CMonitorBaseInfoDlg::CreateCb()
{
	
}

void CMonitorBaseInfoDlg::CreateEd()
{
	
}

void CMonitorBaseInfoDlg::InitDlg()
{
	int iFontH = 16 * m_dTxRatio;
	if(theApp.m_pLangInfo->GetBeEnglish())
	{
		iFontH = 17 * m_dTxRatio;
	}

	SetTextClr1(RGB(0X66,0X66,0X66));
	SetMyFont(m_stTxFont,iFontH);
}

void CMonitorBaseInfoDlg::InitCtrl()
{
	InitBt();
	InitSt();
	InitEd();
	InitCb();

	//m_progressBar.SetTextColor(RGB(206,206,206));
	//m_progressBar.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
}

void CMonitorBaseInfoDlg::InitBt()
{
	
}

void CMonitorBaseInfoDlg::InitSt()
{
	int iFontH = 16 * m_dTxRatio;
	if(theApp.m_pLangInfo->GetBeEnglish())
	{
		iFontH = 17 * m_dTxRatio;
	}

	DWORD clrTx = RGB(0X66,0X66,0X66);

	m_stStartTm.SetTextColor(clrTx);
	m_stStartTm.SetMyFont(m_stTxFont,iFontH);
	m_stStartTm.SetBkColor(RGB(0XFF,0XFF,0XFF));
	m_stStartTm.SetBDynamic(TRUE);

	m_stEndTm.SetTextColor(clrTx);
	m_stEndTm.SetMyFont(m_stTxFont,iFontH);
	m_stEndTm.SetBkColor(RGB(0XFF,0XFF,0XFF));
	m_stEndTm.SetBDynamic(TRUE);

	m_stTemLid.SetTextColor(clrTx);
	m_stTemLid.SetMyFont(m_stTxFont,iFontH);
	m_stTemLid.SetBkColor(RGB(0XFF,0XFF,0XFF));
	m_stTemLid.SetBDynamic(TRUE);

	m_stTemTube.SetTextColor(clrTx);
	m_stTemTube.SetMyFont(m_stTxFont,iFontH);
	m_stTemTube.SetBkColor(RGB(0XFF,0XFF,0XFF));
	m_stTemTube.SetBDynamic(TRUE);

	m_stTotalSec.SetTextColor(clrTx);
	m_stTotalSec.SetMyFont(m_stTxFont,iFontH);
	m_stTotalSec.SetBkColor(RGB(0XFF,0XFF,0XFF));
	m_stTotalSec.SetBDynamic(TRUE);

	m_stRemainSec.SetTextColor(clrTx);
	m_stRemainSec.SetMyFont(m_stTxFont,iFontH);
	m_stRemainSec.SetBkColor(RGB(0XFF,0XFF,0XFF));
	m_stRemainSec.SetBDynamic(TRUE);
}

void CMonitorBaseInfoDlg::InitCb()
{
	return;
}

void CMonitorBaseInfoDlg::InitEd()
{

}

void CMonitorBaseInfoDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	m_rcManage.m_rcInit = rect;
	m_rcManage.ClearRectToDelete();

	int iTempInt1 = 169 * m_dHoriRatio,iTempInt2 = 24 * m_dVertRatio,iTempInt3 = 182 * m_dHoriRatio,iTempInt4 = 169 * m_dHoriRatio,iTempInt5 = 169 * m_dHoriRatio;
	
	if(theApp.GetBeEnglish())
	{
		iTempInt3 = 186 * m_dHoriRatio;
		iTempInt4 = 284 * m_dHoriRatio;
		iTempInt5 = 244 * m_dHoriRatio;
	}

	m_rcManage.AddRectToDelete(iTempInt1,55 * m_dVertRatio,iTempInt1,iTempInt2);
	m_stStartTm.SetWindowPos(this,iTempInt1,55 * m_dVertRatio,iTempInt1,iTempInt2,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iTempInt1,86 * m_dVertRatio,iTempInt1,iTempInt2);
	m_stEndTm.SetWindowPos(this,iTempInt1,86 * m_dVertRatio,iTempInt1,iTempInt2,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iTempInt5,152 * m_dVertRatio,60 * m_dHoriRatio,iTempInt2);
	m_stTemLid.SetWindowPos(this,iTempInt5,152 * m_dVertRatio,60 * m_dHoriRatio,iTempInt2,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iTempInt4,183 * m_dVertRatio,60 * m_dHoriRatio,iTempInt2);
	m_stTemTube.SetWindowPos(this,iTempInt4,183 * m_dVertRatio,60 * m_dHoriRatio,iTempInt2,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iTempInt3,249 * m_dVertRatio,90 * m_dHoriRatio,iTempInt2);
	m_stTotalSec.SetWindowPos(this,iTempInt3,249 * m_dVertRatio,90 * m_dHoriRatio,iTempInt2,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iTempInt3,280 * m_dVertRatio,90 * m_dHoriRatio,iTempInt2);
	m_stRemainSec.SetWindowPos(this,iTempInt3,280 * m_dVertRatio,90 * m_dHoriRatio,iTempInt2,SWP_NOZORDER);

	m_rcManage.CalcRectResult();

	return;
}

void CMonitorBaseInfoDlg::ResetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	m_rcManage.m_rcInit = rect;
	m_rcManage.ClearRectToDelete();

	int iTempInt1 = 169 * m_dHoriRatio,iTempInt2 = 24 * m_dVertRatio,iTempInt3 = 182 * m_dHoriRatio,iTempInt4 = 169 * m_dHoriRatio,iTempInt5 = 169 * m_dHoriRatio;

	if(theApp.GetBeEnglish())
	{
		iTempInt3 = 186 * m_dHoriRatio;
		iTempInt4 = 284 * m_dHoriRatio;
		iTempInt5 = 244 * m_dHoriRatio;
	}

	m_rcManage.AddRectToDelete(iTempInt1,55 * m_dVertRatio,iTempInt1,iTempInt2);
	m_stStartTm.SetWindowPos(this,iTempInt1,55 * m_dVertRatio,iTempInt1,iTempInt2,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iTempInt1,86 * m_dVertRatio,iTempInt1,iTempInt2);
	m_stEndTm.SetWindowPos(this,iTempInt1,86 * m_dVertRatio,iTempInt1,iTempInt2,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iTempInt5,152 * m_dVertRatio,60 * m_dHoriRatio,iTempInt2);
	m_stTemLid.SetWindowPos(this,iTempInt5,152 * m_dVertRatio,60 * m_dHoriRatio,iTempInt2,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iTempInt4,183 * m_dVertRatio,60 * m_dHoriRatio,iTempInt2);
	m_stTemTube.SetWindowPos(this,iTempInt4,183 * m_dVertRatio,60 * m_dHoriRatio,iTempInt2,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iTempInt3,249 * m_dVertRatio,90 * m_dHoriRatio,iTempInt2);
	m_stTotalSec.SetWindowPos(this,iTempInt3,249 * m_dVertRatio,90 * m_dHoriRatio,iTempInt2,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iTempInt3,280 * m_dVertRatio,90 * m_dHoriRatio,iTempInt2);
	m_stRemainSec.SetWindowPos(this,iTempInt3,280 * m_dVertRatio,90 * m_dHoriRatio,iTempInt2,SWP_NOZORDER);

	m_rcManage.CalcRectResult();

	return;
}

void CMonitorBaseInfoDlg::ShowCtrl()
{
	m_stStartTm.ShowWindow(SW_SHOW);
	m_stEndTm.ShowWindow(SW_SHOW);
	m_stTemLid.ShowWindow(SW_SHOW);
	m_stTemTube.ShowWindow(SW_SHOW);
	m_stTotalSec.ShowWindow(SW_SHOW);
	m_stRemainSec.ShowWindow(SW_SHOW);
}

void CMonitorBaseInfoDlg::SetCtrlText()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	if(NULL == pLangInfo)
	{
		return;
	}

	return;
}

void CMonitorBaseInfoDlg::SetCtrlEnableState(BOOL bEnabled)
{
	
}

void CMonitorBaseInfoDlg::ClearPage()
{
	m_stStartTm.SetWindowText(_T(""));
	m_stEndTm.SetWindowText(_T("")); 
	m_stTemLid.SetWindowText(_T("--¡æ")); 
	m_stTemTube.SetWindowText(_T("--¡æ")); 
	m_stTotalSec.SetWindowText(_T("")); 
	m_stRemainSec.SetWindowText(_T("")); 
}

int CMonitorBaseInfoDlg::SetMonitorBaseInfo(CExperimentInfo* pExpInfo)
{
	if(NULL == pExpInfo)
	{
		return -1;
	}

	m_stStartTm.SetWindowText(pExpInfo->GetStartTime());
	m_stEndTm.SetWindowText(pExpInfo->GetEndTime());
	m_stTotalSec.SetWindowText(SecondToHourMinuteSecond(pExpInfo->GetTotalSec()));
	m_stRemainSec.SetWindowText(SecondToHourMinuteSecond(pExpInfo->GetRemainSec()));
}

int CMonitorBaseInfoDlg::RefreshLanguageDisplay()
{
	BOOL bEnglish = theApp.m_pLangInfo->GetBeEnglish();

	int iFontH = 16 * m_dTxRatio;
	if(bEnglish)
	{
		iFontH = 17 * m_dTxRatio;
	}

	SetMyFont(m_stTxFont,iFontH);

	iFontH = 16 * m_dTxRatio;
	if(bEnglish)
	{
		iFontH = 17 * m_dTxRatio;
	}
	m_stStartTm.SetMyFont(m_stTxFont,iFontH);
	m_stEndTm.SetMyFont(m_stTxFont,iFontH);
	m_stTemLid.SetMyFont(m_stTxFont,iFontH);
	m_stTemTube.SetMyFont(m_stTxFont,iFontH);
	m_stTotalSec.SetMyFont(m_stTxFont,iFontH);
	m_stRemainSec.SetMyFont(m_stTxFont,iFontH);

	ResetCtrlPos();

	return 0;
}


