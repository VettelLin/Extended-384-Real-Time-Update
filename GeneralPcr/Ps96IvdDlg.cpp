
// GeneralPcrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralPcr.h"
#include "Ps96IvdDlg.h"
#include "FontSet.h"
#include "afxdialogex.h"
#include "InstrumentPCR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPs96IvdDlg::CPs96IvdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPs96IvdDlg::IDD, pParent),m_btMinimum(TRUE),m_btZoom(TRUE),m_btClose(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_iCapH = 40;
	m_iFrameW = 4;
	m_pCurSelSwt = NULL;
	m_pCurSelWnd = NULL;

	m_clrFrame = RGB(0X60,0X68,0X6F);

	m_pMemDc = NULL;
	m_hBitmap = NULL;
	m_bPreZeroSize = FALSE;
	m_bZeroToNewSize = FALSE;
	m_bCtrlCreated = FALSE;
	m_dSizeRatio = 1;
	m_bZooming = FALSE;

	m_nRunTimer = 0;
	m_iTimerCount = 0;
}

CPs96IvdDlg::~CPs96IvdDlg()
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

void CPs96IvdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPs96IvdDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE() 
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()

	ON_BN_CLICKED(ID_PS96IVDDLG_BT_MINIMUM,OnBnClickedBtMinimum)
	ON_BN_CLICKED(ID_PS96IVDDLG_BT_ZOOM,OnBnClickedBtZoom)
	ON_BN_CLICKED(ID_PS96IVDDLG_BT_CLOSE,OnBnClickedBtClose)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CPs96IvdDlg message handlers

BOOL CPs96IvdDlg::OnInitDialog()
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

	InitDlg();
	InitSonDlg();

	CreateCtrl();
	m_bCtrlCreated = TRUE;
	InitCtrl();
	SetCtrlPos();

	ShowCtrl();

	if(theApp.m_lnkLnkIndex.GetSize() > 0)
	{
		CRunProgressRecord runPrgRcd;
		int iLnkIndex = theApp.m_lnkLnkIndex.GetHead()->data;
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
		}
		else
		{
			//提示重新断电保护实验失败

			m_workDlg.m_ps96FileManagerDlg.AddPage();
		}
	}
	else
	{
		m_workDlg.m_ps96FileManagerDlg.AddPage();
	}

	m_nRunTimer = SetTimer(1, 1000, NULL);

	return TRUE;
}

void CPs96IvdDlg::OnPaint()
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
HCURSOR CPs96IvdDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPs96IvdDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(SC_MINIMIZE == nID)
	{
		CRect rect;
		GetClientRect(&rect);

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
	}
	CDialogEx::OnSysCommand(nID, lParam);
}

int CPs96IvdDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_iCapH *= m_dVertRatio;
	m_iFrameW *= m_dTxRatio;

 	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CPs96IvdDlg::OnEraseBkgnd(CDC* pDC)
{
	if(m_bZooming)
	{
		return TRUE;
	}

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

	COLORREF clrBody = theApp.m_clrSoftLight;

	Graphics graphics(memDc.m_hDC);
	LinearGradientBrush linGrBrush(RectF(0,0,rect.Width(),m_iCapH),theApp.m_gclrSoftTheme,theApp.m_gclrSoftThemeEnd,LinearGradientModeHorizontal);
	Status sRet = graphics.FillRectangle(&linGrBrush,0,0,rect.Width(),m_iCapH);
	graphics.ReleaseHDC(memDc.m_hDC);

	memDc.FillSolidRect(0,m_iCapH,rect.Width(),rect.Height() - m_iCapH,clrBody);

	//绘制边框
	memDc.FillSolidRect(0,0,rect.Width(),m_iFrameW,m_clrFrame);
	memDc.FillSolidRect(rect.Width() - m_iFrameW,0,m_iFrameW,rect.Height(),m_clrFrame);

	memDc.FillSolidRect(0,rect.Height() - m_iFrameW,rect.Width(),m_iFrameW,m_clrFrame);
	memDc.FillSolidRect(0,0,m_iFrameW,rect.Height(),m_clrFrame);


	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetBkMode(TRANSPARENT);
	memDc.SetTextColor(m_txClr1);

	clock_t ckStart = clock();	
	Node<CRect>* pNodeRst = m_rcManage.m_lnkRcRst.GetHead();
	while(NULL != pNodeRst)
	{
		pDC->BitBlt(pNodeRst->data.left,pNodeRst->data.top,pNodeRst->data.Width(),pNodeRst->data.Height(),&memDc,pNodeRst->data.left,pNodeRst->data.top,SRCCOPY);
		pNodeRst = pNodeRst->pNext;
	}
	ckStart = clock() - ckStart;

	memDc.SelectObject(pOldFont);
	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();
	
	return TRUE;
}

void CPs96IvdDlg::OnSize(UINT nType, int cx, int cy)
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
		else
		{
			if(m_bCtrlCreated)
			{
				SetCtrlPos();
				Invalidate();
			}
		}
	}

	CDialogEx::OnSize(nType, cx, cy);
}

void CPs96IvdDlg::OnSizing(UINT nSide,LPRECT pRect)
{
	CDialogEx::OnSizing(nSide,pRect);
}

void CPs96IvdDlg::OnShowWindow(BOOL bShow,UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow,nStatus);

	return;
}

