
// GeneralPcrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralPcr.h"
#include "GeneralPcrDlg.h"
#include "FontSet.h"
#include "SelfCheckDlg.h"
#include "StartPageDlg.h"
#include "UserLoginDlg.h"
#include "LanguageInfo.h"
#include "afxdialogex.h"
#include "InstrumentPCR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGeneralPcrDlg::CGeneralPcrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGeneralPcrDlg::IDD, pParent), m_prjManageDlg(this),m_stUserName(TRUE),m_btMinimum(TRUE),m_btSet(TRUE),
	m_btZoom(TRUE),m_btClose(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_iCapH = 65;
	m_pCurSelSwt = NULL;
	m_pCurSelWnd = NULL;

	m_pMemDc = NULL;
	m_hBitmap = NULL;
	m_bPreZeroSize = FALSE;
	m_bZeroToNewSize = FALSE;

	m_nRunTimer = 0;
	m_iTimerCount = 0;
}

CGeneralPcrDlg::~CGeneralPcrDlg()
{
	if(NULL != m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
	}
	if(NULL != m_pMemDc)
	{
		m_pMemDc->DeleteDC();
		delete m_pMemDc;
		m_pMemDc = NULL;
	}
}

void CGeneralPcrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGeneralPcrDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE() 
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_SHOWWINDOW()

	ON_BN_CLICKED(ID_GENERALPCRDLG_SWTBT_MAINPAGE,OnBnClickedSwtBtMainPage)
	ON_BN_CLICKED(ID_GENERALPCRDLG_SWTBT_PRJMANAGE,OnBnClickedSwtBtPrjManage)
	ON_BN_CLICKED(ID_GENERALPCRDLG_SWTBT_SET,OnBnClickedSwtBtSet)
	ON_BN_CLICKED(ID_GENERALPCRDLG_SWTBT_WORK,OnBnClickedSwtBtWork)
	ON_BN_CLICKED(ID_GENERALPCRDLG_BT_MINIMUM,OnBnClickedBtMinimum)
	ON_BN_CLICKED(ID_GENERALPCRDLG_BT_ZOOM,OnBnClickedBtZoom)
	ON_BN_CLICKED(ID_GENERALPCRDLG_BT_CLOSE,OnBnClickedBtClose)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CGeneralPcrDlg message handlers

BOOL CGeneralPcrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ModifyStyle(0,WS_SYSMENU | WS_MINIMIZEBOX);

	SetWindowPos(&wndTopMost,theApp.m_rcWorkArea.left,theApp.m_rcWorkArea.top,theApp.m_rcWorkArea.Width(),theApp.m_rcWorkArea.Height(),SWP_NOZORDER);
	m_rcManage.m_rcInit = theApp.m_rcWorkArea;

	SetWindowText(SOFRVERION_STRING);
	
	CFuncBtDlg::m_pMainWnd = this;

	InitSonDlg();

	CreateCtrl();
	InitCtrl();
	SetCtrlPos();

	SetCtrlText();
	 
	ShowCtrl();

	SetPageSelected(EMFT_WORK);
	
	if(theApp.m_lnkLnkIndex.GetSize() > 0)
	{
		BOOL bAddPage = FALSE;
		CRunProgressRecord runPrgRcd;

		for (int i=0; i<theApp.m_lnkLnkIndex.GetSize(); i++)
		{
			int iLnkIndex = theApp.m_lnkLnkIndex.GetAt(i)->data;
			int iRet = theApp.m_powerOffProtect.GetRunProgressRecord(iLnkIndex,runPrgRcd);
			if(0 == iRet)
			{
				m_workDlg.OpenPowerOffProtectFile(runPrgRcd.m_strFilePath);
				//发送消息运行实验
				if(NULL != m_workDlg.m_pCurSelPageDlg->GetSafeHwnd())
				{
					m_workDlg.m_pCurSelPageDlg->SetPowerOffExpInfo(runPrgRcd);
					m_workDlg.m_pCurSelPageDlg->PostMessage(WM_RUNPOWEROFFPROTECT,0,0);
					theApp.m_powerOffProtect.DeleteOldProgress(iLnkIndex);
				}

				bAddPage = TRUE;
			}
		}

		if (!bAddPage)
		{
			//提示重新断电保护实验失败

			m_workDlg.m_fileManageDlg.AddPage();
		}
	}
	else
	{
		m_workDlg.m_fileManageDlg.AddPage();
	}

	m_nRunTimer = SetTimer(1, 1000, NULL);

	return TRUE;
}

void CGeneralPcrDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGeneralPcrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGeneralPcrDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(SC_MINIMIZE == nID)
	{
		CRect rect;
		GetClientRect(&rect);
		int tt = 0;
		++tt;

		if(NULL != m_hBitmap)
		{
			::DeleteObject(m_hBitmap);
			m_hBitmap = NULL;
		}
		if(NULL != m_pMemDc)
		{
			m_pMemDc->DeleteDC();
			delete m_pMemDc;
			m_pMemDc = NULL;
		}

		CDC* pDC = GetDC();
		if(NULL != pDC)
		{
			m_pMemDc = new CDC;
			BOOL ret = m_pMemDc->CreateCompatibleDC(pDC);
			m_hBitmap = ::CreateCompatibleBitmap(pDC->m_hDC,rect.Width(),rect.Height()); //创建兼容位图
			m_pMemDc->SelectObject(m_hBitmap);	
			m_pMemDc->BitBlt(0,0,rect.Width(),rect.Height(),pDC,0,0,SRCCOPY);
		}
		ReleaseDC(pDC);

	}
	else if(SC_MAXIMIZE == nID)
	{
		CRect rect;
		GetClientRect(&rect);
		int tt = 0;
		++tt;
	}
	CDialogEx::OnSysCommand(nID, lParam);
}

int CGeneralPcrDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CDC* pDC = GetDC();
	GeneDpi = GetDeviceCaps(pDC->m_hDC,LOGPIXELSY);
	ReleaseDC(pDC);
	
	m_iCapH *= m_dVertRatio;
	theApp.m_iFileManageDlgH *= m_dVertRatio;

	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CGeneralPcrDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect; 
	GetClientRect(&rect);

	if(m_bPreZeroSize)
	{
		m_bPreZeroSize = FALSE;
		if(NULL != m_pMemDc)
		{
			pDC->BitBlt(0,0,rect.Width(),rect.Height(),m_pMemDc,0,0,SRCCOPY);
			return TRUE;
		}
	}

	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	COLORREF clrBody = CLR_SOFT_LIGHT;

#ifdef GRADIENT_MAIN_BK
	Graphics graphics(memDc.m_hDC);
	LinearGradientBrush linGrBrush(RectF(0,0,rect.Width(),m_iCapH),Color(255,0X06,0X91,0X9D),Color(255,0,0XBA,0XCA),LinearGradientModeHorizontal);
	Status sRet = graphics.FillRectangle(&linGrBrush,0,0,rect.Width(),m_iCapH);
	graphics.ReleaseHDC(memDc.m_hDC);
#else
	memDc.FillSolidRect(0,0,rect.Width(),m_iCapH,CLR_SOFT_THEME);
	clrBody = CLR_SOFT_LIGHT;
#endif

	memDc.FillSolidRect(0,m_iCapH,rect.Width(),rect.Height() - m_iCapH,clrBody);

	//DrawRoundRectangleNew(&memDc,1505 * m_dHoriRatio,12 * m_dVertRatio,220 * m_dHoriRatio,56 * m_dVertRatio,8,8,8,8,TRUE,COLOR_2LEVEL_THEME,1,TRUE,Color(0XFF,0XFF,0XFF),1);
	//DrawRoundRectangleNew(&memDc,1677 * m_dHoriRatio + 1,12 * m_dVertRatio,48 * m_dHoriRatio - 1,18 * m_dVertRatio - 1,0,8,0,8,TRUE,COLOR_2LEVEL_THEME,1,TRUE,COLOR_2LEVEL_THEME,1);

	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

	Node<CRect>* pNodeRst = m_rcManage.m_lnkRcRst.GetHead();
	while(NULL != pNodeRst)
	{
		pDC->BitBlt(pNodeRst->data.left,pNodeRst->data.top,pNodeRst->data.Width(),pNodeRst->data.Height(),&memDc,pNodeRst->data.left,pNodeRst->data.top,SRCCOPY);
		pNodeRst = pNodeRst->pNext;
	}
	
	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();
	
	return TRUE;
}

