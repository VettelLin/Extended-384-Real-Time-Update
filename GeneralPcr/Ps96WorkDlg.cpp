#include "StdAfx.h"
#include "Ps96WorkDlg.h"
#include "GeneralPcr.h"


CPs96WorkDlg::CPs96WorkDlg(void): m_ps96FileManagerDlg(this)
{

}

CPs96WorkDlg::~CPs96WorkDlg(void)
{

}

BEGIN_MESSAGE_MAP(CPs96WorkDlg,CWorkDlg)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CPs96WorkDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect; 
	GetClientRect(&rect);

	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	memDc.FillSolidRect(0,rect.bottom - theApp.m_iFileManageDlgH,rect.Width(),theApp.m_iFileManageDlgH,CLR_SOFT_LIGHT);

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

int CPs96WorkDlg::SetPageSelected(int iPageId)
{
	if(iPageId < 0)
	{
		return -1;
	}

	if(iPageId > CMyPageSwitchBt::PageCount)
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
		if(iPageId == pPageDlg->GetPageId())
		{
			pPageDlg->ShowWindow(SW_SHOW);
			m_pCurSelPageDlg = pPageDlg;
			break;
		}
	}

	return 0;
}

void CPs96WorkDlg::InitFileManageDlg()
{
	m_ps96FileManagerDlg.Create(IDD_MIDDLEDLG,this);
	m_ps96FileManagerDlg.ShowWindow(SW_SHOW);
}

int CPs96WorkDlg::OpenFile(int iFileFormat)
{
	CString strTemp(_T(""));

	if(m_ps96FileManagerDlg.m_lnkPageSwitchBtPtr.GetSize() >= MAX_FILEOPEN_NUM)
	{
		CString strFormat = theApp.m_pLangInfo->GetText(159);
		strTemp.Format(strFormat,MAX_FILEOPEN_NUM);
		PopNormalTipInfo(strTemp);
		return -1;
	}

	CString strOpenFormat(_T("")), strSuffix(_T(""));
	strSuffix = PCR_DATA_FILE_SUFFIX;

	if(1 == iFileFormat)
	{
		strOpenFormat.Format(_T("QPCR File(*.%s)|*.%s||"), PCR_DATA_FILE_SUFFIX,PCR_DATA_FILE_SUFFIX);
	}
	else if(2 == iFileFormat)
	{
		strOpenFormat.Format(_T("QPCR File(*.%s)|*.%s||"), PCR_TEMPLATE_FILE_SUFFIX,PCR_TEMPLATE_FILE_SUFFIX);
		strSuffix = PCR_TEMPLATE_FILE_SUFFIX;;
	}
	else
	{
		strOpenFormat.Format(_T("QPCR File(*.%s);(*.%s)|*.%s;*.%s|All File(*.*)|*.*||"), PCR_DATA_FILE_SUFFIX,PCR_TEMPLATE_FILE_SUFFIX,PCR_DATA_FILE_SUFFIX,PCR_TEMPLATE_FILE_SUFFIX);
	}
	
	CFileDialog cf(TRUE,strSuffix,NULL,OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT,strOpenFormat, this);
	if(IDOK != cf.DoModal())
	{
		return 1;
	}

	//判断该路径标识的文件是否已经打开
	if(m_ps96FileManagerDlg.CheckFileExistOrNot(cf.GetPathName()))
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(160));
		return -2;
	}

	if(m_ps96FileManagerDlg.CheckFileNameExistOrNot(cf.GetFileTitle()))
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
		pExperimentData->SetExpFileSaveDirectory(cf.GetFolderPath());
		pExperimentData->SetBeTemplateFile(TRUE);
	}

	m_ps96FileManagerDlg.AddNewPage(pExperimentData,cf.GetFileTitle());	//这个函数已经调用了SetBaseInfo
	m_ps96FileManagerDlg.SetCurrentSelectPageString(cf.GetFileTitle(),cf.GetPathName());

	m_pCurSelPageDlg->RefreshExpEmbody();

#ifdef PS96INSTRUMENT
	//如果打开的是模板文件，更新模板文件列表
	strTemp = _T(".") + PCR_TEMPLATE_FILE_SUFFIX;
	if(-1 != cf.GetPathName().Find(strTemp))
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

int CPs96WorkDlg::OpenPowerOffProtectFile(CString strFilePath)
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

	m_ps96FileManagerDlg.AddNewPage(pExperimentData,strFileTile);	//这个函数已经调用了SetBaseInfo
	m_ps96FileManagerDlg.SetCurrentSelectPageString(strFileTile,strFilePath);

	m_pCurSelPageDlg->RefreshExpEmbody();

	return 0;
}

void CPs96WorkDlg::CreateCtrl()
{
	CreateSt();
}

void CPs96WorkDlg::InitCtrl()
{
	InitSt();
}

void CPs96WorkDlg::SetCtrlPos()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	m_rcManage.m_rcInit = rcClient;
	m_rcManage.ClearRectToDelete();

	int iStartX = 60 * m_dHoriRatio;
	CRect rect;
	rect.left = iStartX;
	rect.right = rcClient.right;
	rect.bottom = rcClient.bottom;
	rect.top = rect.bottom - theApp.m_iFileManageDlgH;

	if(NULL != m_ps96FileManagerDlg.GetSafeHwnd())
	{
		m_ps96FileManagerDlg.SetWindowPos(this,rect.left,rect.top,rect.Width(),rect.Height(),SWP_NOZORDER);
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

void CPs96WorkDlg::ShowCtrl()
{
	
}

void CPs96WorkDlg::CreateSt()
{
	
}

void CPs96WorkDlg::InitSt()
{
	
}

int CPs96WorkDlg::DeleteTemplate(int iIndex)
{
	theApp.DeleteTemplate(iIndex);

	CMiddleDlg* pPageDlg = NULL;
	for(int i = 0;i < m_lnkPageDlgPtr.GetSize();++i)
	{
		pPageDlg = *m_lnkPageDlgPtr[i];
		pPageDlg->DeleteTemplateListRow(iIndex);
	}

	return 0;
}

int CPs96WorkDlg::RefreshExpDlgRunState(int iPageId,BOOL bOccupy)
{
	CMiddleDlg* pPageDlg = NULL;
	for(int i = 0;i < m_lnkPageDlgPtr.GetSize();++i)
	{
		pPageDlg = *m_lnkPageDlgPtr[i];
		if(NULL == pPageDlg->GetSafeHwnd())
		{
			continue;
		}
		
		if(iPageId == pPageDlg->GetPageId())
		{
			pPageDlg->RefreshExpDlgRunState(bOccupy);
		}
		else
		{
			pPageDlg->RefreshExpDlgRunState(FALSE);
		}
	}

	return 0;
}

