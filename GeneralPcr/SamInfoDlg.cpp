// SkipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralPcr.h"
#include "GeneralPcrDlg.h"
#include "SamInfoDlg.h"
#include "FontSet.h"

// CSamInfoDlg dialog
IMPLEMENT_DYNAMIC(CSamInfoDlg, CDialogEx)

CSamInfoDlg::CSamInfoDlg(CWnd* pParent /*=NULL*/,CInterfaceC* pIfC /*= NULL*/)
	: CDialogEx(CSamInfoDlg::IDD, pParent)
{
	m_bEditable = TRUE;
	m_pIfC = pIfC;
	m_bClear = FALSE;
	m_bCtrlCreated = FALSE;
}

CSamInfoDlg::~CSamInfoDlg()
{

}

void CSamInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX,ID_SAMINFODLG_DT_COLLECT_DATE,m_collectDate);
}

BEGIN_MESSAGE_MAP(CSamInfoDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_MESSAGE(WM_DATECTRLCHANGED,OnMsgDateCtrlChanged) 
	ON_MESSAGE(WM_TELLPARENTTEXTCHANGED,OnMsgTellParentTextChanged) 
	ON_MESSAGE(WM_TELLPARENTSETNEWTEXT,OnMsgTellParentSetNewText)
END_MESSAGE_MAP()

BOOL CSamInfoDlg::OnEraseBkgnd(CDC* pDC)
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

	memDc.DrawText(pLangInfo->GetText(47),CRect(iTempInt1,iStartY,iTempInt2,iEndY),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;
	memDc.DrawText(pLangInfo->GetText(48),CRect(iTempInt1,iStartY,iTempInt2,iEndY),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;
	memDc.DrawText(pLangInfo->GetText(64),CRect(iTempInt1,iStartY,iTempInt2,iEndY),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;

	memDc.DrawText(pLangInfo->GetText(66),CRect(iTempInt1,iStartY,iTempInt2,iEndY),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;

	memDc.DrawText(pLangInfo->GetText(67),CRect(iTempInt1,iStartY,iTempInt2,iEndY),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;
	memDc.DrawText(pLangInfo->GetText(68),CRect(iTempInt1,iStartY,iTempInt2,iEndY),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;
	memDc.DrawText(pLangInfo->GetText(69),CRect(iTempInt1,iStartY,iTempInt2,iEndY),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;
	memDc.DrawText(pLangInfo->GetText(70),CRect(iTempInt1,iStartY,iTempInt2,iEndY),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;
	memDc.DrawText(pLangInfo->GetText(71),CRect(iTempInt1,iStartY,iTempInt2,iEndY),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;

	memDc.DrawText(pLangInfo->GetText(72),CRect(iTempInt1,iStartY,iTempInt2,iEndY),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;
	memDc.DrawText(pLangInfo->GetText(73),CRect(iTempInt1,iStartY,iTempInt2,iEndY),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;
	memDc.DrawText(pLangInfo->GetText(74),CRect(iTempInt1,iStartY,iTempInt2,iEndY),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;
	memDc.DrawText(pLangInfo->GetText(75),CRect(iTempInt1,iStartY,iTempInt2,iEndY),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	iStartY += iTempInt4;
	iEndY = iStartY + iTempInt5;
	memDc.DrawText(pLangInfo->GetText(20),CRect(iTempInt1,iStartY,iTempInt2,iEndY),DT_LEFT | DT_VCENTER | DT_SINGLELINE);

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

void CSamInfoDlg::OnSize(UINT nType,int cx,int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}

	return;
}

LRESULT CSamInfoDlg::OnMsgDateCtrlChanged(WPARAM wParam, LPARAM lParam)
{
	if(m_bClear)
	{
		return 1;
	}

	CMyDateTimeCtrl* pDt = NULL;
	COleDateTime* pOleDt = NULL;
	EITEMINFOTYPE eItemInfoType = (EITEMINFOTYPE)wParam;

	switch(eItemInfoType)
	{
	case ITEMSAMCOLLECTDATE: pDt = &m_dtCollectDate; pOleDt = &m_collectDate;break;
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
		return 4;
	}
	
	return 5;
}

LRESULT CSamInfoDlg::OnMsgTellParentTextChanged(WPARAM wParam, LPARAM lParam)
{
	CMyNewEdit* pEdit = NULL;
	EITEMINFOTYPE eItemInfoType = (EITEMINFOTYPE)wParam;
	EITEMINFODATATYPE eItemInfoDataType = EIIDT_NONE;

	switch(eItemInfoType)
	{
	case ITEMSAMTYPE: pEdit = &m_cbSamType;	eItemInfoDataType = EIIDT_STRING;	break;
	case ITEMSAMNAME:	pEdit = &m_edSamName;	eItemInfoDataType = EIIDT_STRING;	break;
	case ITEMSAMID:	pEdit = &m_edNewId;	eItemInfoDataType = EIIDT_STRING;	break;
	case ITEMPATIENTNAME:	pEdit = &m_edPaitentName;	eItemInfoDataType = EIIDT_STRING;	break;
	case ITEMAGE:	pEdit = &m_edAge;	eItemInfoDataType = EIIDT_INT;	break;
	case ITEMSENDINGDOCTOR:	pEdit = &m_edSendingDoctor;	eItemInfoDataType = EIIDT_STRING;	break;
	case ITEMSENDINGSECTION:	pEdit = &m_edSendingSection;	eItemInfoDataType = EIIDT_STRING;	break;
	case ITEMANAMNESISNO:	pEdit = &m_edAnamnesisNo;	eItemInfoDataType = EIIDT_STRING;	break;
	case ITEMBEDNO:	pEdit = &m_edBedNo;	eItemInfoDataType = EIIDT_STRING;	break;
	case ITEMHOSPITALID:	pEdit = &m_edHospitalId;	eItemInfoDataType = EIIDT_STRING;	break;
	case ITEMOUTPATIENTNO:	pEdit = &m_edOutpatientNo;	eItemInfoDataType = EIIDT_STRING;	break;
	case ITEMREMARKS:	pEdit = &m_edRemarks;	eItemInfoDataType = EIIDT_STRING;	break;

	default:	return 0;
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
		return 2;
	}

	return 3;
}

LRESULT CSamInfoDlg::OnMsgTellParentSetNewText(WPARAM wParam, LPARAM lParam)
{
	CMyNewCb* pCb = NULL;
	EITEMINFOTYPE eItemInfoType = (EITEMINFOTYPE)wParam;
	EITEMINFODATATYPE eItemInfoDataType = EIIDT_NONE;

	switch(eItemInfoType)
	{
	case ITEMSAMTYPE:	pCb = &m_cbSamType;	eItemInfoDataType = EIIDT_STRING;	break;
	case ITEMSEXUALITY:	pCb = &m_cbSexuality;	eItemInfoDataType = EIIDT_SEXUALITY;	break;
	
	default:	return 0;
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
	case EIIDT_SEXUALITY: iData = pCb->GetCurSel() + 1;
	default:	break;
	}

	if(NULL != m_pIfC)
	{
		m_pIfC->SetPlateSelectedSiteSamInfo(eItemInfoType,strTemp,iData,dData);
		return 2;
	}

	return 3;
}

BOOL CSamInfoDlg::OnInitDialog()
{ 
	CreateCtrl();
	CDialogEx::OnInitDialog();
	m_bCtrlCreated = TRUE;
	InitDlg();
	InitCtrl();
	SetCtrlPos();
	ShowCtrl();

	SetCtrlText();

	return TRUE;
}

void CSamInfoDlg::InitDlg()
{
	SetMyFont(m_stTxFont,16 * m_dTxRatio);
	SetTextClr1(RGB(0X39,0X45,0X65));
}

void CSamInfoDlg::CreateCtrl()
{
	CreateBt();
	CreateCkbt();
	CreateSt();
	CreateCb();
	CreateEd();
	CreateDt();
}

void CSamInfoDlg::CreateBt()
{

}

void CSamInfoDlg::CreateCkbt()
{
	
}

void CSamInfoDlg::CreateSt()
{
	
}

void CSamInfoDlg::CreateCb()
{
	CRect rcCreate(0,0,0,0);

	m_cbSamType.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rcCreate,this,ID_SAMINFODLG_CB_SAMTYPE);
	m_cbSexuality.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rcCreate,this,ID_SAMINFODLG_CB_SEXUALITY);
}

void CSamInfoDlg::CreateEd()
{
	m_edSamName.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_SAMINFODLG_ED_SAMID);
	m_edPaitentName.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_SAMINFODLG_ED_PATIENTNAME);
	m_edAge.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_SAMINFODLG_ED_AGE);
	
	m_edSendingDoctor.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_SAMINFODLG_ED_SENDINGDOCTOR);
	m_edSendingSection.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_SAMINFODLG_ED_SENDINGSECTION);
	m_edAnamnesisNo.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_SAMINFODLG_ED_ANAMNESISNO);
	m_edBedNo.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_SAMINFODLG_ED_BEDNO);
	m_edHospitalId.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_SAMINFODLG_ED_HOSPITALID);
	m_edOutpatientNo.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_SAMINFODLG_ED_OUTPATIENTNO);
	m_edRemarks.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_SAMINFODLG_ED_REMARKS);
	m_edNewId.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_SAMINFODLG_ED_NEWID);
}