void CGeneralPcrDlg::OnSize(UINT nType, int cx, int cy)
{
	if(0 == cx || 0 == cy)
	{
		m_bPreZeroSize = TRUE;
	}
	else 
	{
		if(m_bPreZeroSize)
		{
			m_bZeroToNewSize = TRUE;
			m_bPreZeroSize = FALSE;
		}
	}
	
	CDialogEx::OnSize(nType, cx, cy);
}

void CGeneralPcrDlg::OnSizing(UINT nSide,LPRECT pRect)
{
	CDialogEx::OnSizing(nSide,pRect);
}

void CGeneralPcrDlg::OnShowWindow(BOOL bShow,UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow,nStatus);
	return;
}

void CGeneralPcrDlg::OnBnClickedSwtBtMainPage()
{

}

void CGeneralPcrDlg::OnBnClickedSwtBtPrjManage()
{
	SetPageSelected(EMFT_PRJMANAGE);
}

void CGeneralPcrDlg::OnBnClickedSwtBtSet()
{
	SetPageSelected(EMFT_SET);
	return;
}

void CGeneralPcrDlg::OnBnClickedSwtBtWork()
{
#ifdef USEMENUFORMAT
	CMenu menu1;
	menu1.CreatePopupMenu();
	menu1.AppendMenu(MF_STRING,ID_EXPMENU_NEWEXP,_T("新建实验"));
	menu1.AppendMenu(MF_STRING,ID_EXPMENU_OPEN,_T("打开"));
	menu1.AppendMenu(MF_STRING,ID_EXPMENU_SAVE,_T("保存"));
	menu1.AppendMenu(MF_STRING,ID_EXPMENU_SAVEAS,_T("另存为"));
	menu1.AppendMenu(MF_STRING,ID_EXPMENU_SAVEASTEMPLATE,_T("保存模板"));
	menu1.AppendMenu(MF_STRING,ID_EXPMENU_EXPORT,_T("导出"));
	CPoint pt;
	GetCursorPos(&pt);
	CRect rcBt;
	m_btWork.GetWindowRect(&rcBt);

	menu1.TrackPopupMenu(TPM_RIGHTBUTTON,rcBt.left,rcBt.bottom,this);
	menu1.DestroyMenu();
#else
	SetPageSelected(EMFT_WORK);
#endif
	
	return;
}

void CGeneralPcrDlg::OnBnClickedBtMinimum()
{
	ShowWindow(SW_SHOWMINIMIZED);
	return;
}

void CGeneralPcrDlg::OnBnClickedBtZoom()
{

}

void CGeneralPcrDlg::OnBnClickedBtClose()
{
	if(m_workDlg.IsRunningProcessExist())
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(313));
		return;
	}

	if(IDYES != PopQueryTipInfo(theApp.m_pLangInfo->GetText(204)))
	{
		return;
	}

	DestroyWindow();
	return;
}


void CGeneralPcrDlg::CreateCtrl()
{
	CreateBt();
	CreateSt();
}

