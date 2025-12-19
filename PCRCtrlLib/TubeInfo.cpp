#include "StdAfx.h"
#include "TubeInfo.h"
#include "PublicFun.h"
#include "Define.h"

CTubeInfo::CTubeInfo()
{
	m_iTubeID = 1;
	m_strTubeLabel.Empty();
	m_strTubeFlag.Empty();
	m_strSampleID.Empty();
	m_strSampleName.Empty();
	m_clrSample = RGB(255, 0, 0);
	m_nSampleType = SAMPLE_TYPE_UNKNOWN;
	m_bRepeatTube = FALSE;
	m_iRepeatNo = -1;
	m_strComments.Empty();

	m_iCalibrateChannel = -1;
	m_iCycleOfMaxMOTRatio = -1;
}

CTubeInfo::CTubeInfo(int nID)
{
	m_iTubeID = nID;
	m_strTubeLabel.Empty();
	m_strTubeFlag.Empty();
	m_strSampleID.Empty();
	m_strSampleName.Empty();
	m_nSampleType = SAMPLE_TYPE_UNKNOWN;
	m_bRepeatTube = FALSE;
	m_iRepeatNo = -1;
	m_strComments.Empty();

	m_iCalibrateChannel = -1;
	m_iCycleOfMaxMOTRatio = -1;
}

CTubeInfo::~CTubeInfo(void)
{
	DeleteAllDyeTargetInfo();
}

void CTubeInfo::DeleteAllDyeTargetInfo()
{
	int nDyeCount = m_arrayDyeTarget.GetCount();
	for(int i=0; i<nDyeCount; i++)
	{
		CDyeTargetInfo* pTarget = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(i);
		SAFE_DELETE(pTarget);
	}
	m_arrayDyeTarget.RemoveAll();
}

CTubeInfo::CTubeInfo(const CTubeInfo& tubeInfo):CSampleInfo(tubeInfo)
{
	m_iTubeID = tubeInfo.m_iTubeID;
	m_strTubeLabel = tubeInfo.m_strTubeLabel;
	m_strTubeFlag = tubeInfo.m_strTubeFlag;
	m_strSampleID = tubeInfo.m_strSampleID;
	m_strSampleName = tubeInfo.m_strSampleName;
	m_nSampleType = tubeInfo.m_nSampleType;
	m_bRepeatTube = tubeInfo.m_bRepeatTube;
	m_iRepeatNo = tubeInfo.m_iRepeatNo;
	m_strComments = tubeInfo.m_strComments;
	m_iCalibrateChannel = tubeInfo.m_iCalibrateChannel;

	DeleteAllDyeTargetInfo();
	int nTargetCount = tubeInfo.m_arrayDyeTarget.GetCount();
	CDyeTargetInfo *pTargetSource = NULL,*pTargetDest = NULL;
	for (int i = 0;i < nTargetCount;++i)
	{
		pTargetDest = new CDyeTargetInfo();
		pTargetSource = (CDyeTargetInfo*)tubeInfo.m_arrayDyeTarget.GetAt(i);
		*pTargetDest = *pTargetSource;
		m_arrayDyeTarget.Add(pTargetDest);
		pTargetDest = NULL;
	}

	return;
}

void CTubeInfo::operator=(const CTubeInfo& tubeInfo)
{
	CSampleInfo::operator=(tubeInfo);
	m_iTubeID = tubeInfo.m_iTubeID;
	m_strTubeLabel = tubeInfo.m_strTubeLabel;
	m_strTubeFlag = tubeInfo.m_strTubeFlag;
	m_strSampleID = tubeInfo.m_strSampleID;
	m_strSampleName = tubeInfo.m_strSampleName;
	m_nSampleType = tubeInfo.m_nSampleType;
	m_bRepeatTube = tubeInfo.m_bRepeatTube;
	m_iRepeatNo = tubeInfo.m_iRepeatNo;
	m_strComments = tubeInfo.m_strComments;
	m_iCalibrateChannel = tubeInfo.m_iCalibrateChannel;

	DeleteAllDyeTargetInfo();
	int nTargetCount = tubeInfo.m_arrayDyeTarget.GetCount();
	CDyeTargetInfo *pTargetSource = NULL,*pTargetDest = NULL;
	for (int i = 0;i < nTargetCount;++i)
	{
		pTargetDest = new CDyeTargetInfo();
		pTargetSource = (CDyeTargetInfo*)tubeInfo.m_arrayDyeTarget.GetAt(i);
		*pTargetDest = *pTargetSource;
		m_arrayDyeTarget.Add(pTargetDest);
		pTargetDest = NULL;
	}

	return;
}

