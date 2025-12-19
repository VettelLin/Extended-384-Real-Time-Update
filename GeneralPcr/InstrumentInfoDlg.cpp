// SkipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralPcr.h"
#include "GeneralPcrDlg.h"
#include "InstrumentInfoDlg.h"
#include "InstrumentPCR.h"

// CInstrumentInfoDlg dialog
IMPLEMENT_DYNAMIC(CInstrumentInfoDlg, CDialogEx)

CInstrumentInfoDlg::CInstrumentInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInstrumentInfoDlg::IDD, pParent)
{
	m_bCtrlCreated = FALSE; 
}

CInstrumentInfoDlg::~CInstrumentInfoDlg()
{

}

void CInstrumentInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInstrumentInfoDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_INSTRUMENTINFODLG_BT_FINDINSTRUMENT,OnBnClickedFindInstrument)
	ON_MESSAGE(WM_TELLPARENTSETNEWTEXT,OnMsgTellParentSetNewText)
END_MESSAGE_MAP()

BOOL CInstrumentInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	CRect rect;
	GetClientRect(&rect);

	pDC->FillSolidRect(0,0,rect.Width(),rect.Height(),CLR_SOFT_LIGHT);
	DrawRoundRectangleNew(pDC,rect.left,rect.top,rect.Width() - 1,rect.Height() - 1,8,8,8,8,TRUE,Color(0XD8,0XD8,0XD8),1,TRUE,Color(0XFF,0XFF,0XFF),1);

	pDC->SetBkMode(TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject(&m_fontSet);

	pDC->FillSolidRect(28 * m_dHoriRatio,33 * m_dVertRatio,12 * m_dHoriRatio,4 * m_dVertRatio,CLR_SOFT_THEME);
	pDC->FillSolidRect(32 * m_dHoriRatio,29 * m_dVertRatio,4 * m_dHoriRatio,12 * m_dVertRatio,CLR_SOFT_THEME);
	pDC->SetTextColor(CLR_SOFT_THEME);
	pDC->DrawText(pLangInfo->GetText(22),CRect(44 * m_dHoriRatio,24 * m_dVertRatio,316 * m_dHoriRatio,46 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	pDC->SetTextColor(RGB(0X39,0X45,0X65));

	int iEndX = 238 * m_dHoriRatio;
	int iTempInt1 = 290 * m_dHoriRatio;
	if(theApp.GetBeEnglish())
	{
		iEndX += 30 * m_dHoriRatio; 
		iTempInt1 += 30 * m_dHoriRatio; 
	}
	pDC->DrawText(pLangInfo->GetText(23),CRect(48 * m_dHoriRatio,138 * m_dVertRatio,iEndX,165 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	pDC->DrawText(pLangInfo->GetText(24),CRect(48 * m_dHoriRatio,218 * m_dVertRatio,iEndX,245 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	pDC->DrawText(pLangInfo->GetText(25),CRect(48 * m_dHoriRatio,298 * m_dVertRatio,iEndX,325 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	
	//ÏÔÊ¾ÈÈ¸ÇÎÂ¶È
	CGeneralPcrDlg* pMainDlg = (CGeneralPcrDlg*)theApp.m_pMainWnd;
	CMiddleDlg* pMiddleDlg = pMainDlg->m_workDlg.m_pCurSelPageDlg;
	if(NULL != pMiddleDlg->GetSafeHwnd() && NULL != pMiddleDlg->m_pExpData)
	{
		float fLidTopTemp = pMiddleDlg->m_pExpData->GetExperimentInfo()->GetLidTemp();
		CString strTemp(_T(""));
		strTemp.Format(_T("%.2lf¡æ"),fLidTopTemp);
		pDC->DrawText(strTemp,CRect(iTempInt1,221 * m_dVertRatio,440 * m_dHoriRatio,241 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}



	return TRUE;
}

void CInstrumentInfoDlg::OnSize(UINT nType,int cx,int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}

	return;
}

BOOL CInstrumentInfoDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

	SetMyFont(m_stTxFont,18 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);

	CreateCtrl();
	m_bCtrlCreated = TRUE;
	InitCtrl();
	SetCtrlPos();

	SetCtrlText();

	ShowCtrl();

	return TRUE;
}

void CInstrumentInfoDlg::CreateCtrl()
{
	m_cbInstrumentType.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,CRect(0,0,0,0),this,ID_INSTRUMENTINFODLG_CB_INSTRUMENTTYPE);
	m_btnFindInstrument.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_INSTRUMENTINFODLG_BT_FINDINSTRUMENT);

	m_ckbtLidSet.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_INSTRUMENTINFODLG_CKBT_LIDSET);
	m_ckbtTempCtrl.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_INSTRUMENTINFODLG_ST_TEMCTRLFORMAT);
}

void CInstrumentInfoDlg::InitCtrl()
{
	InitBt();
	InitCb();
}

void CInstrumentInfoDlg::InitBt()
{
	int iFontH = 18 * m_dTxRatio;

	m_btnFindInstrument.SetMyFont(m_stTxFont,iFontH);
	m_btnFindInstrument.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);

	m_ckbtLidSet.SetBtChecked(TRUE);
	m_ckbtLidSet.SetPngBkPic(_T(".\\BkPic\\check.png"),4,40 * m_dHoriRatio);
	m_ckbtLidSet.SetBeNeedRedrawBk(TRUE);
	m_ckbtLidSet.SetMyFont(m_stTxFont,iFontH);
	m_ckbtLidSet.SetMyFontChecked(m_stTxFont,iFontH);
	m_ckbtLidSet.EnableWindow(FALSE);

	m_ckbtTempCtrl.SetBtChecked(TRUE);
	m_ckbtTempCtrl.SetPngBkPic(_T(".\\BkPic\\check.png"),4,40 * m_dHoriRatio);
	m_ckbtTempCtrl.SetBeNeedRedrawBk(TRUE);
	m_ckbtTempCtrl.SetMyFont(m_stTxFont,iFontH);
	m_ckbtTempCtrl.SetMyFontChecked(m_stTxFont,iFontH);
}

void CInstrumentInfoDlg::InitCb()
{
	int iFontH = 18 * m_dTxRatio;

	m_cbInstrumentType.SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	m_cbInstrumentType.SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	m_cbInstrumentType.SetSelectedBkColor(RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB));
	m_cbInstrumentType.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X97,0X97,0X97));
	m_cbInstrumentType.SetMyFont(m_stTxFont,iFontH);
	m_cbInstrumentType.m_selWnd.SetMyFont(m_stTxFont,iFontH);
	m_cbInstrumentType.SetWorkState(EWS_READONLY);
	m_cbInstrumentType.SetTellParentSetNewText(TRUE, 1);
	m_cbInstrumentType.m_iRightIndent = 44 * m_dHoriRatio;
	m_cbInstrumentType.m_arrowInfo.m_clrArrow = Color(0X39,0X45,0X65);
	m_cbInstrumentType.m_arrowInfo.m_arrowW = 12 * m_dHoriRatio;
	m_cbInstrumentType.m_arrowInfo.m_arrowH = 8 * m_dVertRatio;


	InitInstrument();

	return;
}

