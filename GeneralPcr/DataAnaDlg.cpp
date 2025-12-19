// DataAnaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralPcr.h"
#include "GeneralPcrDlg.h"
#include "DataAnaDlg.h"
#include "MiddleDlg.h"
#include "AnaAmlParamDlg.h"
#include "AxisZoomDlg.h"
#include <WinSock2.h>
#include "TurbidityParaDlg.h"
#include "afxdialogex.h"

// CDataAnaDlg dialog

IMPLEMENT_DYNAMIC(CDataAnaDlg, CDialogEx)

CDataAnaDlg::CDataAnaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataAnaDlg::IDD, pParent)
{
	m_bCtrlCreated = FALSE;
	m_pParent = NULL;
	if(NULL != pParent)
	{ 
		m_pParent = (CMiddleDlg*)pParent;
	}

	m_pCurSelCrvToShow = NULL;
	m_iPreCbSelIndex = -1;
}

CDataAnaDlg::~CDataAnaDlg()
{

}

void CDataAnaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
 
BEGIN_MESSAGE_MAP(CDataAnaDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_DATAANALYSEDLG_BT_AXISZOOM,OnBnClickedBtAxisZoom)
	ON_BN_CLICKED(ID_DATAANALYSEDLG_BT_AXISZOOMSECOND,OnBnClickedBtAxisZoomSecond)
	ON_MESSAGE(WM_TELLPARENTSITESELECTCHANGED,OnMsgPlateSiteSelectChanged)
	ON_MESSAGE(WM_TELLPARENTSETNEWTEXT,OnMsgTellParentSetNewText)
	ON_MESSAGE(WM_THRECHANGED,OnMsgThreChanged) 
	ON_MESSAGE(WM_TELLPARENTTEXTCHANGED,OnMsgTellParentTextChanged)
	//ON_MESSAGE(WM_ROWSELECTCHANGE,OnMsgRowSelectedChange)
END_MESSAGE_MAP()

BOOL CDataAnaDlg::OnEraseBkgnd(CDC* pDC)
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	CRect rect;
	GetClientRect(&rect);

	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	memDc.SelectObject(bitMapCB);
	memDc.SetBkMode(TRANSPARENT);

	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);

	memDc.FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),CLR_SOFT_LIGHT);

	DrawRoundRectangleNew(&memDc,900 * m_dHoriRatio,12 * m_dVertRatio,868 * m_dHoriRatio,480 * m_dVertRatio,8,8,8,8,TRUE,Color(0XD8,0XD8,0XD8),1,TRUE,Color(0XFF,0XFF,0XFF),1);
	DrawRoundRectangleNew(&memDc,900 * m_dHoriRatio,503 * m_dVertRatio,868 * m_dHoriRatio,354 * m_dVertRatio,8,8,8,8,TRUE,Color(0XD8,0XD8,0XD8),1,TRUE,Color(0XFF,0XFF,0XFF),1);

	int iCurSel = m_cbSelCrvToShow.GetCurSel();
	if(0 == iCurSel || 1 == iCurSel)
	{
		memDc.DrawText(pLangInfo->GetText(120),m_rcThresholdText,DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	}

	memDc.SelectObject(pOldFont);
	
	CRect rcLegend;
	rcLegend.left = 17 * m_dHoriRatio;
	rcLegend.right = rcLegend.left + 868 * m_dHoriRatio;
	rcLegend.top = 12 * m_dVertRatio + 453 * m_dVertRatio;
	rcLegend.bottom = rcLegend.top + 27 * m_dVertRatio;
	DrawLegend(&memDc,&rcLegend,m_segLgdInfo);

	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

	Node<CRect>* pNodeRst = m_rcManage.m_lnkRcRst.GetHead();
	while(NULL != pNodeRst)
	{
		pDC->BitBlt(pNodeRst->data.left,pNodeRst->data.top,pNodeRst->data.Width(),pNodeRst->data.Height(),&memDc,pNodeRst->data.left,pNodeRst->data.top,SRCCOPY);
		pNodeRst = pNodeRst->pNext;
	}
	
	bitMapCB.DeleteObject();
	memDc.DeleteDC();

	return TRUE;
}

int CDataAnaDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

#ifdef ADJUST_ANALYSEDLG_THRESHOLD_ED_SIZE_POSITION
	m_rcThresholdText = CRect(1220 * m_dHoriRatio,24 * m_dVertRatio,1306 * m_dHoriRatio,56 * m_dVertRatio); 
#else
	m_rcThresholdText = CRect(1180 * m_dHoriRatio,24 * m_dVertRatio,1266 * m_dHoriRatio,56 * m_dVertRatio);
#endif

	return 0;
}

void CDataAnaDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}
	return;
}

void CDataAnaDlg::OnBnClickedBtAxisZoom()
{
	int iCurSel = m_cbSelCrvToShow.GetCurSel();
	if(iCurSel < 0)
	{
		return;
	}

	if(NULL == m_pParent->GetSafeHwnd())
	{
		return;
	}

	if(NULL == m_pParent->m_pExpData)
	{
		return;
	}

	BOOL bHaveLogFunc = FALSE,bLogMode = FALSE;
	if(0 == iCurSel)
	{
		bHaveLogFunc = TRUE;
		bLogMode = m_pParent->m_pExpData->GetDeltaRnSelectLogState();
	}
	else if(1 == iCurSel)
	{
		bHaveLogFunc = TRUE;
		bLogMode = m_pParent->m_pExpData->GetNormalizeSelectLogState();
	}
	else if(2 == iCurSel)
	{
		bHaveLogFunc = TRUE;
		bLogMode = m_pParent->m_pExpData->GetFilterSelectLogState();
	}

	CAxisZoomDlg axisZoomDlg(NULL,bHaveLogFunc,bLogMode);
	axisZoomDlg.m_defLinearAxisInfo = m_pParent->m_pExpData->GetDefaultAxisInfoAt(iCurSel);

	axisZoomDlg.m_axisInfo.m_dXMin = m_pCurSelCrvToShow->GetHoriStartValue();
	axisZoomDlg.m_axisInfo.m_dXMax = m_pCurSelCrvToShow->GetHoriEndValue();
	axisZoomDlg.m_axisInfo.m_dYMin = m_pCurSelCrvToShow->GetVertStartValue();
	axisZoomDlg.m_axisInfo.m_dYMax = m_pCurSelCrvToShow->GetVertEndValue();

	if(IDOK == axisZoomDlg.DoModal())
	{
		if(axisZoomDlg.m_bChanged)
		{
			m_pCurSelCrvToShow->SetHoriStartValue(axisZoomDlg.m_newAxisInfo.m_dXMin);
			m_pCurSelCrvToShow->SetHoriEndValue(axisZoomDlg.m_newAxisInfo.m_dXMax);
			m_pCurSelCrvToShow->SetVertStartValue(axisZoomDlg.m_newAxisInfo.m_dYMin);
			m_pCurSelCrvToShow->SetVertEndValue(axisZoomDlg.m_newAxisInfo.m_dYMax,TRUE,FALSE);
		}

		if(axisZoomDlg.m_bLogModeChanged)
		{
			BOOL bNewLogMode = axisZoomDlg.GetNewLogMode();
			if(0 == iCurSel)
			{
				m_pParent->m_pExpData->SetDeltaRnSelectLogState(bNewLogMode);
				DeltaRnLogModeChanged();
			}
			else if(1 == iCurSel)
			{
				m_pParent->m_pExpData->SetNormalizeSelectLogState(bNewLogMode);
				NormalizeLogModeChanged();
			}
			else if(2 == iCurSel)
			{
				m_pParent->m_pExpData->SetFilterSelectLogState(bNewLogMode);
				SetFilterCurveData(m_pParent->m_pExpData,TRUE);
			}
		}
	}

	return;
}

void CDataAnaDlg::OnBnClickedBtAxisZoomSecond()
{
	CurveAxisEdit(&m_crvStd);
}

LRESULT CDataAnaDlg::OnMsgPlateSiteSelectChanged(WPARAM wParam,LPARAM lParam)
{
	m_samPlateDlg.GetPlateSelectState(m_vecPlateSelectState);
	m_samPlateDlg.GetPlateSelectedSite(m_vecPlateSelectedSite);

	//��λѡ��״̬�ı䣬������ʾ����
	UpdateCurveSelectState(TRUE);

	m_crvAml.HandSetGeneralLineXYAxisMinMax();
	m_crvAmlDelRn.HandSetGeneralLineXYAxisMinMax();
	m_crvOriginAml.HandSetGeneralLineXYAxisMinMax();
	m_crvPeakMelt.HandSetGeneralLineXYAxisMinMax();
	m_crvOriginMelt.HandSetGeneralLineXYAxisMinMax();

	//��ʾ�������
	ResetResultLst();

	return 1;
}

LRESULT CDataAnaDlg::OnMsgTellParentSetNewText(WPARAM wParam,LPARAM lParam)
{
	EITEMINFOTYPE eItemInfoType = (EITEMINFOTYPE)wParam;
	
	switch(eItemInfoType)
	{
		case ITEM_DATAANADLG_CBCRVSELECT: 
			CbSelectCrvToShowChanged();
			break;
		case ITEM_DATAANADLG_CBTHRESELECT: 
			CbSelectChannelThresholdChanged(); 
			break;

		default:	break;
	}

	return 0;
}

LRESULT CDataAnaDlg::OnMsgThreChanged(WPARAM wParam, LPARAM lParam)
{
	if(wParam < 0)
	{
		return 1;
	}
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return 2;
	}
	if(NULL == m_pParent->m_pExpData)
	{
		return 3;
	}
	
	int iChanNum = m_pParent->m_pExpData->GetChannelCount();
	if(wParam >= iChanNum)
	{
		return 4;
	}

	tagAnaAmplifyPara* pAnaAmpParam = m_pParent->m_pExpData->GetAmplifyParaBy(wParam);
	if(NULL == pAnaAmpParam)
	{
		return 5;
	}

	//���÷������������ֵ��Ϣ
	CThreLineInfo* pThreLineInfo = NULL;
	if(0 == m_cbSelCrvToShow.GetCurSel())	//DeltaRn
	{
		pThreLineInfo = m_crvAmlDelRn.m_vecChanThreInfo[wParam].GetCurSelThreInfo();
		pAnaAmpParam->dCalcDeltaRnThreshold = pThreLineInfo->GetThreLineValue();
		pAnaAmpParam->nThresholdType = SPLINE_THRESHOLD_DELTARN;
	}
	else if(1 == m_cbSelCrvToShow.GetCurSel())	//��һ��
	{
		pThreLineInfo = m_crvAml.m_vecChanThreInfo[wParam].GetCurSelThreInfo();
		pAnaAmpParam->dCalcNormalizeThreshold = pThreLineInfo->GetThreLineValue();
		pAnaAmpParam->nThresholdType = SPLINE_THRESHOLD_NORMALIZE;
	}
	
	if(NULL == pThreLineInfo)
	{
		return 6;
	}
	
	eDataType eDtType = FLU_ORIGINAL;
	//����ͨ����Ӧ���ߵ�Ctֵ
	if(SPLINE_THRESHOLD_DELTARN == pAnaAmpParam->nThresholdType)
	{
		if(m_pParent->m_pExpData->GetDeltaRnSelectLogState())
		{
			eDtType = LOG_FLU_DELTARN;
		}
		else
		{
			eDtType = FLU_DELTARN;
		}
		m_pParent->m_pExpData->GenerateOneChannelCt(METHOD_CT_SPLINE,eDtType,wParam + 1);
	}
	else
	{
		if(m_pParent->m_pExpData->GetNormalizeSelectLogState())
		{
			eDtType = LOG_FLU_NORMALIZATE;
		}
		else
		{
			eDtType = FLU_NORMALIZATE;
		}
		m_pParent->m_pExpData->GenerateOneChannelCt(METHOD_CT_SPLINE,eDtType,wParam + 1);
	}

	//����ͨ��������Ϣ
	StdLineProcess(m_pParent->m_pExpData,wParam);

	//����ͨ����Ӧ�Ľ���б���Ctֵ��
	NewResultCalculated(wParam);

	if(wParam == m_cbSelChanThre.GetCurSel())
	{
		m_edThreValue.SetText(pThreLineInfo->GetThreLineValue(),TRUE,TRUE);
	}

	return 0;
}

LRESULT CDataAnaDlg::OnMsgTellParentTextChanged(WPARAM wParam, LPARAM lParam)
{
	CString strTemp(_T(""));
	strTemp = m_edThreValue.GetText();
	double dNewThre = _ttof(strTemp);
	
	int iCurSel = m_cbSelChanThre.GetCurSel();
	if(iCurSel < 0)
	{
		return 1;
	}

	if(iCurSel >= m_crvAmlDelRn.m_vecChanThreInfo.size())
	{
		return 2;
	}

	if(iCurSel >= m_crvAml.m_vecChanThreInfo.size())
	{
		return 3;
	}

	if(NULL == m_pParent->GetSafeHwnd())
	{
		return 4;
	}
	if(NULL == m_pParent->m_pExpData)
	{
		return 5;
	}

	int iChanNum = m_pParent->m_pExpData->GetChannelCount();
	if(iCurSel >= iChanNum)
	{
		return 6;
	}

	tagAnaAmplifyPara* pAnaAmpParam = m_pParent->m_pExpData->GetAmplifyParaBy(iCurSel);
	if(NULL == pAnaAmpParam)
	{
		return 7;
	}

	//���÷������������ֵ��Ϣ
	CThreLineInfo* pThreLineInfo = NULL;
	if(0 == m_cbSelCrvToShow.GetCurSel())	//DeltaRn
	{
		pThreLineInfo = m_crvAmlDelRn.m_vecChanThreInfo[iCurSel].GetCurSelThreInfo();
	}
	else if(1 == m_cbSelCrvToShow.GetCurSel())	//��һ��
	{
		pThreLineInfo = m_crvAml.m_vecChanThreInfo[iCurSel].GetCurSelThreInfo();
	}

	if(NULL == pThreLineInfo)
	{
		return 8;
	}

	if(dNewThre == pThreLineInfo->GetThreLineValue())
	{
		return 0;
	}

	//�������߿ؼ��ͷ��������е���ֵ
	pThreLineInfo->SetThreLineValue(dNewThre);
	if(pThreLineInfo->m_bAutoThre)
	{
		pThreLineInfo->m_bAutoThre = FALSE;
	}
	pThreLineInfo->SetBeFirstDrawThreLline(TRUE);
	if(0 == m_cbSelCrvToShow.GetCurSel())	//DeltaRn
	{
		pAnaAmpParam->dCalcDeltaRnThreshold = dNewThre;
		pAnaAmpParam->nThresholdType = SPLINE_THRESHOLD_DELTARN;
		m_crvAmlDelRn.Invalidate(FALSE);
	}
	else if(1 == m_cbSelCrvToShow.GetCurSel())	//��һ��
	{
		pAnaAmpParam->dCalcNormalizeThreshold = dNewThre;
		pAnaAmpParam->nThresholdType = SPLINE_THRESHOLD_NORMALIZE;
		m_crvAml.Invalidate(FALSE);
	}

	//����ͨ����Ӧ���ߵ�Ctֵ
	if(SPLINE_THRESHOLD_DELTARN == pAnaAmpParam->nThresholdType)
	{
		m_pParent->m_pExpData->GenerateOneChannelCt(METHOD_CT_SPLINE,FLU_DELTARN,iCurSel + 1);
	}
	else
	{
		m_pParent->m_pExpData->GenerateOneChannelCt(METHOD_CT_SPLINE,FLU_NORMALIZATE,iCurSel + 1);
	}

	//����ͨ����Ӧ�Ľ���б���Ctֵ��
	NewResultCalculated(iCurSel);

	//����ͨ��������Ϣ
	StdLineProcess(m_pParent->m_pExpData,iCurSel);

	return 1;
}

BOOL CDataAnaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CreateCtrl();
	InitCrv();
	m_bCtrlCreated = TRUE;
	InitDlg();
	InitSegLegend();
	InitCtrl();
	SetCtrlPos();

	SetCtrlText();

	ShowCtrl();
	
	return TRUE;
}

void CDataAnaDlg::InitDlg()
{
	int iFontH = 14 * m_dTxRatio;
	if(theApp.m_pLangInfo->GetBeEnglish())
	{
		iFontH = 16 * m_dTxRatio;
	}

	SetMyFont(m_stTxFont,iFontH);
	SetTextClr1(RGB(0X39,0X45,0X65));
}

void CDataAnaDlg::InitSegLegend()
{
	CSingleLgdInfo singleLgdInfo;

#ifdef N16USENEWSAMPLETYPE
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

#else
	singleLgdInfo.m_clrFill = Color(0X20,0X44,0X7C);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(211);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0X20,0X9F,0X85);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(212);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0X33,0X83,0XFD);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(213);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0X24,0XB7,0XEA);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(214);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0XC5,0X64,0X62);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(215);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0XF2,0X31,0X31);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(216);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0XF0,0X31,0XF2);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(217);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0,0X70,0XC0);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(218);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);
#endif

	m_segLgdInfo.m_iLeftIndent = 41 * m_dHoriRatio; 
	m_segLgdInfo.m_iRoundRadius = 6 * m_dHoriRatio; 
	m_segLgdInfo.m_iInrIconToTx = 8 * m_dHoriRatio; 
	m_segLgdInfo.m_iInrTxToIcon = 20 * m_dHoriRatio; 

	int iFontH = 13 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		m_segLgdInfo.m_iOneCharW = 10 * m_dHoriRatio;
		iFontH = 15 * m_dTxRatio;
	}
	else
	{
		m_segLgdInfo.m_iOneCharW = 14 * m_dHoriRatio;
	}

	m_segLgdInfo.SetMyFont(m_stTxFont,iFontH);
	m_segLgdInfo.SetTextClr1(RGB(0X39,0X45,0X65));
}

void CDataAnaDlg::InitSonDlg()
{

}

void CDataAnaDlg::CreateCtrl()
{
	CreateBt();
	CreateSt();
	CreateCb();
	CreateEd();
	CreateLst();
	CreateSamPlateDlg();
}

void CDataAnaDlg::CreateBt()
{
	m_btAxisZoom.Create(_T("ͼ������"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_DATAANALYSEDLG_BT_AXISZOOM);
	m_btAxisZoom2nd.Create(_T("��������"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_DATAANALYSEDLG_BT_AXISZOOMSECOND);
}

void CDataAnaDlg::CreateSt()
{

}

void CDataAnaDlg::CreateCb()
{
	CRect rcCreate(0,0,0,0);

	m_cbSelCrvToShow.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rcCreate,this,ID_DATAANALYSEDLG_CB_SELECTCURVETOSHOW);
	m_cbSelChanThre.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rcCreate,this,ID_DATAANALYSEDLG_CB_SELECTCHANNELTHRE);
}

void CDataAnaDlg::CreateEd()
{
	CRect rcCreate(0,0,0,0);

	m_edThreValue.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rcCreate,this,ID_DATAANALYSEDLG_ED_THREVALUE);
}

void CDataAnaDlg::CreateLst()
{
	m_lstAnaResult.Create(IDD_MIDDLEDLG,this);
}

void CDataAnaDlg::CreateSamPlateDlg()
{
	m_samPlateDlg.Create(IDD_MIDDLEDLG,this);
}

