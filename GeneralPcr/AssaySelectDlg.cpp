// SkipDlg.cpp : implementation file
//

#include "stdafx.h"

#include "AssaySelectDlg.h"
#include "Ps96IvdDlg.h"
#include "GeneralPcr.h"
#include "Ps96SimpleDlg.h"
#include "PublicInfo.h"
#include "Ps96CutOffDlg.h"

// CAssaySelDlg dialog
IMPLEMENT_DYNAMIC(CAssaySelDlg, CDialogEx)

CAssaySelDlg::CAssaySelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAssaySelDlg::IDD, pParent)
{
	m_pParent = NULL;
	if(NULL != pParent)
	{
		m_pParent = (CPs96SimpleDlg*)pParent;
	}
	m_bCtrlCreated = FALSE;
	m_pExpData = NULL;
}

CAssaySelDlg::~CAssaySelDlg()
{

}

void CAssaySelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAssaySelDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_ASSAYSELECTDLG_BT_LOADPROC,OnBnClickedLoadProc)
	ON_BN_CLICKED(ID_ASSAYSELECTDLG_BT_ADDTEMPLATE,OnBnClickedAddTemplate)
	ON_BN_CLICKED(ID_ASSAYSELECTDLG_BT_DELETETEMPLATE,OnBnClickedDeleteTemplate)
	ON_BN_CLICKED(ID_ASSAYSELECTDLG_BT_EXPORTIDTEMPLATE,OnBnClickedExportIdTemplate)
	ON_BN_CLICKED(ID_ASSAYSELECTDLG_BT_EXPORTRESULT,OnBnClickedExportResult)
	ON_MESSAGE(WM_ROWSELECTCHANGE,OnMsgRowSelectedChange)
	ON_MESSAGE(WM_TELLPARENTSETNEWTEXT,OnMsgTellParentSetNewText)
	ON_MESSAGE(WM_TELLPARENTTEXTCHANGED,OnMsgTellParentTextChanged)
	ON_BN_CLICKED(ID_ASSAYSELECTDLG_BT_UP_THRESHOLD,OnBnClickedEditUpThreshold)
END_MESSAGE_MAP()

BOOL CAssaySelDlg::OnEraseBkgnd(CDC* pDC)
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

	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);
	DWORD dwTxStyle = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	int iX1 = 1076 * m_dHoriRatio;
	int iY1 = int(16 * m_dVertRatio) + int(80 * m_dVertRatio) *3 + int(20 * m_dVertRatio);
	memDc.DrawText(theApp.m_pLangInfo->GetText(500), CRect(iX1, iY1, iX1+int(360*m_dHoriRatio), iY1+(60*m_dVertRatio)), dwTxStyle);
	
	iY1 = int(16 * m_dVertRatio) + int(80 * m_dVertRatio) *4 + int(20 * m_dVertRatio);
	memDc.DrawText(theApp.m_pLangInfo->GetText(20), CRect(iX1, iY1, iX1+int(360*m_dHoriRatio), iY1+(60*m_dVertRatio)), dwTxStyle);

	//iY1 = int(16 * m_dVertRatio) + int(80 * m_dVertRatio) *5 + int(20 * m_dVertRatio);
	//memDc.DrawText(theApp.m_pLangInfo->GetText(485), CRect(iX1, iY1, iX1+int(360*m_dHoriRatio), iY1+(60*m_dVertRatio)), dwTxStyle);

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

void CAssaySelDlg::OnSize(UINT nType, int cx, int cy)
{
	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}

	__super::OnSize(nType, cx, cy);
}	

void CAssaySelDlg::OnBnClickedLoadProc()
{
	CPs96IvdDlg* pMainDlg = (CPs96IvdDlg*)theApp.m_pMainWnd;
	pMainDlg->m_workDlg.OpenFile(1);

	return;
}

