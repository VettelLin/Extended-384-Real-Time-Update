#include "StdAfx.h"
#include "VolumeDelayParam.h"

CVolumeDelayParam::CVolumeDelayParam(void)
{
	m_dUpTemThre = 90;
	m_dDownTemThre = 70;
	m_dUpK = 0.1;
	m_dDownK = 0.2;
	m_dBaseVolume = 20;
}

CVolumeDelayParam::~CVolumeDelayParam(void)
{

}

CVolumeDelayParam::CVolumeDelayParam(const CVolumeDelayParam& info)
{
	m_dUpTemThre = info.m_dUpTemThre;
	m_dDownTemThre = info.m_dDownTemThre;
	m_dUpK = info.m_dUpK;
	m_dDownK = info.m_dDownK;
	m_dBaseVolume = info.m_dBaseVolume;
}

void CVolumeDelayParam::operator=(const CVolumeDelayParam& info)
{
	m_dUpTemThre = info.m_dUpTemThre;
	m_dDownTemThre = info.m_dDownTemThre;
	m_dUpK = info.m_dUpK;
	m_dDownK = info.m_dDownK;
	m_dBaseVolume = info.m_dBaseVolume;
}



