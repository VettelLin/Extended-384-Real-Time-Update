#include "StdAfx.h"
#include "HoleSelectSet.h"


CHoleSelectSet::CHoleSelectSet(void)
{
	m_eHoleDefDisplay = EHDD_PATIENTNAME;
}


CHoleSelectSet::~CHoleSelectSet(void)
{
}

CHoleSelectSet::CHoleSelectSet(const CHoleSelectSet& holeSelSet)
{
	m_eHoleDefDisplay = holeSelSet.m_eHoleDefDisplay;
}

void CHoleSelectSet::operator=(const CHoleSelectSet& holeSelSet)
{
	m_eHoleDefDisplay = holeSelSet.m_eHoleDefDisplay;
}