void CSamInfoDlg::CreateDt()
{
	m_dtCollectDate.Create(WS_CHILD | WS_TABSTOP | DTS_SHOWNONE | DTS_SHORTDATEFORMAT,CRect(0,0,0,0),this,ID_SAMINFODLG_DT_COLLECT_DATE);
}

void CSamInfoDlg::InitCtrl()
{
	InitBt();
	InitCkbt();
	InitSt();
	InitEd();
	InitCb();
	InitDt();
}

void CSamInfoDlg::InitBt()
{
	
}

void CSamInfoDlg::InitCkbt()
{
	
}

void CSamInfoDlg::InitSt()
{
	
}

void CSamInfoDlg::InitCb()
{
	int iFontH = 16 * m_dTxRatio;

	InitOneCb(&m_cbSamType);
	m_cbSamType.SetWorkState(EWS_READONLY);
	m_cbSamType.SetTellParentSetNewText(TRUE,ITEMSAMTYPE);

#ifdef N16USENEWSAMPLETYPE
	m_cbSamType.InsertString(0,theApp.m_pLangInfo->GetText(211));
	m_cbSamType.InsertString(1,theApp.m_pLangInfo->GetText(212));
	m_cbSamType.InsertString(2,theApp.m_pLangInfo->GetText(215));
	m_cbSamType.InsertString(3,theApp.m_pLangInfo->GetText(214));
	m_cbSamType.InsertString(4,theApp.m_pLangInfo->GetText(213));
	m_cbSamType.InsertString(5,theApp.m_pLangInfo->GetText(292));
#else
	m_cbSamType.InsertString(0,theApp.m_pLangInfo->GetText(211));
	m_cbSamType.InsertString(1,theApp.m_pLangInfo->GetText(212));
	m_cbSamType.InsertString(2,theApp.m_pLangInfo->GetText(213));
	m_cbSamType.InsertString(3,theApp.m_pLangInfo->GetText(214));
	m_cbSamType.InsertString(4,theApp.m_pLangInfo->GetText(215));
	m_cbSamType.InsertString(5,theApp.m_pLangInfo->GetText(216));
	m_cbSamType.InsertString(6,theApp.m_pLangInfo->GetText(217));
	m_cbSamType.InsertString(7,theApp.m_pLangInfo->GetText(218));
#endif

	InitOneCb(&m_cbSexuality);
	m_cbSexuality.SetWorkState(EWS_READONLY);
	m_cbSexuality.SetTellParentSetNewText(TRUE,ITEMSEXUALITY);

	m_cbSexuality.InsertString(0,theApp.m_pLangInfo->GetText(182));
	m_cbSexuality.InsertString(1,theApp.m_pLangInfo->GetText(183));

	return;
}