void CDataAnaDlg::InitCrv()
{
	int iFontH = 14 * m_dTxRatio;
	if(theApp.m_pLangInfo->GetBeEnglish())
	{
		iFontH = 16 * m_dTxRatio;
	}

	m_crvAml.SetBeUseFloatFunc(TRUE);
	m_crvAml.SetXAxisTag(_T("ѭ����"));
	m_crvAml.SetYAxisTag(_T("Rn"));

	m_crvAml.SetYAxisMinOfMax(0.5);
	m_crvAml.SetVertEndValue(1);
	m_crvAml.SetVertStepNums(10);
	m_crvAml.SetVertDecimalPointLength(2);

	m_crvAml.SetHoriStartValue(1);
	m_crvAml.SetHoriEndValue(40);
	m_crvAml.SetHoriStepNums(39);
	m_crvAml.SetHoriStringOutStep(3);

	m_crvAml.SetLeftIndent(70 * m_dHoriRatio);
	m_crvAml.SetRightIndent(14 * m_dVertRatio);
	m_crvAml.SetTopIndent(12 * m_dHoriRatio);
	m_crvAml.SetBottomIndent(33 * m_dVertRatio);

	m_crvAml.SetHaveCaptionOrNot(FALSE);
	m_crvAml.SetHaveLegendOrNot(FALSE);
	m_crvAml.SetBackgrounColor(RGB(255,255,255));
	m_crvAml.Create(IDD_MIDDLEDLG,this);
	m_crvAml.SetMyFont(m_stTxFont,iFontH);
	m_crvAml.SetMinYDecrementParam(TRUE,0.1,0.02);

	m_crvAmlDelRn.SetYAxisMinOfMax(2000);
	m_crvAmlDelRn.SetBeUseFloatFunc(TRUE);
	m_crvAmlDelRn.SetXAxisTag(_T("ѭ����"));
	m_crvAmlDelRn.SetYAxisTag(_T("Rn"));

	m_crvAmlDelRn.SetVertEndValue(DEFAULT_FLUMAX);
	m_crvAmlDelRn.SetVertStepNums(10);

	m_crvAmlDelRn.SetHoriStartValue(1);
	m_crvAmlDelRn.SetHoriEndValue(40);
	m_crvAmlDelRn.SetHoriStepNums(39);
	m_crvAmlDelRn.SetHoriStringOutStep(3);

	m_crvAmlDelRn.SetLeftIndent(70 * m_dHoriRatio);
	m_crvAmlDelRn.SetRightIndent(14 * m_dVertRatio);
	m_crvAmlDelRn.SetTopIndent(12 * m_dHoriRatio);
	m_crvAmlDelRn.SetBottomIndent(33 * m_dVertRatio);

	m_crvAmlDelRn.SetHaveCaptionOrNot(FALSE);
	m_crvAmlDelRn.SetHaveLegendOrNot(FALSE);
	m_crvAmlDelRn.SetBackgrounColor(RGB(255,255,255));
	m_crvAmlDelRn.Create(IDD_MIDDLEDLG,this);
	m_crvAmlDelRn.SetMyFont(m_stTxFont,iFontH);
	m_crvAmlDelRn.SetMinYDecrementParam(TRUE,100,100);

	m_crvOriginAml.SetBeUseFloatFunc(TRUE);
	m_crvOriginAml.SetVertEndValue(DEFAULT_FLUMAX);
	m_crvOriginAml.SetVertStepNums(10);

	m_crvOriginAml.SetYAxisTag(_T("Fn"));
	m_crvOriginAml.SetXAxisTag(_T("ѭ����"));

	m_crvOriginAml.SetHoriStartValue(1);
	m_crvOriginAml.SetHoriEndValue(40);
	m_crvOriginAml.SetHoriStepNums(39);
	m_crvOriginAml.SetHoriStringOutStep(3);

	m_crvOriginAml.SetLeftIndent(70 * m_dHoriRatio);
	m_crvOriginAml.SetRightIndent(12 * m_dVertRatio);
	m_crvOriginAml.SetTopIndent(14 * m_dHoriRatio);
	m_crvOriginAml.SetBottomIndent(33 * m_dVertRatio);

	m_crvOriginAml.SetHaveCaptionOrNot(FALSE);
	m_crvOriginAml.SetHaveLegendOrNot(FALSE);
	m_crvOriginAml.SetBackgrounColor(RGB(255,255,255));
	m_crvOriginAml.Create(IDD_MIDDLEDLG,this);
	m_crvOriginAml.SetMyFont(m_stTxFont,iFontH);

	m_crvPeakMelt.SetYAxisMinOfMax(0);
	m_crvPeakMelt.SetBeUseFloatFunc(TRUE);
	m_crvPeakMelt.SetVertEndValue(100);
	m_crvPeakMelt.SetVertStepNums(10);
	m_crvPeakMelt.SetVertDecimalPointLength(2);

	m_crvPeakMelt.SetYAxisTag(_T("-d(Rn)/dT"));
	m_crvPeakMelt.SetXAxisTag(_T("�¶�(��)"));

	m_crvPeakMelt.SetHoriStartValue(60);
	m_crvPeakMelt.SetHoriEndValue(90);
	m_crvPeakMelt.SetHoriStep(1);
	m_crvPeakMelt.SetHoriStringOutStep(5);
	m_crvPeakMelt.SetHoriDecimalPointLength(2);

	m_crvPeakMelt.SetLeftIndent(70 * m_dHoriRatio);
	m_crvPeakMelt.SetRightIndent(12 * m_dVertRatio);
	m_crvPeakMelt.SetTopIndent(14 * m_dHoriRatio);
	m_crvPeakMelt.SetBottomIndent(33 * m_dVertRatio);

	m_crvPeakMelt.SetHaveCaptionOrNot(FALSE);
	m_crvPeakMelt.SetHaveLegendOrNot(FALSE);
	m_crvPeakMelt.SetBackgrounColor(RGB(255,255,255));
	m_crvPeakMelt.Create(IDD_MIDDLEDLG,this);
	m_crvPeakMelt.SetMyFont(m_stTxFont,iFontH);


	m_crvOriginMelt.SetYAxisMinOfMax(2000);
	m_crvOriginMelt.SetBeUseFloatFunc(TRUE);
	m_crvOriginMelt.SetVertEndValue(DEFAULT_FLUMAX);
	m_crvOriginMelt.SetVertStepNums(10);

	m_crvOriginMelt.SetYAxisTag(_T("Fn"));
	m_crvOriginMelt.SetXAxisTag(_T("�¶�(��)"));

	m_crvOriginMelt.SetHoriStartValue(60);
	m_crvOriginMelt.SetHoriEndValue(90);
	m_crvOriginMelt.SetHoriStep(1);
	m_crvOriginMelt.SetHoriStringOutStep(5);
	m_crvPeakMelt.SetHoriDecimalPointLength(2);

	m_crvOriginMelt.SetLeftIndent(70 * m_dHoriRatio);
	m_crvOriginMelt.SetRightIndent(12 * m_dVertRatio);
	m_crvOriginMelt.SetTopIndent(14 * m_dHoriRatio);
	m_crvOriginMelt.SetBottomIndent(33 * m_dVertRatio);

	m_crvOriginMelt.SetHaveCaptionOrNot(FALSE);
	m_crvOriginMelt.SetHaveLegendOrNot(FALSE);
	m_crvOriginMelt.SetBackgrounColor(RGB(255,255,255));
	m_crvOriginMelt.Create(IDD_MIDDLEDLG,this);
	m_crvOriginMelt.SetMyFont(m_stTxFont,iFontH);

	m_crvStd.SetBeShowExpression(TRUE);
	m_crvStd.SetExpressionTag(theApp.m_pLangInfo->GetText(94));
	m_crvStd.SetBeStdCrv(TRUE);
	m_crvStd.SetYAxisTag(_T("Ct"));
	m_crvStd.SetXAxisTag(_T("Log(Conc)")); 

	m_crvStd.SetYAxisMinOfMax(20);
	m_crvStd.SetHoriEndValue(1);
	m_crvStd.SetVertEndValue(20);
	m_crvStd.SetVertStepNums(10);
	m_crvStd.SetHoriDecimalPointLength(2);
	m_crvStd.SetVertDecimalPointLength(2);

	m_crvStd.SetLeftIndent(59 * m_dHoriRatio);
	m_crvStd.SetRightIndent(12 * m_dVertRatio);
	m_crvStd.SetTopIndent(52 * m_dHoriRatio);
	m_crvStd.SetBottomIndent(35 * m_dVertRatio);

	m_crvStd.SetHaveCaptionOrNot(FALSE);
	m_crvStd.SetHaveLegendOrNot(FALSE);
	m_crvStd.SetCurveWidth(2 * m_dTxRatio);
	m_crvStd.SetBackgrounColor(RGB(255,255,255));
	m_crvStd.Create(IDD_MIDDLEDLG,this);
	m_crvStd.SetMyFont(m_stTxFont,iFontH);
	m_crvStd.m_fontExpression.SetMyFont(m_stTxFont,16 * m_dTxRatio);
	m_crvStd.SetShowRoundTag(TRUE);
	m_crvStd.SetCurveType(STDLINE);
	m_crvStd.SetXAxisExplainShowState(TRUE);
	
	m_crvStd.SetExpressionAutoXYMinMax(FALSE);
}

void CDataAnaDlg::InitCtrl()
{
	InitBt();
	InitSt();
	InitCb();
	InitEd();
	InitLst();
	InitSamPlateDlg();
}

void CDataAnaDlg::InitBt()
{
	int iFontH = 14 * m_dTxRatio;
	if(theApp.m_pLangInfo->GetBeEnglish())
	{
		iFontH = 16 * m_dTxRatio;
	}

	m_btAxisZoom.SetPngBkPic(_T(".\\BkPic\\zoom.png"),1,14,10);
	m_btAxisZoom.SetMyFont(m_stTxFont,iFontH);
	m_btAxisZoom.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);

	m_btAxisZoom2nd.SetPngBkPic(_T(".\\BkPic\\zoom.png"),1,14,10);
	m_btAxisZoom2nd.SetMyFont(m_stTxFont,iFontH);
	m_btAxisZoom2nd.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);
}

void CDataAnaDlg::InitSt()
{

}

void CDataAnaDlg::InitCb()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	int iFontH = 14 * m_dTxRatio;
	if(theApp.m_pLangInfo->GetBeEnglish())
	{
		iFontH = 15 * m_dTxRatio;
	}

	m_cbSelCrvToShow.SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	m_cbSelCrvToShow.SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	m_cbSelCrvToShow.SetSelectedBkColor(CLR_SOFT_THEME,CLR_SOFT_THEME,CLR_SOFT_THEME,CLR_SOFT_THEME);
	m_cbSelCrvToShow.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XC6,0XC6,0XC6));
	m_cbSelCrvToShow.SetMyFont(m_stTxFont,iFontH);
	m_cbSelCrvToShow.m_selWnd.SetMyFont(m_stTxFont,iFontH);
	m_cbSelCrvToShow.SetWorkState(EWS_READONLY);
	m_cbSelCrvToShow.m_iRightIndent = 44 * m_dHoriRatio;
	m_cbSelCrvToShow.m_arrowInfo.m_clrArrow = Color(0X39,0X45,0X65);
	m_cbSelCrvToShow.m_arrowInfo.m_arrowW = 12 * m_dHoriRatio;
	m_cbSelCrvToShow.m_arrowInfo.m_arrowH = 8 * m_dVertRatio;
	m_cbSelCrvToShow.SetTellParentSetNewText(TRUE,ITEM_DATAANADLG_CBCRVSELECT);

	m_cbSelCrvToShow.InsertString(0,pLangInfo->GetText(186));
	m_cbSelCrvToShow.InsertString(1,pLangInfo->GetText(187));
	m_cbSelCrvToShow.InsertString(2,pLangInfo->GetText(77));

#ifndef SHIELDMELTFUNC
	m_cbSelCrvToShow.InsertString(3,pLangInfo->GetText(92));
	m_cbSelCrvToShow.InsertString(4,pLangInfo->GetText(78));
#endif

	m_cbSelCrvToShow.SetCurSel(0,FALSE,TRUE);
	m_iPreCbSelIndex = 0;


	m_cbSelChanThre.SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	m_cbSelChanThre.SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	m_cbSelChanThre.SetSelectedBkColor(CLR_SOFT_THEME,CLR_SOFT_THEME,CLR_SOFT_THEME,CLR_SOFT_THEME);
	m_cbSelChanThre.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XC6,0XC6,0XC6));
	m_cbSelChanThre.SetMyFont(m_stTxFont,iFontH);
	m_cbSelChanThre.m_selWnd.SetMyFont(m_stTxFont,iFontH);
	m_cbSelChanThre.SetWorkState(EWS_READONLY);
	m_cbSelChanThre.m_iRightIndent = 44 * m_dHoriRatio;
	m_cbSelChanThre.m_arrowInfo.m_clrArrow = Color(0X39,0X45,0X65);
	m_cbSelChanThre.m_arrowInfo.m_arrowW = 12 * m_dHoriRatio;
	m_cbSelChanThre.m_arrowInfo.m_arrowH = 8 * m_dVertRatio;
	m_cbSelChanThre.SetTellParentSetNewText(TRUE,ITEM_DATAANADLG_CBTHRESELECT);
}

void CDataAnaDlg::InitEd()
{
	int iFtH = 18 * m_dTxRatio;

	m_edThreValue.SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	m_edThreValue.SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	m_edThreValue.SetSelectedBkColor(RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB));
	m_edThreValue.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XC6,0XC6,0XC6));
	m_edThreValue.SetMyFont(m_stTxFont,iFtH);
	m_edThreValue.SetTellParentTxChangedParam(TRUE,0);
}

void CDataAnaDlg::InitLst()
{
	int iFontH = 14 * m_dTxRatio;
	if(theApp.m_pLangInfo->GetBeEnglish())
	{
		iFontH = 15 * m_dTxRatio;
	}

	m_lstAnaResult.SetHeaderHeight(24 * m_dVertRatio);
	m_lstAnaResult.SetMyHeaderFont(m_stTxFont,iFontH);
	m_lstAnaResult.SetScrollArrowWH(4 * m_dHoriRatio,8 * m_dVertRatio);
	m_lstAnaResult.SetFourAngleBkColor(CLR_SOFT_LIGHT);
	m_lstAnaResult.SetItemHeight(30 * m_dVertRatio);

	int iTempInt1 = 34 * m_dVertRatio;
	m_lstAnaResult.m_horiScroll.m_arrowWH = iTempInt1;
	m_lstAnaResult.m_horiScroll.m_scrollH = iTempInt1;
	m_lstAnaResult.m_vertScroll.m_arrowWH = iTempInt1;
	m_lstAnaResult.m_vertScroll.m_scrollW = iTempInt1;
	m_lstAnaResult.SetMyFont(m_stTxFont,iFontH);
	m_lstAnaResult.SetTextClr1(RGB(0X39,0X45,0X65));

	m_lstAnaResult.SetBeScrollInsideFrame(TRUE);

	int index = -1;

	m_lstAnaResult.InsertColumn(++index, _T(""));
	m_lstAnaResult.InsertColumn(++index, _T(""));
	m_lstAnaResult.InsertColumn(++index, _T(""));
	m_lstAnaResult.InsertColumn(++index, _T(""));
	m_lstAnaResult.InsertColumn(++index, _T("")); 
	m_lstAnaResult.InsertColumn(++index, _T(""));
	m_lstAnaResult.InsertColumn(++index, GetCtTitle());
	m_lstAnaResult.InsertColumn(++index, _T(""));
	m_lstAnaResult.InsertColumn(++index, _T(""));
	m_lstAnaResult.InsertColumn(++index, _T(""));
	m_lstAnaResult.InsertColumn(++index, _T(""));
	m_lstAnaResult.InsertColumn(++index, _T(""));
	m_lstAnaResult.InsertColumn(++index, _T(""));

	m_lstAnaResult.InsertColumn(++index, _T(""));
	m_lstAnaResult.SetColumnColorFunc(index,TRUE);

	m_lstAnaResult.InsertColumn(++index, _T(""));
	//m_lstAnaResult.InsertColumn(++index, _T("��������"));
	//m_lstAnaResult.InsertColumn(++index, _T("�Ա�"));
	//m_lstAnaResult.InsertColumn(++index, _T("����"));
	//m_lstAnaResult.InsertColumn(++index, _T("��������"));
	//m_lstAnaResult.InsertColumn(++index, _T("�ͼ�ҽ��"));
	//m_lstAnaResult.InsertColumn(++index, _T("�ͼ����"));
	//m_lstAnaResult.InsertColumn(++index, _T("������"));
	//m_lstAnaResult.InsertColumn(++index, _T("������"));
	//m_lstAnaResult.InsertColumn(++index, _T("סԺ��"));
	//m_lstAnaResult.InsertColumn(++index, _T("�����"));
	m_lstAnaResult.InsertColumn(++index, _T(""));

	m_lstAnaResult.SetColumnColorFunc(13,TRUE);

	ResetLstColumnWidth();

	return;
}

void CDataAnaDlg::InitSamPlateDlg()
{
	int iFontH = 18 * m_dTxRatio;
	int iFontH2 = 14 * m_dTxRatio;

	m_samPlateDlg.SetRowHeaderWidth(SAMPLATE_WH * m_dHoriRatio);
	m_samPlateDlg.SetColumnHeaderHeight(SAMPLATE_WH * m_dVertRatio);
	// Shrink cell size for analysis page to match compact 12x32 view
	m_samPlateDlg.SetFixedRowHeightColumnWidth(1 * m_dVertRatio,3 * m_dHoriRatio);
	m_samPlateDlg.SetInterval(15 * m_dHoriRatio,5 * m_dVertRatio);

	m_samPlateDlg.SetRowColNum(32, 12);

	m_samPlateDlg.SetBkColor(RGB(0XE6,0XF4,0XF5));
	m_samPlateDlg.SetMyFont(m_stTxFont,iFontH);
	m_samPlateDlg.SetMyHeaderFont(m_stTxFont,iFontH);
	m_samPlateDlg.SetMyTitleFont(m_stTxFont,iFontH,FW_SEMIBOLD);
	m_samPlateDlg.SetMyFontAbbr(m_stTxFont,iFontH2);
	m_samPlateDlg.SetBeTellParentRowSelChange(TRUE);
	m_samPlateDlg.SetBeSingleSelect(TRUE);
	m_samPlateDlg.SetBeShieldDeleteKey(TRUE);

#ifdef PS96INSTRUMENT
	m_samPlateDlg.SetSiteIdShowState(FALSE);
	m_samPlateDlg.SetLastChanBlockShowState(FALSE);
#else 
	#ifdef N96INSTRUMENT
		m_samPlateDlg.SetSiteIdShowState(FALSE);
	#endif
#endif
}

void CDataAnaDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	m_rcManage.m_rcInit = rect;
	m_rcManage.ClearRectToDelete();

	int tempInt1 = 56 * m_dVertRatio + 1;
	int tempInt2 = 436 * m_dVertRatio - 1;
	int iStartX = 904 * m_dHoriRatio;

	m_rcManage.AddRectToDelete(iStartX,tempInt1,860 * m_dHoriRatio,tempInt2);
	m_crvAml.SetWindowPos(this,iStartX,tempInt1,860 * m_dHoriRatio,tempInt2,SWP_NOZORDER);
	m_crvAmlDelRn.SetWindowPos(this,iStartX,tempInt1,860 * m_dHoriRatio,tempInt2,SWP_NOZORDER);
	m_crvOriginAml.SetWindowPos(this,iStartX,tempInt1,860 * m_dHoriRatio,tempInt2,SWP_NOZORDER);
	m_crvPeakMelt.SetWindowPos(this,iStartX,tempInt1,860 * m_dHoriRatio,tempInt2,SWP_NOZORDER);
	m_crvOriginMelt.SetWindowPos(this,iStartX,tempInt1,860 * m_dHoriRatio,tempInt2,SWP_NOZORDER);

	m_rcManage.AddRectToDelete(iStartX,511 * m_dVertRatio,860 * m_dHoriRatio,346 * m_dVertRatio);
	m_crvStd.SetWindowPos(this,iStartX,511 * m_dVertRatio,860 * m_dHoriRatio,346 * m_dVertRatio,SWP_NOZORDER);

	m_rcManage.AddRectToDelete(17 * m_dHoriRatio,12 * m_dVertRatio,868 * m_dHoriRatio,453 * m_dVertRatio);
	m_samPlateDlg.SetWindowPos(this,17 * m_dHoriRatio,12 * m_dVertRatio,868 * m_dHoriRatio,453 * m_dVertRatio,SWP_NOZORDER);

	m_rcManage.AddRectToDelete(21 * m_dHoriRatio,503 * m_dVertRatio,863 * m_dHoriRatio,354 * m_dVertRatio);
	m_lstAnaResult.SetWindowPos(this,21 * m_dHoriRatio,503 * m_dVertRatio,863 * m_dHoriRatio,354 * m_dVertRatio,SWP_NOZORDER);

	m_rcManage.AddRectToDelete(1660 * m_dHoriRatio,24 * m_dVertRatio,96 * m_dHoriRatio,32 * m_dHoriRatio);
	m_btAxisZoom.SetWindowPos(this,1660 * m_dHoriRatio,24 * m_dVertRatio,96 * m_dHoriRatio,32 * m_dHoriRatio,SWP_NOZORDER);

	//m_rcManage.AddRectToDelete(1660 * m_dHoriRatio,515 * m_dVertRatio,96 * m_dHoriRatio,32 * m_dHoriRatio);
	//m_btAxisZoom2nd.SetWindowPos(this,1660 * m_dHoriRatio,515 * m_dVertRatio,96 * m_dHoriRatio,32 * m_dHoriRatio,SWP_NOZORDER);

