// SkipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralPcr.h"
#include "GeneralPcrDlg.h"
#include "PrjBaseInfoDlg.h"

// CPrjBaseInfoDlg dialog
IMPLEMENT_DYNAMIC(CPrjBaseInfoDlg, CDialogEx)

CPrjBaseInfoDlg::CPrjBaseInfoDlg(BOOL bPrjManage /*= FALSE*/,CWnd* pParent /*=NULL*/,CInterfaceB* pInterfaceB /*= NULL*/)
	: CDialogEx(CPrjBaseInfoDlg::IDD, pParent)
{
	m_bPrjMange = bPrjManage;
	m_bCtrlCreated = FALSE;
	m_pInterfaceB = pInterfaceB;
}

CPrjBaseInfoDlg::~CPrjBaseInfoDlg()
{

}

void CPrjBaseInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPrjBaseInfoDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_MESSAGE(WM_TELLPARENTTEXTCHANGED,OnMsgTellParentTextChanged)
	ON_MESSAGE(WM_TELLPARENTSETNEWTEXT,OnMsgTellParentSetNewText)
END_MESSAGE_MAP()

BOOL CPrjBaseInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	CString str1(_T("项目名称")),str2(_T("项目类型")),str3(_T("反应体系")),str4(_T("单位"));
	str1 = pLangInfo->GetText(30);
	str2 = pLangInfo->GetText(31);
	str3 = pLangInfo->GetText(32);
	str4 = pLangInfo->GetText(34);

	CRect rect; 
	GetClientRect(&rect);
	
	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	memDc.FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(255,255,255));

	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);
	memDc.SetBkMode(TRANSPARENT);

	memDc.FillSolidRect(0,0,rect.Width(),rect.Height(),CLR_SOFT_LIGHT);
	DrawRoundRectangleNew(&memDc,0,0,rect.Width() - 1,rect.Height() - 1,8,8,8,8,TRUE,Color(0XD8,0XD8,0XD8),1,TRUE,Color(0XFF,0XFF,0XFF),1);

	int iTempInt1 = 16 * m_dHoriRatio,iTempInt2 = 99 * m_dHoriRatio,iTempInt3 = 20 * m_dHoriRatio,iStartX = 100 * m_dHoriRatio;
	int iEdWidth = 406 * m_dHoriRatio,iEdWidth2 = 386 * m_dHoriRatio;
	if(theApp.GetBeEnglish())
	{
		iTempInt2 += iTempInt3;
		iStartX += iTempInt3;
		if(!m_bPrjMange)
		{
			iEdWidth2 = 241 * m_dHoriRatio;
		}
	}
	else
	{
		iEdWidth2 = 241 * m_dHoriRatio;
	}
	iEdWidth2 -= iTempInt3;

	DWORD dwFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;

