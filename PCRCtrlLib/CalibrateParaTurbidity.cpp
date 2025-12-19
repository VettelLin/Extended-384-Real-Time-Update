#include "StdAfx.h"
#include "CalibrateParaTurbidity.h"
#include "PublicFun.h"

CCalibrateParaTurbidity::CCalibrateParaTurbidity(void)
{
	m_fMinRatio = 0.06f;
}

CCalibrateParaTurbidity::~CCalibrateParaTurbidity(void)
{
	DeleteAll();
}

void CCalibrateParaTurbidity::DeleteAll()
{
	int iCount = m_arrayDyePara.GetCount();
	for (int i=0; i<iCount; i++)
	{
		tagTurbidityPara* pAdjust = (tagTurbidityPara*)m_arrayDyePara.GetAt(i);
		SAFE_DELETE(pAdjust);
	}
}

BOOL CCalibrateParaTurbidity::ReadPara(CString strFile)
{
	DeleteAll();

	TCHAR c1[250];
	GetPrivateProfileString(_T("TurbidityPara"), _T("MinRatio"), _T("0.05"), c1, sizeof(c1), strFile);
	m_fMinRatio = (float)_ttof(c1);

	CString strFlag;
	tagTurbidityPara* pDyePara = NULL;

	int nDyeCount = ::GetPrivateProfileInt(_T("TurbidityPara"), _T("Count"), 0, strFile);
	for (int i=0; i<nDyeCount; i++)
	{
		pDyePara = new tagTurbidityPara;

		strFlag.Format(_T("TurbidityDye(%d)"), i+1);
		::GetPrivateProfileString(strFlag, _T("DyeName"), _T(""), c1, 250, strFile);
		pDyePara->strDyeName = c1;
		pDyePara->iChannel = ::GetPrivateProfileInt(strFlag, _T("Channel"), -1, strFile);

		pDyePara->bUseMOT = ::GetPrivateProfileInt(strFlag, _T("UseMOT"), 0, strFile);
		pDyePara->iAheadCycle = ::GetPrivateProfileInt(strFlag, _T("AheadCycle"), 5, strFile);
		GetPrivateProfileString(strFlag, _T("Ratio"), _T("0.05"), c1, sizeof(c1), strFile);
		pDyePara->fRatio = (float)_ttof(c1);

		m_arrayDyePara.Add(pDyePara);
	}
	
	return TRUE;
}

BOOL CCalibrateParaTurbidity::SavePara(CString strFile)
{
	CString strFlag, strValue;

	strValue.Format(_T("%.4f"), m_fMinRatio);
	::WritePrivateProfileString(_T("TurbidityPara"), _T("MinRatio"), strValue, strFile);

	int nCount = (int)m_arrayDyePara.GetSize();
	strValue.Format(_T("%d"), nCount);
	::WritePrivateProfileString(_T("TurbidityPara"), _T("Count"), strValue, strFile);

	tagTurbidityPara* pDyePara = NULL;
	for (int i=0; i<nCount; i++)
	{
		pDyePara = (tagTurbidityPara*)m_arrayDyePara.GetAt(i);
		strFlag.Format(_T("TurbidityDye(%d)"), i+1);
		::WritePrivateProfileString(strFlag, _T("DyeName"), pDyePara->strDyeName, strFile);
		strValue.Format(_T("%d"), pDyePara->iChannel);
		::WritePrivateProfileString(strFlag, _T("Channel"), strValue, strFile);
		strValue.Format(_T("%d"), pDyePara->bUseMOT);
		::WritePrivateProfileString(strFlag, _T("UseMOT"), strValue, strFile);
		strValue.Format(_T("%d"), pDyePara->iAheadCycle);
		::WritePrivateProfileString(strFlag, _T("AheadCycle"), strValue, strFile);
		strValue.Format(_T("%.3f"), pDyePara->fRatio);
		::WritePrivateProfileString(strFlag, _T("Ratio"), strValue, strFile);
	}

	return TRUE;
}

// 写入数据到文件中
BOOL CCalibrateParaTurbidity::WriteFile(CFile* pFile)
{
	pFile->Write((const char *)&m_iChannelMOT, sizeof(int));
	pFile->Write((const char *)&m_fMinRatio, sizeof(float));

	int nParaCount = m_arrayDyePara.GetCount();
	pFile->Write((const char*)&nParaCount, sizeof(int));
	for (int i=0; i<nParaCount; i++)
	{
		tagTurbidityPara* pAdjust = (tagTurbidityPara*)m_arrayDyePara.GetAt(i);
		CPublicFun::WriteStringToFile(pFile, pAdjust->strDyeName);
		pFile->Write((const char *)&pAdjust->iChannel, sizeof(int));
		pFile->Write((const char *)&pAdjust->bUseMOT, sizeof(BOOL));
		pFile->Write((const char *)&pAdjust->iAheadCycle, sizeof(int));
		pFile->Write((const char *)&pAdjust->fRatio, sizeof(float));
	}

	return TRUE;
}

