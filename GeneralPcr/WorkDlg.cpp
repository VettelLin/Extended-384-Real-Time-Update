// WorkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralPcr.h"
#include "GeneralPcrDlg.h"
#include "WorkDlg.h"
#include "Ps96SimpleDlg.h"
#include "afxdialogex.h"

// CWorkDlg dialog

IMPLEMENT_DYNAMIC(CWorkDlg, CDialogEx)

CWorkDlg::CWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWorkDlg::IDD, pParent),m_fileManageDlg(this)
{
	m_pCurSelPageDlg = NULL;
	m_bCtrlCreated = FALSE;
}

CWorkDlg::~CWorkDlg()
{
	Node<CMiddleDlg*>* pNodeMiddleDlg = m_lnkPageDlgPtr.GetHead();
	while(NULL != pNodeMiddleDlg)
	{
		delete pNodeMiddleDlg->data;
		pNodeMiddleDlg = pNodeMiddleDlg->pNext;
	}
}

void CWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWorkDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_INSTRUMENT_REINITIALIZE,&CWorkDlg::OnMsgInstrumentReinitialize)
END_MESSAGE_MAP()

void CWorkDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}
	return;
}

BOOL CWorkDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect; 
	GetClientRect(&rect);

	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	int iLeftW = 136 * CFontSet::m_dHoriRatio;
	memDc.FillSolidRect(0,rect.bottom - theApp.m_iFileManageDlgH - 1,iLeftW,theApp.m_iFileManageDlgH + 1,RGB(0XFF,0XFF,0XFF));

	memDc.FillSolidRect(iLeftW,rect.bottom - theApp.m_iFileManageDlgH,rect.Width() - iLeftW,theApp.m_iFileManageDlgH,CLR_SOFT_LIGHT);

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

void CWorkDlg::OnShowWindow(BOOL bShow,UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow,nStatus);
	return;
}

BOOL CWorkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitSonDlg();
	CreateCtrl();
	m_bCtrlCreated = TRUE;
	InitCtrl();
	SetCtrlPos();
	ShowCtrl();

	return TRUE;
}

void CWorkDlg::CreateCtrl()
{
	CreateCkbt();
}

void CWorkDlg::CreateCkbt()
{

}

void CWorkDlg::InitCtrl()
{
	InitCkbt();
}

void CWorkDlg::InitCkbt()
{

}

void CWorkDlg::SetCtrlPos()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	m_rcManage.m_rcInit = rcClient;
	m_rcManage.ClearRectToDelete();

	CRect rect;
	rect.left = 152 * m_dHoriRatio;
	rect.right = rect.left + 1752 * m_dHoriRatio;
	rect.bottom = rcClient.bottom;
	rect.top = rect.bottom - theApp.m_iFileManageDlgH;

	if(NULL != m_fileManageDlg.GetSafeHwnd())
	{
		m_fileManageDlg.SetWindowPos(this,rect.left,rect.top,rect.Width(),rect.Height(),SWP_NOZORDER);
	}
	m_rcManage.AddRectToDelete(rect);

	CMiddleDlg* pPageDlg = NULL;
	for(int i = 0;i < m_lnkPageDlgPtr.GetSize();++i)
	{
		pPageDlg = *m_lnkPageDlgPtr[i];
		if(NULL != pPageDlg->GetSafeHwnd())
		{
			pPageDlg->SetWindowPos(this,0,0,rcClient.Width(),rcClient.Height() - rect.Height(),SWP_NOZORDER);
		}
	}
	m_rcManage.AddRectToDelete(0,0,rcClient.Width(),rcClient.Height() - rect.Height());

	m_rcManage.CalcRectResult();

	return;
}

void CWorkDlg::ShowCtrl()
{
	
}

void CWorkDlg::InitSonDlg()
{
	InitFileManageDlg();
}

void CWorkDlg::InitFileManageDlg()
{
	m_fileManageDlg.Create(IDD_MIDDLEDLG,this);
	m_fileManageDlg.ShowWindow(SW_SHOW);
}

int CWorkDlg::SetPageSelected(int pageId)
{
	if(pageId < 0)
	{
		return -1;
	}

	if(pageId > CMyPageSwitchBt::PageCount)
	{
		return -2;
	}

	if(NULL != m_pCurSelPageDlg->GetSafeHwnd())
	{
		m_pCurSelPageDlg->ShowWindow(FALSE);
	}

	CMiddleDlg* pPageDlg = NULL;
	for(int i = 0;i < m_lnkPageDlgPtr.GetSize();++i)
	{
		pPageDlg = *m_lnkPageDlgPtr[i];
		if(pageId == pPageDlg->GetPageId())
		{
			pPageDlg->ShowWindow(SW_SHOW);
			m_pCurSelPageDlg = pPageDlg;
			break;
		}
	}

	// 设置通道按钮
	m_pCurSelPageDlg->SetChannelDyeBtn();

	//切换到不同的实验文件后，更新底部菜单按钮显示状态
	m_pCurSelPageDlg->UpdateFuncBtMode();

	return 0;
}


int CWorkDlg::AddNewPage(int pageId)
{
	CMiddleDlg* pMiddleDlg = NULL;
#ifdef PS96INSTRUMENT
	if(theApp.GetBeIVDUser())
	{
		pMiddleDlg = new CPs96SimpleDlg;
	}
	else
	{
		pMiddleDlg = new CMiddleDlg;
	}
#else
	pMiddleDlg = new CMiddleDlg;
#endif

	if(NULL == pMiddleDlg)
	{
		PopWarningTipInfo(_T("页面添加失败10"));
		return -1;
	}	

	pMiddleDlg->SetPageId(pageId);
	pMiddleDlg->Create(IDD_MIDDLEDLG,this);

	CRect rcClient;
	GetClientRect(&rcClient);

	pMiddleDlg->SetWindowPos(this,0,0,rcClient.Width(),rcClient.Height() - theApp.m_iFileManageDlgH,SWP_NOZORDER);
	m_lnkPageDlgPtr.AddTail(pMiddleDlg);

	return 0;
}

int CWorkDlg::DeletePage(int pageId)
{
	CMiddleDlg* pPageDlg = NULL;
	for(int i = 0;i < m_lnkPageDlgPtr.GetSize();++i)
	{
		pPageDlg = *m_lnkPageDlgPtr[i];
		if(pageId == pPageDlg->GetPageId())
		{
			if(NULL != m_pCurSelPageDlg->GetSafeHwnd() && pageId == m_pCurSelPageDlg->GetPageId())
			{
				m_pCurSelPageDlg = NULL;
			}

			BOOL ret = pPageDlg->DestroyWindow();
			delete pPageDlg;
			m_lnkPageDlgPtr.DeleteAt(i);
			break;
		}
	}

	return 0;
}

int CWorkDlg::OpenFile()
{
	CString strTemp(_T(""));

	if(m_fileManageDlg.m_lnkPageSwitchBtPtr.GetSize() >= MAX_FILEOPEN_NUM)
	{
		CString strFormat = theApp.m_pLangInfo->GetText(159);
		strTemp.Format(strFormat,MAX_FILEOPEN_NUM);
		PopNormalTipInfo(strTemp);
		return -1;
	}

	CString strOpenFormat(_T("")), strSuffix(_T(""));
	strSuffix.Format(_T(".%s"), PCR_DATA_FILE_SUFFIX);
	strOpenFormat.Format(_T("QPCR File(*.%s);(*.%s)|*.%s;*.%s|All File(*.*)|*.*||"), PCR_DATA_FILE_SUFFIX,PCR_TEMPLATE_FILE_SUFFIX,PCR_DATA_FILE_SUFFIX,PCR_TEMPLATE_FILE_SUFFIX);
	CFileDialog cf(TRUE, strSuffix, NULL,OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT, strOpenFormat, this);
	if(IDOK != cf.DoModal())
	{
		return 1;
	}

	//判断该路径标识的文件是否已经打开
	if(m_fileManageDlg.CheckFileExistOrNot(cf.GetPathName()))
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(160));
		return -2;
	}

	if(m_fileManageDlg.CheckFileNameExistOrNot(cf.GetFileTitle()))
	{
		strTemp = theApp.m_pLangInfo->GetText(161);
		PopWarningTipInfo(strTemp);
		return -3;
	}

	CExperimentData* pExperimentData = new CExperimentData;
	if (NULL == pExperimentData)
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(251));
		return -3;
	}

	int iRet = pExperimentData->OpenFile(cf.GetPathName());
	if(0 != iRet)
	{
		if(-3 == iRet)
		{
			strTemp = theApp.m_pLangInfo->GetText(252);
		}
		else
		{
			strTemp = theApp.m_pLangInfo->GetText(253);
		}

		PopWarningTipInfo(strTemp);
		delete pExperimentData;

		return -4;
	}
	pExperimentData->SetFileName(cf.GetFileTitle());

