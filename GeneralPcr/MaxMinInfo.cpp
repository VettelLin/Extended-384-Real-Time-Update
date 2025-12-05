#include "StdAfx.h"
#include "MaxMinInfo.h"


CMaxMinInfo::CMaxMinInfo(void)
{
	m_bOnlyIntergerNumber = FALSE;	
	m_bOnlyFloatNumber = FALSE;	
	m_bSetMaxValue = FALSE;	
	m_dMaxValue = 1;	
	m_bMaxEqualSignWork = TRUE;	
	m_bSetMinValue = FALSE;	
	m_dMinValue = 0;	
	m_bMinEqualSignWork = TRUE;	
	m_bAllowNegtiveNumber = TRUE;
	m_bMaxMinSimultaneousEffect = FALSE;
	m_bLessThanMinSetMin = TRUE;
}

CMaxMinInfo::~CMaxMinInfo(void)
{
}

CMaxMinInfo::CMaxMinInfo(const CMaxMinInfo& minMaxInfo)
{
	m_bOnlyIntergerNumber = minMaxInfo.m_bOnlyIntergerNumber;	
	m_bOnlyFloatNumber = minMaxInfo.m_bOnlyFloatNumber;	
	m_bSetMaxValue = minMaxInfo.m_bSetMaxValue;	
	m_dMaxValue = minMaxInfo.m_dMaxValue;	
	m_bMaxEqualSignWork = minMaxInfo.m_bMaxEqualSignWork;	
	m_bSetMinValue = minMaxInfo.m_bSetMinValue;	
	m_dMinValue = minMaxInfo.m_dMinValue;	
	m_bMinEqualSignWork = minMaxInfo.m_bMinEqualSignWork;
	m_bAllowNegtiveNumber = minMaxInfo.m_bAllowNegtiveNumber;
	m_bMaxMinSimultaneousEffect = minMaxInfo.m_bMaxMinSimultaneousEffect;
	m_bLessThanMinSetMin = minMaxInfo.m_bLessThanMinSetMin;
}

void CMaxMinInfo::operator=(const CMaxMinInfo& minMaxInfo)
{
	m_bOnlyIntergerNumber = minMaxInfo.m_bOnlyIntergerNumber;	
	m_bOnlyFloatNumber = minMaxInfo.m_bOnlyFloatNumber;	
	m_bSetMaxValue = minMaxInfo.m_bSetMaxValue;	
	m_dMaxValue = minMaxInfo.m_dMaxValue;	
	m_bMaxEqualSignWork = minMaxInfo.m_bMaxEqualSignWork;	
	m_bSetMinValue = minMaxInfo.m_bSetMinValue;	
	m_dMinValue = minMaxInfo.m_dMinValue;	
	m_bMinEqualSignWork = minMaxInfo.m_bMinEqualSignWork;	
	m_bAllowNegtiveNumber = minMaxInfo.m_bAllowNegtiveNumber;
	m_bMaxMinSimultaneousEffect = minMaxInfo.m_bMaxMinSimultaneousEffect;
	m_bLessThanMinSetMin = minMaxInfo.m_bLessThanMinSetMin;
}

void CMaxMinInfo::SetBeOnlyIntergerNumber(BOOL beOnlyIntergerNumber)
{
	m_bOnlyIntergerNumber = beOnlyIntergerNumber;
}

BOOL CMaxMinInfo::GetBeOnlyIntergerNumber()
{
	return m_bOnlyIntergerNumber;
}

void CMaxMinInfo::SetBeOnlyFloatNumber(BOOL beOnlyFloatNumber)
{
	m_bOnlyFloatNumber = beOnlyFloatNumber;
}

BOOL CMaxMinInfo::GetBeOnlyFloatNumber()
{
	return m_bOnlyFloatNumber;
}

void CMaxMinInfo::SetBeSetMaxValue(BOOL bSetMaxValue)
{
	m_bSetMaxValue = bSetMaxValue;
}

BOOL CMaxMinInfo::GetBeSetMaxValue()
{
	return m_bSetMaxValue;
}


void CMaxMinInfo::SetMaxValue(double dMaxValue)
{
	m_dMaxValue = dMaxValue;
}

double CMaxMinInfo::GetMaxValue()
{
	return m_dMaxValue;
}

void CMaxMinInfo::SetBeMaxEqualSignWork(double bMaxEqualSignWork)
{
	m_bMaxEqualSignWork = bMaxEqualSignWork;
}

BOOL CMaxMinInfo::GetBeMaxEqualSignWork()
{
	return m_bMaxEqualSignWork;
}

void CMaxMinInfo::SetBeSetMinValue(BOOL bSetMinValue)
{
	m_bSetMinValue = bSetMinValue;
}

BOOL CMaxMinInfo::GetBeSetMinValue()
{
	return m_bSetMinValue;
}

void CMaxMinInfo::SetMinValue(double dMinValue)
{
	m_dMinValue = dMinValue;
}

double CMaxMinInfo::GetMinValue()
{
	return m_dMinValue;
}


void CMaxMinInfo::SetBeMinEqualSignWork(double bMinEqualSignWork)
{
	m_bMinEqualSignWork = bMinEqualSignWork;
}

BOOL CMaxMinInfo::GetBeMinEqualSignWork()
{
	return m_bMinEqualSignWork;
}

void CMaxMinInfo::SetBeAllowNegtiveNumber(BOOL bAllowNegtiveNumber)
{
	m_bAllowNegtiveNumber = bAllowNegtiveNumber;
}

BOOL CMaxMinInfo::GetBeAllowNegtiveNumber()
{
	return m_bAllowNegtiveNumber;
}

void CMaxMinInfo::SetBeMaxMinSimultaneousEffect(BOOL bMaxMinSimultaneousEffect)
{
	m_bMaxMinSimultaneousEffect = bMaxMinSimultaneousEffect;
}

BOOL CMaxMinInfo::GetBeMaxMinSimultaneousEffect()
{
	return m_bMaxMinSimultaneousEffect;
}

void CMaxMinInfo::SetBeLessThanMinSetMin(BOOL bLessThanMinSetMin)
{
	m_bLessThanMinSetMin = bLessThanMinSetMin;
}

BOOL CMaxMinInfo::GetBeLessThanMinSetMin()
{
	return m_bLessThanMinSetMin;
}

void CMaxMinInfo::ClearMaxMinLimit()
{
	m_bSetMaxValue = FALSE;
	m_bSetMinValue = FALSE;
	m_bOnlyFloatNumber = FALSE;
	m_bOnlyIntergerNumber = FALSE;
	m_bMaxMinSimultaneousEffect = FALSE;
	m_bAllowNegtiveNumber = FALSE;
	m_bMinEqualSignWork = FALSE;
	m_bMaxEqualSignWork = FALSE;
}

