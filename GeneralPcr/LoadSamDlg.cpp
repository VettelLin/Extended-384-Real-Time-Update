// SkipDlg.cpp : implementation file
//

#include "stdafx.h"

#include "LoadSamDlg.h"
#include "GeneralPcrDlg.h"
#include "GeneralPcr.h"
#include "Ps96SimpleDlg.h"

// CLoadSamDlg dialog
IMPLEMENT_DYNAMIC(CLoadSamDlg, CDialogEx)

CLoadSamDlg::CLoadSamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoadSamDlg::IDD, pParent)
{
	m_pParent = NULL;
	if(NULL != pParent)
	{
		m_pParent = (CPs96SimpleDlg*)pParent;
	}
	m_bCtrlCreated = FALSE;
	m_pExpData = NULL;
}

CLoadSamDlg::~CLoadSamDlg()
{

}

void CLoadSamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLoadSamDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_LOADSAMDLG_BT_LOAD,OnBnClickedLoadId)
END_MESSAGE_MAP()

BOOL CLoadSamDlg::OnEraseBkgnd(CDC* pDC)
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	
	CRect rect;
	GetClientRect(&rect);

	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	memDc.FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),CLR_SOFT_LIGHT);
	DrawRoundRectangleNew(&memDc,0,0,rect.Width() - 1,rect.Height() - 1,8,8,8,8,TRUE,Color(0XD8,0XD8,0XD8),1,TRUE,Color(0XFF,0XFF,0XFF),1);
	
	memDc.SetBkMode(TRANSPARENT);
	memDc.SetTextColor(m_txClr1);
	CFont* pOldFont = memDc.SelectObject(&m_fontSet);

	CRect rcLegend;
	rcLegend.left = int(16 * m_dHoriRatio);
//	rcLegend.right = rcLegend.left + 966 * m_dHoriRatio;
	rcLegend.right = rect.right - int(16 * m_dHoriRatio);
	//rcLegend.top = 611 * m_dVertRatio;
	//rcLegend.bottom = rcLegend.top + 35 * m_dVertRatio;
	rcLegend.bottom = rect.bottom;
	rcLegend.top = rcLegend.bottom - int(35 * m_dVertRatio);
	DrawLegend(&memDc,&rcLegend,m_segLgdInfo);

	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

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

void CLoadSamDlg::OnSize(UINT nType, int cx, int cy)
{
	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}

	__super::OnSize(nType, cx, cy);
}	

void CLoadSamDlg::OnBnClickedLoadId()
{
	ImportId();
	return;
}

BOOL CLoadSamDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();
	InitSegLegend();
	CreateCtrl();
	InitDlg();
	InitCtrl();
	m_bCtrlCreated = TRUE;
	SetCtrlPos();

	SetCtrlText();

	ShowCtrl();

	return TRUE;
}

void CLoadSamDlg::InitSegLegend()
{
	CSingleLgdInfo singleLgdInfo;

	singleLgdInfo.m_clrFill = Color(0X9B,0X51,0XE0);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(211);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0X21,0X96,0X53);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(212);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0XEB,0X57,0X57);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(215);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0X2F,0X80,0XED);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(214);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0XF2,0X99,0X4A);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(213);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

#ifdef CAN_YOUNG
	singleLgdInfo.m_clrFill = Color(0,0X70,0XC0);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(457);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);
#endif

	m_segLgdInfo.m_iLeftIndent = 41 * m_dHoriRatio; 
	m_segLgdInfo.m_iRoundRadius = 6 * m_dHoriRatio; 
	m_segLgdInfo.m_iInrIconToTx = 8 * m_dHoriRatio; 
	m_segLgdInfo.m_iInrTxToIcon = 20 * m_dHoriRatio; 

	int iFontH = 17 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		m_segLgdInfo.m_iOneCharW = 14 * m_dHoriRatio;
	}
	else
	{
		m_segLgdInfo.m_iOneCharW = 20 * m_dHoriRatio;
	}

	m_segLgdInfo.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_segLgdInfo.SetTextClr1(RGB(0X39,0X45,0X65));
}

void CLoadSamDlg::CreateCtrl()
{
	CreateBt();
	CreateCkbt();
	CreateSt();
	CreateEd();
	CreateLst();
	CreateSamPlateDlg();

	m_segDlg.Create(IDD_MIDDLEDLG,this);
}

