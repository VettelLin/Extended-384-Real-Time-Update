#include "StdAfx.h"
#include "RunProgressRecord.h"

CRunProgressRecord::CRunProgressRecord(void)
{
	m_iPageId = -1;
	m_iSegId = 0; 
	m_iStepId = 0;
	m_iCycleId = 0;
	m_strFilePath.Empty();
	m_strSN.Empty();

	m_ttStart = 0;
	m_fMeltStartTem = 0;
	m_fMeltEndTem = 0;
	m_fPowerOffTem = 0;
}

CRunProgressRecord::~CRunProgressRecord(void)
{

}

CRunProgressRecord::CRunProgressRecord(const CRunProgressRecord& info)
{
	m_iPageId = info.m_iPageId;
	m_iSegId = info.m_iSegId; 
	m_iStepId = info.m_iStepId;
	m_iCycleId = info.m_iCycleId;
	m_strFilePath = info.m_strFilePath;
	m_strSN = info.m_strSN;

	m_ttStart = info.m_ttStart;
	m_fMeltStartTem = info.m_fMeltStartTem;
	m_fMeltEndTem = info.m_fMeltEndTem;
	m_fPowerOffTem = info.m_fPowerOffTem;
}

void CRunProgressRecord::operator=(const CRunProgressRecord& info)
{
	m_iPageId = info.m_iPageId;
	m_iSegId = info.m_iSegId; 
	m_iStepId = info.m_iStepId;
	m_iCycleId = info.m_iCycleId;
	m_strFilePath = info.m_strFilePath;
	m_strSN = info.m_strSN;

	m_ttStart = info.m_ttStart;
	m_fMeltStartTem = info.m_fMeltStartTem;
	m_fMeltEndTem = info.m_fMeltEndTem;
	m_fPowerOffTem = info.m_fPowerOffTem;
}