void CInstrumentInfoDlg::SetCtrlPos()
{
	ResetCtrlPos();
}

void CInstrumentInfoDlg::ResetCtrlPos(BOOL bRedraw /*= FALSE*/)
{
	CRect rect;
	GetClientRect(&rect);

	int iStartX = 240 * m_dHoriRatio;
	if(theApp.GetBeEnglish())
	{
		iStartX += 30 * m_dHoriRatio;
	}

	CRect rectItem = CRect(iStartX, int(131 * m_dVertRatio), iStartX+int(450 * m_dHoriRatio), int(171*m_dVertRatio));
	m_cbInstrumentType.MoveWindow(rectItem, FALSE);
	CRect rectBtn = rectItem;
	int iSpace = int(10*m_dHoriRatio);
	rectBtn.left = rectItem.right + iSpace;
	rectBtn.right = rect.right - iSpace;
	m_btnFindInstrument.MoveWindow(rectBtn, FALSE);

	m_ckbtLidSet.MoveWindow(iStartX, int(221 * m_dVertRatio),int(40 * m_dHoriRatio),int(20 * m_dVertRatio),FALSE);
	m_ckbtTempCtrl.MoveWindow(iStartX, int(301 * m_dVertRatio),int(40 * m_dHoriRatio),int(20 * m_dVertRatio),FALSE);

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}

	return;
}