BOOL CCalibrateParaTurbidity::ReadFile(CFile* pFile, WORD wVersion)
{
	DeleteAll();

	pFile->Read((char*)&m_iChannelMOT, sizeof(int));
	pFile->Read((char*)&m_fMinRatio, sizeof(float));

	int nParaCount = 0;
	pFile->Read(&nParaCount, sizeof(int));
	for (int i=0; i<nParaCount; i++)
	{
		tagTurbidityPara* pAdjust = new tagTurbidityPara;
		m_arrayDyePara.Add(pAdjust);
		pAdjust->strDyeName = CPublicFun::ReadStringFromFile(pFile);
		if (wVersion >= wPCRVersion102)
		{
			pFile->Read((char*)&pAdjust->iChannel, sizeof(int));
		}

		pFile->Read((char*)&pAdjust->bUseMOT, sizeof(BOOL));
		pFile->Read((char*)&pAdjust->iAheadCycle, sizeof(int));
		pFile->Read((char*)&pAdjust->fRatio, sizeof(float));
	}

	return TRUE;
}

CCalibrateParaTurbidity::tagTurbidityPara* CCalibrateParaTurbidity::GetParaBy(CString strDye)
{
	tagTurbidityPara* pAdjust = NULL;
	int iCount = m_arrayDyePara.GetCount();
	for (int i=0; i<iCount; i++)
	{
		tagTurbidityPara* pTemp = (tagTurbidityPara*)m_arrayDyePara.GetAt(i);
		if (strDye == pTemp->strDyeName)
		{
			pAdjust = pTemp;
			break;
		}
	}

	return pAdjust;
}

int CCalibrateParaTurbidity::GetChannel(CString strDye)
{
	tagTurbidityPara* pAdjust = GetParaBy(strDye);
	if (pAdjust != NULL)
	{
		return pAdjust->iChannel;
	}

	return -1;
}


BOOL CCalibrateParaTurbidity::UseCalibrate(CString strDye)
{
	tagTurbidityPara* pAdjust = GetParaBy(strDye);
	if (pAdjust != NULL)
	{
		return pAdjust->bUseMOT;
	}

	return FALSE;
}

BOOL CCalibrateParaTurbidity::UseCalibrate(int iChannel)
{
	int iCount = m_arrayDyePara.GetCount();
	for (int i=0; i<iCount; i++)
	{
		tagTurbidityPara* pTemp = (tagTurbidityPara*)m_arrayDyePara.GetAt(i);
		if (iChannel == pTemp->iChannel)
		{
			return pTemp->bUseMOT;
		}
	}

	return FALSE;

}

void CCalibrateParaTurbidity::EnableUseCalibrate(CString strDye, BOOL bEnable)
{
	tagTurbidityPara* pAdjust = GetParaBy(strDye);
	if (pAdjust != NULL)
	{
		pAdjust->bUseMOT = bEnable;
	}
}


int CCalibrateParaTurbidity::GetAheadCycle(CString strDye)
{
	tagTurbidityPara* pAdjust = GetParaBy(strDye);
	if (pAdjust != NULL)
	{
		return pAdjust->iAheadCycle;
	}

	//ASSERT(FALSE);
	return -1;
}

void CCalibrateParaTurbidity::SetAheadCycle(CString strDye, int iCycyle)
{
	tagTurbidityPara* pAdjust = GetParaBy(strDye);
	if (pAdjust != NULL)
	{
		pAdjust->iAheadCycle = iCycyle;
	}
}

float CCalibrateParaTurbidity::GetAdjustRatio(CString strDye)
{
	tagTurbidityPara* pAdjust = GetParaBy(strDye);
	if (pAdjust != NULL)
	{
		return pAdjust->fRatio;
	}

	//ASSERT(FALSE);
	return 0.1f;
}

void CCalibrateParaTurbidity::SetAdjustRatio(CString strDye, float fRatio)
{
	tagTurbidityPara* pAdjust = GetParaBy(strDye);
	if (pAdjust != NULL)
	{
		pAdjust->fRatio = fRatio;
	}
}


CString CCalibrateParaTurbidity::GetDye(int iIndex)
{
	CString strDye;
	strDye.Empty();

	if (iIndex >=0 && iIndex < m_arrayDyePara.GetCount())
	{
		tagTurbidityPara* pAdjust = (tagTurbidityPara*)m_arrayDyePara.GetAt(iIndex);
		strDye = pAdjust->strDyeName;
	}

	return strDye;
}

void CCalibrateParaTurbidity::ResetChannel(CPtrArray* pArrayDyeChannel)
{
	for (int i=0; i<m_arrayDyePara.GetCount(); i++)
	{
		tagTurbidityPara* pAdjust = (tagTurbidityPara*)m_arrayDyePara.GetAt(i);
		if (pAdjust == NULL)
			continue;

		for (int j=0; j<pArrayDyeChannel->GetCount(); j++)
		{
			tagDyeChannelInfo* pChannel = (tagDyeChannelInfo*)pArrayDyeChannel->GetAt(j);
			if (pChannel == NULL)
				continue;

			if (pAdjust->strDyeName == pChannel->strDye)
			{
				pAdjust->iChannel = pChannel->iChannel;
				break;
			}
		}

	}
}
