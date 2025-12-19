#include "StdAfx.h"
#include "DyeTargetInfo.h"
#include "PublicFun.h"

CDyeTargetInfo::CDyeTargetInfo(void)
{
	m_bSelected = FALSE;

	m_strDye.Empty();
	m_strTarget.Empty();
	m_strQuencher.Empty();
	m_iChannel = 0;
	m_dQuantity = 0;
	m_iQuanIdpd = 4;
	m_clrTarget = RGB(255,255,255);

	m_bAutoBaseline = TRUE;
	m_iBaselineStart = 3;
	m_iBaselineEnd = 8;
	m_bAutoThreshold = TRUE;
	m_dThreshold = 0;

	m_dBaselineAverage = 0;
	m_bHasSndDrvCt = FALSE;
	m_dSndDrvCt = 0;

	m_dCt = 0;
	m_dCalcQuantity = -1;	//浓度都是正数，默认值设置为-1，用于识别是否是可显示的反算浓度
	m_dRepeatCt = 0;
	m_dRepeatQuantity = 0;

	m_iTmCount = 1;
	m_pMeltTm = new double[1];

	if(NULL != m_pMeltTm)
	{
		m_pMeltTm[0] = 0;
	}
	m_pMeltRm = new double[1];
	if(NULL != m_pMeltRm)
	{
		m_pMeltRm[0] = 0;
	}

	m_eAnaRst = EAR_UNHANDLED;

	m_iCycleOfPlateau = -1;
}

CDyeTargetInfo::~CDyeTargetInfo(void)
{
	DeleteAll();
}

void CDyeTargetInfo::DeleteAll()
{
	if(NULL != m_pMeltTm)
	{
		delete [] m_pMeltTm;
		m_pMeltTm = NULL;
	}
	
	if(NULL != m_pMeltRm)
	{
		delete [] m_pMeltRm;
		m_pMeltRm = NULL;
	}
	
	m_iTmCount = 0;
}

CDyeTargetInfo::CDyeTargetInfo(const CDyeTargetInfo& dyeTargetInfo)
{
	m_bSelected = dyeTargetInfo.m_bSelected;
	m_strDye = dyeTargetInfo.m_strDye;
	m_strTarget = dyeTargetInfo.m_strTarget;
	m_strQuencher = dyeTargetInfo.m_strQuencher;
	m_iChannel = dyeTargetInfo.m_iChannel;
	m_dQuantity = dyeTargetInfo.m_dQuantity;
	m_iQuanIdpd = dyeTargetInfo.m_iQuanIdpd;
	m_clrTarget = dyeTargetInfo.m_clrTarget;

	m_bAutoBaseline = dyeTargetInfo.m_bAutoBaseline;
	m_iBaselineStart = dyeTargetInfo.m_iBaselineStart;
	m_iBaselineEnd = dyeTargetInfo.m_iBaselineEnd;
	m_bAutoThreshold = dyeTargetInfo.m_bAutoThreshold;
	m_dThreshold = dyeTargetInfo.m_dThreshold;

	m_dCt = dyeTargetInfo.m_dCt;
	m_dCalcQuantity = dyeTargetInfo.m_dCalcQuantity;
	m_dRepeatCt = dyeTargetInfo.m_dRepeatCt;
	m_dRepeatQuantity = dyeTargetInfo.m_dRepeatQuantity;

	DeleteAll();
	m_iTmCount = dyeTargetInfo.m_iTmCount;
	if(m_iTmCount > 0)
	{
		m_pMeltTm = new double[m_iTmCount];
		if(NULL != m_pMeltTm)
		{
			for(int i = 0;i < m_iTmCount;++i)
			{
				m_pMeltTm[i] = dyeTargetInfo.m_pMeltTm[i]; 
			}
		}

		m_pMeltRm = new double[m_iTmCount];
		if(NULL != m_pMeltRm)
		{
			for(int i = 0;i < m_iTmCount;++i)
			{
				m_pMeltRm[i] = dyeTargetInfo.m_pMeltRm[i]; 
			}
		}
	}

	m_eAnaRst = dyeTargetInfo.m_eAnaRst;

	return;
}

void CDyeTargetInfo::operator=(const CDyeTargetInfo& dyeTargetInfo)
{
	m_bSelected = dyeTargetInfo.m_bSelected;
	m_strDye = dyeTargetInfo.m_strDye;
	m_strTarget = dyeTargetInfo.m_strTarget;
	m_strQuencher = dyeTargetInfo.m_strQuencher;
	m_iChannel = dyeTargetInfo.m_iChannel;
	m_dQuantity = dyeTargetInfo.m_dQuantity;
	m_iQuanIdpd = dyeTargetInfo.m_iQuanIdpd;
	m_clrTarget = dyeTargetInfo.m_clrTarget;

	m_bAutoBaseline = dyeTargetInfo.m_bAutoBaseline;
	m_iBaselineStart = dyeTargetInfo.m_iBaselineStart;
	m_iBaselineEnd = dyeTargetInfo.m_iBaselineEnd;
	m_bAutoThreshold = dyeTargetInfo.m_bAutoThreshold;
	m_dThreshold = dyeTargetInfo.m_dThreshold;

	m_dCt = dyeTargetInfo.m_dCt;
	m_dCalcQuantity = dyeTargetInfo.m_dCalcQuantity;
	m_dRepeatCt = dyeTargetInfo.m_dRepeatCt;
	m_dRepeatQuantity = dyeTargetInfo.m_dRepeatQuantity;

	DeleteAll();
	m_iTmCount = dyeTargetInfo.m_iTmCount;
	if(m_iTmCount > 0)
	{
		m_pMeltTm = new double[m_iTmCount];
		if(NULL != m_pMeltTm)
		{
			for(int i = 0;i < m_iTmCount;++i)
			{
				m_pMeltTm[i] = dyeTargetInfo.m_pMeltTm[i]; 
			}
		}

		m_pMeltRm = new double[m_iTmCount];
		if(NULL != m_pMeltRm)
		{
			for(int i = 0;i < m_iTmCount;++i)
			{
				m_pMeltRm[i] = dyeTargetInfo.m_pMeltRm[i]; 
			}
		}
	}

	m_eAnaRst = dyeTargetInfo.m_eAnaRst;

	return;
}

BOOL CDyeTargetInfo::WriteFile(CFile* pFile,WORD wVersion)
{
	pFile->Write((const char *)&m_bSelected, sizeof(BOOL));
	CPublicFun::WriteStringToFile(pFile, m_strDye);
	CPublicFun::WriteStringToFile(pFile, m_strTarget);
	CPublicFun::WriteStringToFile(pFile, m_strQuencher);
	pFile->Write((const char *)&m_iChannel, sizeof(int));
	pFile->Write((const char *)&m_dQuantity, sizeof(double));
	pFile->Write((const char *)&m_clrTarget, sizeof(COLORREF));

	pFile->Write((const char *)&m_bAutoBaseline, sizeof(BOOL));
	pFile->Write((const char *)&m_iBaselineStart, sizeof(int));
	pFile->Write((const char *)&m_iBaselineEnd, sizeof(int));
	pFile->Write((const char *)&m_bAutoThreshold, sizeof(BOOL));
	pFile->Write((const char *)&m_dThreshold, sizeof(double));

	pFile->Write((const char *)&m_iTmCount, sizeof(int));

	pFile->Write((const char *)&m_eAnaRst,sizeof(EANALYSERESULT));

	if(wVersion >= wPCRVersion115)
	{
		pFile->Write((const char *)&m_iQuanIdpd,sizeof(int));
	}

	return TRUE;
}

BOOL CDyeTargetInfo::ReadFile(CFile* pFile, WORD wVersion)
{
	DeleteAll();

	pFile->Read(&m_bSelected, sizeof(BOOL));
	m_strDye = CPublicFun::ReadStringFromFile(pFile);
	m_strTarget = CPublicFun::ReadStringFromFile(pFile);
	m_strQuencher = CPublicFun::ReadStringFromFile(pFile);
	pFile->Read(&m_iChannel, sizeof(int));
	pFile->Read(&m_dQuantity, sizeof(double));
	if (wVersion >= wPCRVersion101)
	{
		pFile->Read((char *)&m_clrTarget, sizeof(COLORREF));
	}

	pFile->Read(&m_bAutoBaseline, sizeof(BOOL));
	pFile->Read(&m_iBaselineStart, sizeof(int));
	pFile->Read(&m_iBaselineEnd, sizeof(int));
	pFile->Read(&m_bAutoThreshold, sizeof(BOOL));
	pFile->Read(&m_dThreshold, sizeof(double));

	pFile->Read(&m_iTmCount, sizeof(int));
	SetTmCount(m_iTmCount);

	if(wVersion >= wPCRVersion105)
	{
		pFile->Read(&m_eAnaRst,sizeof(EANALYSERESULT));
	}

	if(wVersion >= wPCRVersion115)
	{
		pFile->Read(&m_iQuanIdpd,sizeof(int));
	}

	return TRUE;
}

void CDyeTargetInfo::SetTmCount(int nCount)
{
	if (m_iTmCount != nCount)
	{
		SAFE_DELETE(m_pMeltTm);
		SAFE_DELETE(m_pMeltRm);
		m_iTmCount = nCount;
	}

	if (m_pMeltTm == NULL)
	{
		m_pMeltTm = new double[m_iTmCount];
		for(int i = 0;i < m_iTmCount;++i)
		{
			m_pMeltTm[i] = 0;
		}
	}
	if (m_pMeltRm == NULL)
	{
		m_pMeltRm = new double[m_iTmCount];
		for(int i = 0;i < m_iTmCount;++i)
		{
			m_pMeltRm[i] = 0;
		}
	}
}

void CDyeTargetInfo::SetBeSelected(BOOL bSelected)
{
	m_bSelected = bSelected;
}

BOOL CDyeTargetInfo::GetBeSelected()
{
	return m_bSelected;
}

void CDyeTargetInfo::SetQuantity(double dQuantity,int iDpd)
{
	m_dQuantity = dQuantity;
	m_iQuanIdpd = iDpd;
	return;
}

double CDyeTargetInfo::GetQuantity()
{
	return m_dQuantity;
}

int CDyeTargetInfo::GetQuanIDPD()
{
	return m_iQuanIdpd;
}

double CDyeTargetInfo::GetTm1()
{
	if(NULL == m_pMeltTm || m_iTmCount < 1)
	{
		return -1;
	}

	return m_pMeltTm[0];
}

CString CDyeTargetInfo::GetTm1String()
{
	if(NULL == m_pMeltTm || m_iTmCount < 1)
	{
		return _T("");
	}

	if(m_pMeltTm[0] <= 0)
	{
		return _T("");
	}

	CString strTemp(_T(""));
	strTemp.Format(_T("%.2lf"),m_pMeltTm[0]);

	return strTemp;
}

void CDyeTargetInfo::SetTm(int nCount, double* pdTm, double* pdRm)
{
	SetTmCount(nCount);
	for (int i=0; i<nCount; i++)
	{
		m_pMeltTm[i] = pdTm[i];
		m_pMeltRm[i] = pdRm[i];
	}
}

double CDyeTargetInfo::GetCalcValue(eCalculateValueType nType)
{
	double dValue = 0;
	switch(nType)
	{
	case VALUE_CT:
		dValue = m_dCt;
		break;
	case VALUE_REPEAT_CT:
		dValue = m_dRepeatCt;
		break;
	case VALUE_SET_QUANTITY:
		dValue = m_dQuantity;
		break;
	case VALUE_CALC_QUANTITY:
		dValue = m_dCalcQuantity;
		break;
	case VALUE_REPEAT_QUANTITY:
		dValue = m_dRepeatQuantity;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return dValue;
}

void CDyeTargetInfo::SetCalcValue(eCalculateValueType nType, double dValue)
{
	switch(nType)
	{
	case VALUE_CT:
		if (m_bHasSndDrvCt)
			m_dCt = dValue;
		else
			m_dCt = 0;
		break;
	case VALUE_REPEAT_CT:
		m_dRepeatCt = dValue;
		break;
	case VALUE_SET_QUANTITY:
		m_dQuantity = dValue;
		break;
	case VALUE_CALC_QUANTITY:
		m_dCalcQuantity = dValue;
		break;
	case VALUE_REPEAT_QUANTITY:
		m_dRepeatQuantity = dValue;
		break;
	case VALUE_TM:
		if(NULL != m_pMeltTm)
		{
			m_pMeltTm[0] = dValue;
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}