#ifdef ADJUST_ANALYSEDLG_THRESHOLD_ED_SIZE_POSITION
	m_rcManage.AddRectToDelete(975 * m_dHoriRatio,24 * m_dVertRatio,240 * m_dHoriRatio,32 * m_dHoriRatio);
	m_cbSelCrvToShow.SetWindowPos(this,975 * m_dHoriRatio,24 * m_dVertRatio,240 * m_dHoriRatio,32 * m_dHoriRatio,SWP_NOZORDER);

	m_edThreValue.SetWindowPos(this,1312 * m_dHoriRatio,24 * m_dVertRatio,160 * m_dHoriRatio,32 * m_dHoriRatio,SWP_NOZORDER);
	m_cbSelChanThre.SetWindowPos(this,1482 * m_dHoriRatio,24 * m_dVertRatio,120 * m_dHoriRatio,32 * m_dHoriRatio,SWP_NOZORDER);
#else
	m_rcManage.AddRectToDelete(935 * m_dHoriRatio,24 * m_dVertRatio,240 * m_dHoriRatio,32 * m_dHoriRatio);
	m_cbSelCrvToShow.SetWindowPos(this,935 * m_dHoriRatio,24 * m_dVertRatio,240 * m_dHoriRatio,32 * m_dHoriRatio,SWP_NOZORDER);

	m_edThreValue.SetWindowPos(this,1272 * m_dHoriRatio,24 * m_dVertRatio,200 * m_dHoriRatio,32 * m_dHoriRatio,SWP_NOZORDER);
	m_cbSelChanThre.SetWindowPos(this,1482 * m_dHoriRatio,24 * m_dVertRatio,120 * m_dHoriRatio,32 * m_dHoriRatio,SWP_NOZORDER);
#endif 

	m_rcManage.CalcRectResult();

	return;
}

void CDataAnaDlg::ShowCtrl()
{
	m_btAxisZoom.ShowWindow(SW_SHOW);
	//m_btAxisZoom2nd.ShowWindow(SW_SHOW);
	m_lstAnaResult.ShowWindow(SW_SHOW);
	m_samPlateDlg.ShowWindow(SW_SHOW);
	m_crvAmlDelRn.ShowWindow(SW_SHOW);
	m_crvStd.ShowWindow(SW_SHOW);
	m_cbSelCrvToShow.ShowWindow(SW_SHOW);
	m_cbSelChanThre.ShowWindow(SW_SHOW);
	m_edThreValue.ShowWindow(SW_SHOW);

	m_pCurSelCrvToShow = &m_crvAmlDelRn;	//�½�ҳ��ʱ��Ĭ����ʾ��������
}

void CDataAnaDlg::SetCtrlText()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	CString strTemp = pLangInfo->GetText(293);

	m_btAxisZoom.SetWindowText(strTemp);
	m_btAxisZoom2nd.SetWindowText(strTemp);

	strTemp = pLangInfo->GetText(184);
	m_crvOriginMelt.SetXAxisTag(strTemp); 
	m_crvPeakMelt.SetXAxisTag(strTemp);

	strTemp = pLangInfo->GetText(56);
	m_crvOriginAml.SetXAxisTag(strTemp);
	m_crvAmlDelRn.SetXAxisTag(strTemp);
	m_crvAml.SetXAxisTag(strTemp);

	ResetLstTitle();

	return;
}

void CDataAnaDlg::SetCrvLineParam(vector<CTubeInfo>& vecTubeInfo)
{
	//CExperimentData* pExpData = m_pParent->m_pExpData;
	CExperimentData* pExpData = m_pParent->m_pExpData;//lzh:���ߵ�����
	int iChanNum = pExpData->GetChannelCount();
	int iTubeNum = pExpData->GetPlateInfo()->GetTubeCount();
	if(iChanNum < MINIMUM_CHANNUM || iTubeNum < MINIMUM_TUBENUM)
		return;

	int iMaxLineNum = iTubeNum * iChanNum;
	m_crvOriginAml.SetLineNum(iMaxLineNum);
	m_crvAml.SetLineNum(iMaxLineNum); // lzh ������������
	m_crvAmlDelRn.SetLineNum(iMaxLineNum);
	m_crvOriginMelt.SetLineNum(iMaxLineNum);
	m_crvPeakMelt.SetLineNum(iMaxLineNum);
	m_crvStd.SetLineNum(iChanNum);

	int index = 0;
	COLORREF clrLine = 0;
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	CString strTubeLabel(_T("")),strDyeName(_T("")),strTargetName(_T(""));

	for(int i = 0;i < iTubeNum;++i)
	{
		for(int j = 0;j < iChanNum;++j)
		{
			pDyeTargetInfo = vecTubeInfo[i].GetTargetInfo(j);
			if(NULL != pDyeTargetInfo)
			{
				clrLine = pDyeTargetInfo->GetTargetColor();
				strDyeName = pDyeTargetInfo->GetDyeName();
				strTargetName = pDyeTargetInfo->GetTargetName();
			}
			else
			{
				if(0 == j)
				{
					clrLine = CHANNEL1COLOR;
				}
				else if(1 == j)
				{
					clrLine = CHANNEL2COLOR;
				}
				else if(2 == j)
				{
					clrLine = CHANNEL3COLOR;
				} 
				else if(3 == j)
				{
					clrLine = CHANNEL4COLOR;
				}
				else if(4 == j)
				{
					clrLine = CHANNEL5COLOR;
				}
				else if(5 == j)
				{
					clrLine = CHANNEL6COLOR;
				}

				strDyeName = _T("");
				strTargetName = _T("");
			}

			index = j * iTubeNum + i;

			m_crvOriginAml.SetLineParam(index,clrLine,j,FALSE);
			m_crvAml.SetLineParam(index,clrLine,j,FALSE);
			m_crvAmlDelRn.SetLineParam(index,clrLine,j,FALSE);
			m_crvOriginMelt.SetLineParam(index,clrLine,j,FALSE);
			m_crvPeakMelt.SetLineParam(index,clrLine,j,FALSE);

			{
				int displayCols = pExpData->GetColCount();
				int displayRows = pExpData->GetRowCount();
				if (displayRows == 16 && displayCols == 24)
				{
					displayCols = 12;
				}
				strTubeLabel = vecTubeInfo[i].GetTubeLabel(displayCols);
			}
			m_crvOriginAml.SetLineString(index,j,strTubeLabel,strDyeName,strTargetName);
			m_crvAml.SetLineString(index,j,strTubeLabel,strDyeName,strTargetName);
			m_crvAmlDelRn.SetLineString(index,j,strTubeLabel,strDyeName,strTargetName);
			m_crvOriginMelt.SetLineString(index,j,strTubeLabel,strDyeName,strTargetName);
			m_crvPeakMelt.SetLineString(index,j,strTubeLabel,strDyeName,strTargetName);
		}
	}	

	for(int i = 0;i < iChanNum;++i)
	{
		clrLine = 0;
		if(0 == i)
		{
			clrLine = CHANNEL1COLOR;
		}
		else if(1 == i)
		{
			clrLine = CHANNEL2COLOR;
		}
		else if(2 == i)
		{
			clrLine = CHANNEL3COLOR;
		} 
		else if(3 == i)
		{
			clrLine = CHANNEL4COLOR;
		}
		else if(4 == i)
		{
			clrLine = CHANNEL5COLOR;
		}
		else if(5 == i)
		{
			clrLine = CHANNEL6COLOR;
		}
		m_crvStd.SetLineParam(i,clrLine,i,TRUE);
		m_crvStd.SetExpressionShowState(i,TRUE);
	}

	return;
}

void CDataAnaDlg::SetCrvLineParam(vector<CTubeInfo*>& vecPTubeInfo)
{
	if(vecPTubeInfo.size() < 1)
	{
		return;
	}
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return;
	}
	CExperimentData* pExpData = m_pParent->m_pExpData;
	if(NULL == pExpData)
	{
		return;
	}

	int iChanNum = pExpData->GetChannelCount();
	int iTubeNum = pExpData->GetPlateInfo()->GetTubeCount();//lzh:��ȡ���ļ���96��
	if(iChanNum < MINIMUM_CHANNUM)
	{
		return;
	}
	if(iTubeNum < MINIMUM_TUBENUM)
	{
		return;
	}

	if(iTubeNum != vecPTubeInfo.size())
	{
		return;
	}

	int iMaxLineNum = iTubeNum * iChanNum;
	m_crvOriginAml.SetLineNum(iMaxLineNum);
	m_crvAml.SetLineNum(iMaxLineNum); // lzh�������ߵ�����
	m_crvAmlDelRn.SetLineNum(iMaxLineNum);
	m_crvOriginMelt.SetLineNum(iMaxLineNum);
	m_crvPeakMelt.SetLineNum(iMaxLineNum);
	m_crvStd.SetLineNum(iChanNum);

	int index = 0;
	COLORREF clrLine = 0;
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	CString strTubeLabel(_T("")),strDyeName(_T("")),strTargetName(_T(""));
	CTubeInfo* pTubeInfo = NULL;

	for(int i = 0;i < iTubeNum;++i)
	{
		pTubeInfo = vecPTubeInfo[i];
		if(NULL == pTubeInfo)
		{
			continue;
		}

		for(int j = 0;j < iChanNum;++j)
		{
			pDyeTargetInfo = pTubeInfo->GetTargetInfo(j);
			if(NULL != pDyeTargetInfo)
			{
				clrLine = pDyeTargetInfo->GetTargetColor();
				strDyeName = pDyeTargetInfo->GetDyeName();
				strTargetName = pDyeTargetInfo->GetTargetName();
			}
			else
			{
				if(0 == j)
				{
					clrLine = CHANNEL1COLOR;
				}
				else if(1 == j)
				{
					clrLine = CHANNEL2COLOR;
				}
				else if(2 == j)
				{
					clrLine = CHANNEL3COLOR;
				} 
				else if(3 == j)
				{
					clrLine = CHANNEL4COLOR;
				}
				else if(4 == j)
				{
					clrLine = CHANNEL5COLOR;
				}
				else if(5 == j)
				{
					clrLine = CHANNEL6COLOR;
				}

				strDyeName = _T("");
				strTargetName = _T("");
			}

			index = j * iTubeNum + i;

			m_crvOriginAml.SetLineParam(index,clrLine,j,FALSE);
			m_crvAml.SetLineParam(index,clrLine,j,FALSE);
			m_crvAmlDelRn.SetLineParam(index,clrLine,j,FALSE);
			m_crvOriginMelt.SetLineParam(index,clrLine,j,FALSE);
			m_crvPeakMelt.SetLineParam(index,clrLine,j,FALSE);

			{
				int displayCols = pExpData->GetColCount();
				int displayRows = pExpData->GetRowCount();
				if (displayRows == 16 && displayCols == 24)
				{
					displayCols = 12;
				}
				strTubeLabel = pTubeInfo->GetTubeLabel(displayCols);
			}
			m_crvOriginAml.SetLineString(index,j,strTubeLabel,strDyeName,strTargetName);
			m_crvAml.SetLineString(index,j,strTubeLabel,strDyeName,strTargetName);
			m_crvAmlDelRn.SetLineString(index,j,strTubeLabel,strDyeName,strTargetName);
			m_crvOriginMelt.SetLineString(index,j,strTubeLabel,strDyeName,strTargetName);
			m_crvPeakMelt.SetLineString(index,j,strTubeLabel,strDyeName,strTargetName);
		}
	}	

	CString strTemp(_T("")),strRealTag(_T("")),strChanId(_T(""));
	strTemp = theApp.m_pLangInfo->GetText(112);

	for(int i = 0;i < iChanNum;++i)
	{
		clrLine = 0;
		if(0 == i)
		{
			clrLine = CHANNEL1COLOR;
		}
		else if(1 == i)
		{
			clrLine = CHANNEL2COLOR;
		}
		else if(2 == i)
		{
			clrLine = CHANNEL3COLOR;
		} 
		else if(3 == i)
		{
			clrLine = CHANNEL4COLOR;
		}
		else if(4 == i)
		{
			clrLine = CHANNEL5COLOR;
		}
		else if(5 == i)
		{
			clrLine = CHANNEL6COLOR;
		}
		m_crvStd.SetLineParam(i,clrLine,i,TRUE);
		m_crvStd.SetExpressionShowState(i,TRUE);

		//����ͨ��Ⱦ����
		strRealTag = pExpData->GetOneChannelDyeName(i);
		if(strRealTag.IsEmpty())
		{
			strChanId.Format(_T("%d"),i + 1);
			strRealTag = strTemp + strChanId;
		}
		m_crvStd.SetLineParam(i,strRealTag);
	}

	return;
}

int CDataAnaDlg::SetCrvThreLineInfo()
{
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -1;
	}
	if(NULL == m_pParent->m_pExpData)
	{
		return -2;
	}
	int iChanNum = m_pParent->m_pExpData->GetChannelCount();
	if(iChanNum < 1)
	{
		return -3;
	}

	//��������������ֵ����
	m_crvAml.m_vecChanThreInfo.clear();
	m_crvAml.SetBeHaveThreLineFunc(TRUE);
	m_crvAml.SetBeUseThreLineFunc(TRUE);

	int iCalibChanIndex = m_pParent->m_pExpData->GetCalibrateChannel() - 1;

	tagAnaAmplifyPara* pAmplifyPara = NULL;
	CChanThreInfo chanThreInfo;
	chanThreInfo.m_tliNormolize.SetBeShow(TRUE);
	for(int i = 0;i < iChanNum;++i)
	{
		//У׼ͨ������ʾ��ֵ��Ϣ
		if(i == iCalibChanIndex)
		{
			continue;
		}

		chanThreInfo.m_iChanId = i;
		chanThreInfo.m_eLineDataType = ELDT_NORMALIZE;
		chanThreInfo.m_eLineShowFormat = ELSF_LINEAR;

		pAmplifyPara = m_pParent->m_pExpData->GetAmplifyParaBy(i);
		if(NULL != pAmplifyPara)
		{
			chanThreInfo.m_strChanName = pAmplifyPara->strDye;
			chanThreInfo.m_strTargetName = pAmplifyPara->strTarget;
			chanThreInfo.m_tliNormolize.m_bAutoThre = (SPLINE_THRESHOLD_AUTO == pAmplifyPara->nThresholdType);
			chanThreInfo.m_tliNormolize.m_dThreAuto = pAmplifyPara->dCalcNormalizeThreshold;

			chanThreInfo.m_tliNormolize.m_dThre = pAmplifyPara->dCalcNormalizeThreshold;
			chanThreInfo.m_tliNormolize.m_bFstDrawThreLine = TRUE;
		}

		m_crvAml.m_vecChanThreInfo.push_back(chanThreInfo);
	}
	m_crvAml.SetThreLineColor(0,CLR_THREVALUE1);
	m_crvAml.SetThreLineColor(1,CLR_THREVALUE2);
	m_crvAml.SetThreLineColor(2,CLR_THREVALUE3);
	m_crvAml.SetThreLineColor(3,CLR_THREVALUE4);
	m_crvAml.SetThreLineColor(4,CLR_THREVALUE5);
	m_crvAml.SetThreLineColor(5,CLR_THREVALUE6);

	//��������������ֵ����
	m_crvAmlDelRn.m_vecChanThreInfo.clear();
	m_crvAmlDelRn.SetBeHaveThreLineFunc(TRUE);
	m_crvAmlDelRn.SetBeUseThreLineFunc(TRUE);

	for(int i = 0;i < iChanNum;++i)
	{
		//У׼ͨ������ʾ��ֵ��Ϣ
		if(i == iCalibChanIndex)
		{
			continue;
		}

		chanThreInfo.m_iChanId = i;
		chanThreInfo.m_eLineDataType = ELDT_DELTARN;
		chanThreInfo.m_eLineShowFormat = ELSF_LINEAR;

		pAmplifyPara = m_pParent->m_pExpData->GetAmplifyParaBy(i);
		if(NULL != pAmplifyPara)
		{
			chanThreInfo.m_strChanName = pAmplifyPara->strDye;
			chanThreInfo.m_strTargetName = pAmplifyPara->strTarget;
			chanThreInfo.m_tliDeltaRn.m_bAutoThre = (SPLINE_THRESHOLD_AUTO == pAmplifyPara->nThresholdType);
			chanThreInfo.m_tliDeltaRn.m_dThreAuto = pAmplifyPara->dCalcDeltaRnThreshold;

			chanThreInfo.m_tliDeltaRn.m_dThre = pAmplifyPara->dCalcDeltaRnThreshold;
			chanThreInfo.m_tliDeltaRn.m_bFstDrawThreLine = TRUE;
		}

		m_crvAmlDelRn.m_vecChanThreInfo.push_back(chanThreInfo);
	}
	m_crvAmlDelRn.SetThreLineColor(0,CLR_THREVALUE1);
	m_crvAmlDelRn.SetThreLineColor(1,CLR_THREVALUE2);
	m_crvAmlDelRn.SetThreLineColor(2,CLR_THREVALUE3);
	m_crvAmlDelRn.SetThreLineColor(3,CLR_THREVALUE4);
	m_crvAmlDelRn.SetThreLineColor(4,CLR_THREVALUE5);
	m_crvAmlDelRn.SetThreLineColor(5,CLR_THREVALUE6);

	return 0;
}