void CLoadSamDlg::CreateBt()
{
	m_btLoad.Create(_T("����"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_LOADSAMDLG_BT_LOAD);
	m_swtBtSegMonitor.Create(_T("���г���"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_RUNMONITORDLG_RUNSEGMENT);
}

void CLoadSamDlg::CreateCkbt()
{
	

}

void CLoadSamDlg::CreateSt()
{
	
}

void CLoadSamDlg::CreateEd()
{
	
}

void CLoadSamDlg::CreateLst()
{
	
}

void CLoadSamDlg::CreateSamPlateDlg()
{
	m_samPlateDlg.Create(IDD_MIDDLEDLG,this);
}

void CLoadSamDlg::InitDlg()
{
	SetTextClr1(RGB(0X39,0X45,0X65));
	SetMyFont(m_stTxFont,18 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
}

void CLoadSamDlg::InitCtrl()
{
	InitBt();
	InitCkbt();
	InitSt();
	InitEd();
	InitLst();
	InitSamPlateDlg();
	InitSegment();
}

void CLoadSamDlg::InitBt()
{
	InitOneBt(&m_btLoad);
	//InitOneBt(&m_swtBtSegMonitor);

	CIconBtInfo iconBtInfo;
	iconBtInfo.m_bUseIcon = FALSE;
	iconBtInfo.m_rcIcon = Rect(18 * m_dHoriRatio,18 * m_dVertRatio,24 * m_dHoriRatio,24 * m_dVertRatio);
	iconBtInfo.m_rcText = CRect(2 * m_dHoriRatio,16 * m_dVertRatio,218 * m_dHoriRatio,43 * m_dVertRatio);
	iconBtInfo.m_bRoundRectBk = FALSE;
	iconBtInfo.m_iSonIconNum = 2;
	iconBtInfo.m_bSonIconPosOrder = FALSE;
	iconBtInfo.m_clrNormal = Color(0XFF,0XFF,0XFF);
	iconBtInfo.m_clrHover = Color(0XFF,0XFF,0XFF);
	iconBtInfo.m_clrSelected = COLOR_SOFT_THEME;
	iconBtInfo.m_clrDisabled = COLOR_SOFT_THEME;
	int iFontH = 26 * CFontSet::m_dTxRatio;

	m_swtBtSegMonitor.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_swtBtSegMonitor.SetIconBtInfo(iconBtInfo);
	m_swtBtSegMonitor.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF));
	m_swtBtSegMonitor.EnableWindow(FALSE);

	return;
}

void CLoadSamDlg::InitOneBt(CMyButton* pBt)
{
	if(NULL == pBt)
	{
		return;
	}

	int iFontH = 26 * CFontSet::m_dTxRatio;

	pBt->SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	pBt->SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);
	pBt->SetBkColor(Color(0XF1,0XF4,0XF9),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF) , Color(0XFF,0XFF,0XFF));
	pBt->SetEdgeColor(Color(0XD8,0XD8,0XD8),Color(0XD8,0XD8,0XD8),Color(0XD8,0XD8,0XD8),Color(0XD8,0XD8,0XD8));
	pBt->SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X97,0X97,0X97));

	return;
}	

void CLoadSamDlg::InitCkbt()
{
	int iFontH = 18 * m_dTxRatio;

}

void CLoadSamDlg::InitSt()
{
	
}

void CLoadSamDlg::InitEd()
{
	int iFontH = 18 * m_dTxRatio;

}

void CLoadSamDlg::InitLst()
{
	
}