int CSamInfoDlg::InitOneCb(CMyNewCb* pCb)
{
	if(NULL == pCb)
	{
		return -1;
	}

	int iFtH = 16 * m_dTxRatio;

	pCb->SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	pCb->SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	pCb->SetSelectedBkColor(RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB));
	pCb->SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XC6,0XC6,0XC6));
	pCb->SetMyFont(m_stTxFont,iFtH);
	pCb->m_selWnd.SetMyFont(m_stTxFont,iFtH);
	pCb->m_iRightIndent = 44 * m_dHoriRatio;
	pCb->m_arrowInfo.m_clrArrow = Color(0X39,0X45,0X65);
	pCb->m_arrowInfo.m_arrowW = 12 * m_dHoriRatio;
	pCb->m_arrowInfo.m_arrowH = 8 * m_dVertRatio;

	return 0;
}

void CSamInfoDlg::InitEd()
{
	InitOneEd(&m_edSamName);
	m_edSamName.SetTellParentTxChangedParam(TRUE,ITEMSAMNAME);

	InitOneEd(&m_edPaitentName);
	m_edPaitentName.SetTellParentTxChangedParam(TRUE,ITEMPATIENTNAME);

	InitOneEd(&m_edAge);
	m_edAge.SetTellParentTxChangedParam(TRUE,ITEMAGE);

	InitOneEd(&m_edSendingDoctor);
	m_edSendingDoctor.SetTellParentTxChangedParam(TRUE,ITEMSENDINGDOCTOR);

	InitOneEd(&m_edSendingSection);
	m_edSendingSection.SetTellParentTxChangedParam(TRUE,ITEMSENDINGSECTION);

	InitOneEd(&m_edAnamnesisNo);
	m_edAnamnesisNo.SetTellParentTxChangedParam(TRUE,ITEMANAMNESISNO);

	InitOneEd(&m_edBedNo);
	m_edBedNo.SetTellParentTxChangedParam(TRUE,ITEMBEDNO);

	InitOneEd(&m_edHospitalId);
	m_edHospitalId.SetTellParentTxChangedParam(TRUE,ITEMHOSPITALID);

	InitOneEd(&m_edOutpatientNo);
	m_edOutpatientNo.SetTellParentTxChangedParam(TRUE,ITEMOUTPATIENTNO);

	InitOneEd(&m_edRemarks);
	m_edRemarks.SetTellParentTxChangedParam(TRUE,ITEMREMARKS);

	InitOneEd(&m_edNewId);
	m_edNewId.SetTellParentTxChangedParam(TRUE,ITEMSAMID);

	return;
}