void CAssaySelDlg::OnBnClickedAddTemplate()
{
	CPs96IvdDlg* pMainDlg = (CPs96IvdDlg*)theApp.m_pMainWnd;

	pMainDlg->m_workDlg.OpenFile(2);

	return;
}

void CAssaySelDlg::OnBnClickedDeleteTemplate()
{
	vector<int> vecSelRow = m_lstAssaySel.GetSelectedRowId();
	if(vecSelRow.size() < 1)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(380));
		return;
	}

	if(IDYES != PopQueryTipInfo(theApp.m_pLangInfo->GetText(381)))
	{
		return;
	}

	m_pParent->DeleteTemplate(vecSelRow[0]);
	
	return;
}

void CAssaySelDlg::OnBnClickedExportIdTemplate()
{
	if(NULL == m_pExpData)
	{
		return;
	}

	m_pExpData->ExportSampleSetInfo(ESEM_VERT == theApp.m_eSamExportMode,1);

	return;
}

void CAssaySelDlg::OnBnClickedExportResult()
{
	m_pParent->ExportResult();
}

LRESULT CAssaySelDlg::OnMsgRowSelectedChange(WPARAM wParam, LPARAM lParam)
{
	if(0 != HandleTemplateFileLstRowSelectChanged())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

BOOL CAssaySelDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

	CreateCtrl();
	InitDlg();
	InitCtrl();
	m_bCtrlCreated = TRUE;
	SetCtrlPos();

	SetCtrlText();

	ShowCtrl();

	return TRUE;
}

void CAssaySelDlg::CreateCtrl()
{
	CreateBt();
	CreateEd();
	CreateCb();
	CreateLst();
}

void CAssaySelDlg::CreateBt()
{
	m_btLoadProc.Create(_T("加载实验"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_ASSAYSELECTDLG_BT_LOADPROC);
	m_btAddTem.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_ASSAYSELECTDLG_BT_ADDTEMPLATE);
	m_btDelTem.Create(_T("删除流程"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_ASSAYSELECTDLG_BT_DELETETEMPLATE);
	m_btExportIdTemplate.Create(_T("导出样本ID模板"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_ASSAYSELECTDLG_BT_EXPORTIDTEMPLATE);
	m_btExportResult.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_ASSAYSELECTDLG_BT_EXPORTRESULT);

	m_btnEditThreshold.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_ASSAYSELECTDLG_BT_UP_THRESHOLD);
}

void CAssaySelDlg::CreateEd()
{
	m_edRemark.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0,0,0,0), this,ID_EXPEINFODLG_ST_REMARK);
}

void CAssaySelDlg::CreateCb()
{
	CRect rcCreate(0,0,0,0);
	m_cbReportTemplate.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, rcCreate,this, ID_ASSAYSELECTDLG_CB_REPORTTEMPLATE);
}


void CAssaySelDlg::CreateLst()
{
	m_lstAssaySel.Create(IDD_MIDDLEDLG,this);
}

void CAssaySelDlg::InitDlg()
{
	SetTextClr1(RGB(0X39,0X45,0X65));
	SetMyFont(m_stTxFont,26 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
}

void CAssaySelDlg::InitCtrl()
{
	InitBt();
	InitEd();
	InitCb();
	InitLst();
}

void CAssaySelDlg::InitBt()
{
	InitOneBt(&m_btLoadProc);
	InitOneBt(&m_btAddTem);
	InitOneBt(&m_btDelTem);
	InitOneBt(&m_btExportIdTemplate);
	InitOneBt(&m_btExportResult);
	InitOneBt(&m_btnEditThreshold);

	return;
}

void CAssaySelDlg::InitEd()
{
	int iFontH = 26 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 27 * m_dTxRatio;
	}

	m_edRemark.SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	m_edRemark.SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	m_edRemark.SetSelectedBkColor(RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB));
	m_edRemark.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X97,0X97,0X97));
	m_edRemark.SetMyFont(m_stTxFont,iFontH);
	m_edRemark.SetTellParentTxChangedParam(TRUE,0);	
}


