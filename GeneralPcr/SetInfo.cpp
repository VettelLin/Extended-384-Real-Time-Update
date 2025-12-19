#include "StdAfx.h"
#include "SetInfo.h"


CSetInfo::CSetInfo(void)
{
	m_eSamSortWay = ESSW_HORI;
	m_eSamSortDirection = ESSD_POSITIVE;
}

CSetInfo::~CSetInfo(void)
{

}

CSetInfo::CSetInfo(const CSetInfo& setInfo)
{
	m_eSamSortWay = setInfo.m_eSamSortWay;
	m_eSamSortDirection = setInfo.m_eSamSortDirection;
}

void CSetInfo::operator=(const CSetInfo& setInfo)
{
	m_eSamSortWay = setInfo.m_eSamSortWay;
	m_eSamSortDirection = setInfo.m_eSamSortDirection;
}