#ifdef HIDE_PROJECTTYPE
	if(m_bPrjMange)
	{
		memDc.DrawText(str1,CRect(iTempInt1,20 * m_dVertRatio,iTempInt2,60 * m_dVertRatio),dwFormat);
		memDc.DrawText(str3,CRect(iTempInt1,72 * m_dVertRatio,iTempInt2,112 * m_dVertRatio),dwFormat);
		memDc.DrawText(_T("ul"),CRect(iStartX + iEdWidth2,72 * m_dVertRatio,iStartX + iEdWidth2 + iTempInt3,112 * m_dVertRatio),DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		memDc.DrawText(str1,CRect(iTempInt1,30 * m_dVertRatio,iTempInt2,54 * m_dVertRatio),dwFormat);
		memDc.DrawText(str3,CRect(iTempInt1,86 * m_dVertRatio,iTempInt2,110 * m_dVertRatio),dwFormat);
		memDc.DrawText(_T("ul"),CRect(iStartX + iEdWidth2,86 * m_dVertRatio,iStartX + iEdWidth2 + iTempInt3,110 * m_dVertRatio),DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	}
#else
	if(m_bPrjMange)
	{
		memDc.DrawText(str1,CRect(iTempInt1,20 * m_dVertRatio,iTempInt2,60 * m_dVertRatio),dwFormat);
		memDc.DrawText(str2,CRect(iTempInt1,72 * m_dVertRatio,iTempInt2,112 * m_dVertRatio),dwFormat);
		memDc.DrawText(str3,CRect(iTempInt1,124 * m_dVertRatio,iTempInt2,164 * m_dVertRatio),dwFormat);
		memDc.DrawText(_T("ul"),CRect(iStartX + iEdWidth2,124 * m_dVertRatio,iStartX + iEdWidth2 + iTempInt3,164 * m_dVertRatio),DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		memDc.DrawText(str1,CRect(iTempInt1,30 * m_dVertRatio,iTempInt2,54 * m_dVertRatio),dwFormat);
		memDc.DrawText(str2,CRect(iTempInt1,86 * m_dVertRatio,iTempInt2,110 * m_dVertRatio),dwFormat);
		memDc.DrawText(str3,CRect(iTempInt1,134 * m_dVertRatio,iTempInt2,174 * m_dVertRatio),dwFormat);
		memDc.DrawText(_T("ul"),CRect(iStartX + iEdWidth2,134 * m_dVertRatio,iStartX + iEdWidth2 + iTempInt3,174 * m_dVertRatio),DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	}
#endif

#ifndef HIDE_PROJECTUNIT

	#ifdef HIDE_PROJECTTYPE
		if(m_bPrjMange)
		{
			memDc.DrawText(str4,CRect(iTempInt1,124 * m_dVertRatio,iTempInt2,164 * m_dVertRatio),dwFormat);
		}
		else
		{
			memDc.DrawText(str4,CRect(iTempInt1,134 * m_dVertRatio,iTempInt2,174 * m_dVertRatio),dwFormat);
		}
	#else
		if(m_bPrjMange)
		{
			memDc.DrawText(str4,CRect(iTempInt1,176 * m_dVertRatio,iTempInt2,216 * m_dVertRatio),dwFormat);
		}
		else
		{
			memDc.DrawText(str4,CRect(iTempInt1,190 * m_dVertRatio,iTempInt2,230 * m_dVertRatio),dwFormat);
		}
	#endif
	
#endif

	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

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

void CPrjBaseInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}
	return;
}

LRESULT CPrjBaseInfoDlg::OnMsgTellParentTextChanged(WPARAM wParam, LPARAM lParam)
{	
	CMyNewEdit* pEdit = NULL;;
	EITEMINFOTYPE eItemInfoType = (EITEMINFOTYPE)wParam;
	EITEMINFODATATYPE eItemInfoDataType = EIIDT_NONE;

	switch(eItemInfoType)
	{
		case ITEM_PRJBASE_CBPRJTYPE:	pEdit = &m_cbPrjType;	eItemInfoDataType = EIIDT_STRING;	break;
		case ITEM_PRJBASE_EDPRJNAME:	pEdit = &m_edPrjName;	eItemInfoDataType = EIIDT_STRING;	break;
		default:	break;
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
	default:	break;
	}

	if(NULL != m_pInterfaceB)
	{
		m_pInterfaceB->SetNewPrjBaseInfo(eItemInfoType,strTemp,iData,dData);
		return 0;
	}

	return 2;
}

LRESULT CPrjBaseInfoDlg::OnMsgTellParentSetNewText(WPARAM wParam, LPARAM lParam)
{
	CMyNewEdit* pEdit = NULL;;
	EITEMINFOTYPE eItemInfoType = (EITEMINFOTYPE)wParam;
	EITEMINFODATATYPE eItemInfoDataType = EIIDT_NONE;

	switch(eItemInfoType)
	{
	case ITEM_PRJBASE_CBPRJTYPE:	pEdit = &m_cbPrjType;	eItemInfoDataType = EIIDT_STRING;	break;
	case ITEM_PRJBASE_EDPRJNAME:	pEdit = &m_edPrjName;	eItemInfoDataType = EIIDT_STRING;	break;
	default:	break;
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
	default:	break;
	}

	if(NULL != m_pInterfaceB)
	{
		m_pInterfaceB->SetNewPrjBaseInfo(eItemInfoType,strTemp,iData,dData);
		return 0;
	}

	return 2;
}

BOOL CPrjBaseInfoDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

	InitDlg();
	CreateCtrl();
	m_bCtrlCreated = TRUE;
	InitCtrl();
	SetCtrlPos();
	ShowCtrl();

	return TRUE;
}

void CPrjBaseInfoDlg::CreateCtrl()
{
	CreateBt();
	CreateSt();
	CreateCb();
	CreateEd();
}

void CPrjBaseInfoDlg::CreateBt()
{
	
}

void CPrjBaseInfoDlg::CreateSt()
{
	
}

void CPrjBaseInfoDlg::CreateCb()
{
	CRect rcCreate(0,0,0,0);

	m_cbPrjType.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rcCreate,this,ID_PRJBASEINFODLG_CB_PRJTYPE);
}

