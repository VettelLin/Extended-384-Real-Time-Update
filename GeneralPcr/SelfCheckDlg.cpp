#include "StdAfx.h"
#include "SelfCheckDlg.h"
#include "GeneralPcr.h"

IMPLEMENT_DYNAMIC(CSelfCheckDlg,CDialogEx)

CSelfCheckDlg::CSelfCheckDlg(CWnd* pParent /*= NULL*/)
	: CDialogEx(CSelfCheckDlg::IDD,pParent)
{
	m_strTitle = _T("仪器自检");
	m_iWidth = 800;
	m_iHeight = 600;
	m_frameWidth = 1;
	m_roundValue = 10;
	m_iHeaderH = 40;
}

CSelfCheckDlg::~CSelfCheckDlg(void)
{

}

void CSelfCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSelfCheckDlg,CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(ID_SELFCHECKDLG_BT_OK,&CSelfCheckDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_SELFCHECKDLG_BT_STOPCHECK,&CSelfCheckDlg::OnBnClickedStopCheck)
END_MESSAGE_MAP()

BOOL CSelfCheckDlg::OnEraseBkgnd(CDC* pDC)
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
	DrawRoundRectangleNew(&memDc,0,0,rect.Width() - m_frameWidth * 2,rect.Height() - m_frameWidth * 2,m_roundValue,m_roundValue,m_roundValue,m_roundValue,TRUE,Color(197,197,197),m_frameWidth,FALSE,Color(3,91,199),1);
	DrawRoundRectangleNew(&memDc,0,0,330 * dHoriRatio,m_iHeaderH,m_roundValue,m_roundValue,0,0,TRUE,Color(0XCC,0XCC,0XDC),2,TRUE,Color(0XCC,0XCC,0XDC),1);

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

void CSelfCheckDlg::OnLButtonDown(UINT nFlags, CPoint point)
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

void CSelfCheckDlg::OnBnClickedOk()
{
	OnOK();
}

void CSelfCheckDlg::OnBnClickedStopCheck()
{

}

BOOL CSelfCheckDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

	InitDlg();

	CreateCtrl();
	InitCtrl();
	SetCtrlPos();
	ShowCtrl();

	return TRUE;
}

