#pragma once

#include "EnumFtUnitType.h"

class CFtSetDensity
{
public:
	CFtSetDensity(void);
	~CFtSetDensity(void);

	void InitFontDensity(int lfHeight);
	void SetMyDensityFont(CString fontName,int nHeight,int nWeight = FW_NORMAL,FONT_UNIT_TYPE fontUnitType = TYPE_PIXEL); //°õ£¬¼´1/72Ó¢´ç;
	void SetMyDensityFont();

public:
	CFont m_ftDensity;
	LOGFONT m_logFtDensity;
};

