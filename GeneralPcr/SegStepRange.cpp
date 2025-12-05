#include "StdAfx.h"
#include "SegStepRange.h"


CSegStepRange::CSegStepRange(void)
{
	m_segStart = -1;
	m_segEnd = -1;
	m_stepStart = -1;
	m_stepEnd = -1;
}

CSegStepRange::~CSegStepRange(void)
{

}

CSegStepRange::CSegStepRange(const CSegStepRange& segStep)
{
	m_segStart = segStep.m_segStart;
	m_segEnd = segStep.m_segEnd;
	m_stepStart = segStep.m_stepStart;
	m_stepEnd = segStep.m_stepEnd;
}

void CSegStepRange::operator=(const CSegStepRange& segStep)
{
	m_segStart = segStep.m_segStart;
	m_segEnd = segStep.m_segEnd;
	m_stepStart = segStep.m_stepStart;
	m_stepEnd = segStep.m_stepEnd;
}

BOOL CSegStepRange::BeValidRange()
{
	return (-1 != m_segStart) && (-1 != m_segEnd) && (-1 != m_stepStart) && (-1 != m_stepEnd) && (m_segStart <= m_segEnd); 
}

void CSegStepRange::ResetSegStepRange()
{
	m_segStart = -1;
	m_segEnd = -1;
	m_stepStart = -1;
	m_stepEnd = -1;
}

void CSegStepRange::SetSegmentStart(int segStart)
{
	m_segStart = segStart;
}

int CSegStepRange::GetSegmentStart()
{
	return m_segStart;
}

void CSegStepRange::SetSegmentEnd(int segEnd)
{
	m_segEnd = segEnd;
}

int CSegStepRange::GetSegmentEnd()
{
	return m_segEnd;
}

void CSegStepRange::SetStepStart(int stepStart)
{
	m_stepStart = stepStart;
}

int CSegStepRange::GetStepStart()
{
	return m_stepStart;
}

void CSegStepRange::SetStepEnd(int stepEnd)
{
	m_stepEnd = stepEnd;
}

int CSegStepRange::GetStepEnd()
{
	return m_stepEnd;
}