void CPs96IvdDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(1 != m_dSizeRatio && point.y < m_iCapH)
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	}
	else
	{
		CDialogEx::OnLButtonDown(nFlags, point);
	}

	return;
}

void CPs96IvdDlg::OnLButtonDblClk(UINT nFlags,CPoint point)
{
	if(point.y < m_iCapH)
	{
		OnBnClickedBtZoom();
	}
	
	return;
}

void CPs96IvdDlg::OnBnClickedBtMinimum()
{
	ShowWindow(SW_SHOWMINIMIZED);
	return;
}

void CPs96IvdDlg::OnBnClickedBtZoom()
{
	return;

	if(1.0 == m_dSizeRatio)
	{
		m_dSizeRatio = 0.8;
	}
	else
	{
		m_dSizeRatio = 1.0;
	}

	int iW = theApp.m_rcWorkArea.Width() * m_dSizeRatio,iH = theApp.m_rcWorkArea.Height() * m_dSizeRatio;
	CRect rcNew;
	rcNew.left = (theApp.m_rcWorkArea.Width() - iW) / 2;
	rcNew.right = rcNew.left + iW;
	rcNew.top = (theApp.m_rcWorkArea.Height() - iH) / 2;
	rcNew.bottom = rcNew.top + iH;

	//更新缩放参数
	m_dHoriRatio = double(iW) / m_uiGeneDesWidth;
	m_dVertRatio = double(iH) / m_uiGeneDesHeight;
	m_dTxRatio = m_dHoriRatio * 0.5 + m_dVertRatio * 0.5;

	DlgSizeChanged();

	m_bZooming = TRUE;
	SetWindowPos(this,rcNew.left,rcNew.top,rcNew.Width(),rcNew.Height(),SWP_NOZORDER);
	m_bZooming = FALSE;

	return;
}

void CPs96IvdDlg::OnBnClickedBtClose()
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

void CPs96IvdDlg::InitSonDlg()
{
	InitWorkDlg();
}

void CPs96IvdDlg::InitWorkDlg()
{
	m_workDlg.Create(IDD_MIDDLEDLG,this);
}

void CPs96IvdDlg::InitDlg()
{
	SetTextClr1(RGB(0XFF,0XFF,0XFF));
	SetMyFont(m_stTxFont,18 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
}

void CPs96IvdDlg::CreateCtrl()
{
	RECT rect;
	rect.left = 0;
	rect.right = 0;
	rect.top = 0;
	rect.bottom = 0;

	m_btMinimum.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,rect,this,ID_PS96IVDDLG_BT_MINIMUM);
	m_btZoom.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,rect,this,ID_PS96IVDDLG_BT_ZOOM);
	m_btClose.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,rect,this,ID_PS96IVDDLG_BT_CLOSE);
}

void CPs96IvdDlg::InitCtrl()
{
	int iFontH = 16 * m_dTxRatio;

	Color clr1 = theApp.m_gclrSoftTheme,clr2 = Color(10,88,197),clr3 = Color(0,39,95),clr4 = Color(107,137,182);

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
}

void CPs96IvdDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	int iX = 0,iY = 0,iWidth = 0,iHeight = 0;
	
	m_rcManage.ClearRectToDelete();

	int iTempInt1 = 24 * m_dHoriRatio,iTempInt2 = 24 * m_dVertRatio,iTempInt3 = 8 * m_dVertRatio,iTempInt4 = 10 * m_dHoriRatio;

	iX = rect.Width() - 14 * m_dHoriRatio - iTempInt1;
	m_btClose.SetWindowPos(this,iX,iTempInt3,iTempInt1,iTempInt2,SWP_NOZORDER);

	iX -= (iTempInt4 + iTempInt1);
	m_btZoom.SetWindowPos(this,iX,iTempInt3,iTempInt1,iTempInt2,SWP_NOZORDER);

	iX -= (iTempInt4 + iTempInt1);
	m_btMinimum.SetWindowPos(this,iX,iTempInt3,iTempInt1,iTempInt2,SWP_NOZORDER);

	iY = m_iCapH + 20;
	m_workDlg.SetWindowPos(this,m_iFrameW,iY,rect.Width() - m_iFrameW * 2,rect.Height() - iY - m_iFrameW * 2,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(m_iFrameW,iY,rect.Width() - m_iFrameW * 2,rect.Height() - iY - m_iFrameW * 2);

	m_rcManage.CalcRectResult();

	return;
}

void CPs96IvdDlg::ShowCtrl()
{
	m_workDlg.ShowWindow(SW_SHOW);
	m_btMinimum.ShowWindow(SW_SHOW);
	m_btZoom.ShowWindow(SW_SHOW);
	m_btClose.ShowWindow(SW_SHOW);

	return;
}

int CPs96IvdDlg::DlgSizeChanged()
{
	m_iCapH = 40 * m_dVertRatio;
	m_iFrameW = 4 * m_dTxRatio;

	//m_runCtrlDlg.DlgSizeChanged();

	return 0;
}


void CPs96IvdDlg::OnTimer(UINT_PTR nIDEvent)
{
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

		if (bHasDevice)
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
					m_workDlg.SetInstrumentConnectState(pInstance->GetModel(), pInstance->GetSN(), TRUE, TRUE);
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


void CPs96IvdDlg::OnDestroy()
{
	__super::OnDestroy();

	KillTimer(m_nRunTimer);
}
