#include "StdAfx.h"
#include "ExpStateInfo.h"


CExpStateInfo::CExpStateInfo(void)
{
	m_strExpName = _T("");	
	m_iRemainSec = 0;
	m_dCurTem = -1;
	m_dCurProgress = 0;
	m_eExpType = EXP_TYPE_ABSOLUTE;

	m_eRunBtState = ERBS_RUN;
	m_eBinBtState = EBBS_IN;
}


CExpStateInfo::~CExpStateInfo(void)
{
}

CExpStateInfo::CExpStateInfo(const CExpStateInfo& info)
{
	m_strExpName = info.m_strExpName;	
	m_iRemainSec = info.m_iRemainSec;
	m_dCurTem = info.m_dCurTem;
	m_dCurProgress = info.m_dCurProgress;
	m_eExpType = info.m_eExpType;
	m_eRunBtState = info.m_eRunBtState;
	m_eBinBtState = info.m_eBinBtState;
}

void CExpStateInfo::operator=(const CExpStateInfo& info)
{
	m_strExpName = info.m_strExpName;	
	m_iRemainSec = info.m_iRemainSec;
	m_dCurTem = info.m_dCurTem;
	m_dCurProgress = info.m_dCurProgress;
	m_eExpType = info.m_eExpType;
	m_eRunBtState = info.m_eRunBtState;
	m_eBinBtState = info.m_eBinBtState;
}

CString CExpStateInfo::GetRemainTimeStr()
{
	int iTemp = m_iRemainSec;
	if(iTemp < 0)
	{
		iTemp = 0;
	}
	int iHour = iTemp / 3600;
	iTemp = iTemp % 3600;
	int iMinute = iTemp / 60;
	int iSec = iTemp % 60;
	CString strTemp(_T(""));
	strTemp.Format(_T("%02d:%02d:%02d"),iHour,iMinute,iSec);

	return strTemp;
}

CString CExpStateInfo::GetCurrentTemperatureStr()
{
	CString strCurTemp(_T("--¡æ"));
	if(m_dCurTem < 0)
	{
		return strCurTemp;
	}
	strCurTemp.Format(_T("%.lf¡æ"),m_dCurTem);
	return strCurTemp;
}

void CExpStateInfo::Clear()
{
	m_strExpName = _T("");	
	m_iRemainSec = 0;
	m_dCurTem = -1;
	m_dCurProgress = 0;
	m_eExpType = EXP_TYPE_ABSOLUTE;
	m_eRunBtState = ERBS_RUN;
	m_eBinBtState = EBBS_IN;
}