#ifdef ORDINARY_USER_CHECK_FILEUSERNAME
	//如果当前是普通用户权限，文件里的用户名不为空，检查用户名是否一致，不一致的话不允许打开
	CString strFileUserName = pExperimentData->GetUserName();
	if(!strFileUserName.IsEmpty() && theApp.GetBeOrdinaryPermission() && strFileUserName != theApp.m_curUserInfo.GetName())
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(377));
		delete pExperimentData;

		return -5;
	}
#endif

	//如果打开的是模板文件，设置实验文件保存文件夹为模板文件所在文件夹，20220511，孙文郎
	if(cf.GetFileExt() == PCR_TEMPLATE_FILE_SUFFIX)
	{
		//pExperimentData->SetExpFileSaveDirectory(cf.GetFolderPath());
		pExperimentData->SetBeTemplateFile(TRUE);
	}

	CString strSgementChs = theApp.m_pLangInfo->GetText(57, LANGUAGE_CHS);
	CString strSegmentEnu = theApp.m_pLangInfo->GetText(57, LANGUAGE_ENG);
	if (theApp.m_pLangInfo->GetLanguage() == LANGUAGE_CHS)
	{
		pExperimentData->CorrectSegmentNameByLanguage(strSegmentEnu, strSgementChs);
	}
	else
	{
		pExperimentData->CorrectSegmentNameByLanguage(strSgementChs, strSegmentEnu);
	}


	m_fileManageDlg.AddNewPage(pExperimentData,cf.GetFileTitle());	//这个函数已经调用了SetBaseInfo
	m_fileManageDlg.SetCurrentSelectPageString(cf.GetFileTitle(),cf.GetPathName());

	m_pCurSelPageDlg->FileOpened();

	//如果是从主页打开，跳转到工作页面
	//OnMsgSwitchToWorkPage(0,0);

	//实验正常打开后，如果已经存在历史文件信息列表中，则更新最近打开时间，否则保存文件信息到主页历史文件信息列表中

#ifdef PS96INSTRUMENT
	//如果打开的是模板文件，更新模板文件列表
	strTemp = _T(".") + PCR_TEMPLATE_FILE_SUFFIX;
	if(theApp.GetBeIVDUser() && -1 != cf.GetPathName().Find(strTemp))
	{
		int iLstIndex = -1;
		if(!theApp.IsTemplateExist(cf.GetFileTitle(),iLstIndex))
		{
			//新的模板文件被打开，先保存到内存中的模板文件链表中，再设置模板文件列表选中行
			CTemplateFileInfo temFileInfo;
			temFileInfo.m_strFileName = cf.GetFileName();
			temFileInfo.m_strFilePath = cf.GetPathName();
			temFileInfo.m_strTitle = cf.GetFileTitle();
			if(0 == theApp.AddNewTemplate(temFileInfo,iLstIndex))
			{
				//新的模板文件已经添加到模板文件列表中，软件已经打开的实验页面模板文件列表框分别添加对应行
				NewTemplateAdded(iLstIndex);
				//模板文件已经存在于模板文件列表中，设置模板文件列表选中行
				m_pCurSelPageDlg->SetTemplateListSelectedRow(iLstIndex);
			}
		}
		else
		{
			//模板文件已经存在于模板文件列表中，设置模板文件列表选中行
			m_pCurSelPageDlg->SetTemplateListSelectedRow(iLstIndex);
		}
	}
#endif
	
	return 0;
}

