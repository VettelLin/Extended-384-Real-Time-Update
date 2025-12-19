#pragma once

#include "FtSetDensity.h"
#include "MyLstCtrl.h"

class CSamLst: public CMyLstCtrl,public CFtSetDensity
{
public:
	CSamLst(void);
	~CSamLst(void);

	virtual	int DrawBody(CDC* pDC);

};

