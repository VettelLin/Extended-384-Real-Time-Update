#include "StdAfx.h"
#include "PlateTubeInfo.h"


CPlateTubeInfo::CPlateTubeInfo(void)
{
	m_pPlateInfo = new tagPlateModeInfo;
	if(NULL != m_pPlateInfo)
	{
#ifdef PS384INSTRUMENT
		// Default to 12x32 display for 384 instrument builds
		m_pPlateInfo->iHoriCount = 12;
		m_pPlateInfo->iVertCount = 32;
#endif
		int iTubeCount = m_pPlateInfo->iHoriCount * m_pPlateInfo->iVertCount;
		for(int i = 0;i < iTubeCount;++i)
		{
			CTubeInfo* pTubeInfo = new CTubeInfo(i + 1);
			m_arrayTube.Add(pTubeInfo);
		}
	}
}


CPlateTubeInfo::~CPlateTubeInfo(void)
{
	DeleteAll();
}

void CPlateTubeInfo::DeleteAll(BOOL bOnlyTube)
{
	if (!bOnlyTube)
	{
		SAFE_DELETE(m_pPlateInfo);
	}

	DeleteAllTubes();
}

void CPlateTubeInfo::DeleteAllTubes()
{
	int iTubeCount = m_arrayTube.GetCount();
	for (int i=0; i<iTubeCount; i++)
	{
		CTubeInfo* pTubeInfo = (CTubeInfo*)m_arrayTube.GetAt(i);
		SAFE_DELETE(pTubeInfo);
	}
	m_arrayTube.RemoveAll();
}

// ��ָ���ļ�����ȡ�װ�����
BOOL CPlateTubeInfo::OpenFile(CString strFilePath)
{
	CFile file;       
	if (!file.Open(strFilePath,CFile::modeRead | CFile::shareDenyNone))
	{
		return FALSE;
	}

	// ��ȡ�ļ���ʶ�Ͱ汾
	WORD wFileMark = 0;
	file.Read(&wFileMark, sizeof(WORD));
	if (wFileMark != wFileMarkPCRPlate)
	{
		return FALSE;
	}

	WORD wVersion = 0;
	file.Read(&wVersion, sizeof(WORD));

	ReadFile(&file, wVersion);

	file.Close();

	return TRUE;
}

// д�����ݵ��ļ���
BOOL CPlateTubeInfo::WriteFile(CFile* pFile,WORD wVersion)
{
	int iTempInt = m_pPlateInfo->nPlateType;
	pFile->Write((const char*)&iTempInt, sizeof(int));
	iTempInt = m_pPlateInfo->iPlateCount;
	pFile->Write((const char*)&iTempInt, sizeof(int));
	iTempInt = m_pPlateInfo->nSortMode;
	pFile->Write((const char*)&iTempInt, sizeof(int));
	iTempInt = m_pPlateInfo->iHoriCount;
	pFile->Write((const char*)&iTempInt, sizeof(int));
	iTempInt = m_pPlateInfo->iVertCount;
	pFile->Write((const char*)&iTempInt, sizeof(int));

	int nTubeCount = m_arrayTube.GetCount();
	pFile->Write((const char*)&nTubeCount,sizeof(int));
	for (int i=0; i<nTubeCount; i++)
	{
		//lzh:�г�����A1�Ĵ���
		CTubeInfo* pTubeInfo = (CTubeInfo*)m_arrayTube.GetAt(i);
		pTubeInfo->WriteFile(pFile, wVersion);
	}

	return TRUE;
}

BOOL CPlateTubeInfo::ReadFile(CFile* pFile, WORD wVersion)
{
	DeleteAll(TRUE);

	int iTempInt = 0;
	pFile->Read(&iTempInt, sizeof(int));
	m_pPlateInfo->nPlateType = (ePlateModeType)iTempInt;
	pFile->Read(&iTempInt, sizeof(int));
	m_pPlateInfo->iPlateCount = iTempInt;
	pFile->Read(&iTempInt, sizeof(int));
	m_pPlateInfo->nSortMode = (ePlateSortMode)iTempInt;
	pFile->Read(&iTempInt, sizeof(int));
	m_pPlateInfo->iHoriCount = iTempInt;
	pFile->Read(&iTempInt, sizeof(int));
	m_pPlateInfo->iVertCount = iTempInt;

	int nTubeCount = 0;
	pFile->Read(&nTubeCount, sizeof(int));
	for (int i=0; i<nTubeCount; i++)
	{
		CTubeInfo* pTubeInfo = new CTubeInfo(i+1);
		pTubeInfo->ReadFile(pFile, wVersion);
		m_arrayTube.Add(pTubeInfo);
	}

	return TRUE;
}