int CWorkDlg::OpenPowerOffProtectFile(CString strFilePath)
{
	CString strTemp(_T(""));

	//一控多时，需要判断文件是否是打开状态（可能断电后没有关闭软件或者实验文件）
	
	CExperimentData* pExperimentData = new CExperimentData;
	if (NULL == pExperimentData)
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(251));
		return -3;
	}

	int iRet = pExperimentData->OpenFile(strFilePath);
	if(0 != iRet)
	{
		if(-3 == iRet)
		{
			strTemp = theApp.m_pLangInfo->GetText(252);
		}
		else
		{
			strTemp = theApp.m_pLangInfo->GetText(253);
		}

		PopWarningTipInfo(strTemp);
		delete pExperimentData;

		return -4;
	}
	
	//熔曲步骤运行断电时，如果继续运行该程序，从熔曲所在程序段第一个步骤开始运行
	pExperimentData->DeleteMeltData();

	CString strFileTile = GetFileNameFromPath(strFilePath);
	pExperimentData->SetFileName(strFileTile);

	m_fileManageDlg.AddNewPage(pExperimentData,strFileTile);	//这个函数已经调用了SetBaseInfo
	m_fileManageDlg.SetCurrentSelectPageString(strFileTile,strFilePath);

	m_pCurSelPageDlg->FileOpened();
	//切换到监控页面
	m_pCurSelPageDlg->SetPageSelected(ESPT_RUNMONITOR);

	return 0;
}

int CWorkDlg::OpenOriginalData()
{
	CString strTemp(_T(""));

	if(m_fileManageDlg.m_lnkPageSwitchBtPtr.GetSize() >= MAX_FILEOPEN_NUM)
	{
		CString strFormat = theApp.m_pLangInfo->GetText(159);
		strTemp.Format(strFormat,MAX_FILEOPEN_NUM);
		PopNormalTipInfo(strTemp);
		return -1;
	}

	CFileDialog cf(TRUE,_T(".xlsx"), NULL,OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT,_T("Excel File(*.xlsx);(*.xlsx)|*.xlsx;*.xlsx|"),this);
	if(IDOK != cf.DoModal())
	{
		return 2;
	}

	CString strTitle = cf.GetFileTitle();
	if(strTitle.Find(_T("True")) < 0)
	{
		PopWarningTipInfo(_T("请选择正确的Excel文件！"));
		return -3;
	}

	int iTubeNum = 16,iChanNum = 4,iLineNum = iTubeNum * iChanNum;

	vector<double>* pVecData = new vector<double>[iLineNum];
	if(NULL == pVecData)
	{
		return -4;
	}

	CExperimentData expData;
	if(0 != expData.ReadOriginalDataFromExcel(cf.GetPathName(),pVecData))
	{
		PopWarningTipInfo(_T("文件加载失败！"));
		return -5;
	}

	int iDataLen = 0;
	for(int i = 0;i < iLineNum;++i)
	{
		if(pVecData[i].size() > 0)
		{
			iDataLen = pVecData[i].size();
			break;
		}
	}
	if(iDataLen > 0)
	{
		for(int i = 0;i < iLineNum;++i)
		{
			if(pVecData[i].size() < 1)
			{
				for(int j = 0;j < iDataLen;++j)
				{
					pVecData[i].push_back(100);
				}
			}
		}
	}

	m_fileManageDlg.AddPage();

	CExperimentData* pExpData = m_pCurSelPageDlg->m_pExpData;
	if(NULL == pExpData)
	{
		delete [] pVecData;
		return -6;
	}

	if(0 != pExpData->SetTubeData(pVecData,iTubeNum,iChanNum))
	{
		delete [] pVecData;
		return -7;
	}

	//设置样本信息，方便立马看到数据曲线
	pExpData->SetTubeInfoToBeAllProcessed();

	delete [] pVecData;

	m_pCurSelPageDlg->FileOpened();

	return 0;
}

int CWorkDlg::SavePrj(int iIndex,BOOL bNew)
{
	if(NULL == m_pCurSelPageDlg->GetSafeHwnd())
	{
		return -1;
	}

	int iExperNum = m_lnkPageDlgPtr.GetSize();
	for(int i = 0;i < iExperNum;++i)
	{
		m_lnkPageDlgPtr.GetAt(i)->data->SaveProject(iIndex, bNew);
	}

	return 0;
}