BOOL CTubeInfo::WriteFile(CFile* pFile,WORD wVersion)
{
	pFile->Write((const char *)&m_iTubeID, sizeof(int));

	CPublicFun::WriteStringToFile(pFile, m_strTubeLabel);
	CPublicFun::WriteStringToFile(pFile, m_strTubeFlag);
	CPublicFun::WriteStringToFile(pFile, m_strSampleID);
	CPublicFun::WriteStringToFile(pFile, m_strSampleName);
	pFile->Write((const char *)&m_clrSample, sizeof(COLORREF));

	int nTempInt = m_nSampleType;
	pFile->Write((const char *)&nTempInt, sizeof(int));
	pFile->Write((const char *)&m_bRepeatTube, sizeof(BOOL));
	pFile->Write((const char *)&m_iRepeatNo, sizeof(int));
	CPublicFun::WriteStringToFile(pFile, m_strComments);

	int nTargetCount = m_arrayDyeTarget.GetCount();
	pFile->Write((const char *)&nTargetCount, sizeof(int));
	for (int i=0; i<nTargetCount; i++)
	{
		CDyeTargetInfo* pTarget = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(i);
		pTarget->WriteFile(pFile,wVersion);
	}

	if(wVersion >= wPCRVersion104)
	{
		CPublicFun::WriteStringToFile(pFile, m_strPatientID);
		CPublicFun::WriteStringToFile(pFile, m_strClinicID);
		CPublicFun::WriteStringToFile(pFile, m_strCaseID);
		CPublicFun::WriteStringToFile(pFile, m_strPatientName);
		CPublicFun::WriteStringToFile(pFile, m_strAreaNo);
		CPublicFun::WriteStringToFile(pFile, m_strRoomNo);
		CPublicFun::WriteStringToFile(pFile, m_strBedNo);
		CPublicFun::WriteStringToFile(pFile, m_strProposer);
		CPublicFun::WriteStringToFile(pFile, m_strDepartment);
		CPublicFun::WriteStringToFile(pFile, m_strHospital);
		CPublicFun::WriteStringToFile(pFile, m_strCollectDate);
		CPublicFun::WriteStringToFile(pFile, m_strApplyDate);

		pFile->Write((const char *)&m_nPatientSex,sizeof(ePatientSexuality));
		pFile->Write((const char *)&m_nPatientAge,sizeof(int));
		pFile->Write((const char *)&m_nAgeUnit,sizeof(ePatientAgeUnit));
	}

	return TRUE;
}

BOOL CTubeInfo::ReadFile(CFile* pFile, WORD wVersion)
{
	DeleteAllDyeTargetInfo();

	pFile->Read(&m_iTubeID, sizeof(int));

	m_strTubeLabel = CPublicFun::ReadStringFromFile(pFile);
	m_strTubeFlag = CPublicFun::ReadStringFromFile(pFile);
	m_strSampleID = CPublicFun::ReadStringFromFile(pFile);
	m_strSampleName = CPublicFun::ReadStringFromFile(pFile);
	if (wVersion >= wPCRVersion101)
	{
		pFile->Read((char *)&m_clrSample, sizeof(COLORREF));
	}

	int tempInt = 0;
	pFile->Read(&tempInt, sizeof(int));
	m_nSampleType = (eSampleType)tempInt;
	pFile->Read(&m_bRepeatTube, sizeof(BOOL));
	pFile->Read(&m_iRepeatNo, sizeof(int));
	m_strComments = CPublicFun::ReadStringFromFile(pFile);

	int nTargetCount = 0;
	pFile->Read(&nTargetCount, sizeof(int));
	for (int i=0; i<nTargetCount; i++)
	{
		CDyeTargetInfo* pTarget = new CDyeTargetInfo;
		pTarget->ReadFile(pFile, wVersion);
		m_arrayDyeTarget.Add(pTarget);
	}

	if(wVersion >= wPCRVersion104)
	{
		m_strPatientID = CPublicFun::ReadStringFromFile(pFile);
		m_strClinicID = CPublicFun::ReadStringFromFile(pFile);
		m_strCaseID = CPublicFun::ReadStringFromFile(pFile);
		m_strPatientName = CPublicFun::ReadStringFromFile(pFile);
		m_strAreaNo = CPublicFun::ReadStringFromFile(pFile);
		m_strRoomNo = CPublicFun::ReadStringFromFile(pFile);
		m_strBedNo = CPublicFun::ReadStringFromFile(pFile);
		m_strProposer = CPublicFun::ReadStringFromFile(pFile);
		m_strDepartment = CPublicFun::ReadStringFromFile(pFile);
		m_strHospital = CPublicFun::ReadStringFromFile(pFile);
		m_strCollectDate = CPublicFun::ReadStringFromFile(pFile);
		m_strApplyDate = CPublicFun::ReadStringFromFile(pFile);

		pFile->Read(&m_nPatientSex,sizeof(ePatientSexuality));
		pFile->Read(&m_nPatientAge,sizeof(int));
		pFile->Read(&m_nAgeUnit,sizeof(ePatientAgeUnit));
	}

	return TRUE;
}

