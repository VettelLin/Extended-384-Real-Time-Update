#include "StdAfx.h"
#include "SampleInfo.h"
#include "LanguageInfo.h"

CSampleInfo::CSampleInfo(void)
{
	m_strPatientID.Empty();
	m_strClinicID.Empty();
	m_strCaseID.Empty();
	m_strPatientName.Empty();
	m_nPatientSex = SEXUALITY_NONE;
	m_nPatientAge = -1;
	m_nAgeUnit = AGE_UNIT_YEAR;
	m_strAreaNo.Empty();
	m_strRoomNo.Empty();
	m_strBedNo.Empty();
	m_strProposer.Empty();
	m_strDepartment.Empty();
	m_strHospital.Empty();
	m_strApplyDate.Empty();

	time_t timer = time(NULL);	struct tm* localTime = localtime(&timer);	m_strCollectDate.Format(_T("%d%s%02d%s%02d%s%02d%s%02d%s%02d"),localTime->tm_year + 1900,_T("-"),localTime->tm_mon + 1,_T("-"),localTime->tm_mday,_T(" "),localTime->tm_hour,_T(":"),localTime->tm_min,_T(":"),localTime->tm_sec);
}

CSampleInfo::~CSampleInfo(void)
{
}

CSampleInfo::CSampleInfo(const CSampleInfo& samInfo)
{
	m_strPatientID = samInfo.m_strPatientID;
	m_strClinicID = samInfo.m_strClinicID;
	m_strCaseID = samInfo.m_strCaseID;
	m_strPatientName = samInfo.m_strPatientName;
	m_nPatientSex = samInfo.m_nPatientSex;
	m_nPatientAge = samInfo.m_nPatientAge;
	m_nAgeUnit = samInfo.m_nAgeUnit;
	m_strAreaNo = samInfo.m_strAreaNo;
	m_strRoomNo = samInfo.m_strRoomNo;
	m_strBedNo = samInfo.m_strBedNo;
	m_strProposer = samInfo.m_strProposer;
	m_strDepartment = samInfo.m_strDepartment;
	m_strHospital = samInfo.m_strHospital;
	m_strCollectDate = samInfo.m_strCollectDate;
	m_strApplyDate = samInfo.m_strApplyDate;
}

void CSampleInfo::operator=(const CSampleInfo& samInfo)
{
	m_strPatientID = samInfo.m_strPatientID;
	m_strClinicID = samInfo.m_strClinicID;
	m_strCaseID = samInfo.m_strCaseID;
	m_strPatientName = samInfo.m_strPatientName;
	m_nPatientSex = samInfo.m_nPatientSex;
	m_nPatientAge = samInfo.m_nPatientAge;
	m_nAgeUnit = samInfo.m_nAgeUnit;
	m_strAreaNo = samInfo.m_strAreaNo;
	m_strRoomNo = samInfo.m_strRoomNo;
	m_strBedNo = samInfo.m_strBedNo;
	m_strProposer = samInfo.m_strProposer;
	m_strDepartment = samInfo.m_strDepartment;
	m_strHospital = samInfo.m_strHospital;
	m_strCollectDate = samInfo.m_strCollectDate;
	m_strApplyDate = samInfo.m_strApplyDate;
}

BOOL CSampleInfo::operator==(const CSampleInfo& samInfo)
{
	return m_strPatientID == samInfo.m_strPatientID && 
	m_strClinicID == samInfo.m_strClinicID && 
	m_strCaseID == samInfo.m_strCaseID && 
	m_strPatientName == samInfo.m_strPatientName && 
	m_nPatientSex == samInfo.m_nPatientSex && 
	m_nPatientAge == samInfo.m_nPatientAge && 
	m_nAgeUnit == samInfo.m_nAgeUnit && 
	m_strAreaNo == samInfo.m_strAreaNo && 
	m_strRoomNo == samInfo.m_strRoomNo && 
	m_strBedNo == samInfo.m_strBedNo && 
	m_strProposer == samInfo.m_strProposer && 
	m_strDepartment == samInfo.m_strDepartment && 
	m_strHospital == samInfo.m_strHospital && 
	m_strCollectDate == samInfo.m_strCollectDate && 
	m_strApplyDate == samInfo.m_strApplyDate;
}

CString CSampleInfo::GetCollectYMD()
{
	int iIndex = m_strCollectDate.Find(_T(' '));
	if(-1 != iIndex)
	{
		return m_strCollectDate.Left(iIndex);
	}

	return _T("");
}

void CSampleInfo::SetCollectDateYMD(CString strDate)
{
	if(strDate.IsEmpty())
	{
		return;
	}

	int iIndex = strDate.Find(_T(' '));
	if(-1 == iIndex)
	{
		strDate += _T(" 00:00:00");
	}
	
	m_strCollectDate = strDate;

	return;
}

void CSampleInfo::SetSQMCollectDateYMD(CString strDate)
{
	if(strDate.IsEmpty())
	{
		return;
	}

	int iIndex = strDate.Find(_T('\''));
	if(strDate.GetLength() - 1 != iIndex)
	{
		if(strDate.GetLength() >= 10)
		{
			strDate = strDate.Left(10);
		}
		else
		{
			return;
		}
	}
	else
	{
		strDate = strDate.Left(strDate.GetLength() - 1);
	}

	
	if(strDate.IsEmpty())
	{
		return;
	}

	m_strCollectDate = strDate;
	iIndex = m_strCollectDate.Find(_T(' '));
	if(-1 == iIndex && m_strCollectDate.GetLength() <= 10)
	{
		m_strCollectDate += _T(" 00:00:00");
	}

	return;
}

void CSampleInfo::Reset()
{
	m_strPatientID.Empty();
	m_strClinicID.Empty();
	m_strCaseID.Empty();
	m_strPatientName.Empty();
	m_nPatientSex = SEXUALITY_NONE;
	m_nPatientAge = -1;
	m_nAgeUnit = AGE_UNIT_YEAR;
	m_strAreaNo.Empty();
	m_strRoomNo.Empty();
	m_strBedNo.Empty();
	m_strProposer.Empty();
	m_strDepartment.Empty();
	m_strHospital.Empty();
	m_strCollectDate.Empty();
	m_strApplyDate.Empty();
}

void CSampleInfo::SetSexuality(CString strData)
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	if(pLangInfo->GetText(182,LANGUAGE_CHS) == strData || pLangInfo->GetText(182,LANGUAGE_ENG) == strData)
	{
		m_nPatientSex = SEXUALITY_MALE;
	}
	else if(pLangInfo->GetText(183,LANGUAGE_CHS) == strData || pLangInfo->GetText(183,LANGUAGE_ENG) == strData)
	{
		m_nPatientSex = SEXUALITY_FEMALE;
	}
	else
	{
		m_nPatientSex = SEXUALITY_NONE;
	}

	return;
}

CString CSampleInfo::GetSexualityString()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	if(SEXUALITY_MALE == m_nPatientSex)
	{
		return pLangInfo->GetText(182);
	}
	else if(SEXUALITY_FEMALE == m_nPatientSex)
	{
		return pLangInfo->GetText(183);
	}
	else
	{
		 return _T("");
	}
}

CString CSampleInfo::GetAgeString()
{
	CString strTemp(_T(""));
	if(m_nPatientAge > 0)
	{
		strTemp.Format(_T("%d"),m_nPatientAge); 
	}
	
	return strTemp; 
}