void CPrjBaseInfoDlg::CreateEd()
{
	m_edPrjName.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0,0,0,0), this,ID_PRJBASEINFODLG_ED_PRJNAME);
	m_edReactSystem.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0,0,0,0), this,ID_PRJBASEINFODLG_ED_SYSTEM);
	m_edUnit.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0,0,0,0), this,ID_PRJBASEINFODLG_ED_UNIT);
}

void CPrjBaseInfoDlg::InitDlg()
{
	int iFontH = 16 * m_dTxRatio;

	SetTextClr1(RGB(0X39,0X45,0X65));
	SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	if(theApp.GetBeEnglish())
	{
		iFontH = 17 * m_dTxRatio;
	}
}

void CPrjBaseInfoDlg::InitCtrl()
{
	InitBt();
	InitSt();
	InitEd();
	InitCb();
}

void CPrjBaseInfoDlg::InitBt()
{
	
}

void CPrjBaseInfoDlg::InitSt()
{
	
}

void CPrjBaseInfoDlg::InitCb()
{
	int iFontH = 16 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 17 * m_dTxRatio;
	}

	m_cbPrjType.SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	m_cbPrjType.SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	m_cbPrjType.SetSelectedBkColor(CLR_SOFT_THEME,CLR_SOFT_THEME,CLR_SOFT_THEME,CLR_SOFT_THEME);
	m_cbPrjType.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XC6,0XC6,0XC6));
	m_cbPrjType.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_cbPrjType.m_selWnd.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_cbPrjType.SetWorkState(EWS_READONLY);
	m_cbPrjType.m_iRightIndent = 44;
	m_cbPrjType.m_arrowInfo.m_clrArrow = Color(0X39,0X45,0X65);
	m_cbPrjType.m_arrowInfo.m_arrowW = 12;
	m_cbPrjType.m_arrowInfo.m_arrowH = 8;

	if(m_bPrjMange)
	{
		m_cbPrjType.SetTellParentTxChangedParam(TRUE,ITEM_PRJBASE_CBPRJTYPE);
	}
	m_cbPrjType.SetTellParentSetNewText(TRUE,ITEM_PRJBASE_CBPRJTYPE);

	m_cbPrjType.InsertString(0,theApp.m_pLangInfo->GetText(33));
	m_cbPrjType.InsertString(1,theApp.m_pLangInfo->GetText(219));
	m_cbPrjType.InsertString(2,theApp.m_pLangInfo->GetText(220));
	m_cbPrjType.SetCurSel(1,FALSE,TRUE);

	return;
}

void CPrjBaseInfoDlg::InitEd()
{
	int iFontH = 16 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 17 * m_dTxRatio;
	}

	m_edPrjName.SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	m_edPrjName.SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	m_edPrjName.SetSelectedBkColor(RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB));
	m_edPrjName.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X97,0X97,0X97));
	m_edPrjName.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	if(m_bPrjMange)
	{
		m_edPrjName.SetTellParentTxChangedParam(TRUE,ITEM_PRJBASE_EDPRJNAME);
	}

	m_edReactSystem.SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	m_edReactSystem.SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	m_edReactSystem.SetSelectedBkColor(RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB));
	m_edReactSystem.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X97,0X97,0X97));
	m_edReactSystem.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_edReactSystem.SetText(_T("20"),FALSE,TRUE);	//新建实验时，默认实验体系为20ul

	m_edUnit.SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	m_edUnit.SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	m_edUnit.SetSelectedBkColor(RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB));
	m_edUnit.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X97,0X97,0X97));
	m_edUnit.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
}

void CPrjBaseInfoDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	m_rcManage.m_rcInit = rect;
	m_rcManage.ClearRectToDelete();

	int iHeight = 40 * m_dVertRatio,iWidth = 406 * m_dHoriRatio,iWidth2 = 386 * m_dHoriRatio;
	int iStartX = 100 * m_dHoriRatio;

	Rect rc1,rc2,rc3,rc4;

#ifdef HIDE_PROJECTTYPE
	if(theApp.GetBeEnglish())
	{
		int iTempInt1 = 20 * m_dHoriRatio;
		iStartX += iTempInt1;

		if(m_bPrjMange)
		{
			iWidth -= iTempInt1;
			iWidth2 -= iTempInt1;

			rc1 = Rect(iStartX,20 * m_dVertRatio,iWidth,iHeight);
			rc2 = Rect(iStartX,72 * m_dVertRatio,iWidth,iHeight);
			rc3 = Rect(iStartX,72 * m_dVertRatio,iWidth2,iHeight);
			rc4 = Rect(iStartX,124 * m_dVertRatio,iWidth,iHeight);
		}
		else
		{
			iWidth = 261 * m_dHoriRatio;
			iWidth -= iTempInt1;
			iWidth2 = 241 * m_dHoriRatio;
			iWidth2 -= iTempInt1;

			rc1 = Rect(iStartX,22 * m_dVertRatio,iWidth,iHeight);
			rc2 = Rect(iStartX,78 * m_dVertRatio,iWidth,iHeight);
			rc3 = Rect(iStartX,78 * m_dVertRatio,iWidth2,iHeight);
			rc4 = Rect(iStartX,134 * m_dVertRatio,iWidth,iHeight);
		}
	}
	else
	{
		if(m_bPrjMange)
		{
			rc1 = Rect(iStartX,20 * m_dVertRatio,iWidth,iHeight);
			rc2 = Rect(iStartX,72 * m_dVertRatio,iWidth,iHeight);
			rc3 = Rect(iStartX,72 * m_dVertRatio,iWidth2,iHeight);
			rc4 = Rect(iStartX,124 * m_dVertRatio,iWidth,iHeight);
		}
		else
		{
			iWidth = 261 * m_dHoriRatio;
			iWidth2 = 241 * m_dHoriRatio;

			rc1 = Rect(iStartX,22 * m_dVertRatio,iWidth,iHeight);
			rc2 = Rect(iStartX,78 * m_dVertRatio,iWidth,iHeight);
			rc3 = Rect(iStartX,78 * m_dVertRatio,iWidth2,iHeight);
			rc4 = Rect(iStartX,134 * m_dVertRatio,iWidth,iHeight);
		}
	}
#else
	if(theApp.GetBeEnglish())
	{
		int iTempInt1 = 20 * m_dHoriRatio;
		iStartX += iTempInt1;

		if(m_bPrjMange)
		{
			iWidth -= iTempInt1;
			iWidth2 -= iTempInt1;

			rc1 = Rect(iStartX,20 * m_dVertRatio,iWidth,iHeight);
			rc2 = Rect(iStartX,72 * m_dVertRatio,iWidth,iHeight);
			rc3 = Rect(iStartX,124 * m_dVertRatio,iWidth2,iHeight);
			rc4 = Rect(iStartX,176 * m_dVertRatio,iWidth,iHeight);
		}
		else
		{
			iWidth = 261 * m_dHoriRatio;
			iWidth -= iTempInt1;
			iWidth2 = 241 * m_dHoriRatio;
			iWidth2 -= iTempInt1;

			rc1 = Rect(iStartX,22 * m_dVertRatio,iWidth,iHeight);
			rc2 = Rect(iStartX,78 * m_dVertRatio,iWidth,iHeight);
			rc3 = Rect(iStartX,134 * m_dVertRatio,iWidth2,iHeight);
			rc4 = Rect(iStartX,190 * m_dVertRatio,iWidth,iHeight);
		}
	}
	else
	{
		if(m_bPrjMange)
		{
			rc1 = Rect(iStartX,20 * m_dVertRatio,iWidth,iHeight);
			rc2 = Rect(iStartX,72 * m_dVertRatio,iWidth,iHeight);
			rc3 = Rect(iStartX,124 * m_dVertRatio,iWidth2,iHeight);
			rc4 = Rect(iStartX,176 * m_dVertRatio,iWidth,iHeight);
		}
		else
		{
			iWidth = 261 * m_dHoriRatio;
			iWidth2 = 241 * m_dHoriRatio;

			rc1 = Rect(iStartX,22 * m_dVertRatio,iWidth,iHeight);
			rc2 = Rect(iStartX,78 * m_dVertRatio,iWidth,iHeight);
			rc3 = Rect(iStartX,134 * m_dVertRatio,iWidth2,iHeight);
			rc4 = Rect(iStartX,190 * m_dVertRatio,iWidth,iHeight);
		}
	}
#endif

	m_edPrjName.SetWindowPos(this,rc1.X,rc1.Y,rc1.Width,rc1.Height,SWP_NOZORDER);
	m_cbPrjType.SetWindowPos(this,rc2.X,rc2.Y,rc2.Width,rc2.Height,SWP_NOZORDER);
	m_edReactSystem.SetWindowPos(this,rc3.X,rc3.Y,rc3.Width,rc3.Height,SWP_NOZORDER);
	m_edUnit.SetWindowPos(this,rc4.X,rc4.Y,rc4.Width,rc4.Height,SWP_NOZORDER);

#ifndef HIDE_PROJECTTYPE
	m_rcManage.AddRectToDelete(rc2.X,rc2.Y,rc2.Width,rc2.Height);
#endif

#ifndef HIDE_PROJECTUNIT
	m_rcManage.AddRectToDelete(rc4.X,rc4.Y,rc4.Width,rc4.Height);
#endif

	m_rcManage.AddRectToDelete(rc1.X,rc1.Y,rc1.Width,rc1.Height);
	m_rcManage.AddRectToDelete(rc3.X,rc3.Y,rc3.Width,rc3.Height);

	m_rcManage.CalcRectResult();

	return;
}

void CPrjBaseInfoDlg::ResetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	m_rcManage.m_rcInit = rect;
	m_rcManage.ClearRectToDelete();

	int iHeight = 40 * m_dVertRatio,iWidth = 406 * m_dHoriRatio,iWidth2 = 386 * m_dHoriRatio;
	int iStartX = 100 * m_dHoriRatio;

	Rect rc1,rc2,rc3,rc4;

#ifdef HIDE_PROJECTTYPE
	if(theApp.GetBeEnglish())
	{
		int iTempInt1 = 20 * m_dHoriRatio;
		iStartX += iTempInt1;

		if(m_bPrjMange)
		{
			iWidth -= iTempInt1;
			iWidth2 -= iTempInt1;

			rc1 = Rect(iStartX,20 * m_dVertRatio,iWidth,iHeight);
			rc2 = Rect(iStartX,72 * m_dVertRatio,iWidth,iHeight);
			rc3 = Rect(iStartX,72 * m_dVertRatio,iWidth2,iHeight);
			rc4 = Rect(iStartX,124 * m_dVertRatio,iWidth,iHeight);
		}
		else
		{
			iWidth = 261 * m_dHoriRatio;
			iWidth -= iTempInt1;
			iWidth2 = 241 * m_dHoriRatio;
			iWidth2 -= iTempInt1;

			rc1 = Rect(iStartX,22 * m_dVertRatio,iWidth,iHeight);
			rc2 = Rect(iStartX,78 * m_dVertRatio,iWidth,iHeight);
			rc3 = Rect(iStartX,78 * m_dVertRatio,iWidth2,iHeight);
			rc4 = Rect(iStartX,134 * m_dVertRatio,iWidth,iHeight);
		}
	}
	else
	{
		if(m_bPrjMange)
		{
			rc1 = Rect(iStartX,20 * m_dVertRatio,iWidth,iHeight);
			rc2 = Rect(iStartX,72 * m_dVertRatio,iWidth,iHeight);
			rc3 = Rect(iStartX,72 * m_dVertRatio,iWidth2,iHeight);
			rc4 = Rect(iStartX,124 * m_dVertRatio,iWidth,iHeight);
		}
		else
		{
			iWidth = 261 * m_dHoriRatio;
			iWidth2 = 241 * m_dHoriRatio;

			rc1 = Rect(iStartX,22 * m_dVertRatio,iWidth,iHeight);
			rc2 = Rect(iStartX,78 * m_dVertRatio,iWidth,iHeight);
			rc3 = Rect(iStartX,78 * m_dVertRatio,iWidth2,iHeight);
			rc4 = Rect(iStartX,134 * m_dVertRatio,iWidth,iHeight);
		}
	}