int CDataAnaDlg::RenewNormalizeCrvThreLineInfo(BOOL bRedraw /*= FALSE*/)
{
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -1;
	}
	if(NULL == m_pParent->m_pExpData)
	{
		return -2;
	}
	int iChanNum = m_pParent->m_pExpData->GetChannelCount();
	if(iChanNum < 1)
	{
		return -3;
	}

	tagAnaAmplifyPara* pAmplifyPara = NULL;
	for(int i = 0;i < iChanNum;++i)
	{
		if(i >= m_crvAml.m_vecChanThreInfo.size())
		{
			return -4;
		}

		CChanThreInfo& chanThreInfo = m_crvAml.m_vecChanThreInfo[i];
		chanThreInfo.m_eLineShowFormat = ELSF_LINEAR;

		pAmplifyPara = m_pParent->m_pExpData->GetAmplifyParaBy(i);
		if(NULL != pAmplifyPara)
		{
			chanThreInfo.m_tliNormolize.m_bAutoThre = (SPLINE_THRESHOLD_AUTO == pAmplifyPara->nThresholdType);
			chanThreInfo.m_tliNormolize.m_dThreAuto = pAmplifyPara->dCalcNormalizeThreshold;
			chanThreInfo.m_tliNormolize.m_dThre = pAmplifyPara->dCalcNormalizeThreshold;
			chanThreInfo.m_tliNormolize.m_bFstDrawThreLine = TRUE;
		}
	}

	if(bRedraw && NULL != m_crvAml.GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CDataAnaDlg::RenewDeltaRnCrvThreLineInfo(BOOL bRedraw /*= FALSE*/)
{
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -1;
	}
	if(NULL == m_pParent->m_pExpData)
	{
		return -2;
	}
	int iChanNum = m_pParent->m_pExpData->GetChannelCount();
	if(iChanNum < 1)
	{
		return -3;
	}

	tagAnaAmplifyPara* pAmplifyPara = NULL;

	for(int i = 0;i < iChanNum;++i)
	{
		if(i >= m_crvAmlDelRn.m_vecChanThreInfo.size())
		{
			return -4;
		}

		CChanThreInfo& chanThreInfo = m_crvAmlDelRn.m_vecChanThreInfo[i];
		chanThreInfo.m_eLineShowFormat = ELSF_LINEAR;

		pAmplifyPara = m_pParent->m_pExpData->GetAmplifyParaBy(i);
		if(NULL != pAmplifyPara)
		{
			chanThreInfo.m_tliDeltaRn.m_bAutoThre = (SPLINE_THRESHOLD_AUTO == pAmplifyPara->nThresholdType);
			chanThreInfo.m_tliDeltaRn.m_dThreAuto = pAmplifyPara->dCalcDeltaRnThreshold;
			chanThreInfo.m_tliDeltaRn.m_dThre = pAmplifyPara->dCalcDeltaRnThreshold;
			chanThreInfo.m_tliDeltaRn.m_bFstDrawThreLine = TRUE;
		}
	}
	
	if(bRedraw && NULL != m_crvAmlDelRn.GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CDataAnaDlg::ThreLineModified()
{
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -1;
	}
	if(NULL == m_pParent->m_pExpData)
	{
		return -2;
	}
	int iChanNum = m_pParent->m_pExpData->GetChannelCount();
	if(iChanNum < 1)
	{
		return -3;
	}

	tagAnaAmplifyPara* pAmplifyPara = NULL;
	for(int i = 0;i < iChanNum;++i)
	{
		pAmplifyPara = m_pParent->m_pExpData->GetAmplifyParaBy(i);
		if(NULL != pAmplifyPara)
		{
			BOOL bValidChanIndex = (i < m_crvAml.m_vecChanThreInfo.size());

			if(bValidChanIndex)
			{
				m_crvAml.m_vecChanThreInfo[i].m_tliNormolize.m_bAutoThre = (SPLINE_THRESHOLD_AUTO == pAmplifyPara->nThresholdType);
				m_crvAml.m_vecChanThreInfo[i].m_tliNormolize.m_dThre = pAmplifyPara->dCalcNormalizeThreshold;
				m_crvAml.m_vecChanThreInfo[i].m_tliNormolize.m_bFstDrawThreLine = TRUE;
			}
			
			if(bValidChanIndex)
			{
				m_crvAmlDelRn.m_vecChanThreInfo[i].m_tliDeltaRn.m_bAutoThre = (SPLINE_THRESHOLD_AUTO == pAmplifyPara->nThresholdType);
				m_crvAmlDelRn.m_vecChanThreInfo[i].m_tliDeltaRn.m_dThre = pAmplifyPara->dCalcDeltaRnThreshold;
				m_crvAmlDelRn.m_vecChanThreInfo[i].m_tliDeltaRn.m_bFstDrawThreLine = TRUE;
			}
		}
	}

	return 0;
}

void CDataAnaDlg::UpdateCurveSelectState(BOOL bRedraw /*= FALSE*/)
{
	CExperimentData* pExpData = m_pParent->m_pExpData;
	int iChanNum = pExpData->GetChannelCount();
	int iTubeNum = pExpData->GetPlateInfo()->GetTubeCount();
	if(iTubeNum != m_samPlateDlg.m_iTotalSiteNum)
	{
		return;
	}

	CTubeInfo* pTubeInfo = m_samPlateDlg.GetTubeInfoPtr();
	if(NULL == pTubeInfo)
	{
		return;
	}

	BOOL bDeveloper = theApp.m_curUserInfo.GetName() == DEFAULT_DEVELOPER;
	vector<int> vecChanSelState = pExpData->GetDataAnaChannelBtSelectState();
	CDyeTargetInfo* pDyeTargetInfo = NULL; 

	int iLineId = 0;
	BOOL bShowLine = FALSE,bValidSamType = FALSE;
	for(int k = 0;k < iChanNum;++k)
	{
		BOOL bChanHaveLineShow = FALSE;
		for(int i = 0;i < iTubeNum;++i)
		{
			iLineId = k * iTubeNum + i;
			bShowLine = FALSE;

			pDyeTargetInfo = pTubeInfo[i].GetTargetInfo(k);
			if(NULL != pDyeTargetInfo && (pDyeTargetInfo->GetBeSelected() || bDeveloper))
			{
#ifdef PS96INSTRUMENT
				//PS96�������������Ϊ��������������IDΪ�գ�����ʾ��Ӧ����
				bValidSamType = !pTubeInfo[i].GetBeNoneSampleType();
				if(bValidSamType)
				{
					if(pTubeInfo[i].GetBeUnknown() && pTubeInfo[i].GetSampleID().IsEmpty())
					{
						bValidSamType = FALSE;
					}
				}
				if(bValidSamType && k < vecChanSelState.size() && (1 == vecChanSelState[k]))
#else
				if(!pTubeInfo[i].GetBeNoneSampleType() && k < vecChanSelState.size() && (1 == vecChanSelState[k]))
#endif
				{
					for(int t = 0;t < m_vecPlateSelectedSite.size();++t)
					{
						if(i == m_vecPlateSelectedSite[t])
						{
							bShowLine = TRUE;
							bChanHaveLineShow = TRUE;
							break;
						}
					}
				}
			}

			m_crvAml.SetLineShow(iLineId,bShowLine);
			m_crvAmlDelRn.SetLineShow(iLineId,bShowLine);
			m_crvOriginAml.SetLineShow(iLineId,bShowLine);
			m_crvPeakMelt.SetLineShow(iLineId,bShowLine);
			m_crvOriginMelt.SetLineShow(iLineId,bShowLine);
		}

		m_crvAml.SetBeShowThreLine(k,bChanHaveLineShow);
		m_crvAmlDelRn.SetBeShowThreLine(k,bChanHaveLineShow);

		m_crvStd.SetLineShow(k,k < vecChanSelState.size() && (1 == vecChanSelState[k]) && bChanHaveLineShow);
	}

	if(bRedraw)
	{
		if(NULL != m_crvAml.GetSafeHwnd())
		{
			m_crvAml.Invalidate(FALSE);
		}
		if(NULL != m_crvAmlDelRn.GetSafeHwnd())
		{
			m_crvAmlDelRn.Invalidate(FALSE);
		}
		if(NULL != m_crvOriginAml.GetSafeHwnd())
		{
			m_crvOriginAml.Invalidate(FALSE);
		}
		if(NULL != m_crvPeakMelt.GetSafeHwnd())
		{
			m_crvPeakMelt.Invalidate(FALSE);
		}
		if(NULL != m_crvOriginMelt.GetSafeHwnd())
		{
			m_crvOriginMelt.Invalidate(FALSE);
		}
		if(NULL != m_crvStd.GetSafeHwnd())
		{
			m_crvStd.Invalidate(FALSE);
		}
	}

	return;
}

int CDataAnaDlg::CurveAxisEdit(CMyDrawCurveDlg* pCrvCtrl,BOOL bHaveLogFunc /*= FALSE*/,BOOL bLogMode /*= FALSE*/)
{
	if(NULL == pCrvCtrl->GetSafeHwnd())
	{
		return -1;
	}

	CAxisZoomDlg axisZoomDlg(NULL,bHaveLogFunc,bLogMode);
	if(pCrvCtrl->GetBeStdCrv())
	{
		axisZoomDlg.m_defLinearAxisInfo = m_pParent->m_pExpData->GetDefaultAxisInfoAt(5);
	}

	axisZoomDlg.m_axisInfo.m_dXMin = pCrvCtrl->GetHoriStartValue();
	axisZoomDlg.m_axisInfo.m_dXMax = pCrvCtrl->GetHoriEndValue();
	axisZoomDlg.m_axisInfo.m_dYMin = pCrvCtrl->GetVertStartValue();
	axisZoomDlg.m_axisInfo.m_dYMax = pCrvCtrl->GetVertEndValue();

	if(IDOK == axisZoomDlg.DoModal())
	{
		if(axisZoomDlg.m_bChanged)
		{
			pCrvCtrl->SetHoriStartValue(axisZoomDlg.m_newAxisInfo.m_dXMin);
			pCrvCtrl->SetHoriEndValue(axisZoomDlg.m_newAxisInfo.m_dXMax);
			pCrvCtrl->SetVertStartValue(axisZoomDlg.m_newAxisInfo.m_dYMin);
			pCrvCtrl->SetVertEndValue(axisZoomDlg.m_newAxisInfo.m_dYMax,TRUE,FALSE);
		}
	}

	return 0;
}

int CDataAnaDlg::SetBaseSet()
{
	if(NULL == m_pParent->GetSafeHwnd() || NULL == m_pParent->m_pExpData)
	{
		return -1;
	}

	CAnaAmlParamDlg anaAmlParamDlg(m_pParent->m_pExpData->GetAnalysisPara());
	if(IDOK != anaAmlParamDlg.DoModal())
	{
		return 1;
	}

	if(anaAmlParamDlg.GetChanged())
	{
		//���ݴ���
		DataProcess(TRUE, 2);

		//�������߿ؼ���ֵ��Ϣ
		ThreLineModified();

		//������������������
		if(0 != SetCurveData(m_pParent->m_pExpData,TRUE))
		{

		}

		//�������
		StdLineProcess(m_pParent->m_pExpData);

		//���ý���б���Ϣ
		ResetResultLst();

		//����ҳ����ʾ����ֵ��Ϣ
		ShowThreValue();
	}

	return 0;
}

int CDataAnaDlg::SetMotChannelAdjust()
{
	if(NULL == m_pParent->GetSafeHwnd() || NULL == m_pParent->m_pExpData)
	{
		return -1;
	}

	if(NULL == m_pParent->m_pExpData->GetTurbidityPara())
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(273));
		return -2;
	}

	//�򿪴���
	CTurbidityParaDlg dlg;
	dlg.SetTurbidityPara(m_pParent->m_pExpData->GetTurbidityPara());
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_bSaveDefault)
		{
			m_pParent->m_pExpData->SaveCalibratePara();
		}

		if (dlg.m_bSetChanged)//���������ı�
		{
			//���ݴ���
			DataProcess(TRUE, 3);
			//������������������
			if(0 != SetCurveData(m_pParent->m_pExpData,TRUE))
			{

			}

			//�������
			StdLineProcess(m_pParent->m_pExpData);

			//���ý���б���Ϣ
			ResetResultLst();
		}
	}

	return 0;
}

void CDataAnaDlg::ClearPage(BOOL bRedraw /*= TRUE*/)
{
	//��շ���ҳ������
	m_samPlateDlg.DeleteAllTubeInfo();
	m_lstAnaResult.Clear(TRUE);
	m_crvAmlDelRn.DeleteLineData(TRUE);
	m_crvAml.DeleteLineData(TRUE);
	m_crvOriginAml.DeleteLineData(TRUE);
	m_crvPeakMelt.DeleteLineData(TRUE);
	m_crvOriginMelt.DeleteLineData(TRUE);
	m_crvStd.DeleteLineData(TRUE);

	//��շ���ҳ��ͨ��ѡ��ť��ѡ��״̬
	CExperimentData* pExpData = GetExpDataPtr();
	if(NULL == pExpData)
	{
		return;
	}
	pExpData->ResetDataAnaChannelBtSelectState();

	return;
}

int CDataAnaDlg::FileOpened(CExperimentData* pExpData /*= NULL*/)
{
	if(NULL == pExpData)
	{
		pExpData = GetExpDataPtr();
	}
	if(NULL == pExpData)
	{
		return -1;
	}

	ClearPage(FALSE);

	//���ļ������÷���ҳ��ͨ��Ϊȫѡ״̬
	pExpData->SetDataAnaChannelBtAllSelected();
	m_pParent->m_funcBtDlg.SetChanBtSelectState(pExpData->GetDataAnaChannelBtSelectState());

	if(pExpData->GetBeTemplateFile())
	{
		return 0;
	}

	//����������Ϣ������ѡ��״̬�ı䣬���½���б���Ϣ��
	vector<CTubeInfo*> vecPTubeInfo;
	pExpData->GetAllTubeInfoPtr(vecPTubeInfo);
	m_samPlateDlg.SetTubeInfo(vecPTubeInfo);

	//��ʼ��������Ϣ
	SetCrvLineParam(vecPTubeInfo);

	//���õ�ǰ�鿴���������ͣ���һ������DeltaRn������CExperimentData����һ��
	CheckAmlDataShowType();

	//���ݴ���
	DataProcess(TRUE);

	//������������������
	if(0 != SetCurveData(pExpData,TRUE))
	{
		return -2;
	}

	//�������ߺ�������Ϣ
	int iCrvEndValue = pExpData->GetFirstEndCollectCycleNum();
	int iAmpDataLen = pExpData->GetAmplifyDataNum();
	if(iAmpDataLen > 1 && iAmpDataLen < iCrvEndValue)
	{
		iCrvEndValue = iAmpDataLen;
	}
	if(iCrvEndValue < 1)
	{
		iCrvEndValue = CURVE_X_DEFMAXVALUE;
	}
	SetCurveHoriAxisParam(1,iCrvEndValue,1,FALSE);

	//�����۽����ߺ�������Ϣ
	double dStartTem = 60,dEndTem = 90;
	pExpData->GetFirstMeltCollectStartAndEndTemperature(dStartTem,dEndTem);
	m_crvOriginMelt.SetHoriStartValue(dStartTem);
	m_crvOriginMelt.SetHoriEndValue(dEndTem);
	m_crvPeakMelt.SetHoriStartValue(dStartTem);
	m_crvPeakMelt.SetHoriEndValue(dEndTem);

	//������������������ֵ����Ϣ
	SetCrvThreLineInfo();

	//�������
	StdLineProcess(pExpData);

	//����������ʾ״̬
	UpdateCurveSelectState(TRUE);

	//ͨ��ȫѡʱ�Ѿ������˵���ResetResultLst����
	m_samPlateDlg.SetAllSelectStateImmediately(TRUE);

	//����ҳ����ʾ����ֵ��Ϣ
	ShowThreValue();

	//���ļ�ʱ�����û��Ĭ����������Ϣ�������¼�¼ÿ�����߿ؼ���Ĭ��ֵ
	if(pExpData->GetDefaultAxisCount() < 1)
	{
		RecordDefaultAxisInfo();
	}

	//�ж���ʾ����������
	AdjudicateAmpMeltWhichToShow();

	return 0;
}

int CDataAnaDlg::ExperimentOver(CExperimentData* pExpData /*= NULL*/)
{
	if(NULL == pExpData)
	{
		pExpData = m_pParent->m_pExpData;
	}

	ClearPage(FALSE);

	//���ļ������÷���ҳ��ͨ��Ϊȫѡ״̬
	pExpData->SetDataAnaChannelBtAllSelected();
	m_pParent->m_funcBtDlg.SetChanBtSelectState(pExpData->GetDataAnaChannelBtSelectState());

	//����������Ϣ������ѡ��״̬�ı䣬���½���б���Ϣ��
	vector<CTubeInfo*> vecPTubeInfo;
	pExpData->GetAllTubeInfoPtr(vecPTubeInfo);
	m_samPlateDlg.SetTubeInfo(vecPTubeInfo);

	//����������ɫ
	SetCrvLineParam(vecPTubeInfo);

	//���ݴ���
	DataProcess(TRUE);

	//������������������
	if(0 != SetCurveData(pExpData,TRUE))
	{
		return -4;
	}

	//�����������ߺ�������Ϣ
	int iCrvEndValue = pExpData->GetFirstEndCollectCycleNum();
	int iAmpDataLen = pExpData->GetAmplifyDataNum();
	if(iAmpDataLen > 1 && iAmpDataLen < iCrvEndValue)
	{
		iCrvEndValue = iAmpDataLen;
	}
	if(iCrvEndValue < 1)
	{
		iCrvEndValue = CURVE_X_DEFMAXVALUE;
	}
	SetCurveHoriAxisParam(1,iCrvEndValue,1,FALSE);

	//�����۽����ߺ�������Ϣ
	double dStartTem = 60,dEndTem = 90;
	pExpData->GetFirstMeltCollectStartAndEndTemperature(dStartTem,dEndTem);
	m_crvOriginMelt.SetHoriStartValue(dStartTem);
	m_crvOriginMelt.SetHoriEndValue(dEndTem);
	m_crvPeakMelt.SetHoriStartValue(dStartTem);
	m_crvPeakMelt.SetHoriEndValue(dEndTem);

	//������������������ֵ����Ϣ
	SetCrvThreLineInfo();

	//�������
	StdLineProcess(pExpData);

	//����������ʾ״̬
	UpdateCurveSelectState(TRUE);

	//ͨ��ȫѡʱ�����ResetResultLst����
	m_samPlateDlg.SetAllSelectStateImmediately(TRUE);

	//����ҳ����ʾ����ֵ��Ϣ
	ShowThreValue();

	//�ж���ʾ����������
	AdjudicateAmpMeltWhichToShow();

	return 0;
}

int CDataAnaDlg::ReAnalysed(CExperimentData* pExpData /*= NULL*/)
{
	//����������Ϣ������ѡ��״̬�ı䣬���½���б���Ϣ��
	m_samPlateDlg.SetTubeInfo(pExpData->GetTubeInfoArrayPtr(),TRUE);

	//����������ɫ
	vector<CTubeInfo*> vecPTubeInfo;
	pExpData->GetAllTubeInfoPtr(vecPTubeInfo);
	SetCrvLineParam(vecPTubeInfo);

	//���ݴ���
	DataProcess(TRUE);

	//������������������
	if(0 != SetCurveData(pExpData))
	{
		return -2;
	}

	//�������
	StdLineProcess(pExpData);

	//����������ʾ״̬
	UpdateCurveSelectState(TRUE);

	//���½���б���Ϣ
	ResetResultLst();

	//����ҳ����ʾ����ֵ��Ϣ
	ShowThreValue();

	ResetCurveMaxMinValue(TRUE);

	return 0;
}