void CPlateTubeInfo::SetMeltTmCount(int nCount)
{
	int nTubeCount = m_arrayTube.GetCount();
	for (int i=0; i<nTubeCount; i++)
	{
		CTubeInfo* pTubeInfo = (CTubeInfo*)m_arrayTube.GetAt(i);
		pTubeInfo->SetTmCount(nCount);
	}
}

// ���ÿװ��������ʼ���װ���Ϣ
void CPlateTubeInfo::SetPlateInfo(tagPlateModeInfo* pInputPara)
{
	DeleteAllTubes();

	m_pPlateInfo->nPlateType = pInputPara->nPlateType;
	m_pPlateInfo->iPlateCount = pInputPara->iPlateCount;
	m_pPlateInfo->nSortMode = pInputPara->nSortMode;
	m_pPlateInfo->iHoriCount = pInputPara->iHoriCount;
	m_pPlateInfo->iVertCount = pInputPara->iVertCount;
	int iTubeCount = CalculateTubeCount(m_pPlateInfo);

	for (int i=0; i<iTubeCount; i++)
	{
		CTubeInfo* pTubeInfo = new CTubeInfo(i+1);
		CString strTubeID = CreateTubeLabel(i+1);
		pTubeInfo->SetTubeLabel(strTubeID);

		m_arrayTube.Add(pTubeInfo);
	}
}

// �������ȡ��ָ������Ϣָ��
CTubeInfo* CPlateTubeInfo::GetTubeInfo(int iIndex)
{
	CTubeInfo* pTubeInfo = NULL;
	if (iIndex >= 0 && iIndex < m_arrayTube.GetCount())
	{
		pTubeInfo = (CTubeInfo*)m_arrayTube.GetAt(iIndex);
	}
	return pTubeInfo;
}

void CPlateTubeInfo::GetAllTubeInfoPtr(vector<CTubeInfo*>& vecPTubeInfo)
{
	vecPTubeInfo.clear();
	int iTubeNum = m_arrayTube.GetSize();
	for(int i = 0;i < iTubeNum;++i)
	{
		vecPTubeInfo.push_back((CTubeInfo*)m_arrayTube.GetAt(i));
	}
	return;
}

CPtrArray* CPlateTubeInfo::GetTubeInfoArrayPtr()
{
	return &m_arrayTube;
}

CTubeInfo* CPlateTubeInfo::GetTubeInfoByID(int iTubeID)
{
	CTubeInfo* pTube = NULL;

	for (int i=0; i<m_arrayTube.GetCount(); i++)
	{
		CTubeInfo* pTemp = (CTubeInfo*)m_arrayTube.GetAt(i);
		if (pTemp->GetTubeID() == iTubeID)
		{
			pTube = pTemp;
			break;
		}
	}

	return pTube;
}

// ���ݹܱ�ǩ�õ�����Ϣָ��
// ���ݹܱ�ǩ�õ�����Ϣָ��
CTubeInfo* CPlateTubeInfo::GetTubeInfoByLabel(CString strLabel)
{
	//lzh
	int iColNum = 6;
	if (NULL != m_pPlateInfo)
	{
		iColNum = m_pPlateInfo->iHoriCount;
	}
	CTubeInfo* pTube = NULL;
	//lzh
	int iCount = m_arrayTube.GetCount();
	for (int i = 0; i < iCount; i++)
	{
		CTubeInfo* pTemp = (CTubeInfo*)m_arrayTube.GetAt(i);

		if (pTemp->GetTubeLabel(iColNum) == strLabel)
		{
			pTube = pTemp;
			break;
		}
	}

	return pTube;
}


int CPlateTubeInfo::CalculateTubeCount(tagPlateModeInfo* pInputInfo)
{
	int nCount = pInputInfo->iPlateCount * pInputInfo->iHoriCount * pInputInfo->iVertCount;
	return nCount;
}

