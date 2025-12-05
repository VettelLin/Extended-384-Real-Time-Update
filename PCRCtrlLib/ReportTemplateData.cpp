#include "StdAfx.h"
#include "ReportTemplateData.h"
#include "PublicFun.h"
#include "DeclaresPCR.h"

CReportTemplateData::CReportTemplateData(void)
{
	m_wFileFlag = wFileReportTemplateMarkPCR;
	m_wFileVersion = wPCRVersion100;

	m_strFilePath.Empty();
	m_iUsedIndex = -1;
	m_bChanged = FALSE;
}


CReportTemplateData::~CReportTemplateData(void)
{
	DeleteAll();
}

void CReportTemplateData::DeleteAll()
{
	for (int i=0; i<m_arrayTemplate.GetCount(); i++)
	{
		tagTemplate* pTemplate = (tagTemplate*)m_arrayTemplate.GetAt(i);
		SAFE_DELETE(pTemplate);
	}

	m_arrayTemplate.RemoveAll();
	m_iUsedIndex = -1;
}



void CReportTemplateData::SetStartupPath(CString strPath)
{
	m_strFilePath = strPath + _T("AtilaReportTemplate") + PCR_REPORT_TEMPLATE_FILE_SUFFIX_DOT;
}

BOOL CReportTemplateData::SaveFile()
{
	return SaveFile(m_strFilePath);
}

BOOL CReportTemplateData::OpenFile()
{
	return OpenFile(m_strFilePath);
}

BOOL CReportTemplateData::SaveFile(CString strFilePath)
{
	CFile file;       
	CFileException e;
	if (!file.Open(strFilePath,CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &e))
	{
		return FALSE;
	}

	// 写入文件标识和版本
	file.Write((const char*)&m_wFileFlag, sizeof(WORD));
	file.Write((const char*)&m_wFileVersion, sizeof(WORD));

	file.Write((const char*)&m_iUsedIndex, sizeof(int));

	int iTemplateCount = m_arrayTemplate.GetCount();
	file.Write((const char*)&iTemplateCount, sizeof(int));
	for(int i=0; i<iTemplateCount; i++)
	{
		tagTemplate* pTemplate = (tagTemplate*)m_arrayTemplate.GetAt(i);
		CPublicFun::WriteStringToFile(&file, pTemplate->strName);
		CPublicFun::WriteStringToFile(&file, pTemplate->strLabName);
		CPublicFun::WriteStringToFile(&file, pTemplate->strLabAddress);
		CPublicFun::WriteStringToFile(&file, pTemplate->strContactUrl);
		CPublicFun::WriteStringToFile(&file, pTemplate->strContactEmail);
		CPublicFun::WriteStringToFile(&file, pTemplate->strContactPhone);
		CPublicFun::WriteStringToFile(&file, pTemplate->strTestName);
		CPublicFun::WriteStringToFile(&file, pTemplate->strDisclaimer);
	}

	return TRUE;
}


BOOL CReportTemplateData::OpenFile(CString strFilePath)
{
	CFile file;       
	if (!file.Open(strFilePath,CFile::modeRead | CFile::shareDenyNone))
	{
		return FALSE;
	}

	// 读取文件标识和版本
	WORD wFileMark = 0;
	file.Read(&wFileMark, sizeof(WORD));
	if (wFileMark != wFileReportTemplateMarkPCR)
	{
		file.Close();
		return FALSE;
	}

	WORD wFileVersion = 0;
	file.Read(&wFileVersion, sizeof(WORD));

	file.Read(&m_iUsedIndex, sizeof(int));

	int iTemplateCount = 0;
	file.Read(&iTemplateCount, sizeof(int));
	for(int i=0; i<iTemplateCount; i++)
	{
		tagTemplate* pTemplate = new tagTemplate;
		pTemplate->strName = CPublicFun::ReadStringFromFile(&file);
		pTemplate->strLabName = CPublicFun::ReadStringFromFile(&file);
		pTemplate->strLabAddress = CPublicFun::ReadStringFromFile(&file);
		pTemplate->strContactUrl = CPublicFun::ReadStringFromFile(&file);
		pTemplate->strContactEmail = CPublicFun::ReadStringFromFile(&file);
		pTemplate->strContactPhone = CPublicFun::ReadStringFromFile(&file);
		pTemplate->strTestName = CPublicFun::ReadStringFromFile(&file);
		pTemplate->strDisclaimer = CPublicFun::ReadStringFromFile(&file);

		m_arrayTemplate.Add(pTemplate);
	}

	file.Close();
	return TRUE;
}

