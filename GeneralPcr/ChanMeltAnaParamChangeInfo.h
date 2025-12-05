#pragma once
class CChanMeltAnaParamChangeInfo
{
public:
	CChanMeltAnaParamChangeInfo(void);
	~CChanMeltAnaParamChangeInfo(void);

	CChanMeltAnaParamChangeInfo(const CChanMeltAnaParamChangeInfo& info);
	void operator=(const CChanMeltAnaParamChangeInfo& info);

	BOOL IsAnaParamChanged();

public:
	BOOL m_bMinTemChanged;
	BOOL m_bMaxTemChanged;
	BOOL m_bPeakThreChanged;
};

