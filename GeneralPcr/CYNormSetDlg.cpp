#include "StdAfx.h"
#include "CYNormSetDlg.h"
#include "GeneralPcr.h"

IMPLEMENT_DYNAMIC(CCYNormSetDlg,CPs96SetDlg)
	
CCYNormSetDlg::CCYNormSetDlg(void)
{

}

CCYNormSetDlg::~CCYNormSetDlg(void)
{

}

BEGIN_MESSAGE_MAP(CCYNormSetDlg,CPs96SetDlg)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_TELLPARENTTEXTCHANGED,OnMsgTellParentTextChanged) 
END_MESSAGE_MAP()

BOOL CCYNormSetDlg::OnEraseBkgnd(CDC* pDC)
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

	int iStartX = 10 * m_dHoriRatio,iEndX = 0,iY = 36 * m_dVertRatio,iH = 40 * m_dVertRatio,iVertItl = 60 * m_dVertRatio,iVertItl2 = 72 * m_dVertRatio,iBtW = 100 * m_dHoriRatio;
	if(theApp.GetBeEnglish())
	{
		iEndX = 268 * m_dHoriRatio;
	}
	else
	{
		iEndX = 136 * m_dHoriRatio;
	}

	CRect rc(iStartX,iY,iEndX,iY + iH);
	memDc.DrawText(theApp.m_pLangInfo->GetText(85),rc,DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	CRect rc2(iEndX + iBtW + 15 * m_dHoriRatio,iY,iEndX + iBtW + 35 * m_dHoriRatio,iY + iH);
	memDc.DrawText(_T("¡æ"),rc2,DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	iY += iVertItl;
	CRect rc3(iStartX,iY,iEndX,iY + iH);
	memDc.DrawText(theApp.m_pLangInfo->GetText(391),rc3,DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	CRect rc4(iEndX + iBtW + 15 * m_dHoriRatio,iY,iEndX + iBtW + 35 * m_dHoriRatio,iY + iH);
	memDc.DrawText(_T("¡æ"),rc4,DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	iY += iVertItl2;
	iH = 22 * m_dVertRatio;
	rc.SetRect(10 * m_dHoriRatio,iY,210 * m_dHoriRatio,iY + iH);
	memDc.DrawText(theApp.m_pLangInfo->GetText(199),rc,DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	//ÓïÑÔ±êÊ¶
	CRect rc5(1000 * m_dHoriRatio,36 * m_dVertRatio,1190 * m_dHoriRatio,76 * m_dVertRatio);
	memDc.DrawText(pLangInfo->GetText(388),rc5,DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	iY += iVertItl2;
	iH = 40 * m_dVertRatio;
	CRect rc6(iStartX,iY,iEndX,iY + iH);
	memDc.DrawText(pLangInfo->GetText(408),rc6,DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	iY += iVertItl;
	CRect rc7(iStartX,iY,iEndX,iY + iH);
	memDc.DrawText(pLangInfo->GetText(409),rc7,DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

	clock_t ckStart = clock();	
	Node<CRect>* pNodeRst = m_lnkRcRst.GetHead();
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

LRESULT CCYNormSetDlg::OnMsgTellParentTextChanged(WPARAM wParam,LPARAM lParam)
{
	CMyNewEdit* pEdit = NULL;
	EITEMINFOTYPE eItemInfoType = (EITEMINFOTYPE)wParam;
	EITEMINFODATATYPE eItemInfoDataType = EIIDT_NONE;

	switch(eItemInfoType)
	{
	case ITEM_CYNORMSET_ED_CLIENT: pEdit = &m_edClient;	eItemInfoDataType = EIIDT_STRING;	break;
	case ITEM_CYNORMSET_ED_CPADR:	pEdit = &m_edCpAdr;	eItemInfoDataType = EIIDT_STRING;	break;

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

	switch(eItemInfoType)
	{
	case ITEM_CYNORMSET_ED_CLIENT: theApp.m_cyInfo.m_strClient = strTemp;	break;
	case ITEM_CYNORMSET_ED_CPADR: theApp.m_cyInfo.m_strCpAdr = strTemp;	break;

	default: break;
	}

	return 2;
}

void CCYNormSetDlg::CreateEd()
{
	CPs96SetDlg::CreateEd();

	CRect rcTemp(0,0,0,0);

	m_edClient.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rcTemp,this,ID_CYNORMSETDLG_ED_CLIENT);
	m_edCpAdr.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rcTemp,this,ID_CYNORMSETDLG_ED_CPADR);
}

void CCYNormSetDlg::InitEd()
{
	CPs96SetDlg::InitEd();

	InitOneEd(&m_edClient);
	m_edClient.SetTellParentTxChangedParam(TRUE,ITEM_CYNORMSET_ED_CLIENT);
	m_edClient.SetText(theApp.m_cyInfo.m_strClient,FALSE,TRUE);

	InitOneEd(&m_edCpAdr);
	m_edCpAdr.SetTellParentTxChangedParam(TRUE,ITEM_CYNORMSET_ED_CPADR);
	m_edCpAdr.SetText(theApp.m_cyInfo.m_strCpAdr,FALSE,TRUE);

	return;
}

void CCYNormSetDlg::SetCtrlPos()
{
	ResetCtrlPos();

	return;
}

void CCYNormSetDlg::ResetCtrlPos()
{
	CRect rect;
	GetClientRect(rect);

	m_rcInit = rect;
	ClearRectToDelete();

	int iX = 0,iX2 = 0,iY = 36 * m_dVertRatio,iBtW = 100 * m_dHoriRatio,iBtH = 40 * m_dVertRatio,iVertItl = 60 * m_dVertRatio,iTempInt1 = 16 * m_dHoriRatio,iTempInt2 = 0,iTempInt3 = 0;
	int iTpInt4 = 0,iVertItl2 = 72 * m_dVertRatio,iEdStartX = 0;

	if(theApp.GetBeEnglish())
	{
		iX = 270 * m_dHoriRatio;
		iTempInt2 = iX;
	}
	else
	{
		iX = 140 * m_dHoriRatio;
		iTempInt2 = iX;
	}
	iTempInt3 = iX + 114 * m_dHoriRatio;
	iEdStartX = iX;

	m_edLidTem.MoveWindow(iX,iY,iBtW,iBtH,FALSE);
	AddRectToDelete(iX,iY,iBtW,iBtH);

	iTpInt4 = iX + iBtW + 25 * m_dHoriRatio + iTempInt1;
	m_btSaveLidTem.MoveWindow(iTpInt4,iY,iBtW,iBtH,FALSE);
	AddRectToDelete(iTpInt4,iY,iBtW,iBtH);

	iX2 = iTpInt4 + iBtW + 32 * m_dHoriRatio;
	m_btReadLidTem.MoveWindow(iX2,iY,iBtW,iBtH,FALSE);
	AddRectToDelete(iX2,iY,iBtW,iBtH);

	iY += iVertItl;
	m_edLidTemInit.MoveWindow(iX,iY,iBtW,iBtH,FALSE);
	AddRectToDelete(iX,iY,iBtW,iBtH);

	m_btSaveLidPreheatTem.MoveWindow(iTpInt4,iY,iBtW,iBtH,FALSE);
	AddRectToDelete(iTpInt4,iY,iBtW,iBtH);

	m_btReadLidPreheatTem.MoveWindow(iX2,iY,iBtW,iBtH,FALSE);
	AddRectToDelete(iX2,iY,iBtW,iBtH);

	iX = iTempInt2;
	iY += iVertItl2;
	iBtW = 110 *m_dHoriRatio;
	iBtH = 22 * m_dVertRatio;
	m_ckbtExportSamHori.MoveWindow(iX,iY,iBtW,iBtH,FALSE);
	AddRectToDelete(iX,iY,iBtW,iBtH);

	iX = iTempInt3;
	m_ckbtExportSamVert.MoveWindow(iX,iY,iBtW,iBtH,FALSE);
	AddRectToDelete(iX,iY,iBtW,iBtH);

	int iW = 500 * m_dHoriRatio,iH = 40 * m_dVertRatio;

	iY += iVertItl2;
	m_edClient.MoveWindow(iEdStartX,iY,iW,iH,FALSE);
	AddRectToDelete(iEdStartX,iY,iW,iH);

	iY += iVertItl;
	m_edCpAdr.MoveWindow(iEdStartX,iY,iW,iH,FALSE);
	AddRectToDelete(iEdStartX,iY,iW,iH);

	iX = 1200 * m_dHoriRatio;
	iY = 36 * m_dVertRatio;
	iBtW = 200 * m_dHoriRatio;
	iBtH = 40 * m_dVertRatio;
	m_cbLangSel.MoveWindow(iX,iY,iBtW,iBtH,FALSE);
	AddRectToDelete(iX,iY,iBtW,iBtH);

	iY += iVertItl2;
	m_btAnaSet.MoveWindow(iX,iY,iBtW,iBtH,FALSE);
	AddRectToDelete(iX,iY,iBtW,iBtH);

	CalcRectResult();

	return;
}

void CCYNormSetDlg::ShowCtrl()
{
	CPs96SetDlg::ShowCtrl();

	m_edClient.ShowWindow(SW_SHOW);
	m_edCpAdr.ShowWindow(SW_SHOW);

	return;
}

void CCYNormSetDlg::SetCtrlText()
{
	CPs96SetDlg::SetCtrlText();
	
	return;
}

int CCYNormSetDlg::RefreshLanguageDisplay()
{
	SetMyFont(m_stTxFont);

	m_edLidTem.SetMyFont(m_stTxFont);
	m_edLidTem.TextParamChanged();

	m_edLidTemInit.SetMyFont(m_stTxFont);
	m_edLidTemInit.TextParamChanged();

	m_cbLangSel.SetMyFont(m_stTxFont);
	m_cbLangSel.m_selWnd.SetMyFont(m_stTxFont);

	int iCurSel = m_cbLangSel.GetCurSel();
	m_cbLangSel.SetItemText(0,theApp.m_pLangInfo->GetText(389));
	m_cbLangSel.SetItemText(1,theApp.m_pLangInfo->GetText(390));
	m_cbLangSel.TextParamChanged();
	m_cbLangSel.RefreshText(iCurSel);

	int iFontH = 18 * m_dTxRatio;
	m_ckbtExportSamHori.SetMyFont(m_stTxFont);
	m_ckbtExportSamHori.SetMyFontChecked(m_stTxFont,iFontH);
	m_ckbtExportSamHori.SetWindowText(theApp.m_pLangInfo->GetText(197));

	m_ckbtExportSamVert.SetMyFont(m_stTxFont);
	m_ckbtExportSamVert.SetMyFontChecked(m_stTxFont,iFontH);
	m_ckbtExportSamVert.SetWindowText(theApp.m_pLangInfo->GetText(198));

	m_btSaveLidTem.SetMyFont(m_stTxFont);
	m_btSaveLidTem.SetWindowText(theApp.m_pLangInfo->GetText(148));

	m_btSaveLidPreheatTem.SetMyFont(m_stTxFont);
	m_btSaveLidPreheatTem.SetWindowText(theApp.m_pLangInfo->GetText(148));

	m_btReadLidTem.SetMyFont(m_stTxFont);
	m_btReadLidTem.SetWindowText(theApp.m_pLangInfo->GetText(148));

	m_btReadLidPreheatTem.SetMyFont(m_stTxFont);
	m_btReadLidPreheatTem.SetWindowText(theApp.m_pLangInfo->GetText(148));

	m_btAnaSet.SetMyFont(m_stTxFont);
	m_btAnaSet.SetWindowText(theApp.m_pLangInfo->GetText(432));

	m_edClient.SetMyFont(m_stTxFont);
	m_edClient.TextParamChanged();

	m_edCpAdr.SetMyFont(m_stTxFont);
	m_edCpAdr.TextParamChanged();

	ResetCtrlPos();

	Invalidate();

	return 0;
}