int CSamInfoDlg::InitOneEd(CMyNewEdit* pEd)
{
	if(NULL == pEd)
	{
		return -1;
	}

	int iFtH = 16 * m_dTxRatio;

	pEd->SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	pEd->SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	pEd->SetSelectedBkColor(RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB));
	pEd->SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XC6,0XC6,0XC6));
	pEd->SetMyFont(m_stTxFont,iFtH);
	pEd->SetTextClr1(RGB(86,86,86));

	return 0;
}

void CSamInfoDlg::InitDt()
{
	m_collectDate = COleDateTime::GetCurrentTime();
	UpdateData(FALSE);

	m_dtCollectDate.SetMyFont(m_stTxFont,16 * m_dTxRatio);
	m_dtCollectDate.SetTellParentDtChangedParam(TRUE,ITEMSAMCOLLECTDATE); 

	return;
}

void CSamInfoDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	m_rcInit = rect;
	ClearRectToDelete();

	int iTempInt1 = 124 * m_dHoriRatio,iTempInt2 = 220 * m_dHoriRatio,iTempInt3 = 40 * m_dVertRatio;

	int iStartY = 20 * m_dVertRatio,iTempInt4 = 56 * m_dVertRatio;

	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);
	m_cbSamType.SetWindowPos(this,iTempInt1,iStartY,iTempInt2,iTempInt3,SWP_NOZORDER);
	iStartY += iTempInt4;
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);
	m_edSamName.SetWindowPos(this,iTempInt1,iStartY,iTempInt2,iTempInt3,SWP_NOZORDER);
	iStartY += iTempInt4;
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);
	m_edNewId.SetWindowPos(this,iTempInt1,iStartY,iTempInt2,iTempInt3,SWP_NOZORDER);
	iStartY += iTempInt4;

	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);
	m_edPaitentName.SetWindowPos(this,iTempInt1,iStartY,iTempInt2,iTempInt3,SWP_NOZORDER);
	iStartY += iTempInt4;
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);
	m_cbSexuality.SetWindowPos(this,iTempInt1,iStartY,iTempInt2,iTempInt3,SWP_NOZORDER);
	iStartY += iTempInt4;
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);
	m_edAge.SetWindowPos(this,iTempInt1,iStartY,iTempInt2,iTempInt3,SWP_NOZORDER);

	iStartY += iTempInt4;
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);
	m_dtCollectDate.SetWindowPos(this,iTempInt1,iStartY,iTempInt2,iTempInt3,SWP_NOZORDER);
	iStartY += iTempInt4;
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);
	m_edSendingDoctor.SetWindowPos(this,iTempInt1,iStartY,iTempInt2,iTempInt3,SWP_NOZORDER);
	iStartY += iTempInt4;
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);
	m_edSendingSection.SetWindowPos(this,iTempInt1,iStartY,iTempInt2,iTempInt3,SWP_NOZORDER);

	iStartY += iTempInt4;
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);
	m_edAnamnesisNo.SetWindowPos(this,iTempInt1,iStartY,iTempInt2,iTempInt3,SWP_NOZORDER);
	iStartY += iTempInt4;
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);
	m_edBedNo.SetWindowPos(this,iTempInt1,iStartY,iTempInt2,iTempInt3,SWP_NOZORDER);
	iStartY += iTempInt4;
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);
	m_edHospitalId.SetWindowPos(this,iTempInt1,iStartY,iTempInt2,iTempInt3,SWP_NOZORDER);

	iStartY += iTempInt4;
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);
	m_edOutpatientNo.SetWindowPos(this,iTempInt1,iStartY,iTempInt2,iTempInt3,SWP_NOZORDER);
	iStartY += iTempInt4;
	AddRectToDelete(iTempInt1,iStartY,iTempInt2,iTempInt3);
	m_edRemarks.SetWindowPos(this,iTempInt1,iStartY,iTempInt2,iTempInt3,SWP_NOZORDER);

	CalcRectResult();

	return;
}

