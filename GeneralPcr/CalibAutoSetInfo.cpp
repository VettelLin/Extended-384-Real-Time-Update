#include "StdAfx.h"
#include "CalibAutoSetInfo.h"


CCalibAutoSetInfo::CCalibAutoSetInfo(void)
{
	m_repeatNum = 1;
	m_dStartDensity = 10000;
	m_eDilutionDirection = EDD_INCREASE;
	m_dilutionMulitiples = 10;
	m_eSamSortWay = ESSW_HORI;
	m_selectHoleNum = 0;
}


CCalibAutoSetInfo::~CCalibAutoSetInfo(void)
{
	
}

CCalibAutoSetInfo::CCalibAutoSetInfo(const CCalibAutoSetInfo& calibAutoSetInfo)
{
	m_repeatNum = calibAutoSetInfo.m_repeatNum;
	m_dStartDensity = calibAutoSetInfo.m_dStartDensity;
	m_eDilutionDirection = calibAutoSetInfo.m_eDilutionDirection;
	m_dilutionMulitiples = calibAutoSetInfo.m_dilutionMulitiples;
	m_eSamSortWay = calibAutoSetInfo.m_eSamSortWay;
	m_selectHoleNum = calibAutoSetInfo.m_selectHoleNum;
}

void CCalibAutoSetInfo::operator=(const CCalibAutoSetInfo& calibAutoSetInfo)
{
	m_repeatNum = calibAutoSetInfo.m_repeatNum;
	m_dStartDensity = calibAutoSetInfo.m_dStartDensity;
	m_eDilutionDirection = calibAutoSetInfo.m_eDilutionDirection;
	m_dilutionMulitiples = calibAutoSetInfo.m_dilutionMulitiples;
	m_eSamSortWay = calibAutoSetInfo.m_eSamSortWay;
	m_selectHoleNum = calibAutoSetInfo.m_selectHoleNum;
}