int CDataAnaDlg::SetCurveData(CExperimentData* pExpData,BOOL bRedraw /*= FALSE*/)
{
	if(NULL == pExpData)
	{
		return -1;
	}

	eDataType eDtFilter = FLU_FILTER,eDtTypeNormalize = FLU_NORMALIZATE,eDtTypeDeltaRn = FLU_DELTARN;

	//���������������ֵ����С���ơ��Ƿ�ȡ�� 
	BOOL bShieldYAxisMinOfMax = FALSE;
	if(pExpData->GetNormalizeSelectLogState())
	{
		bShieldYAxisMinOfMax = TRUE;
		eDtTypeNormalize = LOG_FLU_NORMALIZATE;
	}
	m_crvAml.SetBeShieldYAxisMinOfMax(bShieldYAxisMinOfMax);

	//���������������ֵ����С���ơ��Ƿ�ȡ�� 
	bShieldYAxisMinOfMax = FALSE;
	int iVertDecimalLen = 0;
	if(pExpData->GetDeltaRnSelectLogState())
	{
		bShieldYAxisMinOfMax = TRUE;
		iVertDecimalLen = 2;
		eDtTypeDeltaRn = LOG_FLU_DELTARN;
	}
	m_crvAmlDelRn.SetVertDecimalPointLength(iVertDecimalLen);
	m_crvAmlDelRn.SetBeShieldYAxisMinOfMax(bShieldYAxisMinOfMax);

	//���������������ֵ����С���ơ��Ƿ�ȡ�� 
	bShieldYAxisMinOfMax = FALSE;
	iVertDecimalLen = 0;
	if(pExpData->GetFilterSelectLogState())
	{
		bShieldYAxisMinOfMax = TRUE;
		iVertDecimalLen = 2;
		eDtFilter = LOG_FLU_TURBIDITY;
	}
	m_crvOriginAml.SetVertDecimalPointLength(iVertDecimalLen);
	m_crvOriginAml.SetBeShieldYAxisMinOfMax(bShieldYAxisMinOfMax);

	int iChanNum = pExpData->GetChannelCount();
	if (theApp.GetBeAdminPermission() || theApp.GetBeIVDUser())
		iChanNum --;
	int iTubeNum = pExpData->GetPlateInfo()->GetTubeCount();
	int iAmpDataLen = pExpData->GetAmplifyDataNum();
	int iMeltDataLen = pExpData->GetMeltDataNum();
	int iLineId = 0;
	CMyPoint pt;

	double *pDX = NULL,*pDY = NULL,*pDYGain = NULL,*pDIndex = NULL;
	if(iMeltDataLen > 0)
	{
		pDX = new double[iMeltDataLen];
		if(NULL == pDX)
		{
			return -2;
		}

		pDY = new double[iMeltDataLen];
		if(NULL == pDY)
		{
			delete [] pDX;
			return -3;
		}

		pDYGain = new double[iMeltDataLen];
		if(NULL == pDYGain)
		{
			delete [] pDX;
			delete [] pDY;
			return -4;
		}
		
		pDIndex = new double[iMeltDataLen];
		if(NULL == pDIndex)
		{
			delete [] pDX;
			delete [] pDY;
			delete [] pDYGain;
			return -5;
		}

		int iTempSize = sizeof(double) * iMeltDataLen;
		memset(pDX,0,iTempSize);
		memset(pDY,0,iTempSize);
		memset(pDYGain,0,iTempSize);
		for(int i = 0;i < iMeltDataLen;++i)
		{
			pDIndex[i] = i + 1;
		}
	}

	CTubeInfo* pTubeInfo = NULL;
	
	m_crvOriginAml.DeleteLineData(FALSE);
	m_crvAml.DeleteLineData(FALSE);
	m_crvAmlDelRn.DeleteLineData(FALSE);
	m_crvPeakMelt.DeleteLineData(FALSE);
	m_crvOriginMelt.DeleteLineData(FALSE);

	for(int j = 0;j < iTubeNum;++j)
	{
		pTubeInfo = pExpData->GetTubeInfoAt(j);
		if(NULL == pTubeInfo)
		{
			//��¼������Ϣ
			continue;
		}
		if(SAMPLE_TYPE_NONE == pTubeInfo->GetSampleType())
		{
			continue;
		}

		for(int i = 0;i < iChanNum;++i)
		{
			iLineId = i * iTubeNum + j;
			for(int k = 0;k < iAmpDataLen;++k)
			{
				pt.m_X = k + 1;
				pt.m_Y = pExpData->GetAmplifyChannelCycleFluY(j + 1,eDtFilter,i,k);
				m_crvOriginAml.AddNewDot(iLineId,pt);

				pt.m_Y = pExpData->GetAmplifyChannelCycleFluY(j + 1,eDtTypeNormalize,i,k);
				if(_isnan(pt.m_Y))
				{
					int tt = 0;
					++tt;
					continue;
				}
				m_crvAml.AddNewDot(iLineId,pt);

				pt.m_Y = pExpData->GetAmplifyChannelCycleFluY(j + 1,eDtTypeDeltaRn,i,k);
				if(_isnan(pt.m_Y))
				{
					int tt = 0;
					++tt;
					continue;
				}
				m_crvAmlDelRn.AddNewDot(iLineId,pt);
			}

			if(iMeltDataLen > 0)
			{
				pExpData->GetMeltCycleX(j + 1,iMeltDataLen,pDX);
				pExpData->GetMeltChannelFluY(j + 1,FLU_FILTER,i,iMeltDataLen,pDY);
				pExpData->GetMeltChannelFluY(j + 1,MELT_GAINDATA,i,iMeltDataLen,pDYGain);

				//���¶Ƚ��д�������ֹ���ߴ��
				double dSlope = 0,dIntercept = 0;
				if(iMeltDataLen > 1 && 0 == LeastSquare(pDIndex,pDX,iMeltDataLen,dSlope,dIntercept))
				{
					for(int k = 0;k < iMeltDataLen;++k)
					{
						pDX[k] = pDIndex[k] * dSlope + dIntercept;
					}
				}

				for(int k = 0;k < iMeltDataLen;++k)
				{
					pt.m_X = pDX[k];
					pt.m_Y = pDY[k];
					m_crvOriginMelt.AddNewDot(iLineId,pt);
					pt.m_Y = pDYGain[k];
					m_crvPeakMelt.AddNewDot(iLineId,pt);
				}
			}
		}	
	}

	if(iAmpDataLen > 0)
	{
		m_crvAml.HandSetGeneralLineXYAxisMinMax();
		m_crvAmlDelRn.HandSetGeneralLineXYAxisMinMax();
		m_crvOriginAml.HandSetGeneralLineXYAxisMinMax();
	}
	if(iMeltDataLen > 0)
	{
		m_crvPeakMelt.HandSetGeneralLineXYAxisMinMax();
		m_crvOriginMelt.HandSetGeneralLineXYAxisMinMax();
	}

	if(bRedraw)
	{
		if(NULL != m_crvAml.GetSafeHwnd())
		{
			m_crvAml.Invalidate(FALSE);
		}
		if(NULL != m_crvAmlDelRn.GetSafeHwnd())
		{
			m_crvAmlDelRn.Invalidate(FALSE);
		}
		if(NULL != m_crvOriginAml.GetSafeHwnd())
		{
			m_crvOriginAml.Invalidate(FALSE);
		}
		if(NULL != m_crvPeakMelt.GetSafeHwnd())
		{
			m_crvPeakMelt.Invalidate(FALSE);
		}
		if(NULL != m_crvOriginMelt.GetSafeHwnd())
		{
			m_crvOriginMelt.Invalidate(FALSE);
		}
	}

	if(iMeltDataLen > 0)
	{
		delete [] pDX;
		delete [] pDY;
		delete [] pDYGain;
		delete [] pDIndex;
	}

	return 0;
}

int CDataAnaDlg::SetMeltCurveData(CExperimentData* pExpData,BOOL bRedraw /*= FALSE*/)
{
	if(NULL == pExpData)
	{
		return -1;
	}

	int iChanNum = pExpData->GetChannelCount();
	if (theApp.GetBeAdminPermission() || theApp.GetBeIVDUser())
		iChanNum --;

	int iTubeNum = pExpData->GetPlateInfo()->GetTubeCount();
	int iMeltDataLen = pExpData->GetMeltDataNum();
	int iLineId = 0;
	CMyPoint pt;

	m_crvPeakMelt.DeleteLineData(FALSE);
	m_crvOriginMelt.DeleteLineData(FALSE);

	double *pDX = NULL,*pDY = NULL,*pDYGain = NULL,*pDIndex = NULL;

	if(iMeltDataLen < 1)
	{
		goto REDRAWORNOT;
	}

	pDX = new double[iMeltDataLen];
	if(NULL == pDX)
	{
		goto REDRAWORNOT;
	}

	pDY = new double[iMeltDataLen];
	if(NULL == pDY)
	{
		goto REDRAWORNOT;
	}

	pDYGain = new double[iMeltDataLen];
	if(NULL == pDYGain)
	{
		goto REDRAWORNOT;
	}

	pDIndex = new double[iMeltDataLen];
	if(NULL == pDIndex)
	{
		goto REDRAWORNOT;
	}

	int iTempSize = sizeof(double) * iMeltDataLen;
	memset(pDX,0,iTempSize);
	memset(pDY,0,iTempSize);
	memset(pDYGain,0,iTempSize);
	for(int i = 0;i < iMeltDataLen;++i)
	{
		pDIndex[i] = i + 1;
	}

	CTubeInfo* pTubeInfo = NULL;
	for(int j = 0;j < iTubeNum;++j)
	{
		pTubeInfo = pExpData->GetTubeInfoAt(j);
		if(NULL == pTubeInfo)
		{
			continue;
		}
		if(SAMPLE_TYPE_NONE == pTubeInfo->GetSampleType())
		{
			continue;
		}

		for(int i = 0;i < iChanNum;++i)
		{
			iLineId = i * iTubeNum + j;

			pExpData->GetMeltCycleX(j + 1,iMeltDataLen,pDX);
			pExpData->GetMeltChannelFluY(j + 1,FLU_FILTER,i,iMeltDataLen,pDY);
			pExpData->GetMeltChannelFluY(j + 1,MELT_GAINDATA,i,iMeltDataLen,pDYGain);

			//���¶Ƚ��д�������ֹ���ߴ��
			double dSlope = 0,dIntercept = 0;
			if(iMeltDataLen > 1 && 0 == LeastSquare(pDIndex,pDX,iMeltDataLen,dSlope,dIntercept))
			{
				for(int k = 0;k < iMeltDataLen;++k)
				{
					pDX[k] = pDIndex[k] * dSlope + dIntercept;
				}
			}

			for(int k = 0;k < iMeltDataLen;++k)
			{
				pt.m_X = pDX[k];
				pt.m_Y = pDY[k];
				m_crvOriginMelt.AddNewDot(iLineId,pt);
				pt.m_Y = pDYGain[k];
				m_crvPeakMelt.AddNewDot(iLineId,pt);
			}
		}	
	}

	m_crvPeakMelt.HandSetGeneralLineXYAxisMinMax();
	m_crvOriginMelt.HandSetGeneralLineXYAxisMinMax();

REDRAWORNOT:
	if(bRedraw)
	{
		if(NULL != m_crvPeakMelt.GetSafeHwnd())
		{
			m_crvPeakMelt.Invalidate(FALSE);
		}
		if(NULL != m_crvOriginMelt.GetSafeHwnd())
		{
			m_crvOriginMelt.Invalidate(FALSE);
		}
	}

	if(NULL != pDX)
	{
		delete [] pDX;
	}
	if(NULL != pDY)
	{
		delete [] pDY;
	}
	if(NULL != pDYGain)
	{
		delete [] pDYGain;
	}
	if(NULL != pDIndex)
	{
		delete [] pDIndex;
	}

	return 0;
}

int CDataAnaDlg::SetNormalizeCurveData(CExperimentData* pExpData,BOOL bRedraw /*= FALSE*/)
{
	if(NULL == pExpData)
	{
		return -1;
	}

	//���������������ֵ����С���ơ��Ƿ�ȡ�� 
	BOOL bShieldYAxisMinOfMax = FALSE,bNormSelLog = pExpData->GetNormalizeSelectLogState();
	if(bNormSelLog)
	{
		bShieldYAxisMinOfMax = TRUE;
	}
	m_crvAml.SetBeShieldYAxisMinOfMax(bShieldYAxisMinOfMax);

	int iChanNum = pExpData->GetChannelCount();
	if (theApp.GetBeAdminPermission() || theApp.GetBeIVDUser())
		iChanNum --;
	int iTubeNum = pExpData->GetPlateInfo()->GetTubeCount();
	int iAmpDataLen = pExpData->GetAmplifyDataNum();
	int iLineId = 0;
	CMyPoint pt;
	CTubeInfo* pTubeInfo = NULL;
	eDataType eDtType = FLU_NORMALIZATE;

	if(bNormSelLog)
	{
		eDtType = LOG_FLU_NORMALIZATE;
	}

	m_crvAml.DeleteLineData(FALSE);

	for(int j = 0;j < iTubeNum;++j)
	{
		pTubeInfo = pExpData->GetTubeInfoAt(j);
		if(NULL == pTubeInfo)
		{
			//��¼������Ϣ
			continue;
		}
		if(SAMPLE_TYPE_NONE == pTubeInfo->GetSampleType())
		{
			continue;
		}

		for(int i = 0;i < iChanNum;++i)
		{
			iLineId = i * iTubeNum + j;
			for(int k = 0;k < iAmpDataLen;++k)
			{
				pt.m_X = k + 1;
			
				pt.m_Y = pExpData->GetAmplifyChannelCycleFluY(j + 1,eDtType,i,k);
				if(_isnan(pt.m_Y))
				{
					int tt = 0;
					++tt;
					continue;
				}
				m_crvAml.AddNewDot(iLineId,pt);
			}
		}	
	}

	if(iAmpDataLen > 0)
	{
		if(bNormSelLog)
		{
			m_crvAml.SetMinYDecrementParam(FALSE,0.1,0.02);
		}
		else
		{
			m_crvAml.SetMinYDecrementParam(TRUE,0.1,0.02);
		}
		m_crvAml.HandSetGeneralLineXYAxisMinMax();
	}

	if(bRedraw && NULL != m_crvAml.GetSafeHwnd())
	{
		m_crvAml.Invalidate(FALSE);
	}

	return 0;
}

int CDataAnaDlg::SetDeltaRnCurveData(CExperimentData* pExpData,BOOL bRedraw /*= FALSE*/)
{
	if(NULL == pExpData)
	{
		return -1;
	}

	//���������������ֵ����С���ơ��Ƿ�ȡ�� 
	BOOL bShieldYAxisMinOfMax = FALSE,bDeltaRnSelLog = pExpData->GetDeltaRnSelectLogState();
	int iVertDecimalLen = 0;
	if(bDeltaRnSelLog)
	{
		bShieldYAxisMinOfMax = TRUE;
		iVertDecimalLen = 2;
	}
	m_crvAmlDelRn.SetVertDecimalPointLength(iVertDecimalLen);
	m_crvAmlDelRn.SetBeShieldYAxisMinOfMax(bShieldYAxisMinOfMax);

	int iChanNum = pExpData->GetChannelCount();
	if (theApp.GetBeAdminPermission() || theApp.GetBeIVDUser())
		iChanNum --;

	int iTubeNum = pExpData->GetPlateInfo()->GetTubeCount();
	int iAmpDataLen = pExpData->GetAmplifyDataNum();
	int iLineId = 0;
	CMyPoint pt;
	CTubeInfo* pTubeInfo = NULL;
	eDataType eDtType = FLU_DELTARN;

	if(bDeltaRnSelLog)
	{
		eDtType = LOG_FLU_DELTARN;
	}

	m_crvAmlDelRn.DeleteLineData(FALSE);

	for(int j = 0;j < iTubeNum;++j)
	{
		pTubeInfo = pExpData->GetTubeInfoAt(j);
		if(NULL == pTubeInfo)
		{
			//��¼������Ϣ
			continue;
		}
		if(SAMPLE_TYPE_NONE == pTubeInfo->GetSampleType())
		{
			continue;
		}

		for(int i = 0;i < iChanNum;++i)
		{
			iLineId = i * iTubeNum + j;
			for(int k = 0;k < iAmpDataLen;++k)
			{
				pt.m_X = k + 1;

				pt.m_Y = pExpData->GetAmplifyChannelCycleFluY(j + 1,eDtType,i,k);
				if(_isnan(pt.m_Y))
				{
					int tt = 0;
					++tt;
					continue;
				}
				m_crvAmlDelRn.AddNewDot(iLineId,pt);
			}
		}	
	}

	if(iAmpDataLen > 0)
	{
		if(bDeltaRnSelLog)
		{
			m_crvAmlDelRn.SetMinYDecrementParam(FALSE,100,100);
		}
		else
		{
			m_crvAmlDelRn.SetMinYDecrementParam(TRUE,100,100);
		}
		m_crvAmlDelRn.HandSetGeneralLineXYAxisMinMax();
	}

	if(bRedraw && NULL != m_crvAmlDelRn.GetSafeHwnd())
	{
		m_crvAmlDelRn.Invalidate(FALSE);
	}

	return 0;
}

int CDataAnaDlg::SetFilterCurveData(CExperimentData* pExpData,BOOL bRedraw /*= FALSE*/)
{
	if(NULL == pExpData)
	{
		return -1;
	}

	//���������������ֵ����С���ơ��Ƿ�ȡ�� 
	BOOL bShieldYAxisMinOfMax = FALSE;
	int iVertDecimalLen = 0;
	if(pExpData->GetFilterSelectLogState())
	{
		bShieldYAxisMinOfMax = TRUE;
		iVertDecimalLen = 2;
	}
	m_crvOriginAml.SetVertDecimalPointLength(iVertDecimalLen);
	m_crvOriginAml.SetBeShieldYAxisMinOfMax(bShieldYAxisMinOfMax);

	int iChanNum = pExpData->GetChannelCount();
	if (theApp.GetBeAdminPermission() || theApp.GetBeIVDUser())
		iChanNum --;
	int iTubeNum = pExpData->GetPlateInfo()->GetTubeCount();
	int iAmpDataLen = pExpData->GetAmplifyDataNum();
	int iLineId = 0;
	CMyPoint pt;
	CTubeInfo* pTubeInfo = NULL;
	eDataType eDtType = FLU_FILTER;

	if(pExpData->GetFilterSelectLogState())
	{
		eDtType = LOG_FLU_TURBIDITY;
	}

	m_crvOriginAml.DeleteLineData(FALSE);

	for(int j = 0;j < iTubeNum;++j)
	{
		pTubeInfo = pExpData->GetTubeInfoAt(j);
		if(NULL == pTubeInfo)
		{
			//��¼������Ϣ
			continue;
		}
		if(SAMPLE_TYPE_NONE == pTubeInfo->GetSampleType())
		{
			continue;
		}

		for(int i = 0;i < iChanNum;++i)
		{
			iLineId = i * iTubeNum + j;
			for(int k = 0;k < iAmpDataLen;++k)
			{
				pt.m_X = k + 1;

				pt.m_Y = pExpData->GetAmplifyChannelCycleFluY(j + 1,eDtType,i,k);
				if(_isnan(pt.m_Y))
				{
					int tt = 0;
					++tt;
					continue;
				}
				m_crvOriginAml.AddNewDot(iLineId,pt);
			}
		}	
	}

	if(iAmpDataLen > 0)
	{
		m_crvOriginAml.HandSetGeneralLineXYAxisMinMax();
	}

	if(bRedraw && NULL != m_crvOriginAml.GetSafeHwnd())
	{
		m_crvOriginAml.Invalidate(FALSE);
	}

	return 0;
}

void CDataAnaDlg::SetCurveHoriAxisParam(int iStart,int iEnd,int iHoriStep,BOOL bRedraw /*= TRUE*/)
{
	m_crvOriginAml.SetHoriAxisParam(iStart,iEnd,iHoriStep,bRedraw);
	m_crvAml.SetHoriAxisParam(iStart,iEnd,iHoriStep,bRedraw);
	m_crvAmlDelRn.SetHoriAxisParam(iStart,iEnd,iHoriStep,bRedraw);
}

int CDataAnaDlg::DataProcess(BOOL bNewOver,int iAmlFlag /*= 5*/)
{
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -1;
	}

	CExperimentData* pExpData = m_pParent->m_pExpData;
	if(NULL == pExpData)
	{
		return -2;
	}

	int iAmpDataLen = pExpData->GetAmplifyDataNum();
	int iMeltDataLen = pExpData->GetMeltDataNum();

	if(iAmpDataLen > 0)
	{
		if(!pExpData->ProcessAmplifyFluData(iAmlFlag))
		{
			return -3;
		}
	}

	if(iMeltDataLen > 0)
	{
		if(!pExpData->ProcessMeltFluData())
		{
			return -4;
		}
	}

	eDataType eDtType = FLU_ORIGINAL;

	if(0 == m_iPreCbSelIndex)
	{
		eDtType = FLU_DELTARN;
		if(pExpData->GetDeltaRnSelectLogState())
		{
			eDtType = LOG_FLU_DELTARN;
		}
		pExpData->GenerateCt(bNewOver,METHOD_CT_SPLINE,eDtType);
	}
	else if(1 == m_iPreCbSelIndex)
	{
		eDtType = FLU_NORMALIZATE;
		if(pExpData->GetNormalizeSelectLogState())
		{
			eDtType = LOG_FLU_NORMALIZATE;
		}
		pExpData->GenerateCt(bNewOver,METHOD_CT_SPLINE,eDtType);
	}

	return 0;
}

