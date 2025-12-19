#include "StdAfx.h"
#include "ReagentItemManager.h"
#include "PublicFun.h"
#include "LanguageInfo.h"

CReagentItemManager* CReagentItemManager::m_pManager = NULL;

CReagentItemManager::CReagentItemManager(void)
{
	m_strProjectPath.Empty();
	m_bItemChanged = FALSE;
	m_iNameIndex = 0;
}

CReagentItemManager::~CReagentItemManager(void)
{
	DeleteAll();
}

CReagentItemManager* CReagentItemManager::GetInstance()
{
	if(m_pManager == NULL)
	{
		m_pManager = new CReagentItemManager;
	}

	return m_pManager;
}

void CReagentItemManager::DestroyInstance()
{
	if(m_pManager != NULL)
	{
		if (m_pManager->GetItemChanged())
		{
			// 提示用户是否需要保存修改过的项目信息
			m_pManager->OutputItems(NULL);
		}
		delete m_pManager;
	}
	m_pManager = NULL;
}

void CReagentItemManager::DeleteAll()
{
	int iCount = m_arrayItems.GetCount();
	for (int i=0; i<iCount; i++)
	{
		CReagentItem* pItem = (CReagentItem*)m_arrayItems.GetAt(i);
		SAFE_DELETE(pItem);
	}
	m_arrayItems.RemoveAll();
}


void CReagentItemManager::Initialize(CString strUserPath, CString strStartupPath)
{
	m_strProjectPath = strUserPath + _T("\\Project");
	CFileFind finder;
	if (!finder.FindFile(m_strProjectPath)) // 如果没有ExpData路径
	{
		::CreateDirectory(m_strProjectPath, NULL);
	}

	InitializeFromPath(m_strProjectPath);

	if (m_arrayItems.GetCount() == 0)
	{
		// 读取系统安装默认的项目
		CString strPath = strStartupPath + _T("Users\\System\\Project");
		InitializeFromPath(strPath);
	}

	FindNameIndex();
}

void CReagentItemManager::InitializeFromPath(CString strPath)
{
	DeleteAll();

	CReagentItem* pItem = NULL;
	CFileFind filefind;
	BOOL bFind = filefind.FindFile(strPath + _T("\\*.qprj"));
	while(bFind)
	{
		bFind = filefind.FindNextFile();
		//CString l_strFileTitle = filefind.GetFileTitle();
		CString strFilePath = filefind.GetFilePath();
		if (pItem == NULL)
		{
			pItem = new CReagentItem;
		}
		if (pItem->OpenFile(strFilePath))
		{
			m_arrayItems.Add(pItem);
			pItem = NULL;
		}
	}

	SAFE_DELETE(pItem);

}

// 选择需要导入的项目文件
void CReagentItemManager::InputItems()
{
	CFileDialog dlgFile(TRUE,_T(".qprj"), NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT ,_T("RtPcr Project File(*.qprj)|*.qprj|All File(*.*)|*.*||"));
	if (dlgFile.DoModal() == IDOK)
	{
		CString strFilename = dlgFile.GetPathName();
		CString strFileTitle = dlgFile.GetFileTitle();

		CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
		WORD wLangId = MAKELANGID(LANG_CHINESE_SIMPLIFIED,SUBLANG_CHINESE_SIMPLIFIED);
		if(pLangInfo->GetBeEnglish())
		{
			wLangId = MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US);
		}
		HWND hWnd = NULL;
		CWnd* pMainWnd = AfxGetMainWnd();
		if(NULL != pMainWnd)
		{
			hWnd = pMainWnd->m_hWnd;
		}

		CReagentItem* pItem = FindItemBy(strFileTitle);
		if (pItem != NULL)
		{
			if(IDNO == MessageBoxExW(hWnd,pLangInfo->GetText(265),_T("GeneralPcr"),MB_ICONINFORMATION | MB_YESNO,wLangId))
			{
				return;
			}
		}
		else
		{
			pItem = new CReagentItem;
			m_arrayItems.Add(pItem);
		}

		BOOL bReturn = pItem->OpenFile(strFilename);
		if (bReturn)
		{
			MessageBoxExW(hWnd,pLangInfo->GetText(263),_T("GeneralPcr"),MB_ICONINFORMATION | MB_OK,wLangId);
		}
		else
		{
			MessageBoxExW(hWnd,pLangInfo->GetText(264),_T("GeneralPcr"),MB_ICONINFORMATION | MB_OK,wLangId);
		}
	}

	FindNameIndex();
}

// 保存指定项目信息，如果路径为空，则保存到项目目录下，如果指针为空，则保存全部项目
void CReagentItemManager::OutputItems(CString strSavePath, CStringArray* pArrayName)
{
	BOOL bSave = FALSE;
	if (strSavePath.IsEmpty())
	{
		strSavePath = m_strProjectPath;
	}

	if (pArrayName == NULL)
	{
		int iCount = m_arrayItems.GetCount();
		for (int i=0; i<iCount; i++)
		{
			CReagentItem* pItem = (CReagentItem*)m_arrayItems.GetAt(i);
			if (pItem != NULL)
			{
				CString strPathName = CreateFileNameByName(strSavePath, pItem->GetName());
				bSave = pItem->SaveFile(strPathName);
				if (!bSave)
					break;
			}
		}
	}
	else
	{
		int iCount = pArrayName->GetCount();
		for(int i=0; i<iCount; i++)
		{
			CString strProject = pArrayName->GetAt(i);
			CReagentItem* pItem = FindItemBy(strProject);
			if (pItem != NULL)
			{
				CString strPathName = CreateFileNameByName(strSavePath, strProject);
				bSave = pItem->SaveFile(strPathName);
				if (!bSave)
					break;
			}
		}
	}

	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	WORD wLangId = MAKELANGID(LANG_CHINESE_SIMPLIFIED,SUBLANG_CHINESE_SIMPLIFIED);
	if(pLangInfo->GetBeEnglish())
	{
		wLangId = MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US);
	}
	HWND hWnd = NULL;
	CWnd* pMainWnd = AfxGetMainWnd();
	if(NULL != pMainWnd)
	{
		hWnd = pMainWnd->m_hWnd;
	}

	if (bSave)
	{
		MessageBoxExW(hWnd,pLangInfo->GetText(261),_T("GeneralPcr"),MB_ICONINFORMATION | MB_OK,wLangId);
	}
	else
	{
		MessageBoxExW(hWnd,pLangInfo->GetText(262),_T("GeneralPcr"),MB_ICONINFORMATION | MB_OK,wLangId);
	}
}