void CLoadSamDlg::InitSamPlateDlg()
{
	int iFontH = 16 * m_dTxRatio,iFontTitleH = 18 * m_dTxRatio;
	int iFontAbbrH = 20 * m_dTxRatio;

	m_samPlateDlg.SetRowHeaderWidth(36 * m_dHoriRatio);
	m_samPlateDlg.SetColumnHeaderHeight(36 * m_dVertRatio);
	m_samPlateDlg.SetFixedRowHeightColumnWidth(1 * m_dVertRatio,3 * m_dHoriRatio);
	
	m_samPlateDlg.SetRowColNum(32, 12);
	m_samPlateDlg.SetRectWidth(70 * m_dHoriRatio);
	m_samPlateDlg.SetRectHeight(64 * m_dVertRatio);

	m_samPlateDlg.SetBkColor(RGB(0XE6,0XF4,0XF5));
	m_samPlateDlg.SetFourAngleBkColor(RGB(0XFF,0XFF,0XFF));
	m_samPlateDlg.SetMyFont(m_stTxFont,iFontH);
	m_samPlateDlg.SetMyHeaderFont(m_stTxFont,iFontTitleH);
	m_samPlateDlg.SetMyTitleFont(m_stTxFont,iFontTitleH,FW_SEMIBOLD);
	m_samPlateDlg.SetMyFontAbbr(m_stTxFont,iFontAbbrH);
	m_samPlateDlg.SetBeTellParentRowSelChange(TRUE);
	m_samPlateDlg.SetBeSingleSelect(TRUE);
	m_samPlateDlg.SetBeParentProcessDeleteKey(TRUE);

	m_samPlateDlg.SetBeShowBottomText(TRUE);
	m_samPlateDlg.SetSiteIdShowState(FALSE);
	m_samPlateDlg.SetLastChanBlockShowState(FALSE);

	return;
}

void CLoadSamDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(rect);

	m_rcManage.m_rcInit = rect;
	m_rcManage.ClearRectToDelete();

	CRect rcTemp(16 * m_dHoriRatio,16 * m_dVertRatio,982 * m_dHoriRatio,611 * m_dVertRatio);
	if (rcTemp.bottom > rect.bottom - int(35*m_dVertRatio))
		rcTemp.bottom = rect.bottom - int(35*m_dVertRatio);

	m_samPlateDlg.SetWindowPos(this,rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height(),SWP_NOZORDER);
	m_rcManage.AddRectToDelete(rcTemp);

	rcTemp.left = 1082 * m_dHoriRatio;
	rcTemp.right = 1322 * m_dHoriRatio;
	if(theApp.GetBeEnglish())
	{
		rcTemp.right = 1402 * m_dHoriRatio;
	}
	rcTemp.bottom = 76 * m_dVertRatio;
	m_btLoad.SetWindowPos(this,rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height(),SWP_NOZORDER);
	m_rcManage.AddRectToDelete(rcTemp);

	int iStartY = rcTemp.top + rcTemp.Height() + 30*m_dVertRatio;
	rcTemp.left = 1082 * m_dHoriRatio;
	rcTemp.right = 1322 * m_dHoriRatio;
	if(theApp.GetBeEnglish())
	{
		rcTemp.right = 1402 * m_dHoriRatio;
	}
	rcTemp.bottom = iStartY + rcTemp.Height();
	rcTemp.top = iStartY;
	m_swtBtSegMonitor.SetWindowPos(this,rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height(),SWP_NOZORDER);
	m_rcManage.AddRectToDelete(rcTemp);

	rcTemp.top = rcTemp.top + rcTemp.Height() + 20*m_dVertRatio;
	rcTemp.bottom = 646 * m_dVertRatio;
	rcTemp.right = rect.right - 20 * m_dHoriRatio;
	m_segDlg.SetWindowPos(this,rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height(),SWP_NOZORDER);
	m_rcManage.AddRectToDelete(rcTemp);


	m_rcManage.CalcRectResult();

	return;
}

void CLoadSamDlg::ShowCtrl()
{
	m_samPlateDlg.ShowWindow(SW_SHOW);
	m_btLoad.ShowWindow(SW_SHOW);

	m_swtBtSegMonitor.ShowWindow(SW_SHOW);
	m_segDlg.ShowWindow(SW_SHOW);
}

void CLoadSamDlg::SetCtrlText()
{
	m_btLoad.SetWindowText(theApp.m_pLangInfo->GetText(386));
	m_swtBtSegMonitor.SetWindowText(theApp.m_pLangInfo->GetText(465));
	return;
}

int CLoadSamDlg::RefreshExpEmbody()
{
	if(NULL == m_pExpData)
	{
		return -1;
	}

	vector<CTubeInfo*> vecTubeInfoPtr;
	m_pExpData->GetAllTubeInfoPtr(vecTubeInfoPtr);
	m_samPlateDlg.SetTubeInfo(vecTubeInfoPtr,TRUE);
	m_segDlg.ResetCurItemSubCycle();
	m_segDlg.SetSegmentInfo(m_pExpData->GetSegment(),TRUE);
	m_segDlg.SetSegmentFullSelectState(FALSE,TRUE);

	return 0;
}

