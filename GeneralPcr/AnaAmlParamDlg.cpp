// SkipDlg.cpp : implementation file
//

#include "stdafx.h"

#include "AnaAmlParamDlg.h"
#include "GeneralPcr.h"

// CAnaAmlParamDlg dialog
IMPLEMENT_DYNAMIC(CAnaAmlParamDlg, CDialogEx)

CAnaAmlParamDlg::CAnaAmlParamDlg(CDataAnalysisPara* pAnalysisPara,CWnd* pParent /*=NULL*/)
	: CDialogEx(CAnaAmlParamDlg::IDD, pParent)
{
	m_frameWidth = 1;
	m_roundValue = 10;
	m_iWndWidth = 865;
	m_iWndHeight = 586;
	m_iCaptionHeight = 52;
	m_pAnalysisPara = pAnalysisPara;
	m_bChanged = FALSE;
	m_iChannelCount = 5;
}

CAnaAmlParamDlg::~CAnaAmlParamDlg()
{

}

void CAnaAmlParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAnaAmlParamDlg, CDialogEx)
	ON_WM_ERASEBKGND() 
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(ID_ANAAMLPARAMDLG_BT_SAVE,&CAnaAmlParamDlg::OnBnClickedSave)
	ON_BN_CLICKED(ID_ANAAMLPARAMDLG_BT_CANCEL,&CAnaAmlParamDlg::OnBnClickedCancel)
	ON_BN_CLICKED(ID_ANAAMLPARAMDLG_BT_ADD_TARGET, &CAnaAmlParamDlg::OnBnClickedAddTarget)
	ON_BN_CLICKED(ID_ANAAMLPARAMDLG_BT_DELETE_TARGET, &CAnaAmlParamDlg::OnBnClickedDeleteTarget)
	ON_MESSAGE(WM_ROWSELECTCHANGE,OnMsgRowSelectedChange)
END_MESSAGE_MAP()

BOOL CAnaAmlParamDlg::OnEraseBkgnd(CDC* pDC)
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	if(NULL == pLangInfo)
	{
		return TRUE;
	}

	double dHoriRatio = m_dHoriRatio;
	double dVertRatio = m_dVertRatio;

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
	rcUseful.left = rect.left + (rect.Width() - m_iWndWidth) / 2 - m_iWndWidth / 5 * 2.2;
	rcUseful.top = rect.top + (rect.Height() - m_iWndHeight) / 2;
	rcUseful.right = rcUseful.left + m_iWndWidth;
	rcUseful.bottom = rcUseful.top + m_iWndHeight;

	CPen pen;
	CPen* pOldPen = NULL;

	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top + m_iCaptionHeight,rcUseful.Width(),rcUseful.Height() - m_iCaptionHeight,0,0,m_roundValue,m_roundValue,FALSE,Color(197,197,197),m_frameWidth,TRUE,Color(255,255,255),1);
	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top,rcUseful.Width(),m_iCaptionHeight,m_roundValue,m_roundValue,0,0,FALSE,Color(3,91,199),2,TRUE,COLOR_SOFT_THEME,1);

	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(RGB(255,255,255));
	memDc.DrawText(pLangInfo->GetText(100),CRect(rcUseful.left + 10,rcUseful.top + 5,rcUseful.left + rcUseful.Width() - 10,rcUseful.top + m_iCaptionHeight - 5),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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

int CAnaAmlParamDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_iWndWidth *= m_dHoriRatio;
	m_iWndHeight *= m_dVertRatio;
	m_iCaptionHeight *= m_dVertRatio;
	return CDialogEx::OnCreate(lpCreateStruct);
}

void CAnaAmlParamDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
#ifdef POPDLG_USESHADEBK
	CDialogEx::OnLButtonDown(nFlags, point);
#else
	if(point.y < m_iCaptionHeight)
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

BOOL CAnaAmlParamDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

#ifdef POPDLG_USESHADEBK
	SetWindowPos(GetParent(),theApp.m_rcWorkArea.left,theApp.m_rcWorkArea.top,theApp.m_rcWorkArea.Width(),theApp.m_rcWorkArea.Height(),SWP_NOZORDER);
#else
	SetWindowPos(GetParent(),0 * m_dHoriRatio,0 * m_dVertRatio,m_iWndWidth * m_dHoriRatio,m_iWndHeight * m_dVertRatio,SWP_NOZORDER);
	SetRoundRect(m_roundValue,m_roundValue);
	CenterWindow(GetParent());
