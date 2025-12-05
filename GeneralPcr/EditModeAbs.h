#pragma once

#include "EditModeType.h"

class CEditModeAbs
{
public:
	CEditModeAbs(void);
	~CEditModeAbs(void);

	CEditModeAbs(const CEditModeAbs& editMode);
	void operator=(const CEditModeAbs& editMode);

	virtual int SetEditMode(EEXPEDITMODE eExpEditMode) = 0;

public:
	EEXPEDITMODE m_eExpEditMode;
};