void CInstrumentInfoDlg::ShowCtrl()
{
	m_cbInstrumentType.ShowWindow(SW_SHOW);
	m_btnFindInstrument.ShowWindow(SW_SHOW);
	m_ckbtLidSet.ShowWindow(SW_SHOW);
	m_ckbtTempCtrl.ShowWindow(SW_SHOW);

	return;
}

void CInstrumentInfoDlg::SetCtrlText()
{
	m_btnFindInstrument.SetWindowText(theApp.m_pLangInfo->GetText(474));
}

void CInstrumentInfoDlg::GetInstruInfo(CExperimentInfo& info)
{
	//info.SetInstrument(m_cbInstrumentType.GetText());

	info.SetLidHeader(m_ckbtLidSet.GetCheckState());

	if (m_ckbtTempCtrl.GetCheckState())
	{
		info.SetTempCtrlMode(TEMP_CTRL_TUBE);
	}
	else
	{
		info.SetTempCtrlMode(TEMP_CTRL_NONE);
	}

	return;
}

void CInstrumentInfoDlg::SetInstruInfo(CExperimentInfo& info)
{
	m_ckbtLidSet.SetBtChecked(info.GetLidHeader(),TRUE);
	m_ckbtTempCtrl.SetBtChecked(info.GetTempCtrlMode() != TEMP_CTRL_NONE,TRUE);
}

void CInstrumentInfoDlg::SetCtrlEnableState(BOOL bEnabled)
{
	m_cbInstrumentType.SetBeEditable(bEnabled);
	m_ckbtLidSet.EnableWindow(bEnabled);
	m_ckbtTempCtrl.EnableWindow(bEnabled);
}

void CInstrumentInfoDlg::ClearPage()
{
	m_ckbtLidSet.SetBtChecked(TRUE,TRUE);
	m_ckbtTempCtrl.SetBtChecked(TRUE,TRUE);
}

int CInstrumentInfoDlg::RefreshLanguageDisplay()
{
	SetCtrlText();

	SetMyFont(m_stTxFont);

	int iFontH = 18 * m_dTxRatio;

	m_cbInstrumentType.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_cbInstrumentType.m_selWnd.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_cbInstrumentType.TextParamChanged();

	ResetCtrlPos();

	return 0;
}

int CInstrumentInfoDlg::SetEditMode(EEXPEDITMODE eExpEditMode)
{
	if(m_eExpEditMode == eExpEditMode)
	{
		return 1;
	}

	m_eExpEditMode = eExpEditMode;

	if(EEEM_IVD == m_eExpEditMode)
	{
		m_cbInstrumentType.SetBeEditable(FALSE);
		m_btnFindInstrument.EnableWindow(FALSE);
		m_ckbtTempCtrl.EnableWindow(FALSE);
	}
	else if(EEEM_NOLIMIT == m_eExpEditMode)
	{
		m_cbInstrumentType.SetBeEditable(TRUE);
		m_btnFindInstrument.EnableWindow(TRUE);
		m_ckbtTempCtrl.EnableWindow(TRUE);
	}
	else if(EEEM_RUNNING == m_eExpEditMode)
	{
		m_cbInstrumentType.SetBeEditable(FALSE,TRUE);
		m_btnFindInstrument.EnableWindow(FALSE);
		m_ckbtTempCtrl.EnableWindow(FALSE);
	}
	else if(EEEM_END == m_eExpEditMode)
	{
		m_cbInstrumentType.SetBeEditable(TRUE,TRUE);
		m_btnFindInstrument.EnableWindow(TRUE);
		m_ckbtTempCtrl.EnableWindow(TRUE);
	}

	return 0;
}