void CSamInfoDlg::ShowCtrl()
{
	m_cbSamType.ShowWindow(SW_SHOW);
	m_cbSexuality.ShowWindow(SW_SHOW);
	m_edNewId.ShowWindow(SW_SHOW);

	m_edSamName.ShowWindow(SW_SHOW);
	m_edPaitentName.ShowWindow(SW_SHOW);
	m_edAge.ShowWindow(SW_SHOW);
	m_dtCollectDate.ShowWindow(SW_SHOW);

	m_edSendingDoctor.ShowWindow(SW_SHOW);
	m_edSendingSection.ShowWindow(SW_SHOW);
	m_edAnamnesisNo.ShowWindow(SW_SHOW);
	m_edBedNo.ShowWindow(SW_SHOW);
	m_edHospitalId.ShowWindow(SW_SHOW);
	m_edOutpatientNo.ShowWindow(SW_SHOW);
	m_edRemarks.ShowWindow(SW_SHOW);
}

void CSamInfoDlg::SetCtrlText()
{
	return;
}

void CSamInfoDlg::ClearSamInfo()
{
	m_bClear = TRUE;

	m_cbSamType.SetCurSel(-1,TRUE,TRUE);
	m_cbSexuality.SetCurSel(-1,TRUE,TRUE);
	m_edNewId.SetText(_T(""),TRUE,TRUE);
	m_edSamName.SetText(_T(""),TRUE,TRUE);
	m_edPaitentName.SetText(_T(""),TRUE,TRUE);
	m_edAge.SetText(_T(""),TRUE,TRUE);
	m_collectDate = COleDateTime::GetCurrentTime();
	UpdateData(FALSE);

	m_edSendingDoctor.SetText(_T(""),TRUE,TRUE);
	m_edSendingSection.SetText(_T(""),TRUE,TRUE);
	m_edAnamnesisNo.SetText(_T(""),TRUE,TRUE);
	m_edBedNo.SetText(_T(""),TRUE,TRUE);
	m_edHospitalId.SetText(_T(""),TRUE,TRUE);
	m_edOutpatientNo.SetText(_T(""),TRUE,TRUE);
	m_edRemarks.SetText(_T(""),TRUE,TRUE);

	m_bClear = FALSE;
}