void CLoadSamDlg::ClearExpEmbody()
{
	m_samPlateDlg.DeleteAllTubeInfo();
	m_segDlg.Clear();

	return;
}

int CLoadSamDlg::ImportId()
{
	if(NULL == m_pExpData)
	{
		return -1;
	}

	vector<eSampleType> vecSamType;
	m_samPlateDlg.GetAllSampleType(vecSamType);
	int iRet = m_pExpData->InputSampleSetInfo(&vecSamType);
	if(0 == iRet)
	{
		//����ɹ������¿װ�༭�������Ǽ�ҳ��
		vector<CTubeInfo*> vecTubeInfoPtr;
		m_pExpData->GetAllTubeInfoPtr(vecTubeInfoPtr);
		m_samPlateDlg.SetTubeInfo(vecTubeInfoPtr,TRUE);
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(344));
	}
	else
	{
		if(-5 == iRet)
		{
			PopWarningTipInfo(theApp.m_pLangInfo->GetText(345));
			return -2;
		}
		else if(-6 == iRet || -7 == iRet || -8 == iRet || -9 == iRet || -10 == iRet)
		{
			PopWarningTipInfo(theApp.m_pLangInfo->GetText(346));
			return -3;
		}
		else if(-1 == iRet)
		{
			PopWarningTipInfo(theApp.m_pLangInfo->GetText(347));
			return -2;
		}
	}

	return 0;
}

int CLoadSamDlg::SetEditMode(EEXPEDITMODE eExpEditMode)
{
	if(m_eExpEditMode == eExpEditMode)
	{
		return 1;
	}

	m_eExpEditMode = eExpEditMode;

	if(EEEM_RUNNING == m_eExpEditMode)
	{
		m_btLoad.EnableWindow(FALSE);
	}
	else if(EEEM_END == m_eExpEditMode)
	{
		m_btLoad.EnableWindow(TRUE);
	}

	return 0;
}

void CLoadSamDlg::SetExpDataPtr(CExperimentData* pExpData)
{
	m_pExpData = pExpData;
	m_segDlg.SetSegmentInfo(m_pExpData->GetSegment(),TRUE);
	//m_segDlg.ResetCurItemSubCycle();
	//m_segDlg.SetSegmentInfo(m_pExpData->GetSegment(),TRUE);
	//m_segDlg.SetSegmentFullSelectState(FALSE,TRUE);
	return;
}


void CLoadSamDlg::InitSegment()
{
	int iFontH = 22 * m_dTxRatio;
	if(theApp.m_pLangInfo->GetBeEnglish())
	{
		iFontH = 23 * m_dTxRatio;
	}

	m_segDlg.SetGradientInfoUseState(FALSE);
	m_segDlg.m_fontStepTag.SetMyFont(m_stTxFont,iFontH);
	m_segDlg.m_fontGeneral.SetMyFont(m_stTxFont,iFontH);
	m_segDlg.m_fontSegTag.SetMyFont(m_stTxFont,iFontH);
	m_segDlg.SetBeMonitorFunc(TRUE);

	int iTempInt1 = 34 * m_dVertRatio;
	m_segDlg.m_horiScroll.m_arrowWH = iTempInt1;
	m_segDlg.m_horiScroll.m_scrollH = iTempInt1;
	m_segDlg.m_vertScroll.m_arrowWH = iTempInt1;
	m_segDlg.m_vertScroll.m_scrollW = iTempInt1;
	m_segDlg.SetScrollArrowWH(4 * m_dHoriRatio,8 * m_dVertRatio);

	m_segDlg.SetBeEditable(FALSE);
}

int CLoadSamDlg::CurrentSegmentStepChangedOnRun(int iCycle,int iSeg,int iStep)
{
	if(-1 != m_segDlg.m_priorItem && -1 != m_segDlg.m_priorStep)
	{
		m_segDlg.UpdateCurSelectStepCycleInfo(m_segDlg.m_priorItem,m_segDlg.m_priorStep,iCycle,FALSE);
	}

	m_segDlg.UpdateCurSelectStepCycleInfo(iSeg,iStep,iCycle + 1,TRUE,FALSE);
	m_segDlg.SetSegmentSetpShow(iSeg,iStep,TRUE);	

	m_segDlg.m_priorItem = iSeg;
	m_segDlg.m_priorStep = iStep;

	return 0;
}