void CGeneralPcrDlg::CreateBt()
{
	RECT rect;
	rect.left = 0;
	rect.right = 0;
	rect.top = 0;
	rect.bottom = 0;

	m_btMainPage.Create(_T("主页"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,rect,this,ID_GENERALPCRDLG_SWTBT_MAINPAGE);
	m_btWork.Create(_T("实验"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,rect,this,ID_GENERALPCRDLG_SWTBT_WORK);
	m_btPrjManage.Create(_T("项目"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,rect,this,ID_GENERALPCRDLG_SWTBT_PRJMANAGE);
	m_btSet.Create(_T("设置"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,rect,this,ID_GENERALPCRDLG_SWTBT_SET);
	m_btMinimum.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,rect,this,ID_GENERALPCRDLG_BT_MINIMUM);
	m_btZoom.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,rect,this,ID_GENERALPCRDLG_BT_ZOOM);
	m_btClose.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,rect,this,ID_GENERALPCRDLG_BT_CLOSE);

	return;
}

void CGeneralPcrDlg::CreateSt()
{
	RECT rect;
	rect.left = 0;
	rect.right = 0;
	rect.top = 0;
	rect.bottom = 0;

	m_stUserName.Create(_T("用户名"),WS_CHILD,rect,this,ID_GENERALPCRDLG_ST_USERNAME);
	m_stLisConState.Create(_T("LIS"),WS_CHILD,rect,this,ID_GENERALPCRDLG_ST_LISCONNECTSTATE);
	m_stPrinterConState.Create(_T("PRINTER"),WS_CHILD,rect,this,ID_GENERALPCRDLG_ST_PRINTERCONNECTSTATE);
}

void CGeneralPcrDlg::InitCtrl()
{
	InitBt();
	InitSt();
}

void CGeneralPcrDlg::InitBt()
{
	int iFontH = 12 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 15 * m_dTxRatio;
	}

	int iIconStart = 20 * m_dHoriRatio;
	int iEndX = 58 * m_dHoriRatio;
	if(theApp.GetBeEnglish())
	{
		iIconStart = 27 * m_dHoriRatio;
		iEndX = 72 * m_dHoriRatio;
	}

	CIconBtInfo iconBtInfo;

	iconBtInfo.m_strIconPath = _T(".\\BkPic\\MainPage.png");
	iconBtInfo.m_rcIcon = Rect(18 * m_dHoriRatio,10 * m_dVertRatio, 24 * m_dHoriRatio,24 * m_dVertRatio);
	iconBtInfo.m_rcText = CRect(30 * m_dHoriRatio,10 * m_dVertRatio, 124 * m_dHoriRatio,33 * m_dVertRatio);

	m_btMainPage.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btMainPage.SetIconBtInfo(iconBtInfo);
	m_btMainPage.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF));

	iconBtInfo.m_strIconPath = _T(".\\BkPic\\Trial.png");
	m_btWork.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btWork.SetIconBtInfo(iconBtInfo);
	m_btWork.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF));

	iconBtInfo.m_strIconPath = _T(".\\BkPic\\project.png");
	m_btPrjManage.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btPrjManage.SetIconBtInfo(iconBtInfo);
	m_btPrjManage.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF));

	iconBtInfo.m_strIconPath = _T(".\\BkPic\\set.png");
	m_btSet.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btSet.SetIconBtInfo(iconBtInfo);
	m_btSet.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF));

	iconBtInfo.m_bNormalNotFill = TRUE;
	iconBtInfo.m_bHoverNotFill = TRUE;

	Color clr1 = COLOR_SOFT_THEME,clr2 = Color(0X0A,0XB6,0XC5),clr3 = COLOR_2LEVEL_THEME,clr4 = Color(0X6B,0XAF,0XB6);
	clr1 = COLOR_BTBK_NORMAL;
	clr2 = COLOR_BTBK_HOVER;
	clr3 = COLOR_BTBK_CLICKED;
	clr4 = COLOR_BTBK_DISABLED;

	m_btMinimum.SetDlgSizeBtParam(TRUE,EDSBT_MINIMUM);
	m_btMinimum.SetBkColor(clr1,clr2,clr3,clr4);
	m_btMinimum.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btMinimum.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0,0,0),RGB(0,0,0));
	
	m_btZoom.SetDlgSizeBtParam(TRUE,EDSBT_ZOOM);
	m_btZoom.SetBkColor(clr1,clr2,clr3,clr4);
	m_btZoom.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btZoom.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0,0,0),RGB(0,0,0));
	
	m_btClose.SetDlgSizeBtParam(TRUE,EDSBT_CLOSE);
	m_btClose.SetBkColor(clr1,clr2,clr3,clr4);
	m_btClose.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btClose.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0,0,0),RGB(0,0,0));

	return;
}