void CSamInfoDlg::SetSamInfo(CTubeInfo& tubeInfo)
{
	m_bClear = TRUE;

	CString strTemp(_T(""));

	m_cbSamType.SetCurSel(theApp.GetSampleTypeCbIndex(tubeInfo.GetSampleType()),TRUE,TRUE);

	//如果不是标准品，禁止输入浓度

	if(SEXUALITY_NONE == tubeInfo.m_nPatientSex)
	{
		m_cbSexuality.SetCurSel(-1,TRUE,TRUE);
	}
	else
	{
		m_cbSexuality.SetCurSel(tubeInfo.m_nPatientSex - 1,TRUE,TRUE);
	}
	
	m_edNewId.SetText(tubeInfo.GetSampleID());
	
	m_edSamName.SetText(tubeInfo.GetSampleName());

	m_edPaitentName.SetText(tubeInfo.m_strPatientName);

	m_edAge.SetText(tubeInfo.GetAgeString());

	if(tubeInfo.m_strCollectDate.IsEmpty())
	{
		m_collectDate = COleDateTime::GetCurrentTime();
		tubeInfo.m_strCollectDate = m_collectDate.Format(_T("%Y-%m-%d %H:%M:%S"));
	}
	else
	{
		m_collectDate.ParseDateTime(tubeInfo.m_strCollectDate);
	}
	UpdateData(FALSE);

	m_edSendingDoctor.SetText(tubeInfo.m_strProposer);
	m_edSendingSection.SetText(tubeInfo.m_strDepartment);
	m_edAnamnesisNo.SetText(tubeInfo.m_strCaseID);
	m_edBedNo.SetText(tubeInfo.m_strBedNo);
	m_edHospitalId.SetText(tubeInfo.m_strHospital);
	m_edOutpatientNo.SetText(tubeInfo.m_strClinicID);
	m_edRemarks.SetText(tubeInfo.GetComments());

	m_bClear = FALSE;

	return;
}

void CSamInfoDlg::SetEditable(BOOL bEditable)
{
	m_cbSamType.SetBeEditable(bEditable,TRUE);
	m_cbSexuality.SetBeEditable(bEditable,TRUE);
	m_edNewId.SetBeEditable(bEditable,TRUE);

	m_edSamName.SetBeEditable(bEditable,TRUE);
	m_edPaitentName.SetBeEditable(bEditable,TRUE);
	m_edAge.SetBeEditable(bEditable,TRUE);
	
	m_edSendingDoctor.SetBeEditable(bEditable,TRUE);
	m_edSendingSection.SetBeEditable(bEditable,TRUE);	
	m_edAnamnesisNo.SetBeEditable(bEditable,TRUE);	
	m_edBedNo.SetBeEditable(bEditable,TRUE);	
	m_edHospitalId.SetBeEditable(bEditable,TRUE);	
	m_edOutpatientNo.SetBeEditable(bEditable,TRUE);	
	m_edRemarks.SetBeEditable(bEditable,TRUE);	

	m_dtCollectDate.EnableWindow(bEditable);
	m_bEditable = bEditable;
	return;
}

