// SkipDlg.cpp : implementation file
//

#include "stdafx.h"

#include "CommInterruptTipDlg.h"
#include "GeneralPcrDlg.h"
#include "GeneralPcr.h"

// CCommInterruptTipDlg dialog
IMPLEMENT_DYNAMIC(CCommInterruptTipDlg, CDialogEx)

CCommInterruptTipDlg::CCommInterruptTipDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCommInterruptTipDlg::IDD, pParent)
{
	m_frameWidth = 1;
	m_roundValue = 20;
	m_strNoteInfo = _T("");
	m_noteFontSize = 20;
	m_dwNoteStyle = DT_LEFT | DT_VCENTER | DT_WORDBREAK | DT_NOFULLWIDTHCHARBREAK;
	m_width = 510;
	m_height = 420;
	m_iCapHeight = 60;
	m_retCode = 0;
	m_pInstancePCR = NULL;
	m_pExpData = NULL;
	m_iTimeCount = 0;
	m_iRemainSec = 0;
}

CCommInterruptTipDlg::~CCommInterruptTipDlg()
{
}

void CCommInterruptTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCommInterruptTipDlg, CDialogEx)
	ON_WM_ERASEBKGND() 
	ON_WM_CREATE()
	ON_BN_CLICKED(ID_GENERALTIPDLG_BT_CANCEL,&CCommInterruptTipDlg::OnBnClickedCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CCommInterruptTipDlg::OnEraseBkgnd(CDC* pDC)
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

int CCommInterruptTipDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_width *= m_dHoriRatio;
	m_height *= m_dVertRatio;
	m_iCapHeight *= m_dVertRatio;
	m_noteFontSize *= m_dTxRatio;

	return CDialogEx::OnCreate(lpCreateStruct);
}


BOOL CCommInterruptTipDlg::OnInitDialog()
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

	ShowCtrl();
	m_nRunTimer1 = SetTimer(1, 1000, NULL);
	m_nRunTimer2 = SetTimer(2, 2000, NULL);

	return TRUE;
}

void CCommInterruptTipDlg::CreateCtrl()
{
	CreateBt();
	CreateSt();
}

void CCommInterruptTipDlg::CreateBt()
{
	m_btStop.Create(_T("停止"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_GENERALTIPDLG_BT_CANCEL);
}


void CCommInterruptTipDlg::CreateSt()
{
	m_stNoteInfo.Create(_T(""),WS_CHILD,CRect(0,0,0,0),this,ID_GENERALTIPDLG_ST_NOTEINFO);
	m_stIcon.Create(_T(""),WS_CHILD,CRect(0,0,0,0),this,ID_GENERALTIPDLG_ST_ICON);
	m_stTimeInfo.Create(_T(""),WS_CHILD,CRect(0,0,0,0),this,ID_GENERALTIPDLG_BT_OK);
}


void CCommInterruptTipDlg::InitDlg()
{
	int iFontH = 24 * m_dTxRatio;

	SetTextClr1(RGB(0XFF,0XFF,0XFF));
	SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
}

void CCommInterruptTipDlg::InitCtrl()
{
	InitBt();
	InitSt();
}

void CCommInterruptTipDlg::InitBt()
{
	int iFontH = 18 * m_dTxRatio;

	m_btStop.SetMyFont(m_stTxFont,iFontH);
	m_btStop.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);
	m_btStop.SetBkColor(Color(0X38,0XE0,0XEF),Color(0X3A,0XEF,0XFF),COLOR_SOFT_THEME,Color(0XF1,0XF1,0XF1));
	m_btStop.SetBkColorEnd(COLOR_SOFT_THEME,Color(0X38,0XE0,0XEF),Color(0X02,0X8D,0X99),Color(0XD8,0XD8,0XD8));
	m_btStop.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XC6,0XC4,0XC4));
}


void CCommInterruptTipDlg::InitSt()
{
	m_stNoteInfo.SetBDynamic(TRUE);
	m_stNoteInfo.SetBkColor(RGB(0XFF,0XFF,0XFF));
	m_stNoteInfo.SetMyFont(m_stTxFont,m_noteFontSize,FW_NORMAL,TYPE_PIXEL);
	m_stNoteInfo.SetTextColor(RGB(0X39,0X45,0X65));
	m_stNoteInfo.SetWordStyle(m_dwNoteStyle);
	m_stNoteInfo.SetWindowText(m_strNoteInfo);

	m_stTimeInfo.SetBDynamic(TRUE);
	m_stTimeInfo.SetBkColor(RGB(0XFF,0XFF,0XFF));
	m_stTimeInfo.SetMyFont(m_stTxFont,m_noteFontSize,FW_NORMAL,TYPE_PIXEL);
	m_stTimeInfo.SetTextColor(RGB(0X39,0X45,0X65));
	m_stTimeInfo.SetWordStyle(m_dwNoteStyle);
	m_stTimeInfo.SetWindowText(m_strNoteInfo);

	CString strTemp(_T(""));
	m_stIcon.SetBkColor(RGB(0XFF,0XFF,0XFF));
	strTemp = _T(".\\BkPic\\warningtip1.png");
	m_stIcon.SetPngBkPic(strTemp);
}


