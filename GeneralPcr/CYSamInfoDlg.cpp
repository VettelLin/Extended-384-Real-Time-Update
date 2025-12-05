#include "StdAfx.h"
#include "CYSamInfoDlg.h"
#include "GeneralPcr.h"

IMPLEMENT_DYNAMIC(CCYSamInfoDlg,CSamInfoDlg)

CCYSamInfoDlg::CCYSamInfoDlg(CWnd* pParent /*= NULL*/,CInterfaceC* pIfC /*= NULL*/): CSamInfoDlg(pParent,pIfC)
{

}

CCYSamInfoDlg::~CCYSamInfoDlg(void)
{

}

void CCYSamInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CSamInfoDlg::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX,ID_CYSAMINFODLG_DT_COLLECT_DATE,m_oleCYDt);
}

BEGIN_MESSAGE_MAP(CCYSamInfoDlg,CSamInfoDlg)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_DATECTRLCHANGED,OnMsgDateCtrlChanged) 
	ON_MESSAGE(WM_TELLPARENTTEXTCHANGED,OnMsgTellParentTextChanged) 
	ON_MESSAGE(WM_TELLPARENTSETNEWTEXT,OnMsgTellParentSetNewText)
END_MESSAGE_MAP()

BOOL CCYSamInfoDlg::OnEraseBkgnd(CDC* pDC)
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

	DrawRoundRectangleNew(&memDc,rect.left,rect.top,rect.Width() - 1,rect.Height() - 1,8,8,8,8,TRUE,Color(0XD8,0XD8,0XD8),1,TRUE,Color(0XFF,0XFF,0XFF),1);

	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);
	memDc.SetBkMode(TRANSPARENT);

	int iTempInt1 = 16 * m_dHoriRatio,iTempInt2 = 122 * m_dHoriRatio;
	int iStartY = 20 * m_dVertRatio,iTempInt4 = 56 * m_dVertRatio,iTempInt5 = 40 * m_dVertRatio,iEndY = iStartY + iTempInt5;

	UINT uiTxFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE;

	memDc.DrawText(pLangInfo->GetText(47),CRect(iTempInt1,iStartY,iTempInt2,iEndY),uiTxFormat);

	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;
	memDc.DrawText(pLangInfo->GetText(48),CRect(iTempInt1,iStartY,iTempInt2,iEndY),uiTxFormat);

	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;
	memDc.DrawText(pLangInfo->GetText(64),CRect(iTempInt1,iStartY,iTempInt2,iEndY),uiTxFormat);

	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;
	memDc.DrawText(pLangInfo->GetText(69),CRect(iTempInt1,iStartY,iTempInt2,iEndY),uiTxFormat);

	//¿µÑÐÝá×¨ÊôÄÚÈÝ
	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;
	memDc.DrawText(pLangInfo->GetText(403),CRect(iTempInt1,iStartY,iTempInt2,iEndY),uiTxFormat);

	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;
	memDc.DrawText(pLangInfo->GetText(404),CRect(iTempInt1,iStartY,iTempInt2,iEndY),uiTxFormat);

	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;
	memDc.DrawText(pLangInfo->GetText(405),CRect(iTempInt1,iStartY,iTempInt2,iEndY),uiTxFormat);

	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;
	memDc.DrawText(pLangInfo->GetText(406),CRect(iTempInt1,iStartY,iTempInt2,iEndY),uiTxFormat);

	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;
	memDc.DrawText(pLangInfo->GetText(407),CRect(iTempInt1,iStartY,iTempInt2,iEndY),uiTxFormat);

	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;
	memDc.DrawText(pLangInfo->GetText(20),CRect(iTempInt1,iStartY,iTempInt2,iEndY),uiTxFormat);

	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

	Node<CRect>* pNodeRst = m_lnkRcRst.GetHead();
	while(NULL != pNodeRst)
	{
		pDC->BitBlt(pNodeRst->data.left,pNodeRst->data.top,pNodeRst->data.Width(),pNodeRst->data.Height(),&memDc,pNodeRst->data.left,pNodeRst->data.top,SRCCOPY);
		pNodeRst = pNodeRst->pNext;
	}

	memDc.SelectObject(pOldFont);

	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();

	return TRUE;
}