#endif
	
	CreateCtrl();
	InitDlg();
	InitCtrl();
	SetCtrlPos();

	SetCtrlText();
	SetLstAmlAnaBaseParam(m_pAnalysisPara);

	ShowCtrl();

	return TRUE;
}

void CAnaAmlParamDlg::CreateCtrl()
{
	CreateBt();
	CreateLst();
}

void CAnaAmlParamDlg::CreateBt()
{
	m_btSave.Create(_T("保存"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_ANAAMLPARAMDLG_BT_SAVE);
	m_btCancel.Create(_T("取消"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_ANAAMLPARAMDLG_BT_CANCEL);
	m_btnAddTarget.Create(_T("增加目标"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_ANAAMLPARAMDLG_BT_ADD_TARGET);
	m_btnDeleteTarget.Create(_T("删除目标"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_ANAAMLPARAMDLG_BT_DELETE_TARGET);
}


void CAnaAmlParamDlg::CreateLst()
{
	m_lstAmlParam.Create(IDD_MIDDLEDLG,this);
}

void CAnaAmlParamDlg::InitDlg()
{
	int iFontH = 16 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 17 * m_dTxRatio;
	}	

	SetTextClr1(RGB(0X39,0X45,0X65));
	SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);

	return;
}

void CAnaAmlParamDlg::InitCtrl()
{
	InitBt();
	InitLst();
}

void CAnaAmlParamDlg::InitBt()
{
	int iFontH = 18 * m_dTxRatio;

	m_btnAddTarget.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btnAddTarget.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);

	m_btnDeleteTarget.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btnDeleteTarget.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);
	m_btnDeleteTarget.EnableWindow(FALSE);

	m_btSave.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btSave.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);
	m_btSave.SetBkColor(Color(0X38,0XE0,0XEF),Color(0X3A,0XEF,0XFF),COLOR_SOFT_THEME,Color(0XF1,0XF1,0XF1));
	m_btSave.SetBkColorEnd(COLOR_SOFT_THEME,Color(0X38,0XE0,0XEF),Color(0X02,0X8D,0X99),Color(0XD8,0XD8,0XD8));
	m_btSave.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XC6,0XC4,0XC4));

	m_btCancel.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btCancel.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);
}

