#pragma once

class CMySegDlgParam
{
public:
	CMySegDlgParam(void);
	~CMySegDlgParam(void);

	CMySegDlgParam(const CMySegDlgParam& segDlgParam);
	void operator=(const CMySegDlgParam& segDlgParam);

public:
	COLORREF m_clrGeneral;
	COLORREF m_clrStepTag;
	COLORREF m_clrSegTag;
	COLORREF m_clrRunning;
};