void CTubeInfo::SetTmCount(int nCount)
{
	int nTargetCount = m_arrayDyeTarget.GetCount();
	for (int i=0; i<nTargetCount; i++)
	{
		CDyeTargetInfo* pTarget = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(i);
		pTarget->SetTmCount(nCount);
	}
}

CDyeTargetInfo* CTubeInfo::GetTargetInfo(int iIndex)
{
	CDyeTargetInfo* pDyeInfo = NULL;
	if (iIndex < m_arrayDyeTarget.GetCount())
	{
		pDyeInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(iIndex);
	}

	return pDyeInfo;
}

CString CTubeInfo::GetRepeatNoString()
{
	if(-1 == m_iRepeatNo)
	{
		return _T("");
	}

	CString strTemp(_T(""));
	strTemp.Format(_T("%d"),m_iRepeatNo);
	return strTemp;
}

CDyeTargetInfo* CTubeInfo::FindTargetInfoByTarget(CString strTarget)
{
	CDyeTargetInfo* pDyeInfo = NULL;
	int iCount = m_arrayDyeTarget.GetCount();
	for (int i=0; i<iCount; i++)
	{
		CDyeTargetInfo* pTemp = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(i);
		if (pTemp->GetTargetName() == strTarget)
		{
			pDyeInfo = pTemp;
			break;
		}
	}
	return pDyeInfo;
}

CDyeTargetInfo* CTubeInfo::FindTargetInfoByChannel(int iChannel)
{
	CDyeTargetInfo* pDyeInfo = NULL;
	int iCount = m_arrayDyeTarget.GetCount();
	for (int i=0; i<iCount; i++)
	{
		CDyeTargetInfo* pTemp = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(i);
		if (pTemp->GetChannel() == iChannel)
		{
			pDyeInfo = pTemp;
			break;
		}
	}
	return pDyeInfo;
}

void CTubeInfo::ResetDyeInfo()
{
	CDyeTargetInfo* pDyeInfo = NULL;
	int iCount = m_arrayDyeTarget.GetCount();
	for (int i = 0;i < iCount;++i)
	{
		pDyeInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(i);
		if(NULL != pDyeInfo)
		{
			pDyeInfo->SetBeSelected(FALSE);
			pDyeInfo->SetDyeName(_T(""));
			pDyeInfo->SetTargetName(_T(""));
			pDyeInfo->SetQuantity(0,4);
		}
	}
	return;
}

void CTubeInfo::ResetDyeTargetInfoExceptDyeNameAndColor()
{
	CDyeTargetInfo* pDyeInfo = NULL;
	int iCount = m_arrayDyeTarget.GetCount();
	for (int i = 0;i < iCount;++i)
	{
		pDyeInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(i);
		if(NULL != pDyeInfo)
		{
			pDyeInfo->SetBeSelected(FALSE);
			pDyeInfo->SetQuantity(0,4);
		}
	}
	return;
}

CString CTubeInfo::GetDyeName(int iIndex)
{
	if(iIndex < 0 || iIndex >= m_arrayDyeTarget.GetSize())
	{
		return _T("");
	}

	CDyeTargetInfo* pDyeTargetInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(iIndex);
	if(NULL == pDyeTargetInfo)
	{
		return _T("");
	}

	return pDyeTargetInfo->GetDyeName();
}