void CGeneralPcrDlg::InitSt()
{
	int iFontH = 12 * m_dTxRatio;
	int iFontH2 = 18 * m_dTxRatio;
	int iFontH3 = 16 * m_dTxRatio;
	int iFontH4 = 14 * m_dTxRatio;
	COLORREF clrText = RGB(0XFF,0XFF,0XFF); 

	m_stUserName.SetPngBkPic(_T(".\\BkPic\\user.png"),1,16,0,8 * m_dHoriRatio);
	m_stUserName.SetBkColor(CLR_SOFT_THEME);
	m_stUserName.SetTextColor(clrText);
	m_stUserName.SetMyFont(m_stTxFont,iFontH3,FW_NORMAL,TYPE_PIXEL);
	m_stUserName.SetWordStyle(DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
	m_stUserName.SetWindowText(theApp.m_curUserInfo.GetName());

	m_stLisConState.SetPngBkPic(_T(".\\BkPic\\connect.png"),2,16,0,4 * m_dHoriRatio);
	m_stLisConState.SetRoundRectBkParam(TRUE,CLR_SOFT_THEME,COLOR_2LEVEL_THEME,4);
	m_stLisConState.SetBDynamic(TRUE);
	m_stLisConState.SetBkColor(CLR_SOFT_THEME);
	m_stLisConState.SetTextColor(clrText);
	m_stLisConState.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_stLisConState.SetWordStyle(DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);

	m_stPrinterConState.SetPngBkPic(_T(".\\BkPic\\connect.png"),2,16,0,4 * m_dHoriRatio);
	m_stPrinterConState.SetRoundRectBkParam(TRUE,CLR_SOFT_THEME,COLOR_2LEVEL_THEME,4);
	m_stPrinterConState.SetBDynamic(TRUE);
	m_stPrinterConState.SetBkColor(CLR_SOFT_THEME);
	m_stPrinterConState.SetTextColor(clrText);
	m_stPrinterConState.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_stPrinterConState.SetWordStyle(DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
}


void CGeneralPcrDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	int iTempInt1 = 0;
	int iTempInt2 = rect.Width();

	int x = 0,y = 0,iWidth = 0,iHeight = 0,iHoriInterval = 24 * m_dHoriRatio;
	
	x = 11 * m_dHoriRatio;
	y = 12 * m_dVertRatio;
	iWidth = 120 * m_dHoriRatio;	
	iHeight = 42 * m_dVertRatio;

	m_rcManage.ClearRectToDelete();

	m_btWork.SetWindowPos(this,x,y,iWidth,iHeight,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(x,y,iWidth,iHeight);

	x += (iWidth + iHoriInterval);
	m_btPrjManage.SetWindowPos(this,x,y,iWidth,iHeight,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(x,y,iWidth,iHeight);

#ifdef HIDE_PROJECT
	m_btSet.SetWindowPos(this,x,y,iWidth,iHeight,SWP_NOZORDER);
#else
	if(theApp.GetBeIVDUser())
	{
		m_btSet.SetWindowPos(this,x,y,iWidth,iHeight,SWP_NOZORDER);
	}
	else
	{
		x += (iWidth + iHoriInterval);
		m_btSet.SetWindowPos(this,x,y,iWidth,iHeight,SWP_NOZORDER);
	}
#endif
	m_rcManage.AddRectToDelete(x,y,iWidth,iHeight);
	int iSetX = x,iSetY = y,iSetWidth = iWidth,iSetHeight = iHeight;

	iHeight = rect.Height() - m_iCapH;
	y = m_iCapH;
	m_workDlg.SetWindowPos(this,iTempInt1,y,iTempInt2,iHeight,SWP_NOZORDER);
	m_setDlg.SetWindowPos(this,iTempInt1,y,iTempInt2,iHeight,SWP_NOZORDER);
	m_prjManageDlg.SetWindowPos(this,iTempInt1,y,iTempInt2,iHeight,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iTempInt1,y,iTempInt2,iHeight);

	iTempInt1 = 24 * m_dHoriRatio;
	iTempInt2 = 24 * m_dVertRatio;
	int iTempInt3 = 5 * m_dVertRatio;
	int iTempInt4 = 10 * m_dHoriRatio;

	x = rect.Width() - 14 * m_dHoriRatio - iTempInt1;
	m_btClose.SetWindowPos(this,x,iTempInt3,iTempInt1,iTempInt2,SWP_NOZORDER);

	x -= (iTempInt4 + iTempInt1);
	m_btZoom.SetWindowPos(this,x,iTempInt3,iTempInt1,iTempInt2,SWP_NOZORDER);

	x -= (iTempInt4 + iTempInt1);
	m_btMinimum.SetWindowPos(this,x,iTempInt3,iTempInt1,iTempInt2,SWP_NOZORDER);

	int iStateRegionY = 36 * m_dVertRatio;

	x = 1223 * m_dHoriRatio; 
	y = 20 * m_dVertRatio;
	iWidth = 90 * m_dHoriRatio;
	iHeight = 21 * m_dVertRatio;
	m_stUserName.SetWindowPos(this,x,y,iWidth,iHeight,SWP_NOZORDER);

	x = 1325 * m_dHoriRatio;
	y = 20 * m_dVertRatio;
	iWidth = 45 * m_dHoriRatio;
	iHeight = 20 * m_dVertRatio;
	m_stLisConState.SetWindowPos(this,x,y,iWidth,iHeight,SWP_NOZORDER);	

	x = 1378 * m_dHoriRatio;
	y = 20 * m_dVertRatio;
	iWidth = 77 * m_dHoriRatio;
	iHeight = 20 * m_dHoriRatio;
	m_stPrinterConState.SetWindowPos(this,x,y,iWidth,iHeight,SWP_NOZORDER);

	int iStateRegionX = (1920 - 300) * m_dHoriRatio;

	m_rcManage.CalcRectResult();

	//重绘背景时,刷新设置按钮的四角对应的四个矩形和边框
	m_rcManage.AddRectRoundFrame(iSetX,iSetY,iSetWidth,iSetHeight,8,1);
	return;
}

void CGeneralPcrDlg::ShowCtrl()
{
	m_btWork.ShowWindow(SW_SHOW);
	m_btSet.ShowWindow(SW_SHOW);
	
	m_btMinimum.ShowWindow(SW_SHOW);
	m_btZoom.ShowWindow(SW_SHOW);
	m_btClose.ShowWindow(SW_SHOW);

	m_stUserName.ShowWindow(SW_SHOW);	

	if(!theApp.GetBeIVDUser())
	{
		//m_stLisConState.ShowWindow(SW_SHOW);	
		//m_stPrinterConState.ShowWindow(SW_SHOW);	
	}

#ifndef HIDE_PROJECT
	if(!theApp.GetBeIVDUser())
	{
		m_btPrjManage.ShowWindow(SW_SHOW);
	}
#endif

	return;
}

void CGeneralPcrDlg::InitSonDlg()
{
	InitWorkDlg();
	InitSetDlg();
	InitPrjManageDlg();

	return;
}

void CGeneralPcrDlg::InitWorkDlg()
{
	m_workDlg.Create(IDD_MIDDLEDLG,this);
}

void CGeneralPcrDlg::InitSetDlg()
{
	m_setDlg.Create(IDD_MIDDLEDLG,this);
}

void CGeneralPcrDlg::InitPrjManageDlg()
{
	m_prjManageDlg.Create(IDD_MIDDLEDLG,this);
}

void CGeneralPcrDlg::SetCtrlText(BOOL bOnlyConnectStatus)
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	if (!bOnlyConnectStatus)
	{
		m_btMainPage.SetWindowText(pLangInfo->GetText(6));
		m_btWork.SetWindowText(pLangInfo->GetText(1));
		m_btPrjManage.SetWindowText(pLangInfo->GetText(2));
		m_btSet.SetWindowText(pLangInfo->GetText(3));
	}
}

void CGeneralPcrDlg::SetPageSelected(EMAINFUNCTYPE eMainFuncType)
{
	BOOL bShowChanBt = FALSE;

	if(EMFT_WORK == eMainFuncType)
	{
		if(NULL != m_pCurSelWnd->GetSafeHwnd())
		{
			m_pCurSelWnd->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelSwt->GetSafeHwnd())
		{
			m_pCurSelSwt->EnableWindow(TRUE);
		}
		m_btWork.EnableWindow(FALSE);
		m_workDlg.ShowWindow(SW_SHOW);
		m_pCurSelWnd = &m_workDlg;
		m_pCurSelSwt = &m_btWork;

		//实验监控页面和分析页面显示通道切换按钮
		if(NULL != m_workDlg.m_pCurSelPageDlg->GetSafeHwnd())
		{
			if(ESPT_RUNMONITOR == m_workDlg.m_pCurSelPageDlg->m_curSelSonPage || ESPT_DATAANALYSE == m_workDlg.m_pCurSelPageDlg->m_curSelSonPage)
			{
				bShowChanBt = TRUE;
			}
		}

		if(NULL != m_workDlg.m_pCurSelPageDlg->GetSafeHwnd())
		{
			m_workDlg.m_pCurSelPageDlg->UpdateFuncBtMode();
		}
	}
	else if(EMFT_SET == eMainFuncType)
	{
		if(NULL != m_pCurSelWnd->GetSafeHwnd())
		{
			m_pCurSelWnd->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelSwt->GetSafeHwnd())
		{
			m_pCurSelSwt->EnableWindow(TRUE);
		} 
		m_btSet.EnableWindow(FALSE);
		m_setDlg.ShowWindow(SW_SHOW);
		m_pCurSelWnd = &m_setDlg;
		m_pCurSelSwt = &m_btSet;

		m_setDlg.UpdateFuncBtMode();
	}
	else if(EMFT_PRJMANAGE == eMainFuncType)
	{
		if(NULL != m_pCurSelWnd->GetSafeHwnd())
		{
			m_pCurSelWnd->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelSwt->GetSafeHwnd())
		{
			m_pCurSelSwt->EnableWindow(TRUE);
		}
		m_btPrjManage.EnableWindow(FALSE);
		m_prjManageDlg.ShowWindow(SW_SHOW);
		m_pCurSelWnd = &m_prjManageDlg;
		m_pCurSelSwt = &m_btPrjManage;
	}
}

void CGeneralPcrDlg::New()
{
	if(m_workDlg.IsWindowVisible())
	{
		m_workDlg.m_fileManageDlg.AddPage();
	}
}

void CGeneralPcrDlg::Delete()
{
	if(m_workDlg.IsWindowVisible())
	{
		m_workDlg.Delete();
	}
}

void CGeneralPcrDlg::Open()
{
	if(m_workDlg.IsWindowVisible())
	{
		m_workDlg.OpenFile();
	}

	return;
}

int CGeneralPcrDlg::SetLidTem(int iLidTem)
{
	CLinkList<CMiddleDlg*>& lnkPageDlgPtr = m_workDlg.m_lnkPageDlgPtr;
	int iExpNum = lnkPageDlgPtr.GetSize();
	CMiddleDlg* pMiddlgDlg = NULL;
	BOOL bHaveSuccessWrite = FALSE;

	for(int i = 0;i < iExpNum;++i)
	{
		pMiddlgDlg = *lnkPageDlgPtr[i];
		if(NULL != pMiddlgDlg->GetSafeHwnd() && NULL != pMiddlgDlg->m_pExpData)
		{
			if(0 == pMiddlgDlg->SetInstrumentLidTem(iLidTem))
			{
				bHaveSuccessWrite = TRUE;
				pMiddlgDlg->m_pExpData->GetExperimentInfo()->SetLidTemp(iLidTem);
				// pMiddlgDlg->m_baseSetDlg.m_instrumentInfoDlg.Invalidate();
			}
		}
	}

	if(bHaveSuccessWrite)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int CGeneralPcrDlg::ReadLidTem(double& dLidTem)
{
	CLinkList<CMiddleDlg*>& lnkPageDlgPtr = m_workDlg.m_lnkPageDlgPtr;
	int iExpNum = lnkPageDlgPtr.GetSize();
	CMiddleDlg* pMiddlgDlg = NULL;

	for(int i = 0;i < iExpNum;++i)
	{
		pMiddlgDlg = *lnkPageDlgPtr[i];
		if(NULL != pMiddlgDlg->GetSafeHwnd() && NULL != pMiddlgDlg->m_pExpData)
		{
			if(0 == pMiddlgDlg->ReadInstrumentLidTem(dLidTem))
			{
				return 0;
			}
		}
	}

	return -1;
}

int CGeneralPcrDlg::SetLidPreheatTem(int iLidTem)
{
	CLinkList<CMiddleDlg*>& lnkPageDlgPtr = m_workDlg.m_lnkPageDlgPtr;
	int iExpNum = lnkPageDlgPtr.GetSize();
	CMiddleDlg* pMiddlgDlg = NULL;
	BOOL bHaveSuccessWrite = FALSE;

	for(int i = 0;i < iExpNum;++i)
	{
		pMiddlgDlg = *lnkPageDlgPtr[i];
		if(NULL != pMiddlgDlg->GetSafeHwnd() && NULL != pMiddlgDlg->m_pExpData)
		{
			if(0 == pMiddlgDlg->SetInstrumentLidPreheatTem(iLidTem))
			{
				bHaveSuccessWrite = TRUE;
			}
		}
	}

	if(bHaveSuccessWrite)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int CGeneralPcrDlg::ReadLidPreheatTem(float& fLidTem)
{
	CLinkList<CMiddleDlg*>& lnkPageDlgPtr = m_workDlg.m_lnkPageDlgPtr;
	int iExpNum = lnkPageDlgPtr.GetSize();
	CMiddleDlg* pMiddlgDlg = NULL;

	for(int i = 0;i < iExpNum;++i)
	{
		pMiddlgDlg = *lnkPageDlgPtr[i];
		if(NULL != pMiddlgDlg->GetSafeHwnd() && NULL != pMiddlgDlg->m_pExpData)
		{
			if(0 == pMiddlgDlg->ReadInstrumentLidPreheatTem(fLidTem))
			{
				return 0;
			}
		}
	}

	return -1;
}

int CGeneralPcrDlg::RefreshLanguageDisplay()
{
	//刷新和语言相关的公有信息
	CMySamPalteDlg::InitMapSampleTypeToText();
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	int iFontH = 12 * m_dTxRatio;
	int iFontH3 = 16 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 15 * m_dTxRatio;
	}
	
	m_btWork.SetMyFont(m_stTxFont,iFontH);
	m_btWork.SetWindowText(pLangInfo->GetText(1));

	m_btPrjManage.SetMyFont(m_stTxFont,iFontH);
	m_btPrjManage.SetWindowText(pLangInfo->GetText(2));

	m_btSet.SetMyFont(m_stTxFont,iFontH);
	m_btSet.SetWindowText(pLangInfo->GetText(3));

	m_setDlg.RefreshLanguageDisplay();
	m_workDlg.RefreshLanguageDisplay();
	m_prjManageDlg.RefreshLanguageDisplay();

	return 0;
}

int CGeneralPcrDlg::SetDefaultGeneName()
{
	return m_workDlg.SetDefaultGeneName();
}


void CGeneralPcrDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_workDlg.IsRunningProcessExist())
		return;

	if (m_iTimerCount > 5)
	{
		BOOL bHasDevice = FALSE;
		BOOL bConnect = FALSE;
		CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
		if (pInstrument->GetInstanceCount() == 1)
		{
			CInstancePCR* pInstance = pInstrument->GetInstancePCR(0);
			if (!pInstance->IsDemo())
				bHasDevice = TRUE;
		}
		else if (pInstrument->GetInstanceCount() > 1)
		{
			bHasDevice = TRUE;
		}

		if(bHasDevice)
		{
			for(int i=0; i<pInstrument->GetInstanceCount(); i++)
			{
				CInstancePCR* pInstance = pInstrument->GetInstancePCR(i);
				if (pInstance == NULL || pInstance->IsDemo())
					continue;

				if (pInstance->GetInstanceStatus() == INSTANCE_READY)
				{
					BOOL bReturn = pInstance->Hello();
					if (bReturn)
					{
						bConnect = TRUE;
					}
					else
					{
						pInstance->SetInstanceStatus(INSTANCE_ERROR);
						bConnect = FALSE;
					}
				}
				//else if (pInstance->GetInstanceStatus() != INSTANCE_USING)
				else if (pInstance->GetInstanceStatus() == INSTANCE_ERROR)
				{
					BOOL bReturn = pInstance->Reinitialize();
					if (bReturn)
					{
						bConnect = TRUE;
						pInstance->SetInstanceStatus(INSTANCE_READY);
					}
					else
					{
						bConnect = FALSE;
					}
				}
				else
				{
					bConnect = TRUE;
				}

				m_workDlg.SetInstrumentConnectState(pInstance->GetModel(), pInstance->GetSN(), bConnect, FALSE);
			}
		}
		else
		{
			BOOL bFind = pInstrument->Reinitialize(TRUE);
			if (bFind && pInstrument->GetInstanceCount() > 0)
			{
				CInstancePCR* pInstance = pInstrument->GetAvailableInstance();
				if (pInstance != NULL)
					m_workDlg.SetInstrumentConnectState(pInstance->GetModel(), pInstance->GetSN(), bConnect, TRUE);
			}
		}

		m_iTimerCount = 0;
	}
	else
	{
		m_iTimerCount ++;
	}

	__super::OnTimer(nIDEvent);
}


void CGeneralPcrDlg::OnDestroy()
{
	__super::OnDestroy();

	KillTimer(m_nRunTimer);
}