LRESULT CCYSamInfoDlg::OnMsgDateCtrlChanged(WPARAM wParam,LPARAM lParam)
{
	int iRet = CSamInfoDlg::OnMsgDateCtrlChanged(wParam,lParam);
	if(0 != iRet)
	{
		return iRet;
	}

	if(m_bClear)
	{
		return 1;
	}

	CMyDateTimeCtrl* pDt = NULL;
	COleDateTime* pOleDt = NULL;
	EITEMINFOTYPE eItemInfoType = (EITEMINFOTYPE)wParam;

	switch(eItemInfoType)
	{
	case ITEM_CYSAMINFO_DT_COLLECTDATE: pDt = &m_dtCYCollectDate; pOleDt = &m_oleCYDt;break;
	default: return 0;
	}

	if(NULL == pDt->GetSafeHwnd())
	{
		return 2;
	}
	if(NULL == pOleDt)
	{
		return 3;
	}

	UpdateData(TRUE);
	CString strTemp(_T(""));
	strTemp = pOleDt->Format(_T("%Y-%m-%d %H:%M:%S"));

	CString strYMD = strTemp.Left(10);
	CString strCurTime(_T(""));
	GetCurrentTmTime(strCurTime,2,_T("-"));
	if(strYMD != strCurTime)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(396));
	}

	if(NULL != m_pIfC)
	{
		m_pIfC->SetPlateSelectedSiteSamInfo(eItemInfoType,strTemp);
	}

	return 2;
}

LRESULT CCYSamInfoDlg::OnMsgTellParentTextChanged(WPARAM wParam,LPARAM lParam)
{
	int iRet = CSamInfoDlg::OnMsgTellParentTextChanged(wParam,lParam);
	if(0 != iRet)
	{
		return iRet;
	}

	CMyNewEdit* pEdit = NULL;
	EITEMINFOTYPE eItemInfoType = (EITEMINFOTYPE)wParam;
	EITEMINFODATATYPE eItemInfoDataType = EIIDT_NONE;

	switch(eItemInfoType)
	{
	case ITEM_CYSAMINFO_CB_SAMCATEGORY: pEdit = &m_cbSamCategory;	eItemInfoDataType = EIIDT_STRING;	break;
	case ITEM_CYSAMINFO_ED_PRODUCTBN:	pEdit = &m_edProductBN;	eItemInfoDataType = EIIDT_STRING;	break;
	case ITEM_CYSAMINFO_ED_SAMCOUNT:	pEdit = &m_edSamCount;	eItemInfoDataType = EIIDT_STRING;	break;
	case ITEM_CYSAMINFO_ED_SAMSTATE:	pEdit = &m_edSamState;	eItemInfoDataType = EIIDT_STRING;	break;
	case ITEM_CYSAMINFO_ED_COMMENT:	pEdit = &m_edComment;	eItemInfoDataType = EIIDT_INT;	break;
	case ITEM_CYSAMINFO_ED_TESTITEM:	pEdit = &m_edTestItem;	eItemInfoDataType = EIIDT_STRING;	break;
	
	default: break;
	}

	if(NULL == pEdit->GetSafeHwnd())
	{
		return 1;
	}

	CString strTemp(_T(""));
	strTemp = pEdit->GetText();

	int iData = 0;
	double dData = 0;

	switch(eItemInfoDataType)
	{
	case EIIDT_INT:	iData = _ttoi(strTemp);	break;
	case EIIDT_DOUBLE:	dData = _ttof(strTemp);	break;
	default:	break;
	}

	if(NULL != m_pIfC)
	{
		m_pIfC->SetPlateSelectedSiteSamInfo(eItemInfoType,strTemp,iData,dData);
		return 0;
	}

	return 2;
}

LRESULT CCYSamInfoDlg::OnMsgTellParentSetNewText(WPARAM wParam,LPARAM lParam)
{
	int iRet = CSamInfoDlg::OnMsgTellParentSetNewText(wParam,lParam);
	if(0 != iRet)
	{
		return iRet;
	}

	CMyNewCb* pCb = NULL;
	EITEMINFOTYPE eItemInfoType = (EITEMINFOTYPE)wParam;
	EITEMINFODATATYPE eItemInfoDataType = EIIDT_NONE;

	switch(eItemInfoType)
	{
	case ITEM_CYSAMINFO_CB_SAMCATEGORY:	pCb = &m_cbSamCategory;	eItemInfoDataType = EIIDT_STRING;	break;

	default:	break;
	}

	if(NULL == pCb->GetSafeHwnd())
	{
		return 1;
	}

	CString strTemp(_T(""));
	strTemp = pCb->GetText();

	int iData = 0;
	double dData = 0;

	switch(eItemInfoDataType)
	{
	case EIIDT_INT:	iData = _ttoi(strTemp);	break;
	default:	break;
	}

	if(NULL != m_pIfC)
	{
		m_pIfC->SetPlateSelectedSiteSamInfo(eItemInfoType,strTemp,iData,dData);
		return 0;
	}

	return 2;
}