void CSelfCheckDlg::InitDlg()
{
	SetMyFont(m_stTxFont,16 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
#ifdef POPDLG_USESHADEBK
	SetWindowPos(NULL,theApp.m_rcWorkArea.left,theApp.m_rcWorkArea.top,theApp.m_rcWorkArea.Width(),theApp.m_rcWorkArea.Height(),SWP_NOZORDER);
#else
	SetWindowPos(NULL,0 * dHoriRatio,0 * dVertRatio,m_iWidth,m_iHeight,SWP_NOZORDER);
	CenterWindow(NULL);
#endif
	
}

void CSelfCheckDlg::CreateCtrl()
{
	CreateBt();
	CreateSt();
	CreateCb();
	CreateEd();
	CreateLst();
}

void CSelfCheckDlg::CreateBt()
{
	RECT rect;
	rect.left = 0;
	rect.right = 0;
	rect.top = 0;
	rect.bottom = 0;

	m_btStopCheck.Create(_T("停止自检"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,rect,this,ID_SELFCHECKDLG_BT_STOPCHECK);
	m_btOk.Create(_T("确定"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,rect,this,ID_SELFCHECKDLG_BT_OK);
}

void CSelfCheckDlg::CreateSt()
{
	RECT rect;
	rect.left = 0;
	rect.right = 0;
	rect.top = 0;
	rect.bottom = 0;

	m_stRunningTag.Create(_T("动态图(标识自检正在进行中)"),WS_CHILD,rect,this,ID_SELFCHECKDLG_ST_RUNNINGTAG);
}

void CSelfCheckDlg::CreateCb()
{

}

void CSelfCheckDlg::CreateEd()
{

}

void CSelfCheckDlg::CreateLst()
{
	m_lstSelfCheckInfo.Create(IDD_MIDDLEDLG,this);
}

void CSelfCheckDlg::InitCtrl()
{
	InitBt();
	InitSt();
	InitCb();
	InitEd();
	InitLst();
}

void CSelfCheckDlg::InitBt()
{
	int iFontH = 16 * m_dTxRatio;

	m_btStopCheck.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btStopCheck.EnableWindow(FALSE);

	m_btOk.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
}

void CSelfCheckDlg::InitSt()
{
	int iFontH = 16 * m_dTxRatio;

	m_stRunningTag.SetTextColor(RGB(0X16,0X26,0X36));
	m_stRunningTag.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_stRunningTag.SetWordStyle(DT_LEFT);
	m_stRunningTag.SetSingleLineState(FALSE);
}

void CSelfCheckDlg::InitCb()
{

}

void CSelfCheckDlg::InitEd()
{

}

void CSelfCheckDlg::InitLst()
{
	int iFontH = 16 * m_dTxRatio;

	m_lstSelfCheckInfo.SetHeaderHeight(28);
	m_lstSelfCheckInfo.SetMyHeaderFont(m_stTxFont,iFontH + 1,FW_NORMAL,TYPE_PIXEL);
	m_lstSelfCheckInfo.SetHeaderTextColor(RGB(0X59,0X59,0X59));

	m_lstSelfCheckInfo.m_horiScroll.m_arrowWH = 28;
	m_lstSelfCheckInfo.m_horiScroll.m_scrollH = 28;
	m_lstSelfCheckInfo.m_vertScroll.m_arrowWH = 28;
	m_lstSelfCheckInfo.m_vertScroll.m_scrollW = 28;
	m_lstSelfCheckInfo.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_lstSelfCheckInfo.SetTextClr1(RGB(0X56,0X56,0X56));
	
	m_lstSelfCheckInfo.InsertColumn(0, _T("序号"));
	m_lstSelfCheckInfo.InsertColumn(1, _T("选择"));
	m_lstSelfCheckInfo.InsertColumn(2, _T("自检项目"));
	m_lstSelfCheckInfo.InsertColumn(3, _T("结果描述"));

	m_lstSelfCheckInfo.SetColumnCheckFunc(1,TRUE);
	
	int iWidth = 460 * m_dHoriRatio;

	int iTempInt1 = 50 * m_dVertRatio;
	m_lstSelfCheckInfo.SetColumnWidth(0,iTempInt1);
	iWidth -= iTempInt1;
	iTempInt1 = 50 * m_dVertRatio;
	m_lstSelfCheckInfo.SetColumnWidth(1,iTempInt1);
	iWidth -= iTempInt1;
	iTempInt1 = 220 * m_dVertRatio;
	m_lstSelfCheckInfo.SetColumnWidth(2,iTempInt1);
	iWidth -= iTempInt1;
	iTempInt1 = iWidth - m_lstSelfCheckInfo.GetFrameLineWidth() * 2;
	m_lstSelfCheckInfo.SetColumnWidth(3,iTempInt1);

	CString strTemp(_T(""));
	for(int i = 0;i < 4;++i)
	{
		m_lstSelfCheckInfo.InsertRow(i);
		strTemp.Format(_T("%d"),i + 1);
		m_lstSelfCheckInfo.SetItemText(i,0,strTemp);
	}

	m_lstSelfCheckInfo.SetItemText(0,2,_T("运动模块自检"));
	m_lstSelfCheckInfo.SetItemText(1,2,_T("传感器模块自检"));
	m_lstSelfCheckInfo.SetItemText(2,2,_T("温度模块自检"));
	m_lstSelfCheckInfo.SetItemText(3,2,_T("光学模块自检"));

	m_lstSelfCheckInfo.SetCellCheckState(0,1,TRUE);
	m_lstSelfCheckInfo.SetCellCheckState(1,1,TRUE);
	m_lstSelfCheckInfo.SetCellCheckState(2,1,TRUE);
	m_lstSelfCheckInfo.SetCellCheckState(3,1,TRUE);
}

void CSelfCheckDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	CRect rcUseful;
#ifdef POPDLG_USESHADEBK
	rcUseful.left = rect.left + (rect.Width() - m_iWidth) / 2;
	rcUseful.top = rect.top + (rect.Height() - m_iHeight) / 2;
	rcUseful.right = rcUseful.left + m_iWidth;
	rcUseful.bottom = rcUseful.top + m_iHeight;
#endif

	m_lstSelfCheckInfo.SetWindowPos(this,rcUseful.left + 30 * m_dHoriRatio,rcUseful.top + m_iHeaderH + 20 * m_dVertRatio,460 * m_dHoriRatio,500 * m_dVertRatio,SWP_NOZORDER);
	m_btStopCheck.SetWindowPos(this,rcUseful.left + 520 * m_dHoriRatio,rcUseful.top + 500 * m_dVertRatio,90 * m_dHoriRatio,40 * m_dVertRatio,SWP_NOZORDER);
	m_btOk.SetWindowPos(this,rcUseful.left + 630 * m_dHoriRatio,rcUseful.top + 500 * m_dVertRatio,90 * m_dHoriRatio,40 * m_dVertRatio,SWP_NOZORDER);
	m_stRunningTag.SetWindowPos(this,rcUseful.left + 520 * m_dHoriRatio,rcUseful.top + m_iHeaderH + 100 * m_dVertRatio,260 * m_dHoriRatio,60 * m_dVertRatio,SWP_NOZORDER);
}

void CSelfCheckDlg::ShowCtrl()
{
	m_lstSelfCheckInfo.ShowWindow(SW_SHOW);
	m_btStopCheck.ShowWindow(SW_SHOW);
	m_btOk.ShowWindow(SW_SHOW);
	m_stRunningTag.ShowWindow(SW_SHOW);
}