void CAnaAmlParamDlg::InitLst()
{
	int iFontH = 15 * m_dTxRatio;
	int iFontH2 = 16 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 16 * m_dTxRatio;
		iFontH2 = 17 * m_dTxRatio;
	}

	m_lstAmlParam.SetHeaderHeight(28 * m_dVertRatio);
	m_lstAmlParam.SetMyHeaderFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_lstAmlParam.SetHeaderBkColor(Color(0XF1,0XF4,0XFa));
	m_lstAmlParam.SetHeaderTextColor(RGB(0X39,0X45,0X65));
	m_lstAmlParam.SetMyTitleFont(m_stTxFont,iFontH,FW_BOLD,TYPE_PIXEL);
	m_lstAmlParam.SetItemHeight(30 * m_dVertRatio);

	int iTempInt1 = 34 * m_dVertRatio;
	m_lstAmlParam.m_horiScroll.m_arrowWH = iTempInt1;
	m_lstAmlParam.m_horiScroll.m_scrollH = iTempInt1;
	m_lstAmlParam.m_vertScroll.m_arrowWH = iTempInt1;
	m_lstAmlParam.m_vertScroll.m_scrollW = iTempInt1;
	m_lstAmlParam.SetMyFont(m_stTxFont,iFontH2,FW_NORMAL,TYPE_PIXEL);
	m_lstAmlParam.SetTextClr1(RGB(0X39,0X45,0X65));
	//m_lstAmlParam.SetBeShowRowSelectState(FALSE);
	m_lstAmlParam.SetBeSingleSelect(TRUE);
	m_lstAmlParam.SetRowBkSelectedColor(RGB(0XE1,0XE1,0XE1));
	m_lstAmlParam.SetBeTellParentRowSelChange(TRUE);

	m_lstAmlParam.SetFuncEditCtrlFont(m_stTxFont,iFontH2,FW_NORMAL,TYPE_PIXEL);

	m_lstAmlParam.InsertColumn(0,theApp.m_pLangInfo->GetText(111));
	m_lstAmlParam.InsertColumn(1,theApp.m_pLangInfo->GetText(112));
	m_lstAmlParam.InsertColumn(2,theApp.m_pLangInfo->GetText(53));
	m_lstAmlParam.InsertColumn(3,theApp.m_pLangInfo->GetText(294));
	m_lstAmlParam.InsertColumn(4,theApp.m_pLangInfo->GetText(114));
	m_lstAmlParam.InsertColumn(5,theApp.m_pLangInfo->GetText(115));
	m_lstAmlParam.InsertColumn(6,theApp.m_pLangInfo->GetText(117));
	m_lstAmlParam.InsertColumn(7,theApp.m_pLangInfo->GetText(118));
	m_lstAmlParam.InsertColumn(8,theApp.m_pLangInfo->GetText(119));
	m_lstAmlParam.InsertColumn(9,theApp.m_pLangInfo->GetText(279));
	m_lstAmlParam.InsertColumn(10,theApp.m_pLangInfo->GetText(280));

	CLinkList<CString> lnkCbStr;

	m_lstAmlParam.SetColumnEditable(2,TRUE);
	m_lstAmlParam.SetColumnEditable(4,TRUE);
	m_lstAmlParam.SetColumnEditable(5,TRUE);
	m_lstAmlParam.SetColumnEditable(7,TRUE);
	m_lstAmlParam.SetColumnEditable(8,TRUE);
	m_lstAmlParam.SetColumnEditable(9,TRUE);
	m_lstAmlParam.SetColumnEditable(10,TRUE);

	m_lstAmlParam.SetColumnCheckFunc(3,TRUE);
	m_lstAmlParam.SetColumnCheckFunc(6,TRUE);

	double dHoriRatio = m_dHoriRatio;
	int width = 849 * dHoriRatio;
	int tempInt = 32 * dHoriRatio;

	m_lstAmlParam.SetColumnWidth(0,tempInt);
	width -= tempInt;
	tempInt = 72 * dHoriRatio;
	m_lstAmlParam.SetColumnWidth(1,tempInt);
	width -= tempInt;
	m_lstAmlParam.SetColumnWidth(2,tempInt);
	width -= tempInt;
	tempInt = int(80 * dHoriRatio);
	m_lstAmlParam.SetColumnWidth(3,tempInt);
	width -= tempInt;
	m_lstAmlParam.SetColumnWidth(4,tempInt);
	width -= tempInt;
	m_lstAmlParam.SetColumnWidth(5,tempInt);
	width -= tempInt;
	m_lstAmlParam.SetColumnWidth(6,tempInt);
	width -= tempInt;
	tempInt = int(90 * dHoriRatio);
	m_lstAmlParam.SetColumnWidth(7,tempInt);
	width -= tempInt;
	m_lstAmlParam.SetColumnWidth(8,tempInt);
	width -= tempInt;
	m_lstAmlParam.SetColumnWidth(9,tempInt);
	width -= tempInt;
	width -= m_lstAmlParam.GetFrameLineWidth() * 2;
	m_lstAmlParam.SetColumnWidth(10,width);

	CLinkList<int> lnkControledCol;
	int colId = 4;
	lnkControledCol.AddTail(colId);
	colId = 5;
	lnkControledCol.AddTail(colId);
	m_lstAmlParam.SetMapColumnDisableControl(3,lnkControledCol);

	lnkControledCol.Clear();
	colId = 7;
	lnkControledCol.AddTail(colId);
	colId = 8;
	lnkControledCol.AddTail(colId);
	m_lstAmlParam.SetMapColumnDisableControl(6,lnkControledCol);

	m_lstAmlParam.AddPairToMapDisabledStateKeyString(_T("FALSE"),FALSE);
	m_lstAmlParam.AddPairToMapDisabledStateKeyString(_T("TRUE"),TRUE);

	CMaxMinInfo maxMinInfo;
	maxMinInfo.SetBeOnlyIntergerNumber(TRUE);
	maxMinInfo.SetBeSetMinValue(TRUE);
	maxMinInfo.SetMinValue(1);
	maxMinInfo.SetBeSetMaxValue(TRUE);
	maxMinInfo.SetMaxValue(100);
	maxMinInfo.SetBeMaxMinSimultaneousEffect(TRUE);
	m_lstAmlParam.SetColumnMaxMinInfo(3,maxMinInfo);
	m_lstAmlParam.SetColumnMaxMinInfo(4,maxMinInfo);

	maxMinInfo.ClearMaxMinLimit();
	maxMinInfo.SetBeOnlyFloatNumber(TRUE);
	maxMinInfo.SetBeAllowNegtiveNumber(TRUE);
	m_lstAmlParam.SetColumnMaxMinInfo(7,maxMinInfo);

	return;
}

void CAnaAmlParamDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	CRect rcUseful;
#ifdef POPDLG_USESHADEBK
	rcUseful.left = rect.left + (rect.Width() - m_iWndWidth) / 2 - m_iWndWidth / 5 * 2.2;
	rcUseful.top = rect.top + (rect.Height() - m_iWndHeight) / 2;
	rcUseful.right = rcUseful.left + m_iWndWidth;
	rcUseful.bottom = rcUseful.top + m_iWndHeight;
#endif

	m_lstAmlParam.SetWindowPos(this,rcUseful.left + int(10 * m_dHoriRatio),rcUseful.top + int(70 * m_dVertRatio),int(849 * m_dHoriRatio),int(446 * m_dVertRatio),SWP_NOZORDER);

	m_btnAddTarget.SetWindowPos(this,rcUseful.left + int(20 * m_dHoriRatio),rcUseful.top + int(534 * m_dVertRatio),int(120 * m_dHoriRatio),int(40 * m_dVertRatio),SWP_NOZORDER);
	m_btnDeleteTarget.SetWindowPos(this,rcUseful.left + int(155 * m_dHoriRatio),rcUseful.top + int(534 * m_dVertRatio),int(120 * m_dHoriRatio),int(40 * m_dVertRatio),SWP_NOZORDER);

	m_btSave.SetWindowPos(this,rcUseful.left + int(567 * m_dHoriRatio),rcUseful.top + int(534 * m_dVertRatio),int(120 * m_dHoriRatio),int(40 * m_dVertRatio),SWP_NOZORDER);
	m_btCancel.SetWindowPos(this,rcUseful.left + int(701 * m_dHoriRatio),rcUseful.top + int(534 * m_dVertRatio),int(120 * m_dHoriRatio),int(40 * m_dVertRatio),SWP_NOZORDER);

	return;
}

void CAnaAmlParamDlg::ShowCtrl()
{
	m_lstAmlParam.ShowWindow(SW_SHOW);

	m_btnAddTarget.ShowWindow(SW_SHOW);
	m_btnDeleteTarget.ShowWindow(SW_SHOW);

	m_btSave.ShowWindow(SW_SHOW);
	m_btCancel.ShowWindow(SW_SHOW);

	return;
}

void CAnaAmlParamDlg::SetCtrlText()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	if(NULL == pLangInfo)
	{
		return;
	}
	m_btnAddTarget.SetWindowText(pLangInfo->GetText(522));
	m_btnDeleteTarget.SetWindowText(pLangInfo->GetText(523));

	m_btSave.SetWindowText(pLangInfo->GetText(148));
	m_btCancel.SetWindowText(pLangInfo->GetText(150));

	return;
}