int CReportTemplateData::AddTemplate(eDataFlag nFlag, CString string)
{
	tagTemplate* pTemplate = new tagTemplate;
	SetStringTo(pTemplate, nFlag, string);
	return m_arrayTemplate.Add(pTemplate);
}

void CReportTemplateData::DeleteTemplate(int iIndex)
{
	if (iIndex == m_iUsedIndex)
	{
		tagTemplate* pTemplate = (tagTemplate*)m_arrayTemplate.GetAt(iIndex);
		SAFE_DELETE(pTemplate);
		m_arrayTemplate.RemoveAt(iIndex);
		if (m_iUsedIndex >= m_arrayTemplate.GetCount())
			m_iUsedIndex --;
	}
	else if (iIndex < m_iUsedIndex)
	{
		tagTemplate* pTemplate = (tagTemplate*)m_arrayTemplate.GetAt(iIndex);
		SAFE_DELETE(pTemplate);
		m_arrayTemplate.RemoveAt(iIndex);
		m_iUsedIndex --;
	}
	else
	{
		tagTemplate* pTemplate = (tagTemplate*)m_arrayTemplate.GetAt(iIndex);
		SAFE_DELETE(pTemplate);
		m_arrayTemplate.RemoveAt(iIndex);
	}
}

CString CReportTemplateData::GetStringBy(eDataFlag nFlag, int iIndex)
{
	int iCurIndex = iIndex;
	if (iCurIndex == -1)
	{
		iCurIndex = m_iUsedIndex;
		if (iCurIndex < 0)
			iCurIndex = 0;
	}
	tagTemplate* pTemplate = (tagTemplate*)m_arrayTemplate.GetAt(iCurIndex);
	return GetStringBy(pTemplate, nFlag);
}

void CReportTemplateData::SetStringTo(eDataFlag nFlag, CString string, int iIndex)
{
	int iCurIndex = iIndex;
	if (iCurIndex == -1)
	{
		iCurIndex = m_iUsedIndex;
		if (iCurIndex < 0)
			iCurIndex = 0;
	}
	tagTemplate* pTemplate = (tagTemplate*)m_arrayTemplate.GetAt(iCurIndex);
	SetStringTo(pTemplate, nFlag, string);
}

