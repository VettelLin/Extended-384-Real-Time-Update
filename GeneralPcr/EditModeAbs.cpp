#include "StdAfx.h"
#include "EditModeAbs.h"


CEditModeAbs::CEditModeAbs(void)
{
	m_eExpEditMode = EEEM_NOLIMIT;
}


CEditModeAbs::~CEditModeAbs(void)
{
}

CEditModeAbs::CEditModeAbs(const CEditModeAbs& editMode)
{
	m_eExpEditMode = editMode.m_eExpEditMode;
}

void CEditModeAbs::operator=(const CEditModeAbs& editMode)
{
	m_eExpEditMode = editMode.m_eExpEditMode;
}