CReagentItem* CReagentItemManager::FindItemBy(CString strName)
{
	CReagentItem* pReturn = NULL;
	int iCount = m_arrayItems.GetCount();
	for (int i=0; i<iCount; i++)
	{
		CReagentItem* pItem = (CReagentItem*)m_arrayItems.GetAt(i);
		if (pItem->GetName() == strName)
		{
			pReturn = pItem;
			break;
		}
	}

	return pReturn;
}

// 根据项目名称删除一个项目，同时删除文件
BOOL CReagentItemManager::DeleteItem(CString strName)
{
	int iFindIndex = -1;
	int iCount = m_arrayItems.GetCount();
	for (int i=0; i<iCount; i++)
	{
		CReagentItem* pItem = (CReagentItem*)m_arrayItems.GetAt(i);
		if (pItem->GetName() == strName)
		{
			iFindIndex = i;
			break;
		}
	}

	if (iFindIndex != -1)
	{
		return DeleteItem(iFindIndex);
	}
	return FALSE;
}

BOOL CReagentItemManager::DeleteItem(int iIndex)
{
	CReagentItem* pItem = (CReagentItem*)m_arrayItems.GetAt(iIndex);

	if (pItem->GetStatus() != CReagentItem::STATUS_NEW)
	{
		CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
		WORD wLangId = MAKELANGID(LANG_CHINESE_SIMPLIFIED,SUBLANG_CHINESE_SIMPLIFIED);
		if(pLangInfo->GetBeEnglish())
		{
			wLangId = MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US);
		}
		HWND hWnd = NULL;
		CWnd* pMainWnd = AfxGetMainWnd();
		if(NULL != pMainWnd)
		{
			hWnd = pMainWnd->m_hWnd;
		}

		CString strFilePath = CreateFileNameByName(m_strProjectPath, pItem->GetName());
		BOOL bSuccess = ::DeleteFile(strFilePath);
		if (bSuccess)
		{
			SAFE_DELETE(pItem);
			m_arrayItems.RemoveAt(iIndex);
			FindNameIndex();
			MessageBoxExW(hWnd,pLangInfo->GetText(266),_T("GeneralPcr"),MB_ICONINFORMATION | MB_OK,wLangId);
			return TRUE;
		}
		else
		{
			MessageBoxExW(hWnd,pLangInfo->GetText(267),_T("GeneralPcr"),MB_ICONINFORMATION | MB_OK,wLangId);
			return FALSE;
		}
	}
	else
	{
		SAFE_DELETE(pItem);
		m_arrayItems.RemoveAt(iIndex);
		FindNameIndex();
		return TRUE;
	}
}

BOOL CReagentItemManager::CanDelete(int iIndex)
{
	if (iIndex >=0 && iIndex < m_arrayItems.GetCount())
	{
		CReagentItem* pItem = (CReagentItem*)m_arrayItems.GetAt(iIndex);

		return pItem->GetStatus() != CReagentItem::STATUS_USING;
	}

	return FALSE;
}

CString CReagentItemManager::CreateFileNameByName(CString strPath, CString strName)
{
	CString strFileName;
	strFileName.Format(_T("%s.qprj"), strName);
	CString strPathName = strPath + _T("\\") + strFileName;
	return strPathName;
}

CReagentItem* CReagentItemManager::NewOneItem()
{
	CReagentItem* pItem = new CReagentItem;
	m_iNameIndex ++;
	CString strName;
	strName.Format(_T("Project%d"), m_iNameIndex);
	pItem->SetName(strName);

	return pItem;
}

BOOL CReagentItemManager::GetBeNewPrj(int iIndex)
{
	if(iIndex < 0 || iIndex >= m_arrayItems.GetCount())
	{
		return FALSE;
	}

	CReagentItem* pItem = (CReagentItem*)m_arrayItems.GetAt(iIndex);
	if(NULL == pItem)
	{
		return TRUE;
	}

	return CReagentItem::STATUS_NEW == pItem->GetStatus();
}

void CReagentItemManager::FindNameIndex()
{
	m_iNameIndex = 0;

	int iCount = m_arrayItems.GetCount();
	for (int i=0; i<iCount; i++)
	{
		CReagentItem* pItem = (CReagentItem*)m_arrayItems.GetAt(i);
		if (pItem != NULL)
		{
			CString strName = pItem->GetName();
			int iFind = strName.Find(_T("Project"));
			if (iFind != -1)
			{
				CString strIndex = strName.Right(strName.GetLength() - iFind - 7);
				int iTemp = _ttoi(strIndex);
				if (iTemp > m_iNameIndex)
				{
					m_iNameIndex = iTemp;
				}
			}
		}
	}
}