void CDataAnaDlg::ResetResultLst()
{
	if(m_vecPlateSelectState.size() < 1)
	{
		return;
	}

	CExperimentData* pExpData = m_pParent->m_pExpData;
	if(NULL == pExpData)
	{
		//��¼������Ϣ
		return;
	}

	int iTubeCount = pExpData->GetTubeCount();
	if(iTubeCount < 1 || iTubeCount != m_samPlateDlg.m_iTotalSiteNum)
	{
		//��¼������Ϣ
		return;
	}

	m_lstAnaResult.Clear();

	CString strCt(_T(""));
	CTubeInfo* pTubeInfo = NULL;

	int iChanNum = pExpData->GetChannelCount();
	if (theApp.GetBeAdminPermission() || theApp.GetBeIVDUser())
		iChanNum --;
	int iRowCount = pExpData->GetRowCount();
	int iColCount = pExpData->GetColCount();
	int iAmpDataLen = pExpData->GetAmplifyDataNum();

	vector<int> vecChanSelState = pExpData->GetDataAnaChannelBtSelectState();

	ESAMSORTWAY eSamSortWay = ESSW_HORI;

	int iTubeIndex = 0;
	int kEnd = 0,tEnd = 0;
	int index1 = 0,index2 = 0;
	if(ESSW_HORI == eSamSortWay)
	{
		kEnd = iRowCount;
		tEnd = iColCount;
	}
	else
	{
		kEnd = iColCount;
		tEnd = iRowCount;
	}

	for(int k = 0;k < kEnd;++k)
	{
		for(int t = 0;t < tEnd;++t)
		{
			if(ESSW_HORI == eSamSortWay)
			{
				iTubeIndex = k * iColCount + t;
				index1 = k;
				index2 = t + 1;
			}
			else
			{
				iTubeIndex = t * iColCount + k;
				index1 = t;
				index2 = k + 1;
			}

			pTubeInfo = pExpData->GetTubeInfoAt(iTubeIndex);
			if(NULL == pTubeInfo)
			{
				continue;
			}

			for(int i = 0;i < iChanNum;++i)
			{
				if(!m_vecPlateSelectState[iTubeIndex] || pTubeInfo->GetBeNoneSampleType() || (1 != vecChanSelState[i]))
				{
					continue;
				}
				
				CDyeTargetInfo* pDyeTargetInfo = pTubeInfo->GetTargetInfo(i);
				if(NULL == pDyeTargetInfo || !pDyeTargetInfo->GetBeSelected())
				{
					continue;
				}
				
				double dCt = pDyeTargetInfo->GetCt();
				if(dCt < 1 || dCt >= iAmpDataLen)
				{
					strCt = _T("N/A");
				}
				else
				{
					strCt.Format(_T("%.2lf"),dCt);
				}

				//��ʾ��ϸ��Ϣ
				AddOneResult(*pTubeInfo,i,strCt,pDyeTargetInfo->GetTm1String());
			}
		}
	}

	m_lstAnaResult.Invalidate(FALSE);

	return;
}

void CDataAnaDlg::LstResultLanguageChanged()
{
	int iLstRowNum = m_lstAnaResult.GetRowNum();
	if(iLstRowNum < 1)
	{
		return;
	}

	if(m_vecPlateSelectState.size() < 1)
	{
		return;
	}

	CExperimentData* pExpData = m_pParent->m_pExpData;
	if(NULL == pExpData)
	{
		//��¼������Ϣ
		return;
	}

	int iTubeCount = pExpData->GetTubeCount();
	if(iTubeCount < 1 || iTubeCount != m_samPlateDlg.m_iTotalSiteNum)
	{
		//��¼������Ϣ
		return;
	}

	CTubeInfo* pTubeInfo = NULL;
	int iChanNum = pExpData->GetChannelCount(); 
	int iRowCount = pExpData->GetRowCount();
	int iColCount = pExpData->GetColCount();

	vector<int> vecChanSelState = pExpData->GetDataAnaChannelBtSelectState();

	ESAMSORTWAY eSamSortWay = ESSW_HORI;

	int iTubeIndex = 0,iIndex = 0;
	int kEnd = 0,tEnd = 0;
	int index1 = 0,index2 = 0;
	if(ESSW_HORI == eSamSortWay)
	{
		kEnd = iRowCount;
		tEnd = iColCount;
	}
	else
	{
		kEnd = iColCount;
		tEnd = iRowCount;
	}

	for(int k = 0;k < kEnd;++k)
	{
		for(int t = 0;t < tEnd;++t)
		{
			if(ESSW_HORI == eSamSortWay)
			{
				iTubeIndex = k * iColCount + t;
				index1 = k;
				index2 = t + 1;
			}
			else
			{
				iTubeIndex = t * iColCount + k;
				index1 = t;
				index2 = k + 1;
			}

			pTubeInfo = pExpData->GetTubeInfoAt(iTubeIndex);
			if(NULL == pTubeInfo)
			{
				continue;
			}

			for(int i = 0;i < iChanNum;++i)
			{
				if(!m_vecPlateSelectState[iTubeIndex] || pTubeInfo->GetBeNoneSampleType() || (1 != vecChanSelState[i]))
				{
					continue;
				}

				CDyeTargetInfo* pDyeTargetInfo = pTubeInfo->GetTargetInfo(i);
				if(NULL == pDyeTargetInfo || !pDyeTargetInfo->GetBeSelected())
				{
					continue;
				}

				m_lstAnaResult.SetItemText(iIndex++,2,CMySamPalteDlg::SampleTypeToText(pTubeInfo->GetSampleType()));
			}
		}
	}

	return;
}

void CDataAnaDlg::AddOneResult(CTubeInfo& tubeInfo,int iChanIndex,CString strCt,CString strTm /*= _T("")*/)
{
	int lstAnaRstItemCount = m_lstAnaResult.GetRowNum();
	CString strTemp(_T("")); 
	strTemp.Format(_T("%d"),lstAnaRstItemCount + 1);

	m_lstAnaResult.InsertRowSetHeight(lstAnaRstItemCount,30 * m_dVertRatio);
	m_lstAnaResult.SetItemText(lstAnaRstItemCount,0,strTemp);
	
	int iColCount = 12;
	if(NULL != m_pParent->GetSafeHwnd() && NULL != m_pParent->m_pExpData)
	{
		iColCount = m_pParent->m_pExpData->GetColCount();
	}
	m_lstAnaResult.SetItemText(lstAnaRstItemCount,1,tubeInfo.GetTubeLabel(iColCount));
	m_lstAnaResult.SetItemText(lstAnaRstItemCount,2,CMySamPalteDlg::SampleTypeToText(tubeInfo.GetSampleType()));
	m_lstAnaResult.SetItemText(lstAnaRstItemCount,3,tubeInfo.GetSampleID());
	m_lstAnaResult.SetItemText(lstAnaRstItemCount,4,tubeInfo.GetDyeName(iChanIndex));
	m_lstAnaResult.SetItemText(lstAnaRstItemCount,5,tubeInfo.GetTargetName(iChanIndex));
	m_lstAnaResult.SetItemText(lstAnaRstItemCount,6,strCt);	//Ctֵ
	m_lstAnaResult.SetItemText(lstAnaRstItemCount,7,strTm);	//Tm1
	m_lstAnaResult.SetItemText(lstAnaRstItemCount,8,_T(""));	//Tm2
	m_lstAnaResult.SetItemText(lstAnaRstItemCount,9,tubeInfo.GetCalculatedDensityString(iChanIndex,theApp.m_iStdDensityDpd));	//�����Ũ��
	
	if(tubeInfo.GetBeNeedDensity())
	{
		strTemp = tubeInfo.GetDensityString(iChanIndex,theApp.m_iStdDensityDpd);
	}
	else
	{
		strTemp.Empty();
	}
	m_lstAnaResult.SetItemText(lstAnaRstItemCount,10,strTemp);

	m_lstAnaResult.SetItemText(lstAnaRstItemCount,11,tubeInfo.GetSampleName());

	strTemp.Empty();
	if(NULL != m_pParent && NULL != m_pParent->m_pExpData)
	{
		strTemp = m_pParent->m_pExpData->GetProjectName();
	}
	m_lstAnaResult.SetItemText(lstAnaRstItemCount,12,strTemp);

	m_lstAnaResult.SetItemColorParam(lstAnaRstItemCount,13,tubeInfo.GetSampleDyeTargetColor(iChanIndex),12 * m_dHoriRatio,12 * m_dVertRatio,2);	//��ɫ
	m_lstAnaResult.SetItemText(lstAnaRstItemCount,14,_T(""));	//����
	m_lstAnaResult.SetItemText(lstAnaRstItemCount,25,tubeInfo.GetComments());

	return;
}

int CDataAnaDlg::NewResultCalculated(int iChanId)
{
	if(iChanId < 0)
	{
		return -1;
	}
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -2;
	}
	if(m_vecPlateSelectState.size() < 1)
	{
		return -3;
	}
	int iRowNum = m_lstAnaResult.GetRowNum();
	if(iRowNum < 1)
	{
		return -4;
	}
	if(NULL == m_pParent->m_pExpData)
	{
		return -5;
	}
	CExperimentData* pExpData = m_pParent->m_pExpData;
	int iTubeCount = pExpData->GetTubeCount();
	if(iTubeCount < 1 || iTubeCount != m_samPlateDlg.m_iTotalSiteNum)
	{
		//��¼������Ϣ
		return -6;
	}

	CTubeInfo* pTubeInfo = NULL;
	CString strCt(_T(""));

	int iChanNum = pExpData->GetChannelCount();
	int iRowCount = pExpData->GetPlateInfo()->GetPlateInfo()->iHoriCount;
	int iColCount = pExpData->GetPlateInfo()->GetPlateInfo()->iVertCount;
	int iAmpDataLen = pExpData->GetAmplifyDataNum();

	vector<int> vecChanSelState = pExpData->GetDataAnaChannelBtSelectState();
	ESAMSORTWAY eSamSortWay = ESSW_HORI;

	int iTubeIndex = 0,iLstRowIndex = 0;
	int kEnd = 0,tEnd = 0;
	int index1 = 0,index2 = 0;
	if(ESSW_HORI == eSamSortWay)
	{
		kEnd = iRowCount;
		tEnd = iColCount;
	}
	else
	{
		kEnd = iColCount;
		tEnd = iRowCount;
	}

	for(int k = 0;k < kEnd;++k)
	{
		for(int t = 0;t < tEnd;++t)
		{
			if(ESSW_HORI == eSamSortWay)
			{
				iTubeIndex = k * iColCount + t;
				index1 = k;
				index2 = t + 1;
			}
			else
			{
				iTubeIndex = t * iColCount + k;
				index1 = t;
				index2 = k + 1;
			}

			pTubeInfo = pExpData->GetTubeInfoAt(iTubeIndex);
			if(NULL == pTubeInfo)
			{
				continue;
			}

			for(int i = 0;i < iChanNum;++i)
			{
				BOOL bSelected = FALSE;
				if(!m_vecPlateSelectState[iTubeIndex] || SAMPLE_TYPE_NONE == pTubeInfo->GetSampleType() || (1 != vecChanSelState[i]))
				{
					continue;
				}

				CDyeTargetInfo* pDyeTargetInfo = pTubeInfo->GetTargetInfo(i);
				if(NULL == pDyeTargetInfo || !pDyeTargetInfo->GetBeSelected())
				{
					continue;
				}

				if(i != iChanId)
				{
					++iLstRowIndex;
					continue;
				}

				double dCt = pDyeTargetInfo->GetCt();
				if(dCt < 1 || dCt >= iAmpDataLen)
				{
					strCt = STR_NOCT;
				}
				else
				{
					strCt.Format(_T("%.2lf"),dCt);
				}

				//��ʾ��ϸ��Ϣ
				m_lstAnaResult.SetItemText(iLstRowIndex,CT_COLUMN_INDEX,strCt);
				m_lstAnaResult.SetItemText(iLstRowIndex,CALCDENSITY_COLUMN_INDEX,pTubeInfo->GetCalculatedDensityString(i,theApp.m_iStdDensityDpd));
				++iLstRowIndex;
			}
		}
	}

	m_lstAnaResult.Invalidate(FALSE);

	return 0;
}

int CDataAnaDlg::ExportPicture()
{
	CFileDialog dlg(FALSE,_T(".bmp"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("Picture (*.bmp)|*.bmp||"),NULL);
	if(IDCANCEL == dlg.DoModal()) 
	{
		return 1;
	}
	CString strFileName = dlg.GetPathName();

	CMyDrawCurveDlg* pCurCrv = NULL;
	int iCurSel = m_cbSelCrvToShow.GetCurSel();
	if(0 == iCurSel)
	{
		pCurCrv = &m_crvAmlDelRn;
	}
	else if(1 == iCurSel)
	{
		pCurCrv = &m_crvAml;
	}
	else if(2 == iCurSel)
	{
		pCurCrv = &m_crvOriginAml;
	}
	else if(3 == iCurSel)
	{
		pCurCrv = &m_crvPeakMelt;
	}
	else if(4 == iCurSel)
	{
		pCurCrv = &m_crvOriginMelt;
	}

	if(NULL != pCurCrv->GetSafeHwnd())
	{
		pCurCrv->CurveToPic(strFileName);
	}
	
	return 0;
}

int CDataAnaDlg::AmlDataShowTypeChanged(int iNewIndex)
{
	if(iNewIndex == m_iPreCbSelIndex)
	{
		return 1;
	}

	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -1;
	}
	if(NULL == m_pParent->m_pExpData)
	{
		return -2;
	}

	tagAnaAmplifyPara* pAmplifyPara = NULL;
	int iChanNum = m_pParent->m_pExpData->GetChannelCount();
	for(int i = 0;i < iChanNum;++i)
	{
		pAmplifyPara = m_pParent->m_pExpData->GetAmplifyParaBy(i);
		if(NULL != pAmplifyPara)
		{
			double dThre = 0;
			if(0 == iNewIndex)
			{
				if(SPLINE_THRESHOLD_AUTO != pAmplifyPara->nThresholdType)
				{
					pAmplifyPara->nThresholdType = SPLINE_THRESHOLD_DELTARN;
					dThre = pAmplifyPara->dCalcDeltaRnThreshold;
				}
				else
				{
					dThre = pAmplifyPara->dCalcDeltaRnThreshold;
				}
			}
			else if(1 == iNewIndex)
			{
				if(SPLINE_THRESHOLD_AUTO != pAmplifyPara->nThresholdType)
				{
					pAmplifyPara->nThresholdType = SPLINE_THRESHOLD_NORMALIZE;
					dThre = pAmplifyPara->dCalcNormalizeThreshold;
				}
				else
				{
					dThre = pAmplifyPara->dCalcNormalizeThreshold;
				}
			}

			//��ʾ���������͸ı䣬������ֵ��Ϣ
			if(i == m_cbSelChanThre.GetCurSel())
			{
				m_edThreValue.SetText(dThre,TRUE,TRUE);
			}
		}
	}

	if(0 == iNewIndex)
	{
		m_pParent->m_pExpData->GenerateCt(TRUE,METHOD_CT_SPLINE,FLU_DELTARN);
	}
	else if(1 == iNewIndex)
	{
		m_pParent->m_pExpData->GenerateCt(TRUE,METHOD_CT_SPLINE,FLU_NORMALIZATE);
	}

	//���ý���б���Ϣ
	ResetResultLst();

	//��һ����DeltaRn�л��󣬸��±�����Ϣ
	StdLineProcess(m_pParent->m_pExpData);

	m_iPreCbSelIndex = iNewIndex;
	return 0;
}

int CDataAnaDlg::CheckAmlDataShowType()
{
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -1;
	}
	if(NULL == m_pParent->m_pExpData)
	{
		return -2;
	}

	eSplineThresholdType eSplineThreType = m_pParent->m_pExpData->GetSplineThresholdType();
	if(SPLINE_THRESHOLD_DELTARN == eSplineThreType)
	{
		m_cbSelCrvToShow.SetCurSel(0,FALSE,TRUE);
		if(NULL != m_pCurSelCrvToShow->GetSafeHwnd())
		{
			m_pCurSelCrvToShow->ShowWindow(SW_HIDE);
		}
		m_crvAmlDelRn.ShowWindow(SW_SHOW);
		m_pCurSelCrvToShow = &m_crvAmlDelRn;
		m_iPreCbSelIndex = 0;
	}
	else if(SPLINE_THRESHOLD_NORMALIZE == eSplineThreType)
	{
		m_cbSelCrvToShow.SetCurSel(1,FALSE,TRUE);
		if(NULL != m_pCurSelCrvToShow->GetSafeHwnd())
		{
			m_pCurSelCrvToShow->ShowWindow(SW_HIDE);
		}
		m_crvAml.ShowWindow(SW_SHOW);
		m_pCurSelCrvToShow = &m_crvAml;
		m_iPreCbSelIndex = 1;
	}
	return 0;
}

int CDataAnaDlg::StdLineProcess(CExperimentData* pExpData)
{
	if(NULL == pExpData)
	{
		return -1;
	}

	int iAmpDataLen = pExpData->GetAmplifyDataNum();
	if(iAmpDataLen < 1)
	{
		return -2;
	}

	int iChanCount = pExpData->GetChannelCount();
	if(iChanCount < 1)
	{
		return -3;
	}

	int iTubeNum = pExpData->GetTubeCount();
	if(iTubeNum < 1)
	{
		return -4;
	}

	m_stdProc.Clear();
	m_crvStd.DeleteLineData(FALSE);	//����ǰɾ������ͨ���ı�����Ϣ

	//������������ÿ��ͨ���ķ���Ũ��ֵ
	CTubeInfo* pTubeInfo = NULL;
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	for(int i = 0;i < iTubeNum;++i)
	{
		pTubeInfo = pExpData->GetTubeInfoAt(i);
		if(NULL == pTubeInfo)
		{
			continue;
		}

		for(int j = 0;j < iChanCount;++j)
		{
			pTubeInfo->ResetChannelCalculatedDensity(j);
		}
	}

	double dCt = 0,dQuantity = 0;
	CStdLineTag stdLineTag;

	for(int i = 0;i < iTubeNum;++i)
	{
		pTubeInfo = pExpData->GetTubeInfoAt(i);
		if(NULL == pTubeInfo || SAMPLE_TYPE_STANDARD != pTubeInfo->GetSampleType())
		{
			continue;
		}

		for(int j = 0;j < iChanCount;++j)
		{
			pDyeTargetInfo = pTubeInfo->GetTargetInfo(j);
			if(NULL == pDyeTargetInfo)
			{
				continue;
			}

			dCt = pDyeTargetInfo->GetCt();
			dQuantity = log10(pDyeTargetInfo->GetQuantity());
			if(dCt > 1 && dCt < iAmpDataLen /*&& dQuantity > 0*/)
			{
				stdLineTag.m_iChanId = j;
				m_stdProc.AddPoint(stdLineTag,dQuantity,dCt);
			}
		}
	}
		
	int iStdLineNum = m_stdProc.GetStdLineNum();
	CStdLine* pStdLine = NULL;

	for(int i = 0;i < iStdLineNum;++i)
	{
		pStdLine = m_stdProc.GetStdLinePtr(i);
		if(NULL == pStdLine)
		{
			continue;
		}
		if(0 != pStdLine->LinearFit())
		{
			continue;
		}

		if(0 != pStdLine->m_linearFitData.m_dSlope)
		{
			pStdLine->m_dEfficiency = pow(10.0,-1 / pStdLine->m_linearFitData.m_dSlope) - 1;
		}
		m_crvStd.SetLinearFitData(pStdLine->m_stdLineTag.m_iChanId,&pStdLine->m_linearFitData);

		if(0 != pStdLine->m_linearFitData.m_dSlope)
		{
			//��������Ũ��
			for(int k = 0;k < iChanCount;++k)
			{
				if(k != pStdLine->m_stdLineTag.m_iChanId)
				{
					continue;
				}

				for(int j = 0;j < iTubeNum;++j)
				{
					pTubeInfo = pExpData->GetTubeInfoAt(j);
					if(NULL == pTubeInfo || pTubeInfo->GetBeNoneSampleType() || SAMPLE_TYPE_STANDARD == pTubeInfo->GetSampleType())
					{
						continue;
					}

					pDyeTargetInfo = pTubeInfo->GetTargetInfo(k);
					if(NULL == pDyeTargetInfo || !pDyeTargetInfo->GetBeSelected())
					{
						continue;
					}

					dCt = pDyeTargetInfo->GetCt();
					dQuantity = -1;
					if(dCt > 1 && dCt < iAmpDataLen)
					{
						dQuantity = pow(10.0,(dCt - pStdLine->m_linearFitData.m_dIntercept) / pStdLine->m_linearFitData.m_dSlope); 
					}
					pDyeTargetInfo->SetCalcValue(CDyeTargetInfo::VALUE_CALC_QUANTITY,dQuantity);
				}
			}
		}
	}

	if(iStdLineNum > 0)
	{
		m_crvStd.HandSetExpressionXYAxisMinMax(TRUE);
	}
	else
	{
		m_crvStd.Invalidate(FALSE);
	}

	return 0;
}