int CWorkDlg::DeletePrj(int iIndex)
{
	if(NULL == m_pCurSelPageDlg->GetSafeHwnd())
	{
		return -1;
	}

	int iExperNum = m_lnkPageDlgPtr.GetSize();
	for(int i = 0;i < iExperNum;++i)
	{
		(*m_lnkPageDlgPtr[i])->DeleteProject(iIndex);
	}

	return 0;
}

CMiddleDlg* CWorkDlg::GetMiddleDlg(int iPageId)
{
	int iExperNum = m_lnkPageDlgPtr.GetSize();
	for(int i = 0;i < iExperNum;++i)
	{
		if((*m_lnkPageDlgPtr[i])->GetPageId() == iPageId)
		{
			return (*m_lnkPageDlgPtr[i]);
		}
	}

	return NULL;
}

int CWorkDlg::Delete()
{
	if(NULL == m_pCurSelPageDlg->GetSafeHwnd())
	{
		return -1;
	}

	m_pCurSelPageDlg->Delete();
	return 0;
}

int CWorkDlg::NewTemplateAdded(int iIndex)
{
	Node<CMiddleDlg*>* pNodePageDlgPtr = m_lnkPageDlgPtr.GetHead();
	while(NULL != pNodePageDlgPtr)
	{
		if(NULL != pNodePageDlgPtr->data)
		{
			pNodePageDlgPtr->data->NewTemplateAdded(iIndex);
		}

		pNodePageDlgPtr = pNodePageDlgPtr->pNext;
	}

	return 0;
}

int CWorkDlg::GetCurrentPageId()
{
	if(NULL != m_pCurSelPageDlg->GetSafeHwnd())
	{
		return m_pCurSelPageDlg->GetPageId();
	}
	return -1;
}

BOOL CWorkDlg::IsRunningProcessExist()
{
	Node<CMiddleDlg*>* pNode = m_lnkPageDlgPtr.GetHead();
	while(NULL != pNode)
	{
		if(NULL != pNode->data->GetSafeHwnd() && pNode->data->IsExpRunning())
		{
			return TRUE;
		}

		pNode = pNode->pNext;
	}

	return FALSE;
}

int CWorkDlg::RefreshLanguageDisplay()
{
	Node<CMiddleDlg*>* pNode = m_lnkPageDlgPtr.GetHead();
	while(NULL != pNode)
	{
		if(NULL != pNode->data->GetSafeHwnd())
		{
			pNode->data->RefreshLanguageDisplay();
		}

		pNode = pNode->pNext;
	}

	return 0;
}

int CWorkDlg::SetDefaultGeneName()
{
	Node<CMiddleDlg*>* pNode = m_lnkPageDlgPtr.GetHead();
	while(NULL != pNode)
	{
		if(NULL != pNode->data->GetSafeHwnd())
		{
			pNode->data->SetDefaultGeneName();
		}

		pNode = pNode->pNext;
	}

	return 0;
}

void CWorkDlg::SetInstrumentConnectState(CString strModel, CString strSN, BOOL bConnected, BOOL bReshowList)
{
	Node<CMiddleDlg*>* pNode = m_lnkPageDlgPtr.GetHead();
	while(NULL != pNode)
	{
		if(NULL != pNode->data->GetSafeHwnd())
		{
			if (bReshowList)
			{
				pNode->data->ReshowInstrumentList();
				pNode->data->SetSelectedInstrument(strModel, strSN);
			}
			pNode->data->SetInstrumentConnectState(strModel, strSN, bConnected);
		}

		pNode = pNode->pNext;
	}
}


LRESULT CWorkDlg::OnMsgInstrumentReinitialize(WPARAM wParam,LPARAM lParam)
{
	Node<CMiddleDlg*>* pNode = m_lnkPageDlgPtr.GetHead();
	while(NULL != pNode)
	{
		if (pNode->data == m_pCurSelPageDlg)
		{
			pNode = pNode->pNext;
			continue;
		}

		if(NULL != pNode->data->GetSafeHwnd())
		{
			pNode->data->ReshowInstrumentList();
		}

		pNode = pNode->pNext;
	}

	return 0;
}