void CInstrumentInfoDlg::InitInstrument()
{
	m_cbInstrumentType.Clear(FALSE);

	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	for(int i = 0;i < pInstrument->GetInstanceCount();++i)
	{
		CInstancePCR* pInstance = pInstrument->GetInstancePCR(i);
		if (pInstance->IsDemo())
			continue;

		CString strFlag = CombineModelAndSN(pInstance->GetModel(), pInstance->GetSN());
		m_cbInstrumentType.InsertString(i,strFlag);
	}
}

CString CInstrumentInfoDlg::CombineModelAndSN(CString strModel,CString strSN)
{
	CString strFlag = strModel;
	if (!strSN.IsEmpty())
	{
		strFlag.Format(_T("%s(%s)"), strModel, strSN);
	}
	
	return strFlag;
}

void CInstrumentInfoDlg::SetSelectedInstrument(CString strModel, CString strSN)
{
	CString strFlag = CombineModelAndSN(strModel, strSN);

	int iCount = m_cbInstrumentType.GetItemCount();
	int iCurSel = -1;
	CString strItem;
	for (int i=0; i<iCount; i++)
	{
		m_cbInstrumentType.GetString(i, strItem);
		if (strFlag == strItem)
		{
			iCurSel = i;
			break;
		}
	}

	int iOldSel = m_cbInstrumentType.GetCurSel();
	if (iOldSel != iCurSel)
	{
		m_cbInstrumentType.SetCurSel(iCurSel);
		//CanRunExperiment(iCurSel);
	}
}

void CInstrumentInfoDlg::OnBnClickedFindInstrument()
{
	CString strOldSel;
	int iOldSel = m_cbInstrumentType.GetCurSel();
	if (iOldSel != -1)
	{
		m_cbInstrumentType.GetString(iOldSel, strOldSel);
	}

	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	if (pInstrument->Reinitialize(FALSE))
	{
		InitInstrument();

		int iCount = m_cbInstrumentType.GetItemCount();
		int iCurSel = -1;
		CString strItem;
		for (int i=0; i<iCount; i++)
		{
			m_cbInstrumentType.GetString(i, strItem);
			if (strOldSel == strItem)
			{
				iCurSel = i;
				break;
			}
		}
		m_cbInstrumentType.SetCurSel(iCurSel);

		GetParent()->GetParent()->GetParent()->SendMessage(WM_INSTRUMENT_REINITIALIZE);
	}
}


LRESULT CInstrumentInfoDlg::OnMsgTellParentSetNewText(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 1);
	{
		int iCurSel = m_cbInstrumentType.GetCurSel();
		CanRunExperiment(iCurSel);
		GetParent()->GetParent()->SendMessage(WM_INSTRUMENT_SELECTED, iCurSel);
		Invalidate();
	}

	return 0;
}

void CInstrumentInfoDlg::ReshowInstrumentList()
{
	CString strOldSel;
	int iOldSel = m_cbInstrumentType.GetCurSel();
	if (iOldSel != -1)
	{
		m_cbInstrumentType.GetString(iOldSel, strOldSel);
	}

	InitInstrument();

	int iCount = m_cbInstrumentType.GetItemCount();
	int iCurSel = -1;
	CString strItem;
	for (int i=0; i<iCount; i++)
	{
		m_cbInstrumentType.GetString(i, strItem);
		if (strOldSel == strItem)
		{
			iCurSel = i;
			break;
		}
	}
	m_cbInstrumentType.SetCurSel(iCurSel);
}

void CInstrumentInfoDlg::CanRunExperiment(int iSelDevice)
{
	if (iSelDevice < 0)
		return;

	CInstancePCR* pInstance = CInstrumentPCR::GetInstance()->GetInstancePCR(iSelDevice);
	if (pInstance == NULL)
		return;

	if (pInstance->GetInstanceStatus() == INSTANCE_TEMP_HIGH)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(479));
	}
	else if (pInstance->GetInstanceStatus() == INSTANCE_TEMP_LOW)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(480));
	}
	else if (pInstance->GetInstanceStatus() == INSTANCE_FLUO_HIGH)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(482));
	}
	else if (pInstance->GetInstanceStatus() == INSTANCE_FLUO_LOW)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(483));
	}

}