void CTubeInfo::SetDyeName(int iIndex,CString strDyeName)
{
	if(iIndex < 0 || iIndex >= m_arrayDyeTarget.GetSize())
	{
		return;
	}

	CDyeTargetInfo* pDyeTargetInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(iIndex);
	if(NULL == pDyeTargetInfo)
	{
		return;
	}

	pDyeTargetInfo->SetDyeName(strDyeName);
	return; 
}

CString CTubeInfo::GetTargetName(int iIndex)
{
	if(iIndex < 0 || iIndex >= m_arrayDyeTarget.GetSize())
	{
		return _T("");
	}

	CDyeTargetInfo* pDyeTargetInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(iIndex);
	if(NULL == pDyeTargetInfo)
	{
		return _T("");
	}

	return pDyeTargetInfo->GetTargetName();
}

void CTubeInfo::SetTargetName(int iIndex,CString strTarget)
{
	if(iIndex < 0 || iIndex >= m_arrayDyeTarget.GetSize())
	{
		return;
	}

	CDyeTargetInfo* pDyeTargetInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(iIndex);
	if(NULL == pDyeTargetInfo)
	{
		return;
	}

	pDyeTargetInfo->SetTargetName(strTarget);
	return;
}

int CTubeInfo::GetDyeTargetInfo(int iIndex,CString& strDyeName,CString strTargetName)
{
	strDyeName = _T("");
	strTargetName = _T("");

	if(iIndex < 0 || iIndex >= m_arrayDyeTarget.GetSize())
	{
		return -1;
	}

	CDyeTargetInfo* pDyeTargetInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(iIndex);
	if(NULL == pDyeTargetInfo)
	{
		return -2;
	}

	strDyeName = pDyeTargetInfo->GetDyeName();
	strTargetName = pDyeTargetInfo->GetTargetName();

	return 0;
}

CString CTubeInfo::GetTubeLabel(int iColNum) 
{ 
	if(iColNum < 1)
	{
		m_strTubeLabel = _T("");
		return _T("");
	}

	// helper: zero-based row index to letters
	auto toLetters = [](int zeroBasedIndex) -> CString {
		CString result;
		int index = zeroBasedIndex;
		while (index >= 0)
		{
			int rem = index % 26;
			result.Insert(0, TCHAR('A' + rem));
			index = index / 26 - 1;
		}
		return result;
	};

	int Quotient = (m_iTubeID - 1) / iColNum;
	int Remainder = m_iTubeID % iColNum;

	if (0 == Remainder)
	{
		Remainder = iColNum;
	}

	CString rowLabel = toLetters(Quotient);
	m_strTubeLabel.Format(_T("%s%d"),rowLabel,Remainder);

	return m_strTubeLabel; 
}

void CTubeInfo::InitDyeChannel(int iChannelCount)
{
	DeleteAllDyeTargetInfo();

	for(int i=0; i<iChannelCount; ++i)
	{
		CDyeTargetInfo* pDyeTargetInfo = new CDyeTargetInfo();
		if (pDyeTargetInfo != NULL)
		{
			pDyeTargetInfo->SetChannel(i+1); 
			m_arrayDyeTarget.Add(pDyeTargetInfo);
		}
	}

	ASSERT(m_arrayDyeTarget.GetCount() == iChannelCount);
}

// ������Ŀ�ж����Ⱦ����Ϣ
void CTubeInfo::PasteTargetInfoFromItem(CReagentItem* pItem)
{
	int iChannelCount = pItem->GetChannelTargetCount();
	int iTargetCount = GetTargetCount();
	for (int i=0; i<iTargetCount; i++)
	{
		CDyeTargetInfo* pTarget = GetTargetInfo(i);
		if (pTarget == NULL)
			continue;

		BOOL bFind = FALSE;
		for (int j=0; j<iChannelCount; j++)
		{
			CItemChannelInfo* pChannel = (CItemChannelInfo*)pItem->GetChannelTargetBy(j);
			if (pTarget->GetChannel() == pChannel->GetChannel()) // �ҵ�
			{
				pTarget->SetDyeName(pChannel->GetDyeName());
				pTarget->SetBeSelected(pChannel->GetUse());
				if (pChannel->GetUse())
				{
					pTarget->SetTargetName(pChannel->GetTargetName(m_strTubeFlag));
					pTarget->SetTargetColor(pChannel->GetTargetColor(m_strTubeFlag));
				}

				bFind = TRUE;
				break;
			}
		}

		if (!bFind)
		{
			pTarget->SetBeSelected(FALSE);
		}
	}
}



