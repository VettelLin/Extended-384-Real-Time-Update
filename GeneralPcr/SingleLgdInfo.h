#pragma once

#include<GdiPlus.h>
using namespace Gdiplus;

class CSingleLgdInfo
{
public:
	CSingleLgdInfo(void);
	~CSingleLgdInfo(void);

	CSingleLgdInfo(const CSingleLgdInfo& info);
	void operator=(const CSingleLgdInfo& info);

public:
	Color m_clrFrame;
	Color m_clrFill;

	int m_pxFrameW;
	CString m_strTx;
};

