#include "StdAfx.h"
#include "TubeFullFluInfo.h"

CTubeFullFluInfo::CTubeFullFluInfo(void)
{
	m_pOriginFlu = NULL;
	m_pCrossTalkedFlu = NULL;
	m_pFilteredFlu = NULL;
	m_pNormalizedFlu = NULL;
	m_pLogNormalizedFlu = NULL;
	m_pDeltaRnedFlu = NULL;
	m_pLogDeltaRnedFlu = NULL;
	m_pDMeltTem = NULL;

	m_pCtBySecDev = NULL;
	m_pCtBySpline = NULL;
	
	m_pSplineThreNormalize = NULL;
	m_pLogSplineThreNorm = NULL;
	m_pSplineThreDeltaRn = NULL;
	m_pLogSplineThreDeltaRn = NULL;

	m_pBePeakCrv = NULL;
}
 
CTubeFullFluInfo::~CTubeFullFluInfo(void)
{
	MyMulitDeletePtr(&m_pOriginFlu);
	MyMulitDeletePtr(&m_pCrossTalkedFlu);
	MyMulitDeletePtr(&m_pFilteredFlu);
	MyMulitDeletePtr(&m_pNormalizedFlu);
	MyMulitDeletePtr(&m_pLogNormalizedFlu);
	MyMulitDeletePtr(&m_pDeltaRnedFlu);
	MyMulitDeletePtr(&m_pLogDeltaRnedFlu);
	MyMulitDeletePtr(&m_pDMeltTem);

	MyMulitDeletePtr(&m_pCtBySecDev);
	MyMulitDeletePtr(&m_pCtBySpline);
	MyMulitDeletePtr(&m_pSplineThreNormalize);
	MyMulitDeletePtr(&m_pLogSplineThreNorm);
	MyMulitDeletePtr(&m_pSplineThreDeltaRn);
	MyMulitDeletePtr(&m_pLogSplineThreDeltaRn);
	MyMulitDeletePtr(&m_pBePeakCrv);
}

BOOL CTubeFullFluInfo::BeAllAllocated()
{
	if(NULL == m_pOriginFlu)
	{
		return FALSE;
	}
	if(NULL == m_pCrossTalkedFlu)
	{
		return FALSE;
	}
	if(NULL == m_pFilteredFlu)
	{
		return FALSE;
	}
	if(NULL == m_pNormalizedFlu)
	{
		return FALSE;
	}
	if(NULL == m_pLogNormalizedFlu)
	{
		return FALSE;
	}
	if(NULL == m_pDeltaRnedFlu)
	{
		return FALSE;
	}
	if(NULL == m_pLogDeltaRnedFlu)
	{
		return FALSE;
	}
	if(NULL == m_pDMeltTem)
	{
		return FALSE;
	}

	if(NULL == m_pCtBySecDev)
	{
		return FALSE;
	}

	if(NULL == m_pCtBySpline)
	{
		return FALSE;
	}

	if(NULL == m_pSplineThreNormalize)
	{
		return FALSE;
	}

	if(NULL == m_pLogSplineThreNorm)
	{
		return FALSE;
	}

	if(NULL == m_pSplineThreDeltaRn)
	{
		return FALSE;
	}

	if(NULL == m_pLogSplineThreDeltaRn)
	{
		return FALSE;
	}

	if(NULL == m_pBePeakCrv)
	{
		return FALSE;
	}

	return TRUE;
}

void CTubeFullFluInfo::Clear()
{
	MyMulitDeletePtr(&m_pOriginFlu);
	MyMulitDeletePtr(&m_pCrossTalkedFlu);
	MyMulitDeletePtr(&m_pFilteredFlu);
	MyMulitDeletePtr(&m_pNormalizedFlu);
	MyMulitDeletePtr(&m_pLogNormalizedFlu);
	MyMulitDeletePtr(&m_pDeltaRnedFlu);
	MyMulitDeletePtr(&m_pLogDeltaRnedFlu);
	MyMulitDeletePtr(&m_pDMeltTem);

	MyMulitDeletePtr(&m_pCtBySecDev);
	MyMulitDeletePtr(&m_pCtBySpline);
	MyMulitDeletePtr(&m_pSplineThreNormalize);
	MyMulitDeletePtr(&m_pLogSplineThreNorm);
	MyMulitDeletePtr(&m_pSplineThreDeltaRn);
	MyMulitDeletePtr(&m_pLogSplineThreDeltaRn);
	MyMulitDeletePtr(&m_pBePeakCrv);
}