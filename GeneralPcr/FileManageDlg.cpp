// BottomFuncBtDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralPcr.h"
#include "GeneralPcrDlg.h"
#include "FileManageDlg.h"
#include "MiddleDlg.h"
#include "WorkDlg.h"
#include "afxdialogex.h"
#include "PublicInfo.h"


IMPLEMENT_DYNAMIC(CFileManageDlg, CDialogEx)

CFileManageDlg::CFileManageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileManageDlg::IDD, pParent),m_imgLogo(_T(".\\BkPic\\logo.png")),m_imgLogo2(_T(".\\BkPic\\icon2.png"))
{
	m_pWorkDlg = (CWorkDlg*)pParent;
	m_iBtHeight = 38;
	m_pCurSelBt = NULL;
	m_bCtrlCreated = FALSE;
	m_iLeftIndent = 16;

	m_iRightIndent = 188;
	m_iBtRegionW = 1548;
#ifdef XINYI_THEME
	m_iRightIndent += 108;
	m_iBtRegionW -= 108;
#else
	#ifdef PS96INSTRUMENT
		m_iRightIndent += 105;
		m_iBtRegionW -= 105;
	#endif
#endif

	m_iBtDefWidth = 270;
	m_iBtRealWidth = 270;
	m_iBtPreRealW = 270;
}

CFileManageDlg::~CFileManageDlg()
{
	Node<CMyPageSwitchBt*>* pNodePageSwitchBt = m_lnkPageSwitchBtPtr.GetHead();
	while(NULL != pNodePageSwitchBt)
	{
		delete pNodePageSwitchBt->data;
		pNodePageSwitchBt->data = NULL;
		pNodePageSwitchBt = pNodePageSwitchBt->pNext;
	}
}

void CFileManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFileManageDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_PAGESWITCHBTSTART,ID_PAGESWITCHBTSTART + 1000,OnPageSelected)
	ON_MESSAGE(WM_CLOSEPAGE,OnMsgClosePage)
END_MESSAGE_MAP()

BOOL CFileManageDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect; 
	GetClientRect(&rect);
	
	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	memDc.FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),RGB(0XFF,0XFF,0XFF));

	Graphics graphics(memDc.m_hDC);
	graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
	ImageAttributes ImgAtt;
	ImgAtt.SetWrapMode(WrapModeTileFlipXY);
	Rect destRect;
	destRect.X = 1588 * m_dHoriRatio;
	destRect.Y = 9 * m_dVertRatio;
	destRect.Width = 48 * m_dHoriRatio;
	destRect.Height = 24 * m_dVertRatio;

#ifndef NOLOGO_VERSION

#ifdef XINYI_THEME
	destRect.X -= 108 * m_dHoriRatio;
	destRect.Y = 1 * m_dVertRatio;
	destRect.Width = 79 * m_dHoriRatio;
	destRect.Height = 43 * m_dVertRatio;
	graphics.DrawImage(&m_imgLogo2,destRect,0,0,m_imgLogo2.GetWidth(),m_imgLogo2.GetHeight(),UnitPixel,&ImgAtt);
#else

#ifdef CAN_YOUNG
	destRect.X -= 88 * m_dHoriRatio;
	destRect.Y = 4 * m_dVertRatio;
	destRect.Width = 70 * m_dHoriRatio;
	destRect.Height = 36 * m_dVertRatio;
	graphics.DrawImage(&m_imgLogo2,destRect,0,0,m_imgLogo2.GetWidth(),m_imgLogo2.GetHeight(),UnitPixel,&ImgAtt);
#else
#ifdef PS96INSTRUMENT
	destRect.X -= 105 * m_dHoriRatio;
	destRect.Y = 2 * m_dVertRatio;
	destRect.Width = 76 * m_dHoriRatio;
	destRect.Height = 40 * m_dVertRatio;
	graphics.DrawImage(&m_imgLogo,destRect,0,0,m_imgLogo.GetWidth(),m_imgLogo.GetHeight(),UnitPixel,&ImgAtt);
#else
	graphics.DrawImage(&m_imgLogo,destRect,0,0,m_imgLogo.GetWidth(),m_imgLogo.GetHeight(),UnitPixel,&ImgAtt);
#endif
#endif

#endif

#else
	destRect.X -= 88 * m_dHoriRatio;

#endif

	graphics.ReleaseHDC(memDc.m_hDC);

	//绘制软件版本号，20220513
	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);
	memDc.SetBkMode(TRANSPARENT);
	memDc.DrawText(SOFRVERION_STRING,CRect(destRect.GetRight(),0,rect.right,rect.bottom),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	memDc.SelectObject(pOldFont);

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

void CFileManageDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	m_iBtRegionW = cx - m_iLeftIndent - m_iRightIndent;

	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}
	return;
}

int CFileManageDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_iLeftIndent *= m_dHoriRatio;
	m_iRightIndent *= m_dHoriRatio;
	m_iBtDefWidth *= m_dHoriRatio;
	m_iBtRealWidth *= m_dHoriRatio;
	m_iBtPreRealW *= m_dHoriRatio;
	m_iBtHeight *= m_dVertRatio;

	return CDialogEx::OnCreate(lpCreateStruct);
}

BOOL CFileManageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitDlg();
	m_bCtrlCreated = TRUE;
	SetCtrlPos();

	ShowCtrl();

	return TRUE;
}

void CFileManageDlg::InitDlg()
{
	SetMyFont(m_stTxFont,16 * m_dTxRatio,FW_MEDIUM,TYPE_PIXEL);

#ifndef BIOWET_BK
	SetTextClr1(CLR_SOFT_THEME);
#else
	SetTextClr1(CLR_2LEVEL_THEME);
#endif
}


void CFileManageDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	int startX = m_iLeftIndent;
	for(int i = 0;i < m_lnkPageSwitchBtPtr.GetSize();++i)
	{
		(*m_lnkPageSwitchBtPtr[i])->SetWindowPos(this,startX,BT_TOP_START,m_iBtDefWidth,m_iBtHeight,SWP_NOZORDER);
		startX += m_iBtDefWidth;
	}

	m_rcManage.m_rcInit = rect;
	m_rcManage.ClearRectToDelete();
	m_rcManage.AddRectToDelete(startX,BT_TOP_START,m_iBtRealWidth * m_lnkPageSwitchBtPtr.GetSize(),m_iBtHeight);
	m_rcManage.CalcRectResult();
}

void CFileManageDlg::SetPageSwitchBtPos(int iStart)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int iStartX = m_iLeftIndent;
	int iSwitchBtNum = m_lnkPageSwitchBtPtr.GetSize();
	CMyPageSwitchBt* pPageSwtBt = NULL;

	if(iSwitchBtNum * m_iBtDefWidth > m_iBtRegionW)
	{
		m_iBtRealWidth = m_iBtRegionW / iSwitchBtNum;
	}
	else
	{
		m_iBtRealWidth = m_iBtDefWidth;
	}

	m_rcManage.m_rcInit = rcClient;
	m_rcManage.ClearRectToDelete();
	m_rcManage.AddRectToDelete(m_iLeftIndent,BT_TOP_START,m_iBtRealWidth * iSwitchBtNum,m_iBtHeight);
	m_rcManage.CalcRectResult();

	if(m_iBtPreRealW != m_iBtRealWidth)
	{
		iStart = 0;
	}
	m_iBtPreRealW = m_iBtRealWidth;

	BOOL bProcessed = FALSE;

	iStartX = m_iLeftIndent + iStart * m_iBtRealWidth;
	for(int i = iStart;i < iSwitchBtNum;++i)
	{
		pPageSwtBt = *m_lnkPageSwitchBtPtr[i];
		if(NULL != pPageSwtBt->GetSafeHwnd())
		{
			pPageSwtBt->MoveWindow(iStartX,BT_TOP_START,m_iBtRealWidth,m_iBtHeight,TRUE);
			iStartX += m_iBtRealWidth;

			bProcessed = TRUE;
		}
	}	

	if(!bProcessed)
	{
		Invalidate();
	}

	return;
}

void CFileManageDlg::ShowCtrl()
{
	for(int i = 0;i < m_lnkPageSwitchBtPtr.GetSize();++i)
	{
		(*m_lnkPageSwitchBtPtr[i])->ShowWindow(SW_SHOW);
	}
}


void CFileManageDlg::OnPageSelected(UINT nID)
{
	int pageId = ((CMyPageSwitchBt*)GetDlgItem(nID))->GetPageId();
	SetPageSelected(pageId);
}

LRESULT CFileManageDlg::OnMsgClosePage(WPARAM wParam,LPARAM lParam)
{
	return DeletePage(wParam);
}

int CFileManageDlg::AddPage()
{
	CExperimentData* pExpData = new CExperimentData;
	if(NULL == pExpData)
	{
		return -1;
	}

	CPublicInfo* pPublicInfo = CPublicInfo::GetInstance();

	//设置实验默认保存路径
	CString strFSPOrigin(_T(""));
	strFSPOrigin = pPublicInfo->GetUserDataSavePath();
	pExpData->GetExperimentInfo()->SetFilePath(strFSPOrigin);

	pExpData->GetExperimentInfo()->SetOperator(pPublicInfo->GetUserName());
	AddNewPage(pExpData);

	return 0;
}

int CFileManageDlg::AddNewPage(CExperimentData* pExpData,CString strFileName /*= _T("")*/)
{
	if(NULL == pExpData)
	{
		return -1;
	}

	CMyPageSwitchBt* pPageSwtBt = new CMyPageSwitchBt;
	if(NULL == pPageSwtBt)
	{
		PopWarningTipInfo(_T("AddNewPage failed,memory allocate error!"));
		return -4;
	}	

	pPageSwtBt->Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PAGESWITCHBTSTART + pPageSwtBt->GetPageId());
	InitPageSwitchBt(pPageSwtBt);

	m_lnkPageSwitchBtPtr.AddTail(pPageSwtBt);

	SetPageSwitchBtPos(m_lnkPageSwitchBtPtr.GetSize() - 1);

	pPageSwtBt->ShowWindow(SW_SHOW);

	int ret = m_pWorkDlg->AddNewPage(pPageSwtBt->GetPageId());
	if(0 != ret)
	{
		PopWarningTipInfo(_T("页面添加失败1"));
		return -2;
	}

	CMiddleDlg* pMiddleDlg = m_pWorkDlg->GetMiddleDlg(pPageSwtBt->GetPageId());
	if(NULL == pMiddleDlg->GetSafeHwnd())
	{
		PopWarningTipInfo(_T("页面添加失败2"));
		return -3;
	}

	//创建窗口后保存实验数据对象指针到CMiddleDlg中
	pMiddleDlg->SetExpDataPtr(pExpData);
	pMiddleDlg->InitExperimentData(pExpData);
	pMiddleDlg->EmbodyBaseSetInfo();

	if(strFileName.IsEmpty() || pExpData->GetBeTemplateFile())
	{
		strFileName = pExpData->GetExperimentInfo()->GetExpName();
	}
	pPageSwtBt->SetWindowText(strFileName);

	SetPageSelected(pPageSwtBt->GetPageId());

	return 0;
}

int CFileManageDlg::DeletePage(int pageId)
{
	//只存在一个文档（窗口）时，不允许删除操作
	if(1 == m_lnkPageSwitchBtPtr.GetSize())
	{
		return 1;
	}

	if(pageId < 0)
	{
		return -1;
	}

	if(pageId > CMyPageSwitchBt::PageCount)
	{
		return -2;
	}


	if(NULL == m_pWorkDlg->GetSafeHwnd())
	{
		return -4;
	}
	
	if(NULL == m_pWorkDlg->m_pCurSelPageDlg->GetSafeHwnd())
	{
		return -5;
	}

	if(m_pWorkDlg->m_pCurSelPageDlg->IsExpRunning())
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(289));
		return -3;	
	}

	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	BOOL bChanged = FALSE;//m_pWorkDlg->m_pCurSelPageDlg->m_dataAnaDlg.GetBeChanged();
	if(bChanged)
	{
		if(IDYES == PopQueryTipInfo(pLangInfo->GetText(248)))
		{
			//pMainDlg->SaveAs();
		}
	}
	else
	{
		if(IDYES != PopQueryTipInfo(pLangInfo->GetText(247)))
		{
			return -6;
		}
	}

	CMyPageSwitchBt* pTempPageSwtBt = NULL;
	int iSwtBtNum = m_lnkPageSwitchBtPtr.GetSize();
	for(int i = 0;i < iSwtBtNum;++i)
	{
		pTempPageSwtBt = *m_lnkPageSwitchBtPtr[i];
		if(pageId == pTempPageSwtBt->GetPageId())
		{
			BOOL bNeedResetSelPage = FALSE;
			if(NULL != m_pCurSelBt->GetSafeHwnd() &&  pageId == m_pCurSelBt->GetPageId())
			{
				m_pCurSelBt = NULL;
				bNeedResetSelPage = TRUE;
			}
			
			BOOL ret = pTempPageSwtBt->DestroyWindow();
			delete pTempPageSwtBt;
			m_lnkPageSwitchBtPtr.DeleteAt(i);

			m_pWorkDlg->DeletePage(pageId);

			if(bNeedResetSelPage)
			{
				int curSelBtIndex = -1;
				int iNewSwtBtNum = m_lnkPageSwitchBtPtr.GetSize();
				if(iNewSwtBtNum > 0)
				{
					if(i == iNewSwtBtNum)
					{
						curSelBtIndex = i - 1;
					}
					else if(i < iNewSwtBtNum)
					{
						curSelBtIndex = i;
					}
				}
				if(-1 != curSelBtIndex)
				{
					SetPageSelected((*m_lnkPageSwitchBtPtr[curSelBtIndex])->GetPageId());
				}
			}

			SetPageSwitchBtPos(i);

			break;
		}
	}

	return 0;
}

void CFileManageDlg::SetNewFileSavePath(CString strNewFileSavePath)
{
	if(NULL != m_pCurSelBt->GetSafeHwnd())
	{
		m_pCurSelBt->m_strFilePath = strNewFileSavePath;
	}
}

int CFileManageDlg::SetPageFirstColumn(int pageId)
{
	if(m_lnkPageSwitchBtPtr.GetSize() < 1)
	{
		return 1;
	}

	CMyPageSwitchBt* pTempPageSwtBt = *m_lnkPageSwitchBtPtr[0];
	if(NULL == pTempPageSwtBt)
	{
		return -1;
	}

	if(1 == m_lnkPageSwitchBtPtr.GetSize() || pageId == pTempPageSwtBt->GetPageId())
	{
		pTempPageSwtBt->SetBeBtPageRunning(TRUE);
		return 2;
	}

 	if(pageId < 0)
	{
		return -2;
	}

	if(pageId > CMyPageSwitchBt::PageCount)
	{
		return -3;
	}

	int iSwtBtNum = m_lnkPageSwitchBtPtr.GetSize();
	for(int i = 0;i < iSwtBtNum;++i)
	{
		pTempPageSwtBt = *m_lnkPageSwitchBtPtr[i];
		if(pageId == pTempPageSwtBt->GetPageId())
		{
			m_lnkPageSwitchBtPtr.DeleteAt(i);
			m_lnkPageSwitchBtPtr.AddHead(pTempPageSwtBt);

			int startX = m_iLeftIndent; 
			for(int j = 0;j <= i;++j) 
			{
				pTempPageSwtBt = *m_lnkPageSwitchBtPtr[j];
				if(0 == j)
				{
					pTempPageSwtBt->SetBeBtPageRunning(TRUE);
				}
				pTempPageSwtBt->MoveWindow(startX,BT_TOP_START,m_iBtRealWidth,m_iBtHeight);
				startX += m_iBtRealWidth;
			}

			break;
		}
	}

	return 0;
}

int CFileManageDlg::SetPageSwitchBtState(int pageId,BOOL bRunning,BOOL bRedraw /*= TRUE*/)
{
	if(pageId < 0)
	{
		return -1;
	}

	if(pageId > CMyPageSwitchBt::PageCount)
	{
		return -2;
	}

	CMyPageSwitchBt* pTempPageSwtBt = NULL;
	int iSwtBtNum = m_lnkPageSwitchBtPtr.GetSize();
	for(int i = 0;i < iSwtBtNum;++i)
	{
		pTempPageSwtBt = *m_lnkPageSwitchBtPtr[i];
		if(pageId == pTempPageSwtBt->GetPageId())
		{
			pTempPageSwtBt->SetBeBtPageRunning(bRunning,bRedraw);
			break;
		}
	}

	return 0;
}