BOOL CSamInfoDlg::GetEditable()
{
	return m_bEditable;
}

int CSamInfoDlg::RefreshLanguageDisplay()
{
	SetMyFont(m_stTxFont);

	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	m_cbSamType.SetMyFont(m_stTxFont);
	m_cbSamType.m_selWnd.SetMyFont(m_stTxFont);
	m_cbSamType.TextParamChanged();

	int iCurSel = m_cbSamType.GetCurSel();
#ifdef N16USENEWSAMPLETYPE
	m_cbSamType.SetItemText(0,theApp.m_pLangInfo->GetText(211));
	m_cbSamType.SetItemText(1,theApp.m_pLangInfo->GetText(212));
	m_cbSamType.SetItemText(2,theApp.m_pLangInfo->GetText(215));
	m_cbSamType.SetItemText(3,theApp.m_pLangInfo->GetText(214));
	m_cbSamType.SetItemText(4,theApp.m_pLangInfo->GetText(213));
	m_cbSamType.SetItemText(5,theApp.m_pLangInfo->GetText(292));
#else
	m_cbSamType.SetItemText(0,theApp.m_pLangInfo->GetText(211));
	m_cbSamType.SetItemText(1,theApp.m_pLangInfo->GetText(212));
	m_cbSamType.SetItemText(2,theApp.m_pLangInfo->GetText(213));
	m_cbSamType.SetItemText(3,theApp.m_pLangInfo->GetText(214));
	m_cbSamType.SetItemText(4,theApp.m_pLangInfo->GetText(215));
	m_cbSamType.SetItemText(5,theApp.m_pLangInfo->GetText(216));
	m_cbSamType.SetItemText(6,theApp.m_pLangInfo->GetText(217));
	m_cbSamType.SetItemText(7,theApp.m_pLangInfo->GetText(218));
#endif
	m_cbSamType.RefreshText(iCurSel);

	m_cbSexuality.SetMyFont(m_stTxFont);
	m_cbSexuality.m_selWnd.SetMyFont(m_stTxFont);
	m_cbSexuality.TextParamChanged();

	iCurSel = m_cbSexuality.GetCurSel();
	m_cbSexuality.SetItemText(0,pLangInfo->GetText(182));
	m_cbSexuality.SetItemText(1,pLangInfo->GetText(183));
	m_cbSexuality.RefreshText(iCurSel);

	m_edNewId.SetMyFont(m_stTxFont);
	m_edNewId.TextParamChanged();

	m_edSamName.SetMyFont(m_stTxFont);
	m_edSamName.TextParamChanged();

	m_edPaitentName.SetMyFont(m_stTxFont);
	m_edPaitentName.TextParamChanged();

	m_edAge.SetMyFont(m_stTxFont);
	m_edAge.TextParamChanged();

	m_edSendingDoctor.SetMyFont(m_stTxFont);
	m_edSendingDoctor.TextParamChanged();

	m_edSendingSection.SetMyFont(m_stTxFont);
	m_edSendingSection.TextParamChanged();

	m_edAnamnesisNo.SetMyFont(m_stTxFont);
	m_edAnamnesisNo.TextParamChanged();

	m_edBedNo.SetMyFont(m_stTxFont);
	m_edBedNo.TextParamChanged();

	m_edHospitalId.SetMyFont(m_stTxFont);
	m_edHospitalId.TextParamChanged();

	m_edOutpatientNo.SetMyFont(m_stTxFont);
	m_edOutpatientNo.TextParamChanged();

	m_edRemarks.SetMyFont(m_stTxFont);
	m_edRemarks.TextParamChanged();

	m_dtCollectDate.SetMyFont(m_stTxFont);

	return 0;
}

int CSamInfoDlg::SetEditMode(EEXPEDITMODE eExpEditMode)
{
	if(m_eExpEditMode == eExpEditMode)
	{
		return 1;
	}

	m_eExpEditMode = eExpEditMode;

	return 0;
}

int CSamInfoDlg::SetCYSamInfo(CExperimentData* pExpData)
{
	return 0;
}