#else
	if(theApp.GetBeEnglish())
	{
		int iTempInt1 = 20 * m_dHoriRatio;
		iStartX += iTempInt1;

		if(m_bPrjMange)
		{
			iWidth -= iTempInt1;
			iWidth2 -= iTempInt1;

			rc1 = Rect(iStartX,20 * m_dVertRatio,iWidth,iHeight);
			rc2 = Rect(iStartX,72 * m_dVertRatio,iWidth,iHeight);
			rc3 = Rect(iStartX,124 * m_dVertRatio,iWidth2,iHeight);
			rc4 = Rect(iStartX,176 * m_dVertRatio,iWidth,iHeight);
		}
		else
		{
			iWidth = 261 * m_dHoriRatio;
			iWidth -= iTempInt1;
			iWidth2 = 241 * m_dHoriRatio;
			iWidth2 -= iTempInt1;

			rc1 = Rect(iStartX,22 * m_dVertRatio,iWidth,iHeight);
			rc2 = Rect(iStartX,78 * m_dVertRatio,iWidth,iHeight);
			rc3 = Rect(iStartX,134 * m_dVertRatio,iWidth2,iHeight);
			rc4 = Rect(iStartX,190 * m_dVertRatio,iWidth,iHeight);
		}
	}
	else
	{
		if(m_bPrjMange)
		{
			rc1 = Rect(iStartX,20 * m_dVertRatio,iWidth,iHeight);
			rc2 = Rect(iStartX,72 * m_dVertRatio,iWidth,iHeight);
			rc3 = Rect(iStartX,124 * m_dVertRatio,iWidth2,iHeight);
			rc4 = Rect(iStartX,176 * m_dVertRatio,iWidth,iHeight);
		}
		else
		{
			iWidth = 261 * m_dHoriRatio;
			iWidth2 = 241 * m_dHoriRatio;

			rc1 = Rect(iStartX,22 * m_dVertRatio,iWidth,iHeight);
			rc2 = Rect(iStartX,78 * m_dVertRatio,iWidth,iHeight);
			rc3 = Rect(iStartX,134 * m_dVertRatio,iWidth2,iHeight);
			rc4 = Rect(iStartX,190 * m_dVertRatio,iWidth,iHeight);
		}
	}
#endif

	m_edPrjName.MoveWindow(rc1.X,rc1.Y,rc1.Width,rc1.Height,FALSE);
	m_cbPrjType.MoveWindow(rc2.X,rc2.Y,rc2.Width,rc2.Height,FALSE);
	m_edReactSystem.MoveWindow(rc3.X,rc3.Y,rc3.Width,rc3.Height,FALSE);
	m_edUnit.MoveWindow(rc4.X,rc4.Y,rc4.Width,rc4.Height,FALSE);

#ifndef HIDE_PROJECTTYPE
	m_rcManage.AddRectToDelete(rc2.X,rc2.Y,rc2.Width,rc2.Height);
#endif

#ifndef HIDE_PROJECTUNIT
	m_rcManage.AddRectToDelete(rc4.X,rc4.Y,rc4.Width,rc4.Height);
#endif

	m_rcManage.AddRectToDelete(rc1.X,rc1.Y,rc1.Width,rc1.Height);
	m_rcManage.AddRectToDelete(rc3.X,rc3.Y,rc3.Width,rc3.Height);

	m_rcManage.CalcRectResult();

	return;
}

void CPrjBaseInfoDlg::ShowCtrl()
{
#ifndef HIDE_PROJECTTYPE
	m_cbPrjType.ShowWindow(SW_SHOW);
#endif

	m_edPrjName.ShowWindow(SW_SHOW);
	m_edReactSystem.ShowWindow(SW_SHOW);

#ifndef HIDE_PROJECTUNIT
	m_edUnit.ShowWindow(SW_SHOW);
#endif
}

void CPrjBaseInfoDlg::SetCtrlEnableState(BOOL bEnabled)
{
	m_edPrjName.EnableWindow(bEnabled);
	m_cbPrjType.EnableWindow(bEnabled);
	m_edReactSystem.EnableWindow(bEnabled);
	m_edUnit.EnableWindow(bEnabled);
}

void CPrjBaseInfoDlg::ClearPage()
{
	m_cbPrjType.SetCurSel(0,TRUE,TRUE);

	m_edPrjName.SetText(_T(""),TRUE,TRUE);
	m_edReactSystem.SetText(_T(""),TRUE,TRUE);
	m_edUnit.SetText(_T(""),TRUE,TRUE);
}