void CAssaySelDlg::InitCb()
{
	int iFontH = 26 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 27 * m_dTxRatio;
	}

	m_cbReportTemplate.SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	m_cbReportTemplate.SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	m_cbReportTemplate.SetSelectedBkColor(CLR_SOFT_THEME,CLR_SOFT_THEME,CLR_SOFT_THEME,CLR_SOFT_THEME);
	m_cbReportTemplate.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XC6,0XC6,0XC6));
	m_cbReportTemplate.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_cbReportTemplate.m_selWnd.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_cbReportTemplate.SetWorkState(EWS_READONLY);
	m_cbReportTemplate.m_iRightIndent = 44;
	m_cbReportTemplate.m_arrowInfo.m_clrArrow = Color(0X39,0X45,0X65);
	m_cbReportTemplate.m_arrowInfo.m_arrowW = 12;
	m_cbReportTemplate.m_arrowInfo.m_arrowH = 8;
	m_cbReportTemplate.SetTellParentSetNewText(TRUE, 1);

	ReshowReportTemplate();
}

void CAssaySelDlg::ReshowReportTemplate()
{
	m_cbReportTemplate.Clear();

	CReportTemplateData* pReportData = CPublicInfo::GetInstance()->GetReportTemplateData();
	pReportData->SetChanged(FALSE);

	for (int i=0; i<pReportData->GetTemplateCount(); i++)
	{
		m_cbReportTemplate.InsertString(i, pReportData->GetStringBy(CReportTemplateData::DATA_NAME, i));
	}

	if (pReportData->GetCurSel() != -1)
		m_cbReportTemplate.SetCurSel(pReportData->GetCurSel(), TRUE, TRUE);

}


void CAssaySelDlg::InitLst()
{
	int iFontH = 26 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 27 * m_dTxRatio;
	}

	m_lstAssaySel.InsertColumn(0,theApp.m_pLangInfo->GetText(28),FALSE);
	m_lstAssaySel.InsertColumn(1,theApp.m_pLangInfo->GetText(29),FALSE);

	m_lstAssaySel.InsertColumn(2,theApp.m_pLangInfo->GetText(296),FALSE);

	m_lstAssaySel.SetHeaderHeight(70 * m_dVertRatio);
	int iTempInt1 = 34 * m_dVertRatio;
	m_lstAssaySel.m_horiScroll.m_arrowWH = iTempInt1;
	m_lstAssaySel.m_horiScroll.m_scrollH = iTempInt1;
	m_lstAssaySel.m_vertScroll.m_arrowWH = iTempInt1;
	m_lstAssaySel.m_vertScroll.m_scrollW = iTempInt1;

	m_lstAssaySel.SetMyTitleFont(m_stTxFont,iFontH,FW_MEDIUM,TYPE_PIXEL);

	m_lstAssaySel.SetLargeCheckIconUseState(TRUE);
	m_lstAssaySel.SetBeSingleSelect(TRUE);
	m_lstAssaySel.SetBeAllowCancelRowSelect(TRUE);
	m_lstAssaySel.SetBeTellParentRowSelChange(TRUE);
	m_lstAssaySel.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_lstAssaySel.SetTextClr1(RGB(0X39,0X45,0X65));
	m_lstAssaySel.SetMyHeaderFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	//m_lstAssaySel.SetBeSingleSelect(TRUE);
	m_lstAssaySel.SetColumnCheckFunc(0,TRUE);
	m_lstAssaySel.SetFourAngleBkColor(RGB(0XFF,0XFF,0XFF));
	m_lstAssaySel.SetItemHeight(70 * m_dVertRatio);
	m_lstAssaySel.SetRowBkSelectedColor(RGB(0XF1,0XF1,0XF1));

	int tempInt1 = 0,tempInt2 = 960 * m_dHoriRatio;
	tempInt1 = 140 * m_dHoriRatio;
	m_lstAssaySel.SetColumnWidth(0,tempInt1);
	tempInt2 -= tempInt1;
	tempInt1 = 140 * m_dHoriRatio;
	m_lstAssaySel.SetColumnWidth(1,tempInt1);
	tempInt2 -= tempInt1;
	tempInt2 -= m_lstAssaySel.GetFrameLineWidth() *2;
	m_lstAssaySel.SetColumnWidth(2,tempInt2);

	SetLstAssays();

	return;
}

void CAssaySelDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(rect);

	m_rcManage.m_rcInit = rect;
	m_rcManage.ClearRectToDelete();

	m_lstAssaySel.SetWindowPos(this,16 * m_dHoriRatio,16 * m_dVertRatio, 960 * m_dHoriRatio, rect.Height() - 32 * m_dVertRatio,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(16 * m_dHoriRatio,16 * m_dVertRatio,960 * m_dHoriRatio,rect.Height() - 32 * m_dVertRatio);

	int iTempInt1 = int(1076 * m_dHoriRatio),iStartY = int(16 * m_dVertRatio),iBtW = int(240 * m_dHoriRatio),iBtH = int(60 * m_dVertRatio),iVertInterval = int(80 * m_dVertRatio);

	if(theApp.GetBeEnglish())
	{
		iBtW = 320 * m_dHoriRatio;
	}
	int iTempInt2 = iTempInt1 + iBtW + int(40 * m_dHoriRatio);

	m_btLoadProc.SetWindowPos(this,iTempInt1,iStartY,iBtW,iBtH,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iTempInt1,iStartY,iBtW,iBtH);

	m_btExportResult.SetWindowPos(this,iTempInt2,iStartY,iBtW,iBtH,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iTempInt2,iStartY,iBtW,iBtH);

	iStartY += iVertInterval;
	m_btAddTem.SetWindowPos(this,iTempInt1,iStartY,iBtW,iBtH,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iTempInt1,iStartY,iBtW,iBtH);

	m_btDelTem.SetWindowPos(this,iTempInt2,iStartY,iBtW,iBtH,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iTempInt2,iStartY,iBtW,iBtH);

	iStartY += iVertInterval;
	m_btExportIdTemplate.SetWindowPos(this,iTempInt1,iStartY,iBtW,iBtH,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iTempInt1,iStartY,iBtW,iBtH);

	m_btnEditThreshold.SetWindowPos(this,iTempInt2,iStartY,iBtW,iBtH,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iTempInt2,iStartY,iBtW,iBtH);

	iStartY += iVertInterval + int(20 * m_dHoriRatio);
	m_cbReportTemplate.SetWindowPos(this, iTempInt2-int(150*m_dHoriRatio), iStartY, iBtW+int(150*m_dHoriRatio), iBtH, SWP_NOZORDER);

	iStartY += iVertInterval;
	m_edRemark.SetWindowPos(this, iTempInt2-int(150*m_dHoriRatio), iStartY, iBtW+int(150*m_dHoriRatio), iBtH,SWP_NOZORDER);

	m_rcManage.CalcRectResult();

	return;
}

void CAssaySelDlg::ShowCtrl()
{
	m_lstAssaySel.ShowWindow(SW_SHOW);
	m_btLoadProc.ShowWindow(SW_SHOW);
	m_btAddTem.ShowWindow(SW_SHOW);
	m_btDelTem.ShowWindow(SW_SHOW);
	m_btExportIdTemplate.ShowWindow(SW_SHOW);
	m_btExportResult.ShowWindow(SW_SHOW);

	m_cbReportTemplate.ShowWindow(SW_SHOW);
	m_edRemark.ShowWindow(SW_SHOW);

	m_btnEditThreshold.ShowWindow(SW_SHOW);

	return;
}