void CCYSamInfoDlg::CreateCb()
{
	CSamInfoDlg::CreateCb();

	CRect rcCreate(0,0,0,0);

	m_cbSamCategory.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rcCreate,this,ID_CYSAMINFODLG_CB_SAMCATEGORY);

	return;
}

void CCYSamInfoDlg::CreateEd()
{
	CSamInfoDlg::CreateEd();

	CRect rcTemp(0,0,0,0);

	m_edProductBN.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rcTemp,this,ID_CYSAMINFODLG_ED_PRODUCTBN);
	m_edSamCount.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rcTemp,this,ID_CYSAMINFODLG_ED_SAMCOUNT);
	m_edSamState.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rcTemp,this,ID_CYSAMINFODLG_ED_SAMSTATE);

	m_edComment.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rcTemp,this,ID_CYSAMINFODLG_ED_COMMENT);
	m_edTestItem.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rcTemp,this,ID_CYSAMINFODLG_ED_TESTITEM);

	return;
}

void CCYSamInfoDlg::CreateDt()
{
	CSamInfoDlg::CreateDt();
	m_dtCYCollectDate.Create(WS_CHILD | WS_TABSTOP | DTS_SHOWNONE | DTS_SHORTDATEFORMAT,CRect(0,0,0,0),this,ID_CYSAMINFODLG_DT_COLLECT_DATE);

	return;
}

void CCYSamInfoDlg::InitCb()
{
	CSamInfoDlg::InitCb();

	InitOneCb(&m_cbSamCategory);
	m_cbSamCategory.SetTellParentSetNewText(TRUE,ITEM_CYSAMINFO_CB_SAMCATEGORY);
	m_cbSamCategory.SetTellParentTxChangedParam(TRUE,ITEM_CYSAMINFO_CB_SAMCATEGORY);

	return;
}

void CCYSamInfoDlg::InitEd()
{
	CSamInfoDlg::InitEd();

	InitOneEd(&m_edProductBN);
	m_edProductBN.SetTellParentTxChangedParam(TRUE,ITEM_CYSAMINFO_ED_PRODUCTBN);

	InitOneEd(&m_edSamCount);
	m_edSamCount.SetTellParentTxChangedParam(TRUE,ITEM_CYSAMINFO_ED_SAMCOUNT);

	InitOneEd(&m_edSamState);
	m_edSamState.SetTellParentTxChangedParam(TRUE,ITEM_CYSAMINFO_ED_SAMSTATE);

	InitOneEd(&m_edComment);
	m_edComment.SetTellParentTxChangedParam(TRUE,ITEM_CYSAMINFO_ED_COMMENT);

	InitOneEd(&m_edTestItem);
	m_edTestItem.SetTellParentTxChangedParam(TRUE,ITEM_CYSAMINFO_ED_TESTITEM);

	return;
}

void CCYSamInfoDlg::InitDt()
{
	CSamInfoDlg::InitDt();

	m_oleCYDt = COleDateTime::GetCurrentTime();
	UpdateData(FALSE);

	m_dtCYCollectDate.SetMyFont(m_stTxFont,16 * m_dTxRatio);
	m_dtCYCollectDate.SetTellParentDtChangedParam(TRUE,ITEM_CYSAMINFO_DT_COLLECTDATE);

	return;
}

void CCYSamInfoDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	m_rcInit = rect;
	ClearRectToDelete();

	int iTempInt1 = 124 * m_dHoriRatio,iTempInt2 = 220 * m_dHoriRatio,iTempInt3 = 40 * m_dVertRatio;
	int iStartY = 20 * m_dVertRatio,iTempInt4 = 56 * m_dVertRatio;

	m_cbSamType.MoveWindow(iTempInt1,iStartY,iTempInt2,iTempInt3,FALSE);
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);

	iStartY += iTempInt4;
	m_edSamName.MoveWindow(iTempInt1,iStartY,iTempInt2,iTempInt3,FALSE);
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);

	iStartY += iTempInt4;
	m_edNewId.MoveWindow(iTempInt1,iStartY,iTempInt2,iTempInt3,FALSE);
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);

	iStartY += iTempInt4;
	m_dtCYCollectDate.MoveWindow(iTempInt1,iStartY,iTempInt2,iTempInt3,FALSE);
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);

	iStartY += iTempInt4;
	m_cbSamCategory.MoveWindow(iTempInt1,iStartY,iTempInt2,iTempInt3,FALSE);
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);

	iStartY += iTempInt4;
	m_edSamCount.MoveWindow(iTempInt1,iStartY,iTempInt2,iTempInt3,FALSE);
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);

	iStartY += iTempInt4;
	m_edSamState.MoveWindow(iTempInt1,iStartY,iTempInt2,iTempInt3,FALSE);
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);

	iStartY += iTempInt4;
	m_edProductBN.MoveWindow(iTempInt1,iStartY,iTempInt2,iTempInt3,FALSE);
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);

	iStartY += iTempInt4;
	m_edTestItem.MoveWindow(iTempInt1,iStartY,iTempInt2,iTempInt3,FALSE);
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);

	iStartY += iTempInt4;
	m_edComment.MoveWindow(iTempInt1,iStartY,iTempInt2,iTempInt3,FALSE);
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);

	CalcRectResult();

	return;
}