int CFileManageDlg::SetPageSelected(int pageId)
{
	if(pageId < 0)
	{
		return -1;
	}

	if(pageId > CMyPageSwitchBt::PageCount)
	{
		return -2;
	}

	if(NULL != m_pCurSelBt->GetSafeHwnd())
	{
		m_pCurSelBt->EnableWindow(TRUE);
	}

	CMyPageSwitchBt* pTempPageSwtBt = NULL;
	for(int i = 0;i < m_lnkPageSwitchBtPtr.GetSize();++i)
	{
		pTempPageSwtBt = *m_lnkPageSwitchBtPtr[i];
		if(pageId == pTempPageSwtBt->GetPageId())
		{
			pTempPageSwtBt->EnableWindow(FALSE);
			m_pCurSelBt = pTempPageSwtBt;
			m_pWorkDlg->SetPageSelected(pageId);
			break;
		}
	}
	
	return 0;
}

int CFileManageDlg::SetCurrentSelectPageString(CString strFileName,CString strFilePath)
{
	if(NULL != m_pCurSelBt->GetSafeHwnd())
	{
		m_pCurSelBt->SetWindowText(strFileName);
		m_pCurSelBt->m_strFilePath = strFilePath;
	}
	return 0;
}

void CFileManageDlg::SetExperName(int pageId,CString strExpeName)
{
	CMyPageSwitchBt* pTempPageSwtBt = NULL;
	for(int i = 0;i < m_lnkPageSwitchBtPtr.GetSize();++i)
	{
		pTempPageSwtBt = *m_lnkPageSwitchBtPtr[i];
		if(NULL != pTempPageSwtBt->GetSafeHwnd() && pageId == pTempPageSwtBt->GetPageId())
		{
			pTempPageSwtBt->SetWindowText(strExpeName);
			break;
		}
	}

	return;
}

BOOL CFileManageDlg::CheckFileExistOrNot(CString strFilePath)
{
	CMyPageSwitchBt* pTempPageSwtBt = NULL;
	for(int i = 0;i < m_lnkPageSwitchBtPtr.GetSize();++i)
	{
		pTempPageSwtBt = *m_lnkPageSwitchBtPtr[i];
		if(NULL != pTempPageSwtBt->GetSafeHwnd())
		{
			if(pTempPageSwtBt->m_strFilePath == strFilePath)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

int CFileManageDlg::SetPageFilePath(int iPageId,CString strFilePath)
{
	CMyPageSwitchBt* pTempPageSwtBt = NULL;
	for(int i = 0;i < m_lnkPageSwitchBtPtr.GetSize();++i)
	{
		pTempPageSwtBt = *m_lnkPageSwitchBtPtr[i];
		if(pTempPageSwtBt->GetPageId() == iPageId)
		{
			pTempPageSwtBt->m_strFilePath = strFilePath;
			return 0;
		}
	}

	return -1;
}

BOOL CFileManageDlg::CheckFileNameExistOrNot(CString strFileName)
{
	CString strTemp(_T(""));
	CMyPageSwitchBt* pTempPageSwtBt = NULL;
	for(int i = 0;i < m_lnkPageSwitchBtPtr.GetSize();++i)
	{
		pTempPageSwtBt = *m_lnkPageSwitchBtPtr[i];
		if(NULL != pTempPageSwtBt->GetSafeHwnd())
		{
			pTempPageSwtBt->GetWindowText(strTemp);
			if(strTemp == strFileName)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CFileManageDlg::PreTranslateMessage(MSG* pMsg)
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

void CFileManageDlg::InitPageSwitchBt(CMyPageSwitchBt* pPageSwitchBt)
{
	if(NULL == pPageSwitchBt->GetSafeHwnd())
	{
		return;
	}

	pPageSwitchBt->SetMyFont(m_stTxFont,16 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
	pPageSwitchBt->SetPngBkPic(_T(".\\BkPic\\experiment.png"),2,24,18 * m_dHoriRatio,10 * m_dHoriRatio);
}