void CAssaySelDlg::SetCtrlText()
{
	m_btLoadProc.SetWindowText(theApp.m_pLangInfo->GetText(383));
	m_btAddTem.SetWindowText(theApp.m_pLangInfo->GetText(395));
	m_btDelTem.SetWindowText(theApp.m_pLangInfo->GetText(384));
	m_btExportIdTemplate.SetWindowText(theApp.m_pLangInfo->GetText(385));
	m_btExportResult.SetWindowText(theApp.m_pLangInfo->GetText(392));
	m_btnEditThreshold.SetWindowText(theApp.m_pLangInfo->GetText(485));

	return;
}

int CAssaySelDlg::SetEditMode(EEXPEDITMODE eExpEditMode)
{
	if(m_eExpEditMode == eExpEditMode)
	{
		return 1;
	}

	m_eExpEditMode = eExpEditMode;

	if(EEEM_RUNNING == m_eExpEditMode)
	{
		m_lstAssaySel.SetBeEditable(FALSE,TRUE);
		m_btLoadProc.EnableWindow(FALSE);
		m_btAddTem.EnableWindow(FALSE);
		m_btDelTem.EnableWindow(FALSE);
		m_btExportIdTemplate.EnableWindow(FALSE);
		m_btExportResult.EnableWindow(FALSE);
		m_edRemark.EnableWindow(FALSE);

		m_btnEditThreshold.EnableWindow(FALSE);
	}
	else if(EEEM_END == m_eExpEditMode)
	{
		m_lstAssaySel.SetBeEditable(TRUE,TRUE);
		m_btLoadProc.EnableWindow(TRUE);
		m_btAddTem.EnableWindow(TRUE);
		m_btDelTem.EnableWindow(TRUE);
		m_btExportIdTemplate.EnableWindow(TRUE);
		m_btExportResult.EnableWindow(TRUE);
		m_edRemark.EnableWindow(TRUE);

		m_btnEditThreshold.EnableWindow(TRUE);
	}

	return 0;
}

int CAssaySelDlg::SetLstAssays()
{
	m_lstAssaySel.Clear();
	CString strTemp(_T(""));

	int iTemNum = theApp.m_lnkTotalTemFileInfo.GetSize();
	for(int i = 0;i < iTemNum;++i)
	{
		m_lstAssaySel.InsertRowSetHeight(i,70 * m_dVertRatio);
		strTemp.Format(_T("%d"),i + 1);
		m_lstAssaySel.SetItemText(i,1,strTemp);
		m_lstAssaySel.SetItemText(i,2,theApp.m_lnkTotalTemFileInfo[i]->m_strTitle);
	}

	m_lstAssaySel.InsertRowSetHeight(iTemNum,70 * m_dVertRatio);
	strTemp.Format(_T("%d"),iTemNum + 1);
	m_lstAssaySel.SetItemText(iTemNum,1,strTemp);
	m_lstAssaySel.SetItemText(iTemNum,2,_T("NONE"));

	return 0;
}