void CCYSamInfoDlg::ShowCtrl()
{
	m_cbSamType.ShowWindow(SW_SHOW);
	m_edSamName.ShowWindow(SW_SHOW);
	m_edNewId.ShowWindow(SW_SHOW);

	m_dtCYCollectDate.ShowWindow(SW_SHOW);

	m_cbSamCategory.ShowWindow(SW_SHOW);
	m_edProductBN.ShowWindow(SW_SHOW);
	m_edSamCount.ShowWindow(SW_SHOW);
	m_edSamState.ShowWindow(SW_SHOW);
	m_edComment.ShowWindow(SW_SHOW);
	m_edTestItem.ShowWindow(SW_SHOW);

	return;
}

void CCYSamInfoDlg::SetCtrlText()
{
	CSamInfoDlg::SetCtrlText();

	return;
}

void CCYSamInfoDlg::ClearSamInfo()
{
	CSamInfoDlg::ClearSamInfo();

	return;
}

void CCYSamInfoDlg::SetSamInfo(CTubeInfo& tubeInfo)
{
	CSamInfoDlg::SetSamInfo(tubeInfo);

	return;
}

void CCYSamInfoDlg::SetEditable(BOOL bEditable)
{
	CSamInfoDlg::SetEditable(bEditable);

	return;
}

BOOL CCYSamInfoDlg::GetEditable()
{
	return m_bEditable;
}

int CCYSamInfoDlg::RefreshLanguageDisplay()
{
	CSamInfoDlg::RefreshLanguageDisplay();

	m_cbSamCategory.SetMyFont(m_stTxFont);
	m_cbSamCategory.m_selWnd.SetMyFont(m_stTxFont);
	m_cbSamCategory.TextParamChanged();

	m_edSamCount.SetMyFont(m_stTxFont);
	m_edSamCount.TextParamChanged();

	m_edSamState.SetMyFont(m_stTxFont);
	m_edSamState.TextParamChanged();

	m_edProductBN.SetMyFont(m_stTxFont);
	m_edProductBN.TextParamChanged();

	m_edTestItem.SetMyFont(m_stTxFont);
	m_edTestItem.TextParamChanged();

	m_edComment.SetMyFont(m_stTxFont);
	m_edComment.TextParamChanged();

	m_dtCYCollectDate.SetMyFont(m_stTxFont);

	return 0;
}

int CCYSamInfoDlg::SetEditMode(EEXPEDITMODE eExpEditMode)
{
	if(m_eExpEditMode == eExpEditMode)
	{
		return 1;
	}

	CSamInfoDlg::SetEditMode(eExpEditMode);

	m_eExpEditMode = eExpEditMode;

	return 0;
}

int CCYSamInfoDlg::SetCYSamInfo(CExperimentData* pExpData,BOOL bRedraw /*= FALSE*/)
{
	if(NULL == pExpData)
	{
		return -1;
	}

	m_cbSamCategory.SetText(pExpData->GetCYSampleCategory(),bRedraw,TRUE);

	m_edSamCount.SetText(pExpData->GetCYSampleCount(),bRedraw,TRUE);
	m_edSamState.SetText(pExpData->GetCYSampleState(),bRedraw,TRUE);
	m_edProductBN.SetText(pExpData->GetCYProductBatchNumber(),bRedraw,TRUE);
	m_edTestItem.SetText(pExpData->GetCYTestItem(),bRedraw,TRUE);
	m_edComment.SetText(pExpData->GetCYComment(),bRedraw,TRUE);

	CString strDate = pExpData->GetCYCollectDate();
	if(strDate.IsEmpty())
	{
		m_oleCYDt = COleDateTime::GetCurrentTime();
	}
	else
	{
		m_oleCYDt.ParseDateTime(strDate);
	}
	UpdateData(FALSE);

	return 0;
}