int CPrjBaseInfoDlg::SetPrjBaseInfo(CReagentItem* pReagentItem)
{
	if(NULL == pReagentItem)
	{
		return -1;
	}

	CString strTemp(_T(""));

	m_edPrjName.SetText(pReagentItem->GetName(),TRUE,TRUE);
	m_cbPrjType.SetCurSel(pReagentItem->GetType(),TRUE,TRUE);
	strTemp.Format(_T("%d"),pReagentItem->GetReactVolume());
	m_edReactSystem.SetText(strTemp,TRUE,TRUE);
	m_edUnit.SetText(pReagentItem->GetUnit(),TRUE,TRUE);

	return 0;
}

int CPrjBaseInfoDlg::GetPrjBaseInfo(CReagentItem* pReagentItem)
{
	if(NULL == pReagentItem)
	{
		return -1;
	}

	CString strTemp(_T(""));

	pReagentItem->SetName(m_edPrjName.GetText());
	pReagentItem->SetType((eExperimentType)m_cbPrjType.GetCurSel());
	pReagentItem->SetReactVolume(_ttoi(m_edReactSystem.GetText()));
	pReagentItem->SetUnit(m_edUnit.GetText());

	return 0;
}

void CPrjBaseInfoDlg::SetEditState(BOOL bEditable,BOOL bRedraw /*= FALSE*/)
{
	m_cbPrjType.SetBeEditable(bEditable,bRedraw);
	m_edPrjName.SetBeEditable(bEditable,bRedraw);
	m_edReactSystem.SetBeEditable(bEditable,bRedraw);
	m_edUnit.SetBeEditable(bEditable,bRedraw);
}

int CPrjBaseInfoDlg::RefreshLanguageDisplay()
{
	SetMyFont(m_stTxFont);

	int iFontH = 16 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 17 * m_dTxRatio;
	}

	m_edPrjName.SetMyFont(m_stTxFont,iFontH);
	m_edPrjName.TextParamChanged();

	m_edReactSystem.SetMyFont(m_stTxFont,iFontH);
	m_edPrjName.TextParamChanged();

	m_edUnit.SetMyFont(m_stTxFont,iFontH);
	m_edPrjName.TextParamChanged();

	m_cbPrjType.SetMyFont(m_stTxFont,iFontH);
	m_cbPrjType.m_selWnd.SetMyFont(m_stTxFont,iFontH);
	m_cbPrjType.TextParamChanged();

	int iCurSel = m_cbPrjType.GetCurSel();
	m_cbPrjType.SetItemText(0,theApp.m_pLangInfo->GetText(33));
	m_cbPrjType.SetItemText(1,theApp.m_pLangInfo->GetText(219));
	m_cbPrjType.SetItemText(2,theApp.m_pLangInfo->GetText(220));
	m_cbPrjType.RefreshText(iCurSel);

	ResetCtrlPos();

	return 0;
}

int CPrjBaseInfoDlg::SetEditMode(EEXPEDITMODE eExpEditMode)
{
	if(m_eExpEditMode == eExpEditMode)
	{
		return 1;
	}

	m_eExpEditMode = eExpEditMode;

	if(EEEM_IVD == m_eExpEditMode)
	{
		m_edPrjName.SetBeEditable(FALSE,TRUE);
		m_cbPrjType.SetBeEditable(FALSE,TRUE);
		m_edReactSystem.SetBeEditable(FALSE,TRUE);
		m_edUnit.SetBeEditable(FALSE,TRUE);
	}
	else if(EEEM_NOLIMIT == m_eExpEditMode)
	{
		m_edPrjName.SetBeEditable(TRUE,TRUE);
		m_cbPrjType.SetBeEditable(TRUE,TRUE);
		m_edReactSystem.SetBeEditable(TRUE,TRUE);
		m_edUnit.SetBeEditable(TRUE,TRUE);
	}
	else if(EEEM_RUNNING == m_eExpEditMode)
	{
		m_edPrjName.SetBeEditable(FALSE,TRUE);
		m_cbPrjType.SetBeEditable(FALSE,TRUE);
		m_edReactSystem.SetBeEditable(FALSE,TRUE);
		m_edUnit.SetBeEditable(FALSE,TRUE);
	}
	else if(EEEM_END == m_eExpEditMode)
	{
		m_edPrjName.SetBeEditable(TRUE,TRUE);
		m_cbPrjType.SetBeEditable(TRUE,TRUE);
		m_edReactSystem.SetBeEditable(TRUE,TRUE);
		m_edUnit.SetBeEditable(TRUE,TRUE);
	}

	return 0;
}