int CAssaySelDlg::HandleTemplateFileLstRowSelectChanged()
{
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -1;
	}

	if(NULL == m_pExpData)
	{
		return -2;
	}

	int iRowNum = m_lstAssaySel.GetRowNum();
	vector<int> vecSelectedRowId;
	vecSelectedRowId = m_lstAssaySel.GetSelectedRowId();
	if(vecSelectedRowId.size() < 1)
	{
		return -3;
	}

	if(vecSelectedRowId[0] == iRowNum - 1)
	{
		//清空
		m_pParent->ClearExpEmbody();
		return 0;
	}

	if(vecSelectedRowId[0] >= theApp.m_lnkTotalTemFileInfo.GetSize())
	{
		return -4;
	}

	CTemplateFileInfo& temFileInfo = *theApp.m_lnkTotalTemFileInfo[vecSelectedRowId[0]];
	if(!temFileInfo.IsFileExist())
	{
		return -5;
	}

	CExperimentData expData;
	if(0 != expData.OpenFile(temFileInfo.m_strFilePath,TRUE))
	{
		return -6;
	}

	vector<CTubeInfo*> vecPTubeInfo;
	expData.GetAllTubeInfoPtr(vecPTubeInfo);
	m_pExpData->SetTubeInfo(vecPTubeInfo);
	m_pExpData->SetSegment(expData.GetSegment());
	m_pExpData->SetBeTemplateFile(TRUE);
	m_pExpData->GetExperimentInfo()->CopyExperiment(*expData.GetExperimentInfo());
	m_pExpData->GetAnalysisPara()->CopyFrom(expData.GetAnalysisPara());
	m_pExpData->SetTemplateNane(temFileInfo.m_strTitle);

	m_pExpData->SetTubeChannelSelectState(5,FALSE);
	m_pExpData->PasteReagentItemInfo(expData.GetReagentItem());
	if (m_pExpData->GetAnalysisPara()->GetAmplifyParaCount() == 0)
	{
		m_pExpData->InitAmplifyParamByTubeTarget();
	}
	//显示模板文件中的信息
	m_pParent->RefreshExpEmbody();


	//提示是否加载样本ID信息
	if(IDYES == PopQueryTipInfo(theApp.m_pLangInfo->GetText(298)))
	{
		m_pParent->ImportId();
	}

	ReorderTemplateList(vecSelectedRowId[0]);
	m_pParent->RefreshExpEmbody();

	m_pParent->SetPageSelected(EPS96SSPT_LOADSAM);

	return 0;
}

int CAssaySelDlg::RefreshExpEmbody()
{
	CString strProjectName = m_pExpData->GetTempateName();
	int iRowCount = m_lstAssaySel.GetRowNum();
	int iRowTarget = -1;
	for (int i=0; i<iRowCount; i++)
	{
		CString strTemp = m_lstAssaySel.GetItemText(i, 2);
		if (strTemp == strProjectName)
		{
			iRowTarget = i;
			break;
		}
	}

	if (iRowTarget != -1)
	{
		m_lstAssaySel.SetRowSelectState(iRowTarget, TRUE, TRUE, FALSE);
		m_lstAssaySel.SetCellCheckState(iRowTarget, 0, TRUE, TRUE);
	}

	return 0;
}

void CAssaySelDlg::ClearExpEmbody()
{
	return;
}

void CAssaySelDlg::SetExpDataPtr(CExperimentData* pExpData)
{
	m_pExpData = pExpData;
	CString strText = pExpData->GetExperimentInfo()->GetComments();
	m_edRemark.SetText(pExpData->GetExperimentInfo()->GetComments());
	if (m_pExpData->HasRunData())
	{
		m_lstAssaySel.SetBeEditable(FALSE);
		//m_lstAssaySel.SetBeShowRowSelectState(FALSE);
		//m_lstAssaySel.SetBeSingleSelect(FALSE);
	}
}

void CAssaySelDlg::SetTemplateListSelectedRow(int iIndex)
{
	vector<int> vecSelId;
	vecSelId.push_back(iIndex);
	m_lstAssaySel.SetSelectedRowId(vecSelId,FALSE);
	m_lstAssaySel.SetCellCheckState(iIndex,0,TRUE,TRUE);
	return;
}