void CCommInterruptTipDlg::SetCtrlPos()
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

	m_stIcon.SetWindowPos(this,rcUseful.left + 56 * m_dHoriRatio,rcUseful.top + 150 * m_dVertRatio, int(60 * m_dHoriRatio), int(61 * m_dVertRatio),SWP_NOZORDER);
	
	m_stNoteInfo.SetWindowPos(this,rcUseful.left + 156 * m_dHoriRatio,rcUseful.top + 138 * m_dVertRatio, int(290 * m_dHoriRatio), int(50 * m_dVertRatio),SWP_NOZORDER);
	m_rcManage.AddRectToDelete(rcUseful.left + 156 * m_dHoriRatio,rcUseful.top + 138 * m_dVertRatio, int(290 * m_dHoriRatio), int(50 * m_dVertRatio));

	m_stTimeInfo.SetWindowPos(this,rcUseful.left + 156 * m_dHoriRatio, rcUseful.top + 198 * m_dVertRatio, int(290 * m_dHoriRatio), int(50 * m_dVertRatio),SWP_NOZORDER);
	m_rcManage.AddRectToDelete(rcUseful.left + 156 * m_dHoriRatio,rcUseful.top + 198 * m_dVertRatio, int(290 * m_dHoriRatio), int(50 * m_dVertRatio));

	m_btStop.SetWindowPos(this,rcUseful.left + 362 * m_dHoriRatio,rcUseful.top + 356 * m_dVertRatio,width,height,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(rcUseful.left + 362 * m_dHoriRatio,rcUseful.top + 356 * m_dVertRatio,width,height);	
	m_rcManage.CalcRectResult();
}

void CCommInterruptTipDlg::ShowCtrl()
{
	m_stNoteInfo.ShowWindow(SW_SHOW);
	m_stIcon.ShowWindow(SW_SHOW);
	m_btStop.ShowWindow(SW_SHOW);
	m_stTimeInfo.ShowWindow(SW_SHOW);
}

void CCommInterruptTipDlg::SetCtrlText()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	m_btStop.SetWindowText(pLangInfo->GetText(5));
	m_strNoteInfo = pLangInfo->GetText(525);
	m_stNoteInfo.SetWindowText(m_strNoteInfo);
	m_strReconnect = pLangInfo->GetText(526);
}

void CCommInterruptTipDlg::OnBnClickedCancel()
{
	m_retCode = 2;
	OnCancel();
	return;
}

void CCommInterruptTipDlg::SetNoteInfo(CString strNoteInfo)
{
	m_strNoteInfo = strNoteInfo;
}

CString CCommInterruptTipDlg::GetNoteInfo()
{
	return m_strNoteInfo;
}

void CCommInterruptTipDlg::SetNoteInfoStyle(DWORD style)
{
	m_dwNoteStyle = style;
}

DWORD CCommInterruptTipDlg::GetNoteInfoStyle()
{
	return m_dwNoteStyle;
}

BOOL CCommInterruptTipDlg::SetRoundRect(int x /*= 5*/,int y /*= 5*/)
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


void CCommInterruptTipDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_nRunTimer1)
	{
		m_iTimeCount ++;
		CString strTime = SecondToHourMinuteSecond(m_iTimeCount);
		m_stTimeInfo.SetWindowText(m_strReconnect + strTime);
		if (m_iTimeCount  >= m_iRemainSec)
		{
			m_retCode = 1;
			KillTimer(m_nRunTimer1);
			KillTimer(m_nRunTimer2);
			OnCancel();
		}
	}
	else if (nIDEvent == m_nRunTimer2)
	{
		BOOL bReturn = m_pInstancePCR->Reinitialize();
		if (bReturn)
		{
			Sleep(5000);
			bReturn = PrepareRunning();
			if (bReturn)
			{
				bReturn = RunLidHeader();
				if (bReturn)
				{
					m_pInstancePCR->SetInstanceStatus(INSTANCE_USING);
					KillTimer(m_nRunTimer1);
					KillTimer(m_nRunTimer2);
					OnCancel();
				}
			}
		}
	}

	__super::OnTimer(nIDEvent);
}

// 实验前准备工作，如设备Home等
BOOL CCommInterruptTipDlg::PrepareRunning()
{
	BOOL bReturn = m_pInstancePCR->Home();
	if(!bReturn)
	{
		return bReturn;
	}

	// 打开状态灯时下位机同时打开热盖，所有不使用热盖时只能不改变状态灯
	if (m_pExpData->GetExperimentInfo()->GetTempCtrlMode() != TEMP_CTRL_NONE)
	{
		//系统忙时绿灯均速闪烁
		bReturn = m_pInstancePCR->SetStateLED(STALED_GREEN_BUSY);
		if(!bReturn)
		{
			return bReturn;
		}
	}

	// 设置控温模式
	bReturn = m_pInstancePCR->SetTempCtrlMode(m_pExpData->GetExperimentInfo()->GetTempCtrlMode());
	if(!bReturn)
	{
		return bReturn;
	}

	// 设置PMT控制电压
	WORD wPmtGain = m_pInstancePCR->GetPMTGain(); // 需要修改，当前是PS96模式
	//临时使用，需要通过修改EEPROM实现
	if(NULL != m_pInstancePCR && m_pInstancePCR->GetBe96SiteQPCR())
	{
		wPmtGain = m_pInstancePCR->GetMppcGainMiddle();
	}

	bReturn = m_pInstancePCR->SetPMTGain(wPmtGain);
	if(!bReturn)
	{
		return bReturn;
	}

	return bReturn;
}


BOOL CCommInterruptTipDlg::RunLidHeader()
{
	if(!m_pExpData->GetExperimentInfo()->GetLidHeader() || m_pExpData->GetExperimentInfo()->GetTempCtrlMode() == TEMP_CTRL_NONE)
	{
		return TRUE;
	}

	//设置热盖温度
	float fTopTemp = m_pExpData->GetExperimentInfo()->GetLidTemp();
	BOOL bReturn = m_pInstancePCR->SetLidHeaterTemp(fTopTemp);
	if (!bReturn)
	{
		return FALSE;
	}

	return bReturn;
}