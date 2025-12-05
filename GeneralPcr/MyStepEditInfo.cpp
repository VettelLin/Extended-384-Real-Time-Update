#include "StdAfx.h"
#include "MyStepEditInfo.h"


CMyStepEditInfo::CMyStepEditInfo(void)
{
	m_strSegName = _T("");
	m_strStepName = _T("");
	m_fAimTem = 50.0;
	m_bSelGradient = FALSE;
	m_fGradientMaxTem = 90.0;
	m_holdSec = 10;
	m_iCollect = FALSE;
	m_fRate = 4.0;	
	m_fIncrement = 0.5;
	m_bCollectExistExceptThisStep = FALSE;
	m_bCollectAllExistExceptThisStep = FALSE;
	m_bMeltSeg = FALSE;
}

CMyStepEditInfo::~CMyStepEditInfo(void)
{
}

CMyStepEditInfo::CMyStepEditInfo(const CMyStepEditInfo& stepEditInfo)
{
	m_strSegName = stepEditInfo.m_strSegName;
	m_strStepName = stepEditInfo.m_strStepName;
	m_fAimTem = stepEditInfo.m_fAimTem;
	m_bSelGradient = stepEditInfo.m_bSelGradient;
	m_fGradientMaxTem = stepEditInfo.m_fGradientMaxTem;
	m_holdSec = stepEditInfo.m_holdSec;
	m_iCollect = stepEditInfo.m_iCollect;
	m_fRate = stepEditInfo.m_fRate;	
	m_fIncrement = stepEditInfo.m_fIncrement;
	m_bCollectExistExceptThisStep = stepEditInfo.m_bCollectExistExceptThisStep;
	m_bCollectAllExistExceptThisStep = stepEditInfo.m_bCollectAllExistExceptThisStep;
	m_bMeltSeg = stepEditInfo.m_bMeltSeg;
}

void CMyStepEditInfo::operator=(const CMyStepEditInfo& stepEditInfo)
{
	m_strSegName = stepEditInfo.m_strSegName;
	m_strStepName = stepEditInfo.m_strStepName;
	m_fAimTem = stepEditInfo.m_fAimTem;
	m_bSelGradient = stepEditInfo.m_bSelGradient;
	m_fGradientMaxTem = stepEditInfo.m_fGradientMaxTem;
	m_holdSec = stepEditInfo.m_holdSec;
	m_iCollect = stepEditInfo.m_iCollect;
	m_fRate = stepEditInfo.m_fRate;	
	m_fIncrement = stepEditInfo.m_fIncrement;
	m_bCollectExistExceptThisStep = stepEditInfo.m_bCollectExistExceptThisStep;
	m_bCollectAllExistExceptThisStep = stepEditInfo.m_bCollectAllExistExceptThisStep;
	m_bMeltSeg = stepEditInfo.m_bMeltSeg;
}


