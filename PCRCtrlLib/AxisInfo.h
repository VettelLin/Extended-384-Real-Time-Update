#pragma once

class AFX_EXT_CLASS CAxisInfo
{
public:
	CAxisInfo(void);
	~CAxisInfo(void);

	CAxisInfo(const CAxisInfo& axisInfo);
	void operator=(const CAxisInfo& axisInfo);

	BOOL operator==(const CAxisInfo& axisInfo);

	CAxisInfo CalculateLogValaue();

public:
	double m_dXMin;
	double m_dXMax;
	double m_dYMin;
	double m_dYMax;
};

