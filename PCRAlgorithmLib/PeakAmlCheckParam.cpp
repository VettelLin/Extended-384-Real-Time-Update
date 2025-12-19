#include "StdAfx.h"
#include "PeakAmlCheckParam.h"


CPeakAmlCheckParam::CPeakAmlCheckParam(void)
{
	m_dStdThreshold = 45;
	m_dRatioThreshold = 1.1;

	m_dCrestBkRatio = 0.001; 
	m_bkCalcDotNum = 3;

	m_start = 1;
	m_forwardNum = 3;
	m_sdMultiples = 1.5;
}


CPeakAmlCheckParam::~CPeakAmlCheckParam(void)
{
}

CPeakAmlCheckParam::CPeakAmlCheckParam(const CPeakAmlCheckParam& param)
{
	m_dStdThreshold = param.m_dStdThreshold;
	m_dRatioThreshold = param.m_dRatioThreshold;

	m_dCrestBkRatio = param.m_dCrestBkRatio; 
	m_bkCalcDotNum = param.m_bkCalcDotNum;

	m_start = param.m_start;
	m_forwardNum = param.m_forwardNum;
	m_sdMultiples = param.m_sdMultiples;
}

void CPeakAmlCheckParam::operator=(const CPeakAmlCheckParam& param)
{
	m_dStdThreshold = param.m_dStdThreshold;
	m_dRatioThreshold = param.m_dRatioThreshold;

	m_dCrestBkRatio = param.m_dCrestBkRatio; 
	m_bkCalcDotNum = param.m_bkCalcDotNum;

	m_start = param.m_start;
	m_forwardNum = param.m_forwardNum;
	m_sdMultiples = param.m_sdMultiples;
}

BOOL CPeakAmlCheckParam::operator==(const CPeakAmlCheckParam& param)
{
	return m_dStdThreshold == param.m_dStdThreshold && 
	m_dRatioThreshold == param.m_dRatioThreshold && 
	m_dCrestBkRatio == param.m_dCrestBkRatio &&  
	m_bkCalcDotNum == param.m_bkCalcDotNum && 
	m_start == param.m_start && 
	m_forwardNum == param.m_forwardNum && 
	m_sdMultiples == param.m_sdMultiples;
}