void CTubeInfo::Reset(BOOL bTemplateMode)
{
	m_strSampleID.Empty();
	m_strSampleName.Empty();
	m_nSampleType = SAMPLE_TYPE_NONE;
	m_bRepeatTube = FALSE;
	m_iRepeatNo = -1;
	m_strComments.Empty();

	m_iCalibrateChannel = -1;
	if(!bTemplateMode)
	{
		ResetDyeInfo();
	}

	CSampleInfo::Reset();
}

void CTubeInfo::ResetInfo1(BOOL bTemplateMode)
{
	m_strSampleID.Empty();
	m_nSampleType = SAMPLE_TYPE_NONE;
	m_bRepeatTube = FALSE;
	m_iRepeatNo = -1;

	if(!bTemplateMode)
	{
		ResetDyeTargetInfoExceptDyeNameAndColor();
	}

	CSampleInfo::Reset();
}

void CTubeInfo::ResetInfo2(BOOL bTemplateMode)
{
	m_strSampleID.Empty();
	m_strSampleName.Empty();
	m_nSampleType = SAMPLE_TYPE_NONE;
	m_strComments.Empty();

	CSampleInfo::Reset();
}

int CTubeInfo::GetChannelNum()
{
	return m_arrayDyeTarget.GetCount();
}

BOOL CTubeInfo::IsAllSelectedChannelPositive()
{
	int iChanNum = GetChannelNum();
	if(iChanNum < 1)
	{
		return FALSE;
	}

	BOOL bCompared = FALSE;
	CDyeTargetInfo* pDyeTargetInfo = NULL;

#ifdef PS96INSTRUMENT
	iChanNum -= 1;
#endif

	for(int i = 0;i < iChanNum;++i)
	{
		pDyeTargetInfo = GetTargetInfo(i);
		if(NULL == pDyeTargetInfo || !pDyeTargetInfo->GetBeSelected())
		{
			continue;
		}

		if (pDyeTargetInfo->GetBeUnhandledAnaResult())
			continue;

		bCompared = TRUE;
		if(!pDyeTargetInfo->GetBePositiveAnaResult())
		{
			return FALSE;
		}
	}

	return bCompared;
}

BOOL CTubeInfo::IsAllSelectedChannelNegative()
{
	int iChanNum = GetChannelNum();
	if(iChanNum < 1)
	{
		return FALSE;
	}

	BOOL bCompared = FALSE;
	CDyeTargetInfo* pDyeTargetInfo = NULL;

#ifdef PS96INSTRUMENT
	iChanNum -= 1;
#endif

	for(int i = 0;i < iChanNum;++i)
	{
		pDyeTargetInfo = GetTargetInfo(i);
		if(NULL == pDyeTargetInfo || !pDyeTargetInfo->GetBeSelected())
		{
			continue;
		}
		if (pDyeTargetInfo->GetBeUnhandledAnaResult())
			continue;

		bCompared = TRUE;

		if (IsInternalControlSPC(pDyeTargetInfo->GetTargetName()))
		{
			if (pDyeTargetInfo->GetBeNegativeAnaResult())
				return FALSE;
		}
		else if(!pDyeTargetInfo->GetBeNegativeAnaResult())
		{
			return FALSE;
		}
	}

	return bCompared;
}

ESAMPLETOTALRESULT CTubeInfo::GetSampleTotalResultIfIncludeIC()
{
	BOOL bExistIC = FALSE,bICPositve = FALSE,bOtherChanExistPositive = FALSE;
	int iChanNum = GetChannelNum(),iIndex = 0;
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	for(int i = 0;i < iChanNum;++i)
	{
		pDyeTargetInfo = GetTargetInfo(i);
		if(NULL == pDyeTargetInfo || !pDyeTargetInfo->GetBeSelected())
		{
			continue;
		}

		if(IsInternalControl(pDyeTargetInfo->GetTargetName()))
		{
			bExistIC = TRUE;
			iIndex = i;
			bICPositve = pDyeTargetInfo->GetBePositiveAnaResult();
		}
		else
		{
			if(pDyeTargetInfo->GetBePositiveAnaResult())
			{
				bOtherChanExistPositive = TRUE;
			}
		}
	}

	if(!bExistIC)
	{
		return ESTR_NORMAL;
	}

	if(bICPositve)
	{
		if(bOtherChanExistPositive)
		{
			return ESTR_NORMAL;
		}
		else
		{
			return ESTR_NEGATIVE;
		}
	}
	else
	{
		if(bOtherChanExistPositive)
		{
			return ESTR_NORMAL;
		}
		else
		{
			return ESTR_INVALID;
		}
	}
}

BOOL CTubeInfo::IsExistInternalControl()
{
	int iChanNum = GetChannelNum();
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	for(int i = 0;i < iChanNum;++i)
	{
		pDyeTargetInfo = GetTargetInfo(i);
		if(NULL == pDyeTargetInfo || !pDyeTargetInfo->GetBeSelected())
		{
			continue;
		}

		if(IsInternalControl(pDyeTargetInfo->GetTargetName()))
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CTubeInfo::IsExistInternalControl(BOOL& bICPositive)
{
	int iChanNum = GetChannelNum();
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	for(int i = 0;i < iChanNum;++i)
	{
		pDyeTargetInfo = GetTargetInfo(i);
		if(NULL == pDyeTargetInfo || !pDyeTargetInfo->GetBeSelected())
		{
			continue;
		}

		if(IsInternalControl(pDyeTargetInfo->GetTargetName()))
		{
			bICPositive = pDyeTargetInfo->GetBePositiveAnaResult();
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CTubeInfo::IsExistPositiveExceptICGene()
{
	int iChanNum = GetChannelNum();
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	for(int i = 0;i < iChanNum;++i)
	{
		pDyeTargetInfo = GetTargetInfo(i);
		if(NULL == pDyeTargetInfo || !pDyeTargetInfo->GetBeSelected())
		{
			continue;
		}

		if(!IsInternalControl(pDyeTargetInfo->GetTargetName()) && pDyeTargetInfo->GetBePositiveAnaResult())
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CTubeInfo::IsInternalControl(CString& str)
{
	str.TrimLeft();
	str.TrimRight();
	if(_T("IC") == str || _T("ic") == str || _T("Ic") == str || _T("iC") == str || IsInternalControlSPC(str))
	{
		return TRUE;
	}
	else if (str.Find(_T("-IC")) != -1 || str.Find(_T("IC-")) != -1 || str.Find(_T("-ic")) != -1 || str.Find(_T("-Ic")) != -1
		|| str.Find(_T("-iC")) != -1 || str.Find(_T("ic-")) != -1 || str.Find(_T("Ic-")) != -1 || str.Find(_T("iC-")) != -1)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CTubeInfo::IsInternalControlSPC(CString strSPC)
{
	if(_T("SPC") == strSPC || _T("spc") == strSPC || _T("Spc") == strSPC)
	{
		return TRUE;
	}
	return FALSE;
}


// ��ѡ��ͨ����û���������Ƶ�Ⱦ�Ϻ�Ŀ����Ϣ��������
void CTubeInfo::PerfectTargetDyeInfo(CPtrArray* pArrayChannel)
{
	int iTargetCount = m_arrayDyeTarget.GetCount();
	for (int i=0; i<iTargetCount; i++)
	{
		CDyeTargetInfo* pTargetInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(i);
		if (pTargetInfo == NULL)
			continue;

		if (pTargetInfo->GetDyeName().IsEmpty())
		{
			int iDyeCount = pArrayChannel->GetCount();
			tagDyeChannelInfo* pFindChannel = NULL;
			for (int j=0; j<iDyeCount; j++)
			{
				tagDyeChannelInfo* pChannel = (tagDyeChannelInfo*)pArrayChannel->GetAt(j);
				if (pChannel == NULL)
					continue;

				if (pChannel->iChannel == pTargetInfo->GetChannel())
				{
					if (pFindChannel == NULL)
					{
						pFindChannel = pChannel;			
					}
					if (pChannel->bDefault)
					{
						pFindChannel = pChannel;
						break;
					}
				}
			}

			if (pFindChannel == NULL)
				continue;

			pTargetInfo->SetDyeName(pFindChannel->strDye);
			if (pTargetInfo->GetTargetName().IsEmpty())
			{
				pTargetInfo->SetTargetName(pFindChannel->strTarget);
			}
		}
		else if (pTargetInfo->GetTargetName().IsEmpty())
		{
			pTargetInfo->SetTargetName(pTargetInfo->GetDyeName());
		}
	}
}

BOOL CTubeInfo::GetBeNeedDensity()
{
	return SAMPLE_TYPE_STANDARD == m_nSampleType || SAMPLE_TYPE_CALIBRATOR == m_nSampleType;
}

BOOL CTubeInfo::GetBeNoneSampleType()
{
	return SAMPLE_TYPE_NONE == m_nSampleType;
}

BOOL CTubeInfo::GetBeUnknown()
{
	return SAMPLE_TYPE_UNKNOWN == m_nSampleType;
}

BOOL CTubeInfo::GetBePositive()
{
	return SAMPLE_TYPE_POSITIVE == m_nSampleType;
}

BOOL CTubeInfo::GetBeNegtive()
{
	return SAMPLE_TYPE_NEGATIVE == m_nSampleType;
}

COLORREF CTubeInfo::GetSampleDyeTargetColor(int iChanIndex)
{
	if(iChanIndex < 0 || iChanIndex >= m_arrayDyeTarget.GetSize())
	{
		return RGB(0,0,0);
	}

	CDyeTargetInfo* pDyeTargetInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(iChanIndex);
	if(NULL == pDyeTargetInfo)
	{
		return RGB(0,0,0);
	}
	
	return pDyeTargetInfo->GetTargetColor();
}

void CTubeInfo::SetSampleDyeTargetColor(int iChanIndex,COLORREF clrDyeTarget)
{
	if(iChanIndex < 0 || iChanIndex >= m_arrayDyeTarget.GetSize())
	{
		return;
	}

	CDyeTargetInfo* pDyeTargetInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(iChanIndex);
	if(NULL == pDyeTargetInfo)
	{
		return;
	}

	pDyeTargetInfo->SetTargetColor(clrDyeTarget);
	return;
}

BOOL CTubeInfo::GetSampleChannelSelectState(int iChanIndex)
{
	if(iChanIndex < 0 || iChanIndex >= m_arrayDyeTarget.GetSize())
	{
		return FALSE;
	}

	CDyeTargetInfo* pDyeTargetInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(iChanIndex);
	if(NULL == pDyeTargetInfo)
	{
		return FALSE;
	}

	return pDyeTargetInfo->GetBeSelected();
}

void CTubeInfo::SetSampleChannelSelectState(int iChanIndex,BOOL bSelectState)
{
	if(iChanIndex < 0 || iChanIndex >= m_arrayDyeTarget.GetSize())
	{
		return;
	}

	CDyeTargetInfo* pDyeTargetInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(iChanIndex);
	if(NULL == pDyeTargetInfo)
	{
		return;
	}

	pDyeTargetInfo->SetBeSelected(bSelectState);
	return;
}

double CTubeInfo::GetFirstNonZeroDensity()
{
	double dTemp = 0.0;
	int iDyeNum = m_arrayDyeTarget.GetCount();
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	for(int i = 0;i < iDyeNum;++i)
	{
		pDyeTargetInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(i);
		if(NULL == pDyeTargetInfo)
		{
			continue;
		}

		dTemp = pDyeTargetInfo->GetQuantity();
		if(0 != dTemp)
		{
			break;
		}
	}

	return dTemp;
}

CString CTubeInfo::GetFirstNonZeroDensityString(int iDPd /*= 4*/)
{
	double dTemp = 0.0;
	int iDyeNum = m_arrayDyeTarget.GetCount();
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	for(int i = 0;i < iDyeNum;++i)
	{
		pDyeTargetInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(i);
		if(NULL == pDyeTargetInfo)
		{
			continue;
		}

		dTemp = pDyeTargetInfo->GetQuantity();
		if(0 != dTemp)
		{
			break;
		}
	}

	CString strTemp(_T(""));
	if(dTemp > 0)
	{
		 CString strFormat(_T(""));
		 strFormat.Format(_T("%%.%dlf"),pDyeTargetInfo->GetQuanIDPD());
		strTemp.Format(strFormat,dTemp);
	}

	return strTemp;
}

CString CTubeInfo::GetAllChannelDensityString()
{
	double dTemp = 0.0;
	int iDyeNum = m_arrayDyeTarget.GetCount();
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	CString strData(_T(""));

	for(int i = 0;i < iDyeNum;++i)
	{
		pDyeTargetInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(i);
		if(NULL == pDyeTargetInfo)
		{
			strData += _T("\n");
			continue;
		}

		dTemp = pDyeTargetInfo->GetQuantity();
		if(dTemp > 0)
		{
			CString strFormat(_T(""));
			strFormat.Format(_T("%%.%dlf"),pDyeTargetInfo->GetQuanIDPD());
			strData.AppendFormat(strFormat,dTemp);
		}
		strData += _T("\n");
	}

	return strData;
}

double CTubeInfo::GetDensity(int iChanIndex)
{
	int iDyeNum = m_arrayDyeTarget.GetCount();
	if(iChanIndex < 0 || iChanIndex >= iDyeNum)
	{
		return 0;
	}

	CDyeTargetInfo* pDyeTargetInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(iChanIndex);
	if(NULL == pDyeTargetInfo)
	{
		return 0;
	}

	return pDyeTargetInfo->GetQuantity();
}

CString CTubeInfo::GetDensityString(int iChanIndex,int iDPd /*= 4*/)
{
	int iDyeNum = m_arrayDyeTarget.GetCount();
	if(iChanIndex < 0 || iChanIndex >= iDyeNum)
	{
		return 0;
	}

	CDyeTargetInfo* pDyeTargetInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(iChanIndex);
	if(NULL == pDyeTargetInfo)
	{
		return 0;
	}

	double dTemp = pDyeTargetInfo->GetQuantity();
	CString strTemp(_T(""));
	if(dTemp > 0)
	{
		CString strFormat(_T(""));
		strFormat.Format(_T("%%.%dlf"),pDyeTargetInfo->GetQuanIDPD());
		strTemp.Format(strFormat,dTemp);
	}

	return strTemp;
}

double CTubeInfo::GetCalculatedDensity(int iChanIndex)
{
	int iDyeNum = m_arrayDyeTarget.GetCount();
	if(iChanIndex < 0 || iChanIndex >= iDyeNum)
	{
		return -1;
	}

	CDyeTargetInfo* pDyeTargetInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(iChanIndex);
	if(NULL == pDyeTargetInfo)
	{
		return -1;
	}

	return pDyeTargetInfo->GetCalcValue(CDyeTargetInfo::VALUE_CALC_QUANTITY);
}

CString CTubeInfo::GetCalculatedDensityString(int iChanIndex,int iDPd /*= 4*/)
{
	CString strTemp(_T(""));
	int iDyeNum = m_arrayDyeTarget.GetCount();
	if(iChanIndex < 0 || iChanIndex >= iDyeNum)
	{
		return strTemp;
	}

	CDyeTargetInfo* pDyeTargetInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(iChanIndex);
	if(NULL == pDyeTargetInfo)
	{
		return strTemp;
	}

	double dTemp = pDyeTargetInfo->GetCalcValue(CDyeTargetInfo::VALUE_CALC_QUANTITY);
	if(dTemp > 0) //-1�Ƿ������жϣ�������20220614��������
	{
		CString strFormat(_T(""));
		strFormat.Format(_T("%%.%dlf"),iDPd);
		strTemp.Format(strFormat,dTemp);
		int iIndex = strTemp.Find(_T('.')),iLen = strTemp.GetLength(),iZeroNum = 0;
		for(int i = iLen - 1;i > iIndex;--i)
		{
			if(_T('0') != strTemp[i])
			{
				break;
			}
			++iZeroNum;
		}
		strTemp = strTemp.Left(iLen - iZeroNum);
	}

	return strTemp;
}

BOOL CTubeInfo::ResetChannelCalculatedDensity(int iChanIndex)
{
	int iDyeNum = m_arrayDyeTarget.GetCount();
	if(iChanIndex < 0 || iChanIndex >= iDyeNum)
	{
		return FALSE;
	}

	CDyeTargetInfo* pDyeTargetInfo = (CDyeTargetInfo*)m_arrayDyeTarget.GetAt(iChanIndex);
	if(NULL == pDyeTargetInfo)
	{
		return FALSE;
	}

	pDyeTargetInfo->SetCalcValue(CDyeTargetInfo::VALUE_CALC_QUANTITY,-1);
	return TRUE;
}