void CAnaAmlParamDlg::OnBnClickedSave()
{
	m_lstAmlParam.EndEDit();

	int iRowNum = m_lstAmlParam.GetRowNum();
	for (int i=0; i<iRowNum; i++)
	{
		CString strChannel = m_lstAmlParam.GetItemText(i, 1);
		if (strChannel.IsEmpty())
		{
			PopNormalTipInfo(theApp.m_pLangInfo->GetText(489));
			return;
		}

		CString strTarget = m_lstAmlParam.GetItemText(i, 2);

		for (int j=i+1; j<iRowNum; j++)
		{
			CString strChannel2 = m_lstAmlParam.GetItemText(j, 1);
			CString strTarget2 = m_lstAmlParam.GetItemText(j, 2);

			if (strChannel == strChannel2 && strTarget == strTarget2)
			{
				PopNormalTipInfo(theApp.m_pLangInfo->GetText(490));
				return;
			}
		}
	}

	int iIndex = 0;
	for(int i=0; i<iRowNum; ++i)
	{
		tagAnaAmplifyPara* pAmlParam = m_pAnalysisPara->GetAmplifyParaBy(iIndex);
		if (pAmlParam != NULL && pAmlParam->strDye == _T("MOT"))
		{
			iIndex ++;
			pAmlParam = m_pAnalysisPara->GetAmplifyParaBy(iIndex);
		}
		if (pAmlParam == NULL)
		{
			pAmlParam = new tagAnaAmplifyPara;
			m_bChanged = TRUE;
			m_pAnalysisPara->AddAmplifyPara(pAmlParam);
		}

		CString strDye = m_lstAmlParam.GetItemText(i,1);
		CString strTarget = m_lstAmlParam.GetItemText(i,2);
		if (strDye != pAmlParam->strDye || strTarget != pAmlParam->strTarget)
		{
			m_bChanged = TRUE;
			pAmlParam->strDye = strDye;
			pAmlParam->strTarget = strTarget;
		}

		BOOL bAutoBaseline = FALSE;
		m_lstAmlParam.GetCellCheckState(i,3, bAutoBaseline);
		if (bAutoBaseline != pAmlParam->bAutoBaseline)
		{
			m_bChanged = TRUE;
			pAmlParam->bAutoBaseline = bAutoBaseline;
		}

		int iBaselineStart = _ttoi(m_lstAmlParam.GetItemText(i,4));
		int iBaselineEnd = _ttoi(m_lstAmlParam.GetItemText(i,5));
		if (!bAutoBaseline && (iBaselineStart != pAmlParam->iBaselineStart || iBaselineEnd != pAmlParam->iBaselineEnd))
		{
			m_bChanged = TRUE;
			pAmlParam->iBaselineStart = iBaselineStart;
			pAmlParam->iBaselineEnd = iBaselineEnd;
		}
		else
		{
			pAmlParam->iBaselineStart = iBaselineStart;
			pAmlParam->iBaselineEnd = iBaselineEnd;
		}

		BOOL bAutoThreshold = FALSE;
		m_lstAmlParam.GetCellCheckState(i, 6, bAutoThreshold);
		eSplineThresholdType nSplineType = SPLINE_THRESHOLD_AUTO;
		if(!bAutoThreshold)
		{
			nSplineType = SPLINE_THRESHOLD_NORMALIZE;
		}

		if (nSplineType != pAmlParam->nThresholdType)
		{
			m_bChanged = TRUE;
			pAmlParam->nThresholdType = nSplineType;
		}

		double dNormalizeThreshold = _ttof(m_lstAmlParam.GetItemText(i,7));
		double dDeltaRnThreshold = _ttof(m_lstAmlParam.GetItemText(i,8));
		if (int(dNormalizeThreshold*100) != int(pAmlParam->dNormalizeThreshold*100) || int(dDeltaRnThreshold*100) != int(pAmlParam->dDeltaRnThreshold*100))
		{
			m_bChanged = TRUE;
			pAmlParam->dNormalizeThreshold = dNormalizeThreshold;
			pAmlParam->dDeltaRnThreshold = dDeltaRnThreshold;
		}

		double dLowerThreshold = _ttof(m_lstAmlParam.GetItemText(i,9));;
		double dUpperThreshold = _ttof(m_lstAmlParam.GetItemText(i,10));;
		if (int(dLowerThreshold*100) != int(pAmlParam->dLowerThreshold*100) || int(dUpperThreshold*100) != int(pAmlParam->dUpperThreshold*100))
		{
			m_bChanged = TRUE;
			pAmlParam->dLowerThreshold = dLowerThreshold;
			pAmlParam->dUpperThreshold = dUpperThreshold;
		}

		iIndex ++;
	}

	for (int i=m_pAnalysisPara->GetAmplifyParaCount()-1; i>=iIndex; i--)
	{
		m_pAnalysisPara->DeleteOneAmplifyPara(i);
	}

	OnOK();
}

void CAnaAmlParamDlg::OnBnClickedCancel()
{
	OnCancel();
	return;
}


BOOL CAnaAmlParamDlg::SetRoundRect(int x /*= 5*/,int y /*= 5*/)
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

int CAnaAmlParamDlg::SetLstAmlAnaBaseParam(CDataAnalysisPara* pAnalysisPara)
{
	m_lstAmlParam.Clear();

	if(NULL == pAnalysisPara)
	{
		return -1;
	}

	CString strTemp(_T(""));
	int iAmpParamCount = pAnalysisPara->GetAmplifyParaCount();
	
	int iIndex = 0;
	for(int i = 0;i < iAmpParamCount;++i)
	{
		CPtrArray* pAryAmpParam = pAnalysisPara->GetArrayAmplifyParaPtr();
		tagAnaAmplifyPara* pAnaAmpParam = (tagAnaAmplifyPara*)pAryAmpParam->GetAt(i);
		if(NULL == pAnaAmpParam || pAnaAmpParam->strDye == _T("MOT"))
			continue;

		m_lstAmlParam.InsertRowSetHeight(iIndex,24 * m_dVertRatio);
		SetOneAmlBaseParam(iIndex, pAnaAmpParam);
		if (iIndex < m_iChannelCount)
		{
			m_lnkCbStrChannel.AddTail(pAnaAmpParam->strDye);
		}
		else
		{
			m_lstAmlParam.SetCellSelfCbInfo(iIndex, 1, TRUE, TRUE, m_lnkCbStrChannel, FALSE);
		}
		iIndex ++;
	}
	
	m_lstAmlParam.SetAllRowHeight(int(35 * m_dVertRatio));
	m_lstAmlParam.Invalidate(FALSE);

	return 0;
}