int CAssaySelDlg::NewTemplateAdded(int iIndex,BOOL bRedraw /*= TRUE*/)
{
	CString strTemp(_T(""));
	m_lstAssaySel.InsertRowSetHeight(iIndex,108 * m_dVertRatio);
	if(NULL != theApp.m_lnkTotalTemFileInfo[iIndex])
	{
		strTemp = theApp.m_lnkTotalTemFileInfo[iIndex]->m_strTitle;
	}
	m_lstAssaySel.SetItemText(iIndex,2,strTemp);

	int iNewRowNum = m_lstAssaySel.GetRowNum();
	for(int i = iIndex;i < iNewRowNum;++i)
	{
		strTemp.Format(_T("%d"),i + 1);
		m_lstAssaySel.SetItemText(i,1,strTemp);
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

void CAssaySelDlg::DeleteTemplateListRow(int iIndex)
{
	m_lstAssaySel.DeleteRow(iIndex,FALSE);
	m_lstAssaySel.RefreshSerialNumber(1,iIndex,TRUE);

	return;
}	
	
void CAssaySelDlg::InitOneBt(CMyButton* pBt)
{
	if(NULL == pBt)
	{
		return;
	}

	int iFontH = 26 * m_dTxRatio;

	pBt->SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	pBt->SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);
	pBt->SetBkColor(Color(0XF1,0XF4,0XF9),Color(0XFF,0XFF,0XFF),Color(0XE7,0XE7,0XE7),Color(0XF1,0XF4,0XF9));
	pBt->SetEdgeColor(Color(0XD8,0XD8,0XD8),Color(0XD8,0XD8,0XD8),Color(0XD8,0XD8,0XD8),Color(0XD8,0XD8,0XD8));
	pBt->SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X97,0X97,0X97));

	return;
}	

LRESULT CAssaySelDlg::OnMsgTellParentSetNewText(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 1)
	{
		CString strText = m_cbReportTemplate.GetText();
		CPublicInfo::GetInstance()->GetReportTemplateData()->SetCurSel(strText);
		CPublicInfo::GetInstance()->GetReportTemplateData()->SaveFile();
	}

	return 0;
}

LRESULT CAssaySelDlg::OnMsgTellParentTextChanged(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0)
	{
		CString strTemp = m_edRemark.GetText();
		m_pExpData->GetExperimentInfo()->SetComments(strTemp);
	}

	return 2;
}

void CAssaySelDlg::ReorderTemplateList(int iIndex)
{
	if (iIndex == 0)
		return;

	CTemplateFileInfo temFileInfo;
	temFileInfo.m_strFileName = theApp.m_lnkTotalTemFileInfo[iIndex]->m_strFileName;
	temFileInfo.m_strFilePath = theApp.m_lnkTotalTemFileInfo[iIndex]->m_strFilePath;
	temFileInfo.m_strTitle = theApp.m_lnkTotalTemFileInfo[iIndex]->m_strTitle;
	
	theApp.m_lnkTotalTemFileInfo.DeleteAt(iIndex);
	theApp.m_lnkTotalTemFileInfo.AddHead(temFileInfo);
	SetLstAssays();
	theApp.SaveTempalteOrder();
}


void CAssaySelDlg::OnBnClickedEditUpThreshold()
{
	CPs96CutOffDlg dlg;
	dlg.SetAnalysisParam(m_pExpData->GetAnalysisPara());
	if (dlg.DoModal() == IDOK)
	{
		vector<int> vecSelectedRowId;
		vecSelectedRowId = m_lstAssaySel.GetSelectedRowId();
		if(vecSelectedRowId.size() > 0 && vecSelectedRowId[0] < theApp.m_lnkTotalTemFileInfo.GetSize())
		{
			CTemplateFileInfo& temFileInfo = *theApp.m_lnkTotalTemFileInfo[vecSelectedRowId[0]];
			CExperimentData expData;
			if(expData.OpenFile(temFileInfo.m_strFilePath,TRUE) == 0)
			{
				expData.GetAnalysisPara()->CopyFrom(m_pExpData->GetAnalysisPara());
				expData.SaveFile(temFileInfo.m_strFilePath, TRUE);
			}

		}

		if (dlg.GetBeChanged() && m_pExpData->HasRunData())
		{
			m_pExpData->GenerateCt(TRUE, METHOD_CT_SPLINE, FLU_DELTARN);
		}
	}
}