int CDataAnaDlg::StdLineProcess(CExperimentData* pExpData,int iChanId)
{
	if(NULL == pExpData)
	{
		return -1;
	}

	if(iChanId < 0)
	{
		return -2;
	}

	int iAmpDataLen = pExpData->GetAmplifyDataNum();
	if(iAmpDataLen < 1)
	{
		return -3;
	}

	int iChanCount = pExpData->GetChannelCount();
	if(iChanCount < 1)
	{
		return -4;
	}

	if(iChanId >= iChanCount)
	{
		return -5;
	}

	int iTubeNum = pExpData->GetTubeCount();
	if(iTubeNum < 1)
	{
		return -6;
	}

	CStdLineTag stdLineTag;
	stdLineTag.m_iChanId = iChanId;

	m_stdProc.DeleteOneStdLine(stdLineTag);
	m_crvStd.DeleteOneLineData(iChanId,FALSE);

	//��������������iChanIdͨ���ķ���Ũ��ֵ
	CTubeInfo* pTubeInfo = NULL;
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	for(int i = 0;i < iTubeNum;++i)
	{
		pTubeInfo = pExpData->GetTubeInfoAt(i);
		if(NULL == pTubeInfo)
		{
			continue;
		}
		pTubeInfo->ResetChannelCalculatedDensity(iChanId);
	}

	double dCt = 0,dQuantity = 0;
	for(int i = 0;i < iTubeNum;++i)
	{
		pTubeInfo = pExpData->GetTubeInfoAt(i);
		if(NULL == pTubeInfo || SAMPLE_TYPE_STANDARD != pTubeInfo->GetSampleType())
		{
			continue;
		}

		pDyeTargetInfo = pTubeInfo->GetTargetInfo(iChanId);
		if(NULL == pDyeTargetInfo)
		{
			continue;
		}

		dCt = pDyeTargetInfo->GetCt();
		dQuantity = log10(pDyeTargetInfo->GetQuantity());
		if(dCt > 1 && dCt < iAmpDataLen /*&& dQuantity > 0*/)
		{
			m_stdProc.AddPoint(stdLineTag,dQuantity,dCt);
		}
	}

	CStdLine* pStdLine = m_stdProc.GetStdLinePtr(stdLineTag);
	if(NULL != pStdLine)
	{
		if(0 == pStdLine->LinearFit())
		{
			if(0 != pStdLine->m_linearFitData.m_dSlope)
			{
				pStdLine->m_dEfficiency = pow(10.0,-1 / pStdLine->m_linearFitData.m_dSlope) - 1;
			}

			m_crvStd.SetLinearFitData(pStdLine->m_stdLineTag.m_iChanId,&pStdLine->m_linearFitData);

			if(0 != pStdLine->m_linearFitData.m_dSlope)
			{
				//��������Ũ��
				for(int k = 0;k < iChanCount;++k)
				{
					if(k != pStdLine->m_stdLineTag.m_iChanId)
					{
						continue;
					}

					for(int j = 0;j < iTubeNum;++j)
					{
						pTubeInfo = pExpData->GetTubeInfoAt(j);
						if(NULL == pTubeInfo || pTubeInfo->GetBeNoneSampleType() || SAMPLE_TYPE_STANDARD == pTubeInfo->GetSampleType())
						{
							continue;
						}

						pDyeTargetInfo = pTubeInfo->GetTargetInfo(k);
						if(NULL == pDyeTargetInfo || !pDyeTargetInfo->GetBeSelected())
						{
							continue;
						}

						dCt = pDyeTargetInfo->GetCt();
						dQuantity = -1;
						if(dCt > 1 && dCt < iAmpDataLen)
						{
							dQuantity = pow(10.0,(dCt - pStdLine->m_linearFitData.m_dIntercept) / pStdLine->m_linearFitData.m_dSlope); 
						}
						pDyeTargetInfo->SetCalcValue(CDyeTargetInfo::VALUE_CALC_QUANTITY,dQuantity);
					}
				}
			}
		}
	}

	vector<int> vecChanSelState = pExpData->GetDataAnaChannelBtSelectState();
	m_crvStd.SetLineShow(iChanId,iChanId < vecChanSelState.size() && vecChanSelState[iChanId]);

	m_crvStd.HandSetExpressionXYAxisMinMax(TRUE);

	return 0;
}

CString CDataAnaDlg::GetCtTitle()
{
	//PS96��Ctֵ������Tt��ֵ��������������ȷ����ʶ
	if(NULL == m_pParent->GetSafeHwnd() || NULL == m_pParent->m_pExpData)
	{
		return CTTEXTTAG;
	}

	if(m_pParent->m_pExpData->GetBePS96ExperFile())
	{
		return _T("Tt");
	}
	else
	{
		return _T("Ct");
	}
}

CString CDataAnaDlg::GetAverageCtTitle()
{
	//PS96��Ctֵ������Tt��ֵ��������������ȷ����ʶ
	return theApp.m_pLangInfo->GetText(227) + GetCtTitle();
}

int CDataAnaDlg::CbSelectCrvToShowChanged()
{
	if(NULL == m_cbSelCrvToShow.GetSafeHwnd())
	{
		return -1;
	}

	int iCurSel = m_cbSelCrvToShow.GetCurSel();
	if(iCurSel < 0)
	{
		return -2;
	}

	if(NULL != m_pCurSelCrvToShow->GetSafeHwnd())
	{
		m_pCurSelCrvToShow->ShowWindow(SW_HIDE);
	}

	BOOL bShowThreEdCb = FALSE;

	if(0 == iCurSel)
	{
		m_crvAmlDelRn.ShowWindow(SW_SHOW);
		m_pCurSelCrvToShow = &m_crvAmlDelRn;
		AmlDataShowTypeChanged(iCurSel);
		bShowThreEdCb = TRUE;
	}
	else if(1 == iCurSel)
	{
		m_crvAml.ShowWindow(SW_SHOW);
		m_pCurSelCrvToShow = &m_crvAml;
		AmlDataShowTypeChanged(iCurSel);
		bShowThreEdCb = TRUE;
	}
	else if(2 == iCurSel)
	{
		//lzh��ԭʼ��������
		m_crvOriginAml.ShowWindow(SW_SHOW);
		m_pCurSelCrvToShow = &m_crvOriginAml;
	}
	else if(3 == iCurSel)
	{
		m_crvPeakMelt.ShowWindow(SW_SHOW);
		m_pCurSelCrvToShow = &m_crvPeakMelt;
	}
	else if(4 == iCurSel)
	{
		m_crvOriginMelt.ShowWindow(SW_SHOW);
		m_pCurSelCrvToShow = &m_crvOriginMelt;
	}

	if(bShowThreEdCb)
	{
		m_edThreValue.ShowWindow(SW_SHOW);
		m_cbSelChanThre.ShowWindow(SW_SHOW);
	}
	else
	{
		m_edThreValue.ShowWindow(SW_HIDE);
		m_cbSelChanThre.ShowWindow(SW_HIDE);
	}

	//ˢ����ֵ�ַ�

	InvalidateRect(m_rcThresholdText);

	//����ǹ�һ����DeltaRn�л������½���б�Ctֵ��Ϣ,20220408
	
	return 0;
}

int CDataAnaDlg::CbSelectChannelThresholdChanged()
{
	if(NULL == m_cbSelChanThre.GetSafeHwnd())
	{
		return -1;
	}

	int iCurSel = m_cbSelChanThre.GetCurSel();
	if(iCurSel < 0)
	{
		return -2;
	}

	if(iCurSel >= m_crvAmlDelRn.m_vecChanThreInfo.size())
	{
		return -3;
	}

	if(iCurSel >= m_crvAml.m_vecChanThreInfo.size())
	{
		return -4;
	}

	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -5;
	}
	if(NULL == m_pParent->m_pExpData)
	{
		return -6;
	}

	int iChanNum = m_pParent->m_pExpData->GetChannelCount();
	if(iCurSel >= iChanNum)
	{
		return -7;
	}

	//���÷������������ֵ��Ϣ
	CThreLineInfo* pThreLineInfo = NULL;
	if(0 == m_cbSelCrvToShow.GetCurSel())	//DeltaRn
	{
		pThreLineInfo = m_crvAmlDelRn.m_vecChanThreInfo[iCurSel].GetCurSelThreInfo();
	}
	else if(1 == m_cbSelCrvToShow.GetCurSel())	//��һ��
	{
		pThreLineInfo = m_crvAml.m_vecChanThreInfo[iCurSel].GetCurSelThreInfo();
	}

	if(NULL == pThreLineInfo)
	{
		return -8;
	}

	m_edThreValue.SetText(pThreLineInfo->GetThreLineValue(),TRUE,TRUE);

	return 0;
}

int CDataAnaDlg::ShowThreValue()
{
	if(NULL == m_cbSelChanThre.GetSafeHwnd())
	{
		return -1;
	}

	int iCurSel = m_cbSelChanThre.GetCurSel();
	if(iCurSel < 0)
	{
		return -2;
	}

	if(iCurSel >= m_crvAmlDelRn.m_vecChanThreInfo.size())
	{
		return -3;
	}

	if(iCurSel >= m_crvAml.m_vecChanThreInfo.size())
	{
		return -4;
	}

	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -5;
	}
	if(NULL == m_pParent->m_pExpData)
	{
		return -6;
	}

	int iChanNum = m_pParent->m_pExpData->GetChannelCount();
	if(iCurSel >= iChanNum)
	{
		return -7;
	}

	//���÷������������ֵ��Ϣ
	CThreLineInfo* pThreLineInfo = NULL;
	if(0 == m_cbSelCrvToShow.GetCurSel())	//DeltaRn
	{
		pThreLineInfo = m_crvAmlDelRn.m_vecChanThreInfo[iCurSel].GetCurSelThreInfo();
	}
	else if(1 == m_cbSelCrvToShow.GetCurSel())	//��һ��
	{
		pThreLineInfo = m_crvAml.m_vecChanThreInfo[iCurSel].GetCurSelThreInfo();
	}

	if(NULL == pThreLineInfo)
	{
		return -8;
	}

	m_edThreValue.SetText(pThreLineInfo->GetThreLineValue(),TRUE,TRUE);

	return 0;
}

int CDataAnaDlg::ResetCurveMaxMinValue(BOOL bRedraw)
{
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -1;
	}

	if(NULL == m_pParent->m_pExpData)
	{
		return -2;
	}

	int iAmpDataLen = m_pParent->m_pExpData->GetAmplifyDataNum();
	int iMeltDataLen = m_pParent->m_pExpData->GetMeltDataNum();

	if(iAmpDataLen > 0)
	{
		m_crvAml.HandSetGeneralLineXYAxisMinMax();
		m_crvAmlDelRn.HandSetGeneralLineXYAxisMinMax();
		m_crvOriginAml.HandSetGeneralLineXYAxisMinMax();
	}
	if(iMeltDataLen > 0)
	{
		m_crvPeakMelt.HandSetGeneralLineXYAxisMinMax();
		m_crvOriginMelt.HandSetGeneralLineXYAxisMinMax();
	}

	if(bRedraw)
	{
		if(NULL != m_crvAml.GetSafeHwnd())
		{
			m_crvAml.Invalidate(FALSE);
		}
		if(NULL != m_crvAmlDelRn.GetSafeHwnd())
		{
			m_crvAmlDelRn.Invalidate(FALSE);
		}
		if(NULL != m_crvOriginAml.GetSafeHwnd())
		{
			m_crvOriginAml.Invalidate(FALSE);
		}
		if(NULL != m_crvPeakMelt.GetSafeHwnd())
		{
			m_crvPeakMelt.Invalidate(FALSE);
		}
		if(NULL != m_crvOriginMelt.GetSafeHwnd())
		{
			m_crvOriginMelt.Invalidate(FALSE);
		}

		//��׼�������ӵ����߿ؼ���ʱ�������Сֵ�Ѿ����ù��������ٴ�����
		if(NULL != m_crvStd.GetSafeHwnd())
		{
			m_crvStd.Invalidate(FALSE);
		}
	}

	return 0;
}

CExperimentData* CDataAnaDlg::GetExpDataPtr()
{
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return NULL;
	}

	return m_pParent->m_pExpData;
}

void CDataAnaDlg::SetCbSelectChanThreString()
{
	CString strTemp(_T("")),strRealTag(_T(""));
	strTemp = theApp.m_pLangInfo->GetText(112);

	CExperimentData* pExpData = GetExpDataPtr();
	if(NULL != pExpData)
	{
		CString strChanId(_T(""));
		int iChanCount = pExpData->GetChannelCount();
		if(pExpData->GetBePS96ExperFile())	//PS96����ͨ������ʾ��ֵ
		{
			iChanCount -= 1;
		}

		for(int i = 0;i < iChanCount;++i)
		{
			strRealTag = pExpData->GetOneChannelDyeName(i);
			if(strRealTag.IsEmpty())
			{
				strChanId.Format(_T("%d"),i + 1);
				strRealTag = strTemp + strChanId;
			}
			
			m_cbSelChanThre.InsertString(i,strRealTag);
		}
	}

	m_cbSelChanThre.SetCurSel(0,FALSE,TRUE);
	return;
}

int CDataAnaDlg::SetOneChannelDyeName(int iChanId,CString strNewDyeName)
{
	if(iChanId < 0)
	{
		return -1;
	}

	//ˢ�±���Ⱦ������Ϣ
	m_crvStd.SetLineParam(iChanId,strNewDyeName,TRUE);

	//ˢ��ͨ������ѡ�񴰿�����
	int iItemCount = m_cbSelChanThre.GetItemCount();
	if(iChanId >= iItemCount)
	{
		return -2;
	}

	if(strNewDyeName.IsEmpty())
	{
		return -3;
	}

	int iCurSel = m_cbSelChanThre.GetCurSel();
	if(iChanId == iCurSel)
	{
		m_cbSelChanThre.SetText(strNewDyeName,TRUE,TRUE);
	}

	int iRet = m_cbSelChanThre.SetItemText(iChanId,strNewDyeName);
	if(0 != iRet)
	{
		return -4;
	}

	return 0;
}

int CDataAnaDlg::NormalizeLogModeChanged()
{
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -1;
	}

	if(NULL == m_pParent->m_pExpData)
	{
		return -2;
	}

	//�������߿ؼ���Ӧ������
	if(0 != SetNormalizeCurveData(m_pParent->m_pExpData,TRUE))
	{
		return -3;
	}

	//���·��������е���ֵ��Ϣ
	if(0 != m_pParent->m_pExpData->AutoTransformThresholdValue(0))
	{
		return -4;
	}

	//�������߿ؼ��е���ֵ��Ϣ
	if(0 != RenewNormalizeCrvThreLineInfo(TRUE))
	{
		return -5;
	}

	//����ҳ����ʾ����ֵ��Ϣ
	CThreLineInfo* pThreLineInfo = NULL;
	int iCurSel = m_cbSelChanThre.GetCurSel();
	if(iCurSel < 0)
	{
		return -6;
	}
	
	pThreLineInfo = m_crvAml.m_vecChanThreInfo[iCurSel].GetCurSelThreInfo();
	if(NULL == pThreLineInfo)
	{
		return -7;
	}

	m_edThreValue.SetText(pThreLineInfo->GetThreLineValue(),TRUE,TRUE);

	return 0;
}

int CDataAnaDlg::DeltaRnLogModeChanged()
{
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -1;
	}

	if(NULL == m_pParent->m_pExpData)
	{
		return -2;
	}

	//�������߿ؼ���Ӧ������
	if(0 != SetDeltaRnCurveData(m_pParent->m_pExpData,TRUE))
	{
		return -3;
	}

	//���·��������е���ֵ��Ϣ
	if(0 != m_pParent->m_pExpData->AutoTransformThresholdValue(1))
	{
		return -4;
	}

	//�������߿ؼ��е���ֵ��Ϣ
	if(0 != RenewDeltaRnCrvThreLineInfo(TRUE))
	{
		return -5;
	}

	//����ҳ����ʾ����ֵ��Ϣ
	CThreLineInfo* pThreLineInfo = NULL;
	int iCurSel = m_cbSelChanThre.GetCurSel();
	if(iCurSel < 0)
	{
		return -6;
	}

	pThreLineInfo = m_crvAmlDelRn.m_vecChanThreInfo[iCurSel].GetCurSelThreInfo();
	if(NULL == pThreLineInfo)
	{
		return -7;
	}

	m_edThreValue.SetText(pThreLineInfo->GetThreLineValue(),TRUE,TRUE);

	return 0;
}

int CDataAnaDlg::RecordDefaultAxisInfo()
{
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -1;
	}

	CExperimentData* pExpData = m_pParent->m_pExpData;
	if(NULL == pExpData)
	{
		return -2;
	}

	pExpData->SetDefaultAxisCount(6);

	CAxisInfo axisInfo;

	axisInfo.m_dXMin = m_crvAmlDelRn.GetHoriStartValue();
	axisInfo.m_dXMax = m_crvAmlDelRn.GetHoriEndValue();
	axisInfo.m_dYMin = m_crvAmlDelRn.GetVertStartValue();
	axisInfo.m_dYMax = m_crvAmlDelRn.GetVertEndValue();
	pExpData->SetDefaultAxisInfoAt(0,axisInfo);

	axisInfo.m_dXMin = m_crvAml.GetHoriStartValue();
	axisInfo.m_dXMax = m_crvAml.GetHoriEndValue();
	axisInfo.m_dYMin = m_crvAml.GetVertStartValue();
	axisInfo.m_dYMax = m_crvAml.GetVertEndValue();
	pExpData->SetDefaultAxisInfoAt(1,axisInfo);

	axisInfo.m_dXMin = m_crvOriginAml.GetHoriStartValue();
	axisInfo.m_dXMax = m_crvOriginAml.GetHoriEndValue();
	axisInfo.m_dYMin = m_crvOriginAml.GetVertStartValue();
	axisInfo.m_dYMax = m_crvOriginAml.GetVertEndValue();
	pExpData->SetDefaultAxisInfoAt(2,axisInfo);

	axisInfo.m_dXMin = m_crvPeakMelt.GetHoriStartValue();
	axisInfo.m_dXMax = m_crvPeakMelt.GetHoriEndValue();
	axisInfo.m_dYMin = m_crvPeakMelt.GetVertStartValue();
	axisInfo.m_dYMax = m_crvPeakMelt.GetVertEndValue();
	pExpData->SetDefaultAxisInfoAt(3,axisInfo);

	axisInfo.m_dXMin = m_crvOriginMelt.GetHoriStartValue();
	axisInfo.m_dXMax = m_crvOriginMelt.GetHoriEndValue();
	axisInfo.m_dYMin = m_crvOriginMelt.GetVertStartValue();
	axisInfo.m_dYMax = m_crvOriginMelt.GetVertEndValue();
	pExpData->SetDefaultAxisInfoAt(4,axisInfo);

	axisInfo.m_dXMin = m_crvStd.GetHoriStartValue();
	axisInfo.m_dXMax = m_crvStd.GetHoriEndValue();
	axisInfo.m_dYMin = m_crvStd.GetVertStartValue();
	axisInfo.m_dYMax = m_crvStd.GetVertEndValue();
	pExpData->SetDefaultAxisInfoAt(5,axisInfo);

	return 0;
}

