#include "StdAfx.h"
#include "ExperStateInfo.h"


CExperStateInfo::CExperStateInfo(void)
{
	m_eInstruConnectState = EICS_NONE;
	m_eExperType = EET_NONE;

	m_strModel = _T("");
	m_strExperName = _T("");	
	m_strRemainTime = _T("");	
	m_strExperStartTime = _T("");	
	m_dProgress = 0;
	m_dExperTem = -1;
}

CExperStateInfo::~CExperStateInfo(void)
{

}

CExperStateInfo::CExperStateInfo(const CExperStateInfo& info)
{
	m_eInstruConnectState = info.m_eInstruConnectState;
	m_eExperType = info.m_eExperType;

	m_strModel = info.m_strModel;
	m_strExperName = info.m_strExperName;	
	m_strRemainTime = info.m_strRemainTime;	
	m_strExperStartTime = info.m_strExperStartTime;	
	m_dProgress = info.m_dProgress;
	m_dExperTem = info.m_dExperTem;
}

void CExperStateInfo::operator=(const CExperStateInfo& info)
{
	m_eInstruConnectState = info.m_eInstruConnectState;
	m_eExperType = info.m_eExperType;

	m_strModel = info.m_strModel;
	m_strExperName = info.m_strExperName;	
	m_strRemainTime = info.m_strRemainTime;	
	m_strExperStartTime = info.m_strExperStartTime;	
	m_dProgress = info.m_dProgress;
	m_dExperTem = info.m_dExperTem;
}