// Helper: convert zero-based row index to Excel-like letters (A..Z, AA..)
static CString RowIndexToLetters(int zeroBasedIndex)
{
	CString result;
	int index = zeroBasedIndex;
	while (index >= 0)
	{
		int rem = index % 26;
		result.Insert(0, TCHAR('A' + rem));
		index = index / 26 - 1;
	}
	return result;
}

//lzh:�����ܺ��ı�
CString CPlateTubeInfo::CreateTubeLabel(int iIndex)
{
	CString strLable = _T("");
	int iColNum = 6;
	if (NULL != m_pPlateInfo && m_pPlateInfo->iHoriCount > 0)
	{
		iColNum = m_pPlateInfo->iHoriCount;
	}

	int Quotient = (iIndex - 1) / iColNum;
	int Remainder = iIndex % iColNum;

	if (0 == Remainder)
	{
		Remainder = iColNum;
	}

	CString rowLabel = RowIndexToLetters(Quotient);
	strLable.Format(_T("%s%d"), rowLabel, Remainder);

	return strLable;
}

// ������Ŀ�����ʼ��ͨ��ʹ�����
void CPlateTubeInfo::InitDyeTargetInfo(int iTubeID, CReagentItem* pReagentItem)
{
	CTubeInfo* pTubeInfo = GetTubeInfoByID(iTubeID);
	if (pTubeInfo == NULL)
	{
		ASSERT(FALSE);
		return;
	}
	pTubeInfo->PasteTargetInfoFromItem(pReagentItem);
}

void CPlateTubeInfo::SetTubeInfo(vector<CTubeInfo>& vecTubeInfo)
{
	if(m_arrayTube.GetSize() != vecTubeInfo.size())
	{
		DeleteAll(TRUE);

		for(int i = 0;i < vecTubeInfo.size();++i)
		{
			CTubeInfo* pTubeInfo = new CTubeInfo;
			m_arrayTube.Add(pTubeInfo);
		}
	}

	for(int i = 0;i < vecTubeInfo.size();++i)
	{
		CTubeInfo* pTubeInfo = (CTubeInfo*)m_arrayTube.GetAt(i);
		*pTubeInfo = vecTubeInfo[i];
	}

	return;
}

void CPlateTubeInfo::SetTubeInfo(vector<CTubeInfo*>& vecPTubeInfo)
{
	if(m_arrayTube.GetSize() != vecPTubeInfo.size())
	{
		DeleteAll(TRUE);

		for(int i = 0;i < vecPTubeInfo.size();++i)
		{
			CTubeInfo* pTubeInfo = new CTubeInfo;
			m_arrayTube.Add(pTubeInfo);
		}
	}

	for(int i = 0;i < vecPTubeInfo.size();++i)
	{
		CTubeInfo* pTubeInfo = (CTubeInfo*)m_arrayTube.GetAt(i);
		*pTubeInfo = *vecPTubeInfo[i];
	}

	return;
}

int CPlateTubeInfo::SetTubeInfo(CTubeInfo* pTotalTubeInfo,int iTubeNum)
{
	if(NULL == pTotalTubeInfo)
	{
		return -1;
	}
	if(iTubeNum < 1)
	{
		return -2;
	}

	if(m_arrayTube.GetSize() != iTubeNum)
	{
		DeleteAll(TRUE);

		for(int i = 0;i < iTubeNum;++i)
		{
			CTubeInfo* pTubeInfo = new CTubeInfo;
			m_arrayTube.Add(pTubeInfo);
		}
	}

	for(int i = 0;i < iTubeNum;++i)
	{
		CTubeInfo* pTubeInfo = (CTubeInfo*)m_arrayTube.GetAt(i);
		*pTubeInfo = pTotalTubeInfo[i];
	}

	return 0;
}

void CPlateTubeInfo::GetTubeInfo(vector<CTubeInfo>& vecTubeInfo)
{
	vecTubeInfo.clear();

	for(int i = 0;i < m_arrayTube.GetSize();++i)
	{
		CTubeInfo* pTubeInfo = (CTubeInfo*)m_arrayTube.GetAt(i);
		vecTubeInfo.push_back(*pTubeInfo);
	}

	return;
}

void CPlateTubeInfo::GetUsedDyeChannel(CStringArray* pArrayDye, CIntArray* pArrayChannel)
{
	for(int i = 0;i < m_arrayTube.GetSize();++i)
	{
		CTubeInfo* pTubeInfo = (CTubeInfo*)m_arrayTube.GetAt(i);		
		int iTargetCount = pTubeInfo->GetTargetCount();
		for (int j=0; j<iTargetCount; j++)
		{
			CDyeTargetInfo* pTarget = pTubeInfo->GetTargetInfo(j);
			if (!pTarget->GetBeSelected())
				continue;

			BOOL bFind = FALSE;
			for (int x=0; x<pArrayDye->GetCount(); x++)
			{
				if (pTarget->GetDyeName() == pArrayDye->GetAt(x))
				{
					bFind = TRUE;
					break;
				}
			}

			if (!bFind)
			{
				pArrayDye->Add(pTarget->GetDyeName());
				pArrayChannel->Add(pTarget->GetChannel());
			}
		}
	}
}

// �ҳ�ȫ�����õ�Ⱦ��ָ��ͨ��
void CPlateTubeInfo::GetUsedTarget(CPtrArray* pArrrayTarget)
{
	for(int i = 0;i < m_arrayTube.GetSize();++i)
	{
		CTubeInfo* pTubeInfo = (CTubeInfo*)m_arrayTube.GetAt(i);		
		int iTargetCount = pTubeInfo->GetTargetCount();
		for (int j=0; j<iTargetCount; j++)
		{
			CDyeTargetInfo* pTarget = pTubeInfo->GetTargetInfo(j);
			if (!pTarget->GetBeSelected())
				continue;

			BOOL bFind = FALSE;
			for (int x=0; x<pArrrayTarget->GetCount(); x++)
			{
				tagAnaAmplifyPara* pTempPara = (tagAnaAmplifyPara*)pArrrayTarget->GetAt(x);
				if (pTempPara->strTarget == pTarget->GetTargetName() && pTempPara->strDye == pTarget->GetDyeName())
				{
					bFind = TRUE;
					break;
				}
			}

			if (!bFind)
			{
				tagAnaAmplifyPara* pTemp = new tagAnaAmplifyPara;
				pTemp->strDye = pTarget->GetDyeName();
				pTemp->strTarget = pTarget->GetTargetName();
				pTemp->iChannelNo = pTarget->GetChannel();
				pArrrayTarget->Add(pTemp);
			}
		}
	}
}

// �õ����б�ǩ
void CPlateTubeInfo::GetTubeRowAndColLabel(CStringArray* pArrayRow, CStringArray* pArrayCol)
{
	pArrayRow->RemoveAll();
	pArrayCol->RemoveAll();

	if (m_pPlateInfo->iPlateCount == 1)
	{
		if (m_pPlateInfo->nSortMode == PLATE_SORT_HORZ)
		{
			CreateLetterLabel(pArrayRow, m_pPlateInfo->iVertCount);
			CreateNumberLabel(pArrayCol, m_pPlateInfo->iHoriCount);
		}
		else
		{
			CreateNumberLabel(pArrayRow, m_pPlateInfo->iVertCount);
			CreateLetterLabel(pArrayCol, m_pPlateInfo->iHoriCount);
		}
	}
	else
	{
		CString strPlateIndex;
		for (int i=0; i<m_pPlateInfo->iPlateCount; i++)
		{
			strPlateIndex.Format(_T("%d"), i+1);

			if (m_pPlateInfo->nSortMode == PLATE_SORT_HORZ)
			{
				CreateLetterLabel(pArrayRow, m_pPlateInfo->iVertCount, strPlateIndex);
				CreateNumberLabel(pArrayCol, m_pPlateInfo->iHoriCount, strPlateIndex);
			}
			else
			{
				CreateNumberLabel(pArrayRow, m_pPlateInfo->iVertCount, strPlateIndex);
				CreateLetterLabel(pArrayCol, m_pPlateInfo->iHoriCount, strPlateIndex);
			}
		}
	}
}

void CPlateTubeInfo::CreateLetterLabel(CStringArray* pArray, int iCount, CString strIndex)
{
	CString strTemp;
	for (int i=0; i<iCount; i++)
	{
		strTemp = RowIndexToLetters(i);
		pArray->Add(strTemp);
	}
}

void CPlateTubeInfo::CreateNumberLabel(CStringArray* pArray, int iCount, CString strIndex)
{
	CString strTemp;
	for (int i=0; i<iCount; i++)
	{
		strTemp.Format(_T("%d"), i+1);
		pArray->Add(strTemp);
	}
}
