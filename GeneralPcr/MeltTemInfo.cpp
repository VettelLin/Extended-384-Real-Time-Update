#include "StdAfx.h"
#include "MeltTemInfo.h"

CMeltTemInfo::CMeltTemInfo(void)
{
	m_dTargetTem = 0;
	m_dPeltier1Tem = 0;   
	m_dPeltier2Tem = 0;	
	m_dPeltier3Tem = 0;	

	m_dPeltier1StartTem = 0; 
	m_dPeltier1EndTem = 0;   
	m_dPeltier2StartTem = 0; 
	m_dPeltier2EndTem = 0;   
	m_dPeltier3StartTem = 0; 
	m_dPeltier3EndTem = 0;
}


CMeltTemInfo::~CMeltTemInfo(void)
{
}

CMeltTemInfo::CMeltTemInfo(const CMeltTemInfo& meltTemInfo)
{
	m_dTargetTem = meltTemInfo.m_dTargetTem;
	m_dPeltier1Tem = meltTemInfo.m_dPeltier1Tem;   
	m_dPeltier2Tem = meltTemInfo.m_dPeltier2Tem;	
	m_dPeltier3Tem = meltTemInfo.m_dPeltier3Tem;	

	m_dPeltier1StartTem = meltTemInfo.m_dPeltier1StartTem; 
	m_dPeltier1EndTem = meltTemInfo.m_dPeltier1EndTem;   
	m_dPeltier2StartTem = meltTemInfo.m_dPeltier2StartTem; 
	m_dPeltier2EndTem = meltTemInfo.m_dPeltier2EndTem;   
	m_dPeltier3StartTem = meltTemInfo.m_dPeltier3StartTem; 
	m_dPeltier3EndTem = meltTemInfo.m_dPeltier3EndTem;
}

void CMeltTemInfo::operator=(const CMeltTemInfo& meltTemInfo)
{
	m_dTargetTem = meltTemInfo.m_dTargetTem;
	m_dPeltier1Tem = meltTemInfo.m_dPeltier1Tem;   
	m_dPeltier2Tem = meltTemInfo.m_dPeltier2Tem;	
	m_dPeltier3Tem = meltTemInfo.m_dPeltier3Tem;	

	m_dPeltier1StartTem = meltTemInfo.m_dPeltier1StartTem; 
	m_dPeltier1EndTem = meltTemInfo.m_dPeltier1EndTem;   
	m_dPeltier2StartTem = meltTemInfo.m_dPeltier2StartTem; 
	m_dPeltier2EndTem = meltTemInfo.m_dPeltier2EndTem;   
	m_dPeltier3StartTem = meltTemInfo.m_dPeltier3StartTem; 
	m_dPeltier3EndTem = meltTemInfo.m_dPeltier3EndTem;
}