int CDataAnaDlg::AdjudicateAmpMeltWhichToShow()
{
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -1;
	}

	CExperimentData* pExpData = m_pParent->m_pExpData;
	if(NULL == pExpData)
	{
		return -2;
	}

	int iAmpDataNum = pExpData->GetAmplifyDataNum();
	int iMeltDataNum = pExpData->GetMeltDataNum();
	int iIndex = 0;

	if(iAmpDataNum <= 0 && iMeltDataNum > 0)
	{
		iIndex = 3;
	}
	else
	{
		iIndex = 0;
	}

	if(iIndex != m_cbSelCrvToShow.GetCurSel())
	{
		m_cbSelCrvToShow.SetCurSel(iIndex,TRUE,FALSE);
	}

	return 0;
}

int CDataAnaDlg::RefreshTheChannelToShowThre(int iBtIndex,BOOL bChecked)
{
	if(iBtIndex < 0)
	{
		return -1;
	}

	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -2;
	}

	CExperimentData* pExpData = m_pParent->m_pExpData;
	if(NULL == pExpData)
	{
		return -3;
	}

	//iBtIndex�����ѡ��״̬������ʾ��ͨ����ֵ��������ʾ��һ����ʾ��ͨ����ֵ
	vector<int> vecChanSelState = pExpData->GetDataAnaChannelBtSelectState();
	if(iBtIndex >= vecChanSelState.size())
	{
		return -4;
	}

	if(bChecked)
	{
		m_cbSelChanThre.SetCurSel(iBtIndex,TRUE,FALSE);
		return 0;
	}

	for(int i = 0;i < vecChanSelState.size();++i)
	{
		if(vecChanSelState[i] && i != iBtIndex)
		{
			m_cbSelChanThre.SetCurSel(i,TRUE,FALSE);
			return 1;
		}
	}

	return 2;
}

int CDataAnaDlg::UploadInspectResultToLis()
{
	//CString strA;
	//strA.AppendFormat(_T("%c"),0X0B);
	//strA += L"MSH|^~\&|C1||host||20220831121212||ORU^R01|1|P|2.3.1||||NE||UTF-8";
	//strA += L"OBX|1|ST|0000000039|10|1.21|||A|||F";
	////strA += L"OBX|2|ST|0000000039|10|1.21|||A|||F";
	////strA += L"OBX|3|ST|0000000039|10|1.21|||A|||F";
	//strA.AppendFormat(_T("%c"),0X1C);
	//strA += L"\r";

	//int cbMultiByte = WideCharToMultiByte(CP_ACP, 0, strA, -1, NULL, 0, NULL, NULL);
	//char *pszText = NULL;
	//pszText = new char[cbMultiByte + 1];
	//memset((void* )pszText, 0, sizeof(char) * (cbMultiByte + 1));
	//WideCharToMultiByte(CP_ACP,0,strA,-1,pszText,cbMultiByte,NULL,NULL);


	//CStringA strB = pszText;

	//delete [] pszText;

	//vector<CStringA> vecRst;
	//vecRst.push_back(strB);
	//SendDataByTcpIp(vecRst);
	//return 1;


	vector<int> vexSelRowId = m_lstAnaResult.GetSelectedRowId();
	if(vexSelRowId.size() < 1)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(368));
		return 1;
	}

	double dCt = 0;
	CString strCt(_T(""));
	CStringA strTemp(_T(""));
	vector<CStringA> vecLisData;
	int iNum = vexSelRowId.size();
	for(int i = 0;i < iNum;++i)
	{
		//֡ͷ
		strTemp.Format("%c",0X0B);

		//Ƕ��ʱ��
		strTemp += "MSH|^~\&|C1||host||"; 
		strTemp += GetLisCurrentTmTimeA();
		strTemp += "||ORU^R01|1|P|2.3.1||||NE||";

		strTemp += "\r";

		//Ƕ��Ctֵ
		strCt = m_lstAnaResult.GetItemText(vexSelRowId[i],CT_COLUMN_INDEX);
		if(STR_NOCT != strCt)
		{
			dCt = _ttof(strCt);	
			strTemp.AppendFormat("OBX|%d|ST|%010d|10|%.2lf|||A|||F",i + 1,39 + i,dCt);
		}
		else
		{
			strTemp.AppendFormat("OBX|%d|ST|0000000039|10||||A|||F",i + 1,39 + i,dCt);
		}
	
		//֡β
		strTemp.AppendFormat("%c",0X1C);
		strTemp += "\r";
		vecLisData.push_back(strTemp);
	}

	int iRet = SendDataByTcpIp(vecLisData);
	if(0 != iRet)
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(371));
		return -1;
	}
	
	PopNormalTipInfo(theApp.m_pLangInfo->GetText(370));
	return 0;
}

int CDataAnaDlg::SendDataByTcpIp(vector<CStringA>& vecLisInfo)
{
	int iSize = WideCharToMultiByte(CP_ACP,0,theApp.m_lisSetInfo.m_strIpAddress,-1,NULL,0,NULL,NULL);
	char *pCIpAddr = new char[iSize + 1];
	if(NULL == pCIpAddr)
	{
		return -1;
	}

	memset(pCIpAddr,0,sizeof(char) * (iSize + 1));
	WideCharToMultiByte(CP_ACP,0,theApp.m_lisSetInfo.m_strIpAddress,-1,pCIpAddr,iSize,NULL,NULL);

	int iRet = 0;

	USHORT uDefPort = _ttoi(theApp.m_lisSetInfo.m_strPort);
	WORD wVersionRequested;
	WSADATA	wsaData;
	wVersionRequested = MAKEWORD(2,2);
	int iErr = WSAStartup(wVersionRequested,&wsaData);
	if(0 != iErr)
	{
		iRet = -2;
		goto END_MEMORY;
	}

	SOCKET sockClt = socket(AF_INET,SOCK_STREAM,0);
	if(INVALID_SOCKET == sockClt)
	{
		iRet = -3;
		goto END_CLEAN;
	}

	SOCKADDR_IN	addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_addr.s_addr = inet_addr(pCIpAddr);
	addrSrv.sin_port = htons(uDefPort);

	iErr = connect(sockClt,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	DWORD dwErr = GetLastError();

	if(0 != iErr)
	{
		iRet = -4;
		goto END_SOCKET;
	}

	int iRecvWaitMs = 3600;
	iErr = setsockopt(sockClt,SOL_SOCKET,SO_RCVTIMEO,(const char*)&iRecvWaitMs,sizeof(int));

	char aryBuf[1024] = {0};
	for(int i = 0;i < vecLisInfo.size();++i)
	{
		if(vecLisInfo[i].GetLength() < 1)
		{
			continue;
		}

		iErr = send(sockClt,vecLisInfo[i].GetBuffer(),vecLisInfo[i].GetLength(),0);
		if(iErr < 1)
		{
			iRet = -5;
			goto END_SOCKET;
		}

		iErr = recv(sockClt,aryBuf,1024,0);
		if(-1 == iErr)
		{
			iRet = -6;
			goto END_SOCKET;
		}
		else if(iErr < 0)
		{
			iRet = -7;
			goto END_SOCKET;
		}
		
		//�����յ�������
		CStringA strRst = aryBuf;
		if(-1 == strRst.Find("MSA|AA"))
		{
			iRet = -8;
			goto END_SOCKET;
		}
		memset(aryBuf,0,1024);
	}

END_SOCKET:
	closesocket(sockClt);

END_CLEAN:
	WSACleanup();

END_MEMORY:
	if(NULL != pCIpAddr)
	{
		delete [] pCIpAddr;
	}

	return iRet;
}

void CDataAnaDlg::ResetLstTitle()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	int index = -1;
	m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(29));	
	m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(76));	
	m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(47));	
	m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(64));	
	m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(51));

	m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(53));	
	m_lstAnaResult.SetColumnText(++index,GetCtTitle());
	m_lstAnaResult.SetColumnText(++index,_T("Tm1"));
	m_lstAnaResult.SetColumnText(++index,_T("Tm2"));
	m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(55));

	m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(95));
	m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(48));	
	m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(30));	
	m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(54));	
	m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(98));	

	//m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(66));	
	//m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(67));	
	//m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(68));	
	//m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(69));	
	//m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(70));	

	//m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(71));	
	//m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(72));	
	//m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(73));	
	//m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(74));
	//m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(75));	

	m_lstAnaResult.SetColumnText(++index,pLangInfo->GetText(20));

	return;
}

void CDataAnaDlg::ResetLstColumnWidth()
{
	int iIndex = -1;
	if(theApp.GetBeEnglish())
	{
		m_lstAnaResult.SetColumnWidth(++iIndex,36 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,51 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,82 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,68 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,75 * m_dHoriRatio);

		m_lstAnaResult.SetColumnWidth(++iIndex,75 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,49 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,75 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,71 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,110 * m_dHoriRatio);

		m_lstAnaResult.SetColumnWidth(++iIndex,156 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,86 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,78 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,71 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,81 * m_dHoriRatio);

		//m_lstAnaResult.SetColumnWidth(++iIndex,76 * m_dHoriRatio);
		//m_lstAnaResult.SetColumnWidth(++iIndex,48 * m_dHoriRatio);
		//m_lstAnaResult.SetColumnWidth(++iIndex,49 * m_dHoriRatio);
		//m_lstAnaResult.SetColumnWidth(++iIndex,101 * m_dHoriRatio);
		//m_lstAnaResult.SetColumnWidth(++iIndex,79 * m_dHoriRatio);

		//m_lstAnaResult.SetColumnWidth(++iIndex,79 * m_dHoriRatio);
		//m_lstAnaResult.SetColumnWidth(++iIndex,62 * m_dHoriRatio);
		//m_lstAnaResult.SetColumnWidth(++iIndex,62 * m_dHoriRatio);
		//m_lstAnaResult.SetColumnWidth(++iIndex,100 * m_dHoriRatio);
		//m_lstAnaResult.SetColumnWidth(++iIndex,62 * m_dHoriRatio);

		m_lstAnaResult.SetColumnWidth(++iIndex,160 * m_dHoriRatio);
	}
	else
	{
		m_lstAnaResult.SetColumnWidth(++iIndex,51 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,64 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,79 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,131 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,49 * m_dHoriRatio);

		m_lstAnaResult.SetColumnWidth(++iIndex,75 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,49 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,75 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,71 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,78 * m_dHoriRatio);

		m_lstAnaResult.SetColumnWidth(++iIndex,131 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,63 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,110 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,71 * m_dHoriRatio);
		m_lstAnaResult.SetColumnWidth(++iIndex,49 * m_dHoriRatio);

		//m_lstAnaResult.SetColumnWidth(++iIndex,76 * m_dHoriRatio);
		//m_lstAnaResult.SetColumnWidth(++iIndex,48 * m_dHoriRatio);
		//m_lstAnaResult.SetColumnWidth(++iIndex,49 * m_dHoriRatio);
		//m_lstAnaResult.SetColumnWidth(++iIndex,101 * m_dHoriRatio);
		//m_lstAnaResult.SetColumnWidth(++iIndex,79 * m_dHoriRatio);
		//m_lstAnaResult.SetColumnWidth(++iIndex,79 * m_dHoriRatio);
		//m_lstAnaResult.SetColumnWidth(++iIndex,62 * m_dHoriRatio);
		//m_lstAnaResult.SetColumnWidth(++iIndex,62 * m_dHoriRatio);
		//m_lstAnaResult.SetColumnWidth(++iIndex,62 * m_dHoriRatio);

		//m_lstAnaResult.SetColumnWidth(++iIndex,62 * m_dHoriRatio);

		m_lstAnaResult.SetColumnWidth(++iIndex,160 * m_dHoriRatio);
	}

	return;
}

int CDataAnaDlg::RefreshLanguageDisplay()
{
	BOOL bEnglish = theApp.m_pLangInfo->GetBeEnglish();

	int iFontH = 14 * m_dTxRatio;
	if(bEnglish)
	{
		iFontH = 16 * m_dTxRatio;
	}

	SetMyFont(m_stTxFont,iFontH);

	iFontH = 14 * m_dTxRatio;
	if(bEnglish)
	{
		iFontH = 16 * m_dTxRatio;
	}

	CString strTemp = theApp.m_pLangInfo->GetText(293);

	m_btAxisZoom.SetMyFont(m_stTxFont,iFontH);
	m_btAxisZoom.SetWindowText(strTemp);

	m_btAxisZoom2nd.SetMyFont(m_stTxFont,iFontH);
	m_btAxisZoom2nd.SetWindowText(strTemp);

	iFontH = 14 * m_dTxRatio;
	if(bEnglish)
	{
		iFontH = 15 * m_dTxRatio;
	}
	m_cbSelCrvToShow.SetMyFont(m_stTxFont,iFontH);
	m_cbSelCrvToShow.m_selWnd.SetMyFont(m_stTxFont,iFontH);
	m_cbSelCrvToShow.TextParamChanged();

	int iCurSel = m_cbSelCrvToShow.GetCurSel();
	m_cbSelCrvToShow.SetItemText(0,theApp.m_pLangInfo->GetText(186));
	m_cbSelCrvToShow.SetItemText(1,theApp.m_pLangInfo->GetText(187));
	m_cbSelCrvToShow.SetItemText(2,theApp.m_pLangInfo->GetText(77));
#ifndef SHIELDMELTFUNC
	m_cbSelCrvToShow.SetItemText(3,theApp.m_pLangInfo->GetText(92));
	m_cbSelCrvToShow.SetItemText(4,theApp.m_pLangInfo->GetText(78));
#endif
	m_cbSelCrvToShow.RefreshText(iCurSel);

	m_cbSelChanThre.SetMyFont(m_stTxFont,iFontH);
	m_cbSelChanThre.m_selWnd.SetMyFont(m_stTxFont,iFontH);
	m_cbSelChanThre.TextParamChanged();

	iFontH = 18 * m_dTxRatio;
	m_edThreValue.SetMyFont(m_stTxFont,iFontH);
	m_edThreValue.TextParamChanged();

	iFontH = 14 * m_dTxRatio;
	if(bEnglish)
	{
		iFontH = 16 * m_dTxRatio;
	}

	strTemp = theApp.m_pLangInfo->GetText(56);
	m_crvAml.SetXAxisTag(strTemp);
	m_crvAml.SetMyFont(m_stTxFont,iFontH);

	m_crvAmlDelRn.SetXAxisTag(strTemp);
	m_crvAmlDelRn.SetMyFont(m_stTxFont,iFontH);

	m_crvOriginAml.SetXAxisTag(strTemp);
	m_crvOriginAml.SetMyFont(m_stTxFont,iFontH);

	strTemp = theApp.m_pLangInfo->GetText(184);
	m_crvPeakMelt.SetXAxisTag(strTemp);
	m_crvPeakMelt.SetMyFont(m_stTxFont,iFontH);

	m_crvOriginMelt.SetXAxisTag(strTemp); 
	m_crvOriginMelt.SetMyFont(m_stTxFont,iFontH);

	m_crvStd.SetMyFont(m_stTxFont,iFontH);
	m_crvStd.m_fontExpression.SetMyFont(m_stTxFont,16 * m_dTxRatio);
	m_crvStd.SetExpressionTag(theApp.m_pLangInfo->GetText(94));

	iFontH = 18 * m_dTxRatio;
	int iFontH2 = 14 * m_dTxRatio;	
	m_samPlateDlg.SetMyFont(m_stTxFont,iFontH);
	m_samPlateDlg.SetMyHeaderFont(m_stTxFont,iFontH);
	m_samPlateDlg.SetMyTitleFont(m_stTxFont,iFontH,FW_SEMIBOLD);
	m_samPlateDlg.SetMyFontAbbr(m_stTxFont,iFontH2);

	iFontH = 13 * m_dTxRatio;
	if(bEnglish)
	{
		m_segLgdInfo.m_iOneCharW = 10 * m_dHoriRatio;
		iFontH = 15 * m_dTxRatio;
	}
	else
	{
		m_segLgdInfo.m_iOneCharW = 14 * m_dHoriRatio;
	}
	CLegendInfo::ResetSegLegendText(&m_segLgdInfo);
	m_segLgdInfo.SetMyFont(m_stTxFont,iFontH);
	
	//�б���
	iFontH = 14 * m_dTxRatio;
	if(bEnglish)
	{
		iFontH = 15 * m_dTxRatio;
	}
	ResetLstTitle();
	ResetLstColumnWidth();
	m_lstAnaResult.SetMyHeaderFont(m_stTxFont,iFontH);
	m_lstAnaResult.SetMyFont(m_stTxFont,iFontH);
	LstResultLanguageChanged();

	return 0;
}
//
//
//LRESULT CDataAnaDlg::OnMsgRowSelectedChange(WPARAM wParam, LPARAM lParam)
//{
//	CArray<int>	arraySelIndex;
//	int iRowNum = m_lstAnaResult.GetRowNum();
//	vector<int> vecSelectedRowId;
//	vecSelectedRowId = m_lstAnaResult.GetSelectedRowId();
//	CString strTubeID, strDye;
//	int iLineID = -1;
//	BOOL bLineSelected = FALSE;
//	for (int i=0; i<iRowNum; i++)
//	{
//		strTubeID = m_lstAnaResult.GetItemText(i, 1);
//		strDye = m_lstAnaResult.GetItemText(i, 4);
//		iLineID = m_pParent->m_pExpData->GenerateLineIDBy(strTubeID, strDye);
//		if (iLineID == -1)
//			continue;
//
//		bLineSelected = FALSE;
//		for (int j=0; j<vecSelectedRowId.size(); j++)
//		{
//			if (i == vecSelectedRowId[j])
//			{
//				bLineSelected = TRUE;
//				break;
//			}
//		}
//
//		m_crvAml.SetLineShow(iLineID, bLineSelected);
//		m_crvAmlDelRn.SetLineShow(iLineID, bLineSelected);
//		m_crvOriginAml.SetLineShow(iLineID,bLineSelected);
//		m_crvPeakMelt.SetLineShow(iLineID, bLineSelected);
//		m_crvOriginMelt.SetLineShow(iLineID, bLineSelected);
//	}
//
//	CTubeInfo* pTubeParam = m_pParent->m_pExpData->GetFirstUsedTube();
//	if (pTubeParam != NULL)
//	{
//		int iDyeCount = pTubeParam->GetChannelNum();
//		for (int i=0; i<iDyeCount; i++)
//		{
//			BOOL bChanHaveLineShow = FALSE;
//			CDyeTargetInfo* pTargetCol = pTubeParam->GetTargetInfo(i);
//			for (int j=0; j<vecSelectedRowId.size(); j++)
//			{
//				strDye = m_lstAnaResult.GetItemText(vecSelectedRowId[j], 4);
//				if (strDye == pTargetCol->GetDyeName())
//				{
//					bChanHaveLineShow = TRUE;
//					break;
//				}
//			}
//
//			m_crvAml.SetBeShowThreLine(i, bChanHaveLineShow);
//			m_crvAmlDelRn.SetBeShowThreLine(i, bChanHaveLineShow);
//		}
//	}
//
//
//	if(NULL != m_crvAml.GetSafeHwnd())
//	{
//		m_crvAml.Invalidate(TRUE);
//	}
//	if(NULL != m_crvAmlDelRn.GetSafeHwnd())
//	{
//		m_crvAmlDelRn.Invalidate(TRUE);
//	}
//	if(NULL != m_crvOriginAml.GetSafeHwnd())
//	{
//		m_crvOriginAml.Invalidate(TRUE);
//	}
//	if(NULL != m_crvPeakMelt.GetSafeHwnd())
//	{
//		m_crvPeakMelt.Invalidate(TRUE);
//	}
//	if(NULL != m_crvOriginMelt.GetSafeHwnd())
//	{
//		m_crvOriginMelt.Invalidate(TRUE);
//	}
//	if(NULL != m_crvStd.GetSafeHwnd())
//	{
//		m_crvStd.Invalidate(TRUE);
//	}
//
//	return 0;
//}