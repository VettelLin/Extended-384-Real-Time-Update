// SkipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FuncBtDlg.h"
#include "GeneralPcr.h"
#include "MiddleDlg.h"
#include "GeneralPcrDlg.h"

CGeneralPcrDlg* CFuncBtDlg::m_pMainWnd = NULL;

// CFuncBtDlg dialog
IMPLEMENT_DYNAMIC(CFuncBtDlg, CDialogEx)

CFuncBtDlg::CFuncBtDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFuncBtDlg::IDD, pParent)
{
	m_bCtrlCreated = FALSE;
	m_iChanBtW = 36/*28*/;
	m_ichanBtH = 34/*28*/;
	m_iChanBtInterval = 8;
	m_bChanShow = FALSE;
}

CFuncBtDlg::~CFuncBtDlg()
{
	Node<CMyCheckButton*>* pNode = m_lnkChanBtPtr.GetHead();
	while(NULL != pNode)
	{
		if(NULL != pNode->data)
		{
			delete pNode->data;
			pNode->data = NULL;
		}

		pNode = pNode->pNext;
	}
}

void CFuncBtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFuncBtDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_BN_CLICKED(ID_BOTTOMFUNCDLG_BT_DELETE,&CFuncBtDlg::OnBnClickedDelete) 
	ON_BN_CLICKED(ID_BOTTOMFUNCDLG_BT_SAVEAS,&CFuncBtDlg::OnBnClickedSaveAs)	
	ON_BN_CLICKED(ID_BOTTOMFUNCDLG_BT_NEW,&CFuncBtDlg::OnBnClickedNew)	
	ON_BN_CLICKED(ID_BOTTOMFUNCDLG_BT_OPEN,&CFuncBtDlg::OnBnClickedOpen)	
	ON_BN_CLICKED(ID_BOTTOMFUNCDLG_BT_SAVE,&CFuncBtDlg::OnBnClickedSave)	
	ON_BN_CLICKED(ID_BOTTOMFUNCDLG_BT_ANALYSE,&CFuncBtDlg::OnBnClickedAnalyse)	
	ON_BN_CLICKED(ID_BOTTOMFUNCDLG_BT_EXPORT,&CFuncBtDlg::OnBnClickedExport)	
	ON_BN_CLICKED(ID_BOTTOMFUNCDLG_BT_IMPORT,&CFuncBtDlg::OnBnClickedImport)
	ON_BN_CLICKED(ID_BOTTOMFUNCDLG_BT_ANAPARAMSET,&CFuncBtDlg::OnBnClickedAnaParamSet)	
	ON_BN_CLICKED(ID_BOTTOMFUNCDLG_BT_LOADORIGINALDATA,&CFuncBtDlg::OnBnClickedLoadOriginalAmpData)
	ON_BN_CLICKED(ID_BOTTOMFUNCDLG_BT_LISFUNC,OnBnClickedLisFunc)

	ON_COMMAND(ID_ANAPARAMMENU_SETCROSS,OnAnaParamMenuSetCross)
	ON_COMMAND(ID_ANAPARAMMENU_SETFILTER,OnAnaParamMenuSetFilter)
	ON_COMMAND(ID_ANAPARAMMENU_SETBASEST,OnSetBaseSet)
	ON_COMMAND(ID_ANAPARAMMENU_SETMOTCHANNELADJUST,OnSetMotChannelAdjust)
	ON_COMMAND(ID_ANAPARAMMENU_SETMELTANAPARAM,OnSetMeltAnaParam)

	ON_COMMAND(ID_EXPORTMENU_EXPERIMENTDATA,OnExportExperimentData)
	ON_COMMAND(ID_EXPORTMENU_CURVEPICTURE,OnExportCurvePicture)
	ON_COMMAND(ID_EXPORTMENU_TEMPERATUREDATA,OnExportTemperatureData)
	ON_COMMAND(ID_SAVEASMENU_EXPERIMENTFILE,OnSaveAsExperimentFile)
	ON_COMMAND(ID_SAVEASMENU_TEMPLATEFILE,OnSaveAsTemplateFile)

	ON_COMMAND_RANGE(ID_CHANNELBT_START,ID_CHANNELBT_START + 100,OnChanBtClicked)
END_MESSAGE_MAP()

BOOL CFuncBtDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect; 
	GetClientRect(&rect);

	CDC memDc;
	memDc.CreateCompatibleDC(pDC);
	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	memDc.FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(0XFF,0XFF,0XFF));
	
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

void CFuncBtDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}
	return;
}

int CFuncBtDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int iRet = CDialogEx::OnCreate(lpCreateStruct);

	if(CFontSet::m_dHoriRatio < CFontSet::m_dVertRatio)	//通道切换按钮，高度优先
	{
		m_iChanBtW *= CFontSet::m_dVertRatio;
	}
	else
	{
		m_iChanBtW *= CFontSet::m_dHoriRatio;
	}

	m_ichanBtH *= CFontSet::m_dVertRatio;
	m_iChanBtInterval *= CFontSet::m_dHoriRatio;

	return iRet;
}

BOOL CFuncBtDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CreateCtrl();
	m_bCtrlCreated = TRUE;
	InitCtrl();
	SetCtrlPos();

	SetCtrlText();

	ShowCtrl();

	return TRUE;
}

void CFuncBtDlg::CreateCtrl()
{
	CreateBt();
}

void CFuncBtDlg::CreateBt()
{
	m_btClearList.Create(_T("全部删除"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_CLEARLIST);
	m_btNew.Create(_T("新建"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_NEW);
	m_btOpen.Create(_T("打开"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_OPEN);
	m_btSave.Create(_T("保存"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_SAVE);
	m_btSaveAs.Create(_T("另存为"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_SAVEAS);
	m_btDelete.Create(_T("删除"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_DELETE);
	m_btAnalyse.Create(_T("分析"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_ANALYSE);
	m_btExport.Create(_T("导出"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_EXPORT);
	m_btImport.Create(_T("导入"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_IMPORT);
	m_btAnaParamSet.Create(_T("分析参数"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_ANAPARAMSET);
	m_btLis.Create(_T("LIS传输"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_LISFUNC);

	m_btReadExeclData.Create(_T("加载扩增数据"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_LOADORIGINALDATA);

	m_btPrint.Create(_T("打印"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_PRINT);
	m_ckbtVerify.Create(_T("审核"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_CANCELVERIFY);
	m_btSift.Create(_T("筛选"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_SIFT);
	m_btSetBadHole.Create(_T("标记为坏孔"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_SETBADHOLE);
	m_btUpLoadLisData.Create(_T("手工传输"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_UPLOADLISDATA);
	m_btReadEeprom.Create(_T("读取EEPROM"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_READEEPROM);
	m_btWriteEeprom.Create(_T("写入EEPROM"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_WRITEEEPROM);

	return;
}

void CFuncBtDlg::InitCtrl()
{
	InitBt();
}

void CFuncBtDlg::InitBt()
{
	int iFontH = 16 * m_dTxRatio;

	int iTempInt1 = 16 * m_dHoriRatio;
	int iTempInt2 = 10 * m_dHoriRatio;

	m_btNew.SetPngBkPic(_T(".\\BkPic\\btnew.png"),2,iTempInt1,0,iTempInt2);
	m_btNew.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btNew.SetWordStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	m_btOpen.SetPngBkPic(_T(".\\BkPic\\btopen.png"),2,iTempInt1,0,iTempInt2);
	m_btOpen.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btOpen.SetWordStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	
	m_btSave.SetPngBkPic(_T(".\\BkPic\\btsave.png"),2,iTempInt1,0,iTempInt2);
	m_btSave.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btSave.SetWordStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	
	m_btSaveAs.SetPngBkPic(_T(".\\BkPic\\btsaveas.png"),2,iTempInt1,0,iTempInt2);
	m_btSaveAs.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btSaveAs.SetWordStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	if(!theApp.GetBeIVDUser())
	{
		m_btSaveAs.SetBeHaveTxPushedFormat(FALSE);
	}

	m_btAnalyse.SetPngBkPic(_T(".\\BkPic\\btanalyse.png"),2,iTempInt1,0,iTempInt2);
	m_btAnalyse.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btAnalyse.SetWordStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	
	m_btExport.SetPngBkPic(_T(".\\BkPic\\btexport.png"),2,20 * m_dHoriRatio,0,6 * m_dHoriRatio);
	m_btExport.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btExport.SetWordStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	m_btExport.SetBeHaveTxPushedFormat(FALSE);

	m_btImport.SetPngBkPic(_T(".\\BkPic\\import.png"),2,16 * m_dHoriRatio,0,6 * m_dHoriRatio);
	m_btImport.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btImport.SetWordStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	m_btDelete.SetPngBkPic(_T(".\\BkPic\\btdelete.png"),2,iTempInt1,0,iTempInt2);
	m_btDelete.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btDelete.SetWordStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	m_btAnaParamSet.SetPngBkPic(_T(".\\BkPic\\AnalyseParam.png"),2,16 * m_dHoriRatio,0,6 * m_dHoriRatio);
	m_btAnaParamSet.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btAnaParamSet.SetWordStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	m_btAnaParamSet.SetBeHaveTxPushedFormat(FALSE);

	m_btLis.SetPngBkPic(_T(".\\BkPic\\btnew.png"),2,iTempInt1,0,iTempInt2);
	m_btLis.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btLis.SetWordStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	m_btReadExeclData.SetPngBkPic(_T(".\\BkPic\\AnalyseParam.png"),2,16 * m_dHoriRatio,0,6 * m_dHoriRatio);
	m_btReadExeclData.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btReadExeclData.SetWordStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	m_btReadExeclData.SetBeHaveTxPushedFormat(FALSE);

	return;
}

void CFuncBtDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	int iTempInt1 = 24 * m_dVertRatio;
	int iTempInt2 = 7 * m_dVertRatio;
	int iTempInt3 = 72 * m_dHoriRatio;
	int iWidth = 0,iStartX = 0,iTempInt4 = 90 * m_dHoriRatio,iHoriInterval = 36 * m_dHoriRatio;

	if(!theApp.GetBeIVDUser())
	{
		m_btNew.SetWindowPos(this,18 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		m_btOpen.SetWindowPos(this,114 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		//m_btSave.SetWindowPos(this,210 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		iWidth = iTempInt4;
		//iStartX = 306 * m_dHoriRatio;
		iStartX = 210 * m_dHoriRatio;
		m_btSaveAs.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = 190 * m_dHoriRatio;
		m_btReadExeclData.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt4;
		m_btDelete.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt4;
		m_btAnalyse.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt4;
		m_btExport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt4;
		m_btImport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = 190 * m_dHoriRatio;
		m_btAnaParamSet.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
	}
	else
	{
#ifdef IVD_SHOW_NEWBT
		m_btNew.SetWindowPos(this,18 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		m_btOpen.SetWindowPos(this,114 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		//m_btSave.SetWindowPos(this,210 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		iWidth = iTempInt4;
		iStartX = 210 * m_dHoriRatio;
		//iStartX = 306 * m_dHoriRatio;
		m_btSaveAs.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = 190 * m_dHoriRatio;
		m_btReadExeclData.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt4;
		m_btDelete.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt4;
		m_btAnalyse.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt4;
		m_btExport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt4;
		m_btImport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = 190 * m_dHoriRatio;
		m_btAnaParamSet.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
#else
		m_btOpen.SetWindowPos(this,18 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		m_btSave.SetWindowPos(this,114 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		iWidth = iTempInt4;
		iStartX = 210 * m_dHoriRatio;
		m_btSaveAs.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt4;
		m_btDelete.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt4;
		m_btAnalyse.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt4;
		m_btExport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt4;
		m_btImport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
#endif
	}

	SetChanBtPos();
	ResetRectManage();
}

void CFuncBtDlg::SetCtrlPosModeExpProcSet()
{
	int iTempInt1 = 24 * m_dVertRatio;
	int iTempInt2 = 7 * m_dVertRatio;
	int iTempInt3 = 72 * m_dHoriRatio;
	int iWidth = 0,iStartX = 0,iTempInt4 = 90 * m_dHoriRatio,iTempInt5 = 82 * m_dHoriRatio,iHoriInterval = 36 * m_dHoriRatio;

	if(!theApp.GetBeIVDUser())
	{
		m_btNew.SetWindowPos(this,18 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		m_btOpen.SetWindowPos(this,114 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		//m_btSave.SetWindowPos(this,210 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		iWidth = iTempInt5;
		iStartX = 210 * m_dHoriRatio;
		//iStartX = 306 * m_dHoriRatio;
		m_btSaveAs.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btDelete.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btExport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btImport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = 190 * m_dHoriRatio;
		m_btReadExeclData.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
	}
	else
	{
#ifdef IVD_SHOW_NEWBT
		m_btNew.SetWindowPos(this,18 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		m_btOpen.SetWindowPos(this,114 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		//m_btSave.SetWindowPos(this,210 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		iWidth = iTempInt5;
		iStartX = 210 * m_dHoriRatio;
		//iStartX = 306 * m_dHoriRatio;
		m_btSaveAs.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btDelete.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btExport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btImport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = 190 * m_dHoriRatio;
		m_btReadExeclData.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
#else
		m_btOpen.SetWindowPos(this,18 * dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		m_btSave.SetWindowPos(this,114 * dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		iWidth = iTempInt5;
		iStartX = 210 * m_dHoriRatio;
		m_btSaveAs.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btDelete.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btExport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btImport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
#endif
	}	
	return;
}

void CFuncBtDlg::SetCtrlPosModeExpSamSet()
{
	int iTempInt1 = 24 * m_dVertRatio;
	int iTempInt2 = 7 * m_dVertRatio;
	int iTempInt3 = 72 * m_dHoriRatio;
	int iWidth = 0,iStartX = 0,iTempInt4 = 90 * m_dHoriRatio,iTempInt5 = 82 * m_dHoriRatio,iHoriInterval = 36 * m_dHoriRatio;

	if(!theApp.GetBeIVDUser())
	{
		m_btNew.SetWindowPos(this,18 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		m_btOpen.SetWindowPos(this,114 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		//m_btSave.SetWindowPos(this,210 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		iWidth = iTempInt5;
		iStartX = 210 * m_dHoriRatio;
		//iStartX = 306 * m_dHoriRatio;
		m_btSaveAs.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btDelete.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btExport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btImport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = 190 * m_dHoriRatio;
		m_btReadExeclData.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
	}
	else
	{
#ifdef IVD_SHOW_NEWBT
		m_btNew.SetWindowPos(this,18 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		m_btOpen.SetWindowPos(this,114 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		//m_btSave.SetWindowPos(this,210 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		iWidth = iTempInt5;
		iStartX = 210 * m_dHoriRatio;
		//iStartX = 306 * m_dHoriRatio;
		m_btSaveAs.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btDelete.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btExport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btImport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = 190 * m_dHoriRatio;
		m_btReadExeclData.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
#else
		m_btOpen.SetWindowPos(this,18 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		m_btSave.SetWindowPos(this,114 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		iWidth = iTempInt5;
		iStartX = 210 * m_dHoriRatio;
		m_btSaveAs.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btDelete.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btExport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btImport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
#endif
	}	
}

void CFuncBtDlg::SetCtrlPosModeExpDataAnalyse()
{
	CRect rect;
	GetClientRect(&rect);

	int iTempInt1 = 24 * m_dVertRatio,iTempInt2 = 7 * m_dVertRatio,iTempInt3 = 72 * m_dHoriRatio;
	int iWidth = 0,iStartX = 0,iTempInt4 = 90 * m_dHoriRatio,iTempInt5 = 82 * m_dHoriRatio,iHoriInterval = 36 * m_dHoriRatio;
	int iFourCharW = 102;
	if(theApp.GetBeEnglish())
	{
		iFourCharW = 166;
	}

	if(!theApp.GetBeIVDUser())
	{
		m_btNew.SetWindowPos(this,18 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		m_btOpen.SetWindowPos(this,114 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		//m_btSave.SetWindowPos(this,210 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		iWidth = iTempInt5;
		iStartX = 210 * m_dHoriRatio;
		//iStartX = 306 * m_dHoriRatio;
		m_btSaveAs.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;

		if(theApp.GetBeEnglish())
		{
			iWidth = iTempInt4;
		}
		else
		{
			iWidth = iTempInt3;
		}
		m_btAnalyse.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btExport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iFourCharW;
		m_btAnaParamSet.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		m_btLis.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt4;
		m_btReadExeclData.SetWindowPos(this,iStartX,iTempInt2,190 * m_dHoriRatio,iTempInt1,SWP_NOZORDER);
	}
	else
	{
#ifdef IVD_SHOW_NEWBT
		m_btNew.SetWindowPos(this,18 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		m_btOpen.SetWindowPos(this,114 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		//m_btSave.SetWindowPos(this,210 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		iWidth = iTempInt5;
		iStartX = 210 * m_dHoriRatio;
		//iStartX = 306 * m_dHoriRatio;
		m_btSaveAs.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;

		if(theApp.GetBeEnglish())
		{
			iWidth = iTempInt4;
		}
		else
		{
			iWidth = iTempInt3;
		}
		m_btAnalyse.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btExport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iFourCharW;
		m_btAnaParamSet.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		m_btLis.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt4;
		m_btReadExeclData.SetWindowPos(this,iStartX,iTempInt2,190 * m_dHoriRatio,iTempInt1,SWP_NOZORDER);
#else
		m_btOpen.SetWindowPos(this,18 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		m_btSave.SetWindowPos(this,114 * m_dHoriRatio,iTempInt2,iTempInt3,iTempInt1,SWP_NOZORDER);
		iWidth = iTempInt5;
		iStartX = 210 * m_dHoriRatio;
		m_btSaveAs.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		if(theApp.GetBeEnglish())
		{
			iWidth = iTempInt4;
		}
		else
		{
			iWidth = iTempInt3;
		}
		m_btAnalyse.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iTempInt3;
		m_btExport.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
		iStartX += iWidth;
		iStartX += iHoriInterval;
		iWidth = iFourCharW;
		m_btLis.SetWindowPos(this,iStartX,iTempInt2,iWidth,iTempInt1,SWP_NOZORDER);
#endif
	}

	return;
}

void CFuncBtDlg::SetChanBtPos()
{
	int iChanBtNum = m_lnkChanBtPtr.GetSize();
	if(iChanBtNum < 0)
	{
		return;
	}

	CRect rect;
	GetClientRect(&rect);
	
	int iStartX = rect.right - (m_iChanBtW + m_iChanBtInterval) * iChanBtNum;
	int iStartY = (rect.Height() - m_ichanBtH) / 2;

	Node<CMyCheckButton*>* pNode = m_lnkChanBtPtr.GetHead();
	while(NULL != pNode)
	{
		if(NULL != pNode->data->GetSafeHwnd())
		{
			pNode->data->SetWindowPos(this,iStartX,iStartY,m_iChanBtW,m_ichanBtH,SWP_NOZORDER);
		}
		pNode = pNode->pNext;
		iStartX += (m_iChanBtW + m_iChanBtInterval);
	}

	return;
}

void CFuncBtDlg::ShowCtrl()
{
	if(!theApp.GetBeIVDUser())
	{
		m_btNew.ShowWindow(SW_SHOW);
		m_btAnaParamSet.ShowWindow(SW_SHOW);
#ifdef ALLOW_LOADDATA_FROMEXCELFILE
		m_btReadExeclData.ShowWindow(SW_SHOW);
#endif
	}
	
	m_btOpen.ShowWindow(SW_SHOW);
	//m_btSave.ShowWindow(SW_SHOW);
	m_btSaveAs.ShowWindow(SW_SHOW);
	m_btAnalyse.ShowWindow(SW_SHOW);
	m_btDelete.ShowWindow(SW_SHOW);
	m_btExport.ShowWindow(SW_SHOW);
	m_btImport.ShowWindow(SW_SHOW);

	SetChanBtShowState(TRUE);
}

void CFuncBtDlg::SetChanBtShowState(BOOL bShow)
{
	int iChanBtNum = m_lnkChanBtPtr.GetSize();
	if(iChanBtNum < 1)
	{
		return;
	}

	if(m_bChanShow == bShow)
	{
		return;
	}
	m_bChanShow = bShow;

	int nCmdShow = SW_HIDE;
	if(m_bChanShow)
	{
		nCmdShow = SW_SHOW;
	}

	Node<CMyCheckButton*>* pNode = m_lnkChanBtPtr.GetHead();
	while(NULL != pNode)
	{
		pNode->data->ShowWindow(nCmdShow);
		pNode = pNode->pNext;
	}

	ResetRectManage();
	return;
}

void CFuncBtDlg::SetChannelBtNum(int iChanBtNum,vector<CString>& vecChanDyeName,BOOL bShowChanBt /*= TRUE*/)
{
	Node<CMyCheckButton*>* pNode = m_lnkChanBtPtr.GetHead();
	while(NULL != pNode)
	{
		if(NULL != pNode->data)
		{
			delete pNode->data;
			pNode->data = NULL;
		}

		pNode = pNode->pNext;
	}
	m_lnkChanBtPtr.Clear();

	m_bChanShow = FALSE;

	int iFontH = 14 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 15 * m_dTxRatio;
	}

	CString strTemp(_T(""));
	for(int i = 0;i < iChanBtNum;++i)
	{
		CMyCheckButton* pChanPt = new CMyCheckButton();
		if(NULL == pChanPt)
		{
			//异常状态记录
			return;
		}

		//创建按钮
		if(i < vecChanDyeName.size() && !vecChanDyeName[i].IsEmpty())
		{
			strTemp = vecChanDyeName[i];
		}
		else
		{
			if (i < theApp.m_vecDefDyeName.size() && !theApp.m_vecDefDyeName[i].IsEmpty())
				strTemp = theApp.m_vecDefDyeName[i];
			else
				strTemp.Format(_T("%d"),i + 1);
		}
		
		pChanPt->Create(strTemp,WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_CHANNELBT_START + i);

		//初始化按钮
		pChanPt->SetRoundStyle(TRUE,RGB(0XFF,0XFF,0XFF));
		pChanPt->SetWordStyle(DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pChanPt->SetBkColor(RGB(0XF1,0XF4,0XF9),CLR_SOFT_THEME);
		pChanPt->SetTextColor(CLR_2LEVEL_THEME,RGB(0XFF,0XFF,0XFF));
		pChanPt->SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
		pChanPt->SetBeParentProcessMouseClick(TRUE);

		m_lnkChanBtPtr.AddTail(pChanPt);
	}

	//设置按钮位置
	SetChanBtPos();

	//设置按钮显示状态
	if(bShowChanBt)
	{
		SetChanBtShowState(TRUE);
	}

	return;
}

int CFuncBtDlg::SetChanBtSelectState(vector<int> vecSelState)
{
	int iVecSize = vecSelState.size();

	if(iVecSize < 1)
	{
		return -1;
	}

	if(iVecSize < m_lnkChanBtPtr.GetSize())
	{
		return -2;
	}

	int iRealNum = iVecSize;
	if(iVecSize > m_lnkChanBtPtr.GetSize())
	{
		iRealNum = m_lnkChanBtPtr.GetSize();
	}

	for(int i = 0;i < iRealNum;++i)
	{
		(*m_lnkChanBtPtr[i])->SetBtChecked(1 == vecSelState[i],TRUE);
	}

	return 0;
}

int CFuncBtDlg::SetOneChannelBtText(int iChanId,CString strText)
{
	if(iChanId < 0)
	{
		return -1;
	}

	int iChanBtNum = m_lnkChanBtPtr.GetSize();
	if(iChanId >= iChanBtNum)
	{
		return -2;
	}

	(*m_lnkChanBtPtr[iChanId])->SetWindowText(strText);
	return 0;
}

void CFuncBtDlg::SetCtrlText()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	
	m_btNew.SetWindowText(pLangInfo->GetText(167));
	m_btOpen.SetWindowText(pLangInfo->GetText(168));
	m_btSave.SetWindowText(pLangInfo->GetText(169));
	m_btSaveAs.SetWindowText(pLangInfo->GetText(170));
	m_btAnalyse.SetWindowText(pLangInfo->GetText(171));
	m_btExport.SetWindowText(pLangInfo->GetText(174));
	m_btImport.SetWindowText(pLangInfo->GetText(173));
	m_btDelete.SetWindowText(pLangInfo->GetText(172));
	m_btAnaParamSet.SetWindowText(pLangInfo->GetText(175));
	m_btReadExeclData.SetWindowText(pLangInfo->GetText(290));
	m_btLis.SetWindowText(pLangInfo->GetText(369));

	return;
}

void CFuncBtDlg::ResetRectManage()
{
	m_rcManage.ClearRectToDelete();

	int iChanBtNum = m_lnkChanBtPtr.GetSize();
	if(iChanBtNum < 0)
	{
		Invalidate();
		return;
	}

	CRect rect,rcTemp;
	GetClientRect(&rect);
	m_rcManage.m_rcInit = rect;

	Node<CMyCheckButton*>* pNode = m_lnkChanBtPtr.GetHead();
	while(NULL != pNode)
	{
		if(NULL != pNode->data->GetSafeHwnd() && pNode->data->IsWindowVisible())
		{
			pNode->data->GetWindowRect(&rcTemp);
			ScreenToClient(&rcTemp);
			m_rcManage.AddRectToDelete(rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height());
		}
		pNode = pNode->pNext;
	}
	m_rcManage.CalcRectResult();

	int iStartX = rect.right - (m_iChanBtW + m_iChanBtInterval) * iChanBtNum;
	rcTemp.left = iStartX;
	rcTemp.top = (rect.Height() - m_ichanBtH) / 2;
	rcTemp.right = rect.right;
	rcTemp.bottom = rect.bottom - rcTemp.top;
	InvalidateRect(&rcTemp);

	return;
}

void CFuncBtDlg::OnBnClickedDelete()
{
	m_pMainWnd->Delete();
}

void CFuncBtDlg::OnBnClickedSaveAs()
{
	if(theApp.GetBeIVDUser())
	{
		((CMiddleDlg*)GetParent())->SaveAsFile(FALSE);
	}
	else
	{
		CString str1(_T("实验文件")),str2(_T("模板文件"));
		if(NULL != theApp.m_pLangInfo)
		{
			str1 = theApp.m_pLangInfo->GetText(200);
			str2 = theApp.m_pLangInfo->GetText(201);
		}

		CMenu menu1;
		menu1.CreatePopupMenu();
		menu1.AppendMenu(MF_STRING,ID_SAVEASMENU_EXPERIMENTFILE,str1);
		menu1.AppendMenu(MF_STRING,ID_SAVEASMENU_TEMPLATEFILE,str2);
		CPoint pt;
		GetCursorPos(&pt);
		CRect rcBt;
		m_btSaveAs.GetWindowRect(&rcBt);

		menu1.TrackPopupMenu(TPM_RIGHTBUTTON,rcBt.left,rcBt.bottom,this);
		menu1.DestroyMenu();
	}
	
	return;
}

void CFuncBtDlg::OnBnClickedNew()
{
	m_pMainWnd->New();
	return;
}

void CFuncBtDlg::OnBnClickedOpen()
{
	m_pMainWnd->Open();
	return;
}

void CFuncBtDlg::OnBnClickedSave()
{
	((CMiddleDlg*)GetParent())->Save();
}

void CFuncBtDlg::OnBnClickedAnalyse()
{
	((CMiddleDlg*)GetParent())->Analyse();
}

void CFuncBtDlg::OnBnClickedExport()
{
	if(((CMiddleDlg*)GetParent())->GetBeDataAnaDlgVisible())
	{
		CString str1(_T("实验数据")),str2(_T("曲线(图片)"));
		str1 = theApp.m_pLangInfo->GetText(202);
		str2 = theApp.m_pLangInfo->GetText(203);
		CString str3 = theApp.m_pLangInfo->GetText(491);

		CMenu menu1;
		menu1.CreatePopupMenu();
		menu1.AppendMenu(MF_STRING,ID_EXPORTMENU_EXPERIMENTDATA,str1);
		menu1.AppendMenu(MF_STRING,ID_EXPORTMENU_CURVEPICTURE,str2);

		BOOL bDeveloper = theApp.m_curUserInfo.GetName() == DEFAULT_DEVELOPER;
		if (bDeveloper)
		{
			menu1.AppendMenu(MF_STRING,ID_EXPORTMENU_TEMPERATUREDATA,str3);
		}

#ifdef CAN_YOUNG
		menu1.AppendMenu(MF_STRING,ID_EXPORTMENU_RESULTREPORT,theApp.m_pLangInfo->GetText(397));
#endif

		CPoint pt;
		GetCursorPos(&pt);
		CRect rcBt;
		m_btExport.GetWindowRect(&rcBt);

		menu1.TrackPopupMenu(TPM_RIGHTBUTTON,rcBt.left,rcBt.bottom,this);
		menu1.DestroyMenu();
	}
	else
	{
		((CMiddleDlg*)GetParent())->Export();
	}

	return;
}

// 导出实验数据
void CFuncBtDlg::OnExportExperimentData()
{
	((CMiddleDlg*)GetParent())->Export();
}

void CFuncBtDlg::OnExportCurvePicture()
{
	((CMiddleDlg*)GetParent())->ExportPicture();
}

// 导出实验数据
void CFuncBtDlg::OnExportTemperatureData()
{
	((CMiddleDlg*)GetParent())->ExportTemperature();
}

void CFuncBtDlg::OnSaveAsExperimentFile()
{
	((CMiddleDlg*)GetParent())->SaveAsFile(FALSE);
	return;
}

void CFuncBtDlg::OnSaveAsTemplateFile()
{
	((CMiddleDlg*)GetParent())->SaveAsFile(TRUE);
	return;
}

void CFuncBtDlg::OnBnClickedImport()
{
	((CMiddleDlg*)GetParent())->Import();
	return;
}

void CFuncBtDlg::OnBnClickedLoadOriginalAmpData()
{
	m_pMainWnd->m_workDlg.OpenOriginalData();
	return;
}


void CFuncBtDlg::OnBnClickedLisFunc()
{
	if(NULL != ((CMiddleDlg*)GetParent())->GetSafeHwnd())
	{
		((CMiddleDlg*)GetParent())->UploadInspectResultToLis();
	}
	return;
}

void CFuncBtDlg::OnBnClickedAnaParamSet()
{
	CString str1(_T("基本参数")),str2(_T("串扰系数")),str3(_T("滤波参数")),str4(_T("浊度参数")),str5(_T("熔曲分析参数"));
	str1 = theApp.m_pLangInfo->GetText(100);
	str2 = theApp.m_pLangInfo->GetText(178);
	str3 = theApp.m_pLangInfo->GetText(177);
	str4 = theApp.m_pLangInfo->GetText(176);
	str5 = theApp.m_pLangInfo->GetText(311);

	CMenu menu1;
	menu1.CreatePopupMenu();
	menu1.AppendMenu(MF_STRING,ID_ANAPARAMMENU_SETBASEST,str1);
	menu1.AppendMenu(MF_STRING,ID_ANAPARAMMENU_SETCROSS,str2);
	menu1.AppendMenu(MF_STRING,ID_ANAPARAMMENU_SETFILTER,str3);


#ifdef PS96INSTRUMENT
	menu1.AppendMenu(MF_STRING,ID_ANAPARAMMENU_SETMOTCHANNELADJUST,str4);
#else
	menu1.AppendMenu(MF_STRING,ID_ANAPARAMMENU_SETMELTANAPARAM,str5);

	if(NULL != m_pMainWnd->GetSafeHwnd() && NULL != m_pMainWnd->m_workDlg.GetSafeHwnd() && NULL != m_pMainWnd->m_workDlg.m_pCurSelPageDlg->GetSafeHwnd())
	{
		CExperimentData* pExpData = m_pMainWnd->m_workDlg.m_pCurSelPageDlg->m_pExpData;
		if(NULL != pExpData && pExpData->GetBePS96ExperFile())
		{
			menu1.AppendMenu(MF_STRING,ID_ANAPARAMMENU_SETMOTCHANNELADJUST,str4);
		}
	}
#endif
	
	CPoint pt;
	GetCursorPos(&pt);
	CRect rcBt;
	m_btAnaParamSet.GetWindowRect(&rcBt);

	menu1.TrackPopupMenu(TPM_RIGHTBUTTON,rcBt.left,rcBt.bottom,this);
	menu1.DestroyMenu();

	return;
}

void CFuncBtDlg::OnAnaParamMenuSetCross()
{
	((CMiddleDlg*)GetParent())->SetCrossParam();
}

void CFuncBtDlg::OnAnaParamMenuSetFilter()
{
	((CMiddleDlg*)GetParent())->SetFilterParam();
}

void CFuncBtDlg::OnSetBaseSet()
{
	((CMiddleDlg*)GetParent())->SetBaseSet();
}

void CFuncBtDlg::OnSetMotChannelAdjust()
{
	((CMiddleDlg*)GetParent())->SetMotChannelAdjust();
}

void CFuncBtDlg::OnSetMeltAnaParam()
{
	((CMiddleDlg*)GetParent())->SetMeltAnalyseParam();
}

void CFuncBtDlg::OnChanBtClicked(UINT nID)
{
	if(nID < ID_CHANNELBT_START || nID > ID_CHANNELBT_START + 100)
	{
		return;
	}
	int iChanIndex = nID - ID_CHANNELBT_START;
	CMyCheckButton* pChanBt = *m_lnkChanBtPtr[iChanIndex];
	pChanBt->SetBtChecked(!pChanBt->GetCheckState(),TRUE);

	//通知父窗口，通道选择状态改变
	CWnd* pParent = GetParent();
	if(NULL != GetSafeHwnd())
	{
		pParent->PostMessage(WM_CHANBTCLICKED,iChanIndex,pChanBt->GetCheckState());
	}
	return;
}

void CFuncBtDlg::SetModeExpBaseSet()
{
	SetCtrlPos();

	if(!theApp.GetBeIVDUser())
	{
		m_btNew.ShowWindow(SW_SHOW);

#ifdef ALLOW_LOADDATA_FROMEXCELFILE
		m_btReadExeclData.ShowWindow(SW_SHOW);
#endif
	}
	else
	{
#ifdef IVD_SHOW_NEWBT
		m_btNew.ShowWindow(SW_SHOW);
#else
		m_btNew.ShowWindow(SW_HIDE);
#endif
	}

	m_btOpen.ShowWindow(SW_SHOW);

#ifdef BASESETPAGE_HIDE_SAVEBT
	//m_btSave.ShowWindow(SW_HIDE);
	m_btSaveAs.ShowWindow(SW_HIDE);
#else
	//m_btSave.ShowWindow(SW_SHOW);
	m_btSaveAs.ShowWindow(SW_SHOW);
#endif
	
	m_btAnalyse.ShowWindow(SW_HIDE);
	m_btDelete.ShowWindow(SW_HIDE);
	m_btExport.ShowWindow(SW_HIDE);
	m_btImport.ShowWindow(SW_HIDE);
	m_btAnaParamSet.ShowWindow(SW_HIDE);
	m_btLis.ShowWindow(SW_HIDE);

	return;
}

void CFuncBtDlg::SetModeExpProcSet()
{
	SetCtrlPosModeExpProcSet();

	if(!theApp.GetBeIVDUser())
	{
		m_btNew.ShowWindow(SW_SHOW);

#ifdef ALLOW_LOADDATA_FROMEXCELFILE
		m_btReadExeclData.ShowWindow(SW_SHOW);
#endif
	}
	else
	{
#ifdef IVD_SHOW_NEWBT
		m_btNew.ShowWindow(SW_SHOW);
#else
		m_btNew.ShowWindow(SW_HIDE);
#endif
	}

	m_btOpen.ShowWindow(SW_SHOW);
	m_btSave.ShowWindow(SW_HIDE);
	m_btSaveAs.ShowWindow(SW_SHOW);
	m_btAnalyse.ShowWindow(SW_HIDE);
	m_btDelete.ShowWindow(SW_SHOW);
	m_btExport.ShowWindow(SW_SHOW);
	m_btImport.ShowWindow(SW_SHOW);
	m_btAnaParamSet.ShowWindow(SW_HIDE);
	m_btLis.ShowWindow(SW_HIDE);

	return;
}

void CFuncBtDlg::SetModeExpSamSet()
{
	SetCtrlPosModeExpSamSet();

	if(!theApp.GetBeIVDUser())
	{
		m_btNew.ShowWindow(SW_SHOW);

#ifdef ALLOW_LOADDATA_FROMEXCELFILE
		m_btReadExeclData.ShowWindow(SW_SHOW);
#endif
	}
	else
	{
#ifdef IVD_SHOW_NEWBT
		m_btNew.ShowWindow(SW_SHOW);
#else
		m_btNew.ShowWindow(SW_HIDE);
#endif
	}

	m_btOpen.ShowWindow(SW_SHOW);
	m_btSave.ShowWindow(SW_HIDE);
	m_btSaveAs.ShowWindow(SW_SHOW);
	m_btAnalyse.ShowWindow(SW_HIDE);
	m_btDelete.ShowWindow(SW_SHOW);
	m_btExport.ShowWindow(SW_SHOW);
	m_btImport.ShowWindow(SW_SHOW);
	m_btAnaParamSet.ShowWindow(SW_HIDE);
	m_btLis.ShowWindow(SW_HIDE);

	return;
}

void CFuncBtDlg::SetModeExpRunMonitor()
{
	SetCtrlPos();

	if(!theApp.GetBeIVDUser())
	{
		m_btNew.ShowWindow(SW_SHOW);

#ifdef ALLOW_LOADDATA_FROMEXCELFILE
		m_btReadExeclData.ShowWindow(SW_SHOW);
#endif
	}
	else
	{
#ifdef IVD_SHOW_NEWBT
		m_btNew.ShowWindow(SW_SHOW);
#else
		m_btNew.ShowWindow(SW_HIDE);
#endif
	}

	m_btOpen.ShowWindow(SW_SHOW);
	m_btSave.ShowWindow(SW_HIDE);
	m_btSaveAs.ShowWindow(SW_SHOW);
	m_btAnalyse.ShowWindow(SW_HIDE);
	m_btDelete.ShowWindow(SW_HIDE);
	m_btExport.ShowWindow(SW_HIDE);
	m_btImport.ShowWindow(SW_HIDE);
	m_btAnaParamSet.ShowWindow(SW_HIDE);
	m_btLis.ShowWindow(SW_HIDE);

	return;
}

void CFuncBtDlg::SetModeExpDataAnalyse()
{
	SetCtrlPosModeExpDataAnalyse();

	if(!theApp.GetBeIVDUser())
	{
		m_btNew.ShowWindow(SW_SHOW);

#ifdef ALLOW_LOADDATA_FROMEXCELFILE
		m_btReadExeclData.ShowWindow(SW_SHOW);
#endif
	}
	else
	{
#ifdef IVD_SHOW_NEWBT
		m_btNew.ShowWindow(SW_SHOW);
#else
		m_btNew.ShowWindow(SW_HIDE);
#endif
	}

	m_btLis.ShowWindow(SW_SHOW);
	m_btOpen.ShowWindow(SW_SHOW);
	m_btSave.ShowWindow(SW_HIDE);
	m_btSaveAs.ShowWindow(SW_SHOW);
	m_btAnalyse.ShowWindow(SW_SHOW);
	m_btExport.ShowWindow(SW_SHOW);
	m_btAnaParamSet.ShowWindow(SW_SHOW);
	m_btDelete.ShowWindow(SW_HIDE);
	m_btImport.ShowWindow(SW_HIDE);

	return;
}

BOOL CFuncBtDlg::PreTranslateMessage(MSG* pMsg)
{
	if(WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:	return TRUE;
		case VK_ESCAPE:	return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

int CFuncBtDlg::RefreshLanguageDisplay()
{
	int iFontH = 16 * m_dTxRatio;

	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	m_btNew.SetMyFont(m_stTxFont,iFontH);
	m_btNew.SetWindowText(pLangInfo->GetText(167));

	m_btOpen.SetMyFont(m_stTxFont,iFontH);
	m_btOpen.SetWindowText(pLangInfo->GetText(168));

	m_btSave.SetMyFont(m_stTxFont,iFontH);
	m_btSave.SetWindowText(pLangInfo->GetText(169));

	m_btSaveAs.SetMyFont(m_stTxFont,iFontH);
	m_btSaveAs.SetWindowText(pLangInfo->GetText(170));

	m_btAnalyse.SetMyFont(m_stTxFont,iFontH);
	m_btAnalyse.SetWindowText(pLangInfo->GetText(171));

	m_btExport.SetMyFont(m_stTxFont,iFontH);
	m_btExport.SetWindowText(pLangInfo->GetText(174));

	m_btImport.SetMyFont(m_stTxFont,iFontH);
	m_btImport.SetWindowText(pLangInfo->GetText(173));

	m_btDelete.SetMyFont(m_stTxFont,iFontH);
	m_btDelete.SetWindowText(pLangInfo->GetText(172));

	m_btAnaParamSet.SetMyFont(m_stTxFont,iFontH);
	m_btAnaParamSet.SetWindowText(pLangInfo->GetText(175));

	m_btReadExeclData.SetMyFont(m_stTxFont,iFontH);
	m_btReadExeclData.SetWindowText(pLangInfo->GetText(290));

	m_btLis.SetMyFont(m_stTxFont,iFontH);
	m_btLis.SetWindowText(pLangInfo->GetText(369));

	//刷新通道切换按钮的字体
	iFontH = 14 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 15 * m_dTxRatio;
	}
	Node<CMyCheckButton*>* pNode = m_lnkChanBtPtr.GetHead();
	while(NULL != pNode)
	{
		if(NULL != pNode->data && NULL != pNode->data->GetSafeHwnd())
		{
			pNode->data->SetMyFont(m_stTxFont,iFontH);
		}
		pNode = pNode->pNext;
	}

	return 0;
}