void CAnaAmlParamDlg::SetOneAmlBaseParam(int iRowIndex, tagAnaAmplifyPara* pInputParam)
{
	CString tempStr(_T(""));

	tempStr.Format(_T("%d"),iRowIndex + 1);
	m_lstAmlParam.SetItemText(iRowIndex,0,tempStr);

	m_lstAmlParam.SetItemText(iRowIndex,1,pInputParam->strDye);
	m_lstAmlParam.SetItemText(iRowIndex,2,pInputParam->strTarget);

	m_lstAmlParam.SetCellCheckState(iRowIndex,3,pInputParam->bAutoBaseline);

	tempStr.Format(_T("%d"),pInputParam->iBaselineStart);
	m_lstAmlParam.SetItemText(iRowIndex,4,tempStr);
	tempStr.Format(_T("%d"),pInputParam->iBaselineEnd);
	m_lstAmlParam.SetItemText(iRowIndex,5,tempStr);

	m_lstAmlParam.SetCellCheckState(iRowIndex,6,SPLINE_THRESHOLD_AUTO == pInputParam->nThresholdType);
	tempStr.Format(_T("%.2lf"),pInputParam->dNormalizeThreshold);
	m_lstAmlParam.SetItemText(iRowIndex,7,tempStr);
	tempStr.Format(_T("%.2lf"),pInputParam->dDeltaRnThreshold);
	m_lstAmlParam.SetItemText(iRowIndex,8,tempStr);

	tempStr.Format(_T("%.2lf"), pInputParam->dLowerThreshold);
	m_lstAmlParam.SetItemText(iRowIndex, 9,tempStr);
	tempStr.Format(_T("%.2lf"), pInputParam->dUpperThreshold);
	m_lstAmlParam.SetItemText(iRowIndex, 10,tempStr);
}


void CAnaAmlParamDlg::OnBnClickedAddTarget()
{
	tagAnaAmplifyPara sTargetParam;
	int iRowNum = m_lstAmlParam.GetRowNum();	
	m_lstAmlParam.InsertRow(iRowNum);
	SetOneAmlBaseParam(iRowNum, &sTargetParam);
	m_lstAmlParam.SetAllRowHeight(int(35 * m_dVertRatio));

	m_lstAmlParam.SetCellSelfCbInfo(iRowNum, 1, TRUE, TRUE, m_lnkCbStrChannel, FALSE);
	m_lstAmlParam.Invalidate();
}

void CAnaAmlParamDlg::OnBnClickedDeleteTarget()
{
	vector<int> vecSelectedRowId = m_lstAmlParam.GetSelectedRowId();
	if(vecSelectedRowId.size() < 1)
		return;

	m_lstAmlParam.DeleteMultiRow(vecSelectedRowId);	//m_lstSelPrj是 行单选类型

	int iRowNum = m_lstAmlParam.GetRowNum();
	if(iRowNum > vecSelectedRowId[0])
	{
		CString strTemp(_T(""));
		for(int i = vecSelectedRowId[0];i < iRowNum;++i)
		{
			strTemp.Format(_T("%d"),i + 1);
			m_lstAmlParam.SetItemText(i,0,strTemp);
		}
	}

	m_btnDeleteTarget.EnableWindow(FALSE);
}

LRESULT CAnaAmlParamDlg::OnMsgRowSelectedChange(WPARAM wParam, LPARAM lParam)
{
	vector<int> vecSelectedRowId;
	vecSelectedRowId = m_lstAmlParam.GetSelectedRowId();
	if(vecSelectedRowId.size() < 1)
	{
		return 1;
	}

	m_btnDeleteTarget.EnableWindow(vecSelectedRowId[0] >= m_iChannelCount);

	return 0;
}
