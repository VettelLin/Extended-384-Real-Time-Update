#pragma once
class AFX_EXT_CLASS CPeltierCheckParam
{
public:
	CPeltierCheckParam(void);
	~CPeltierCheckParam(void);

	CPeltierCheckParam(const CPeltierCheckParam& info);
	void operator=(const CPeltierCheckParam& info);

	BOOL IsCheck();
	double GetTemperatureThreshold();
	double GetRiseTemError();
	double GetDropTemError();
	double GetTemRiseDropRange();
	float  GetRiseTimeSec();
	float  GetDropTimeSec();

protected:
	BOOL m_bCheck;
	double m_dTemThre;
	double m_dRiseTemError;	//ÉıÎÂÎÂ¶ÈÎó²î
	double m_dDropTemError;	//½µÎÂÎÂ¶ÈÎó²î
	double m_dTemRiseDropRange;	//Éı½µÎÂ·ù¶È
	float m_fRiseTimeSec;	//ÉıÎÂÊ±¼ä
	float m_fDropTimeSec;	//½µÎÂÊ±¼ä
};

