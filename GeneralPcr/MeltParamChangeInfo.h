#pragma once
class CMeltParamChangeInfo
{
public:
	CMeltParamChangeInfo(void);
	~CMeltParamChangeInfo(void);

	CMeltParamChangeInfo(const CMeltParamChangeInfo& info);
	void operator=(const CMeltParamChangeInfo& info);

	BOOL GetBeChanged();

public:
	BOOL m_bChanged;
	BOOL m_bMeltTemMinChanged;	//熔曲最低温度设置是否改变
	BOOL m_bMeltTemMaxChanged;	//熔曲最高温度设置是否改变
	BOOL m_bPeakHeightThreChanged;	//峰高阈值是否改变

	BOOL m_bCrvMeltPeakNeedRedraw;	//熔解峰值曲线是否需要重绘
	BOOL m_bRstLstNeedRedraw;	//结果列表是否需要重绘
};

