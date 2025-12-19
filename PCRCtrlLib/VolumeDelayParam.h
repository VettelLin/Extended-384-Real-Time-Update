#pragma once
class CVolumeDelayParam
{
public:
	CVolumeDelayParam(void);
	~CVolumeDelayParam(void);

	CVolumeDelayParam(const CVolumeDelayParam& info);
	void operator=(const CVolumeDelayParam& info);

public:
	double m_dUpTemThre;
	double m_dDownTemThre;
	double m_dUpK;
	double m_dDownK;
	double m_dBaseVolume;
};