CString CReportTemplateData::GetStringBy(tagTemplate* pTemplate, eDataFlag nFlag)
{
	CString strReturn;
	strReturn.Empty();

	switch(nFlag)
	{
	case DATA_NAME:
		strReturn = pTemplate->strName;
		break;
	case DATA_LAB_NAME:
		strReturn = pTemplate->strLabName;
		break;
	case DATA_ADDRESS:
		strReturn = pTemplate->strLabAddress;
		break;
	case DATA_CONTACT_URL:
		strReturn = pTemplate->strContactUrl;
		break;
	case DATA_CONTACT_EMAIL:
		strReturn = pTemplate->strContactEmail;
		break;
	case DATA_CONTACT_PHONE:
		strReturn = pTemplate->strContactPhone;
		break;
	case DATA_TEST_NAME:
		strReturn = pTemplate->strTestName;
		break;
	case DATA_DISCLAIMER:
		strReturn = pTemplate->strDisclaimer;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return strReturn;
}

void CReportTemplateData::SetStringTo(tagTemplate* pTemplate, eDataFlag nFlag, CString string)
{
	switch(nFlag)
	{
	case DATA_NAME:
		pTemplate->strName = string;
		break;
	case DATA_LAB_NAME:
		pTemplate->strLabName = string;
		break;
	case DATA_ADDRESS:
		pTemplate->strLabAddress = string;
		break;
	case DATA_CONTACT_URL:
		pTemplate->strContactUrl = string;
		break;
	case DATA_CONTACT_EMAIL:
		pTemplate->strContactEmail = string;
		break;
	case DATA_CONTACT_PHONE:
		pTemplate->strContactPhone = string;
		break;
	case DATA_TEST_NAME:
		pTemplate->strTestName = string;
		break;
	case DATA_DISCLAIMER:
		pTemplate->strDisclaimer = string;
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void CReportTemplateData::CopyTo(CReportTemplateData* pTargetData)
{
	pTargetData->DeleteAll();

	for (int i=0; i<m_arrayTemplate.GetCount(); i++)
	{
		tagTemplate* pTemplate = (tagTemplate*)m_arrayTemplate.GetAt(i);
		int iIndex = pTargetData->AddTemplate(DATA_NAME, pTemplate->strName);
		pTargetData->SetStringTo(DATA_LAB_NAME, pTemplate->strLabName, iIndex);
		pTargetData->SetStringTo(DATA_ADDRESS, pTemplate->strLabAddress, iIndex);
		pTargetData->SetStringTo(DATA_CONTACT_URL, pTemplate->strContactUrl, iIndex);
		pTargetData->SetStringTo(DATA_CONTACT_EMAIL, pTemplate->strContactEmail, iIndex);
		pTargetData->SetStringTo(DATA_CONTACT_PHONE, pTemplate->strContactPhone, iIndex);
		pTargetData->SetStringTo(DATA_TEST_NAME, pTemplate->strTestName, iIndex);
		pTargetData->SetStringTo(DATA_DISCLAIMER, pTemplate->strDisclaimer, iIndex);
	}
}

void CReportTemplateData::CopyFrom(CReportTemplateData* pSourceData)
{
	int iOldSel = m_iUsedIndex;
	DeleteAll();

	for (int i=0; i<pSourceData->GetTemplateCount(); i++)
	{
		int iIndex = AddTemplate(DATA_NAME, pSourceData->GetStringBy(DATA_NAME, i));
		SetStringTo(DATA_LAB_NAME, pSourceData->GetStringBy(DATA_LAB_NAME, i), iIndex);
		SetStringTo(DATA_ADDRESS, pSourceData->GetStringBy(DATA_ADDRESS, i), iIndex);
		SetStringTo(DATA_CONTACT_URL, pSourceData->GetStringBy(DATA_CONTACT_URL, i), iIndex);
		SetStringTo(DATA_CONTACT_EMAIL, pSourceData->GetStringBy(DATA_CONTACT_EMAIL, i), iIndex);
		SetStringTo(DATA_CONTACT_PHONE, pSourceData->GetStringBy(DATA_CONTACT_PHONE, i), iIndex);
		SetStringTo(DATA_TEST_NAME, pSourceData->GetStringBy(DATA_TEST_NAME, i), iIndex);
		SetStringTo(DATA_DISCLAIMER, pSourceData->GetStringBy(DATA_DISCLAIMER, i), iIndex);
	}

	if (iOldSel >= m_arrayTemplate.GetCount())
		m_iUsedIndex = m_arrayTemplate.GetCount()-1;
	else
		m_iUsedIndex = iOldSel;
}


void CReportTemplateData::SetCurSel(CString strName)
{
	int iSelected = -1;
	for (int i=0; i<m_arrayTemplate.GetCount(); i++)
	{
		if (strName == GetStringBy(DATA_NAME, i))
		{
			iSelected = i;
			break;
		}
	}
	if (iSelected != -1)
	{
		m_iUsedIndex = iSelected;
	}
}



BOOL CReportTemplateData::FindSameName(CString strName)
{
	BOOL bFind = FALSE;
	for (int i=0; i<GetTemplateCount(); i++)
	{
		if (strName == GetStringBy(DATA_NAME, i))
		{
			bFind = TRUE;
			break;
		}
	}

	return bFind;
}

int CReportTemplateData::AddTemplate(tagTemplate* pInputTemplate)
{
	tagTemplate* pTemplate = new tagTemplate;
	//pTemplate->strName = pInputTemplate->strName;
	//pTemplate->strLabName = pInputTemplate->strLabName;
	//pTemplate->strLabAddress = pInputTemplate->strLabAddress;
	//pTemplate->strContactUrl = pInputTemplate->strContactUrl;
	//pTemplate->strContactEmail = pInputTemplate->strContactEmail;
	//pTemplate->strContactPhone = pInputTemplate->strContactPhone;
	//pTemplate->strTestName = pInputTemplate->strTestName;
	//pTemplate->strDisclaimer = pInputTemplate->strDisclaimer;
	*pTemplate = *pInputTemplate;

	return m_arrayTemplate.Add(pTemplate);
}

CReportTemplateData::tagTemplate* CReportTemplateData::GetTemplate(int iIndex)
{
	if (iIndex < 0 || iIndex >= m_arrayTemplate.GetCount())
		return NULL;
	return (tagTemplate*)m_arrayTemplate.GetAt(iIndex);
}
