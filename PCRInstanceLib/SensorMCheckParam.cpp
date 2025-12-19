#include "StdAfx.h"
#include "SensorMCheckParam.h"


CSensorMCheckParam::CSensorMCheckParam(void)
{
	m_bCheckEnvirTsm = TRUE;
	m_bCheckHeatSinkTsm = TRUE;
	m_bCheckLidTsm = TRUE;
	m_bCheckPelt1Tsm = TRUE;
	m_bCheckPelt2Tsm = TRUE;
	m_bCheckPelt3Tsm = TRUE;
	m_bCheckPowerTsm = FALSE;
	m_bCheckScanTsm = TRUE;

	m_dEnvirTsmRefH = 40;
	m_dEnvirTsmRefL = 10;

	m_dHeatSinkTsmRefH = 75;
	m_dHeatSinkTsmRefL = 10;

	m_dLidTsmRefH = 110;
	m_dLidTsmRefL = 10;

	m_dPelt1TsmRefH = 95;
	m_dPelt1TsmRefL = 5;

	m_dPelt2TsmRefH = 95;
	m_dPelt2TsmRefL = 5;

	m_dPelt3TsmRefH = 95;
	m_dPelt3TsmRefL = 5;

	m_dPowerTsmRefH = 5;
	m_dPowerTsmRefL = 100;

	m_dScanTsmRefH = 40;
	m_dScanTsmRefL = 10;

	m_strChkRst.Empty();
}

CSensorMCheckParam::~CSensorMCheckParam(void)
{
	
}

CSensorMCheckParam::CSensorMCheckParam(const CSensorMCheckParam& info)
{
	m_bCheckEnvirTsm = info.m_bCheckEnvirTsm;
	m_bCheckHeatSinkTsm = info.m_bCheckHeatSinkTsm;
	m_bCheckLidTsm = info.m_bCheckLidTsm;
	m_bCheckPelt1Tsm = info.m_bCheckPelt1Tsm;
	m_bCheckPelt2Tsm = info.m_bCheckPelt2Tsm;
	m_bCheckPelt3Tsm = info.m_bCheckPelt3Tsm;
	m_bCheckPowerTsm = info.m_bCheckPowerTsm;
	m_bCheckScanTsm = info.m_bCheckScanTsm;

	m_dEnvirTsmRefH = info.m_dEnvirTsmRefH;
	m_dEnvirTsmRefL = info.m_dEnvirTsmRefL;
	m_dHeatSinkTsmRefH = info.m_dHeatSinkTsmRefH;
	m_dHeatSinkTsmRefL = info.m_dHeatSinkTsmRefL;
	m_dLidTsmRefH = info.m_dLidTsmRefH;
	m_dLidTsmRefL = info.m_dLidTsmRefL;
	m_dPelt1TsmRefH = info.m_dPelt1TsmRefH;
	m_dPelt1TsmRefL = info.m_dPelt1TsmRefL;
	m_dPelt2TsmRefH = info.m_dPelt2TsmRefH;
	m_dPelt2TsmRefL = info.m_dPelt2TsmRefL;
	m_dPelt3TsmRefH = info.m_dPelt3TsmRefH;
	m_dPelt3TsmRefL = info.m_dPelt3TsmRefL;
	m_dPowerTsmRefH = info.m_dPowerTsmRefH;
	m_dPowerTsmRefL = info.m_dPowerTsmRefL;
	m_dScanTsmRefH = info.m_dScanTsmRefH;
	m_dScanTsmRefL = info.m_dScanTsmRefL;
	m_strChkRst = info.m_strChkRst;
}

void CSensorMCheckParam::operator=(const CSensorMCheckParam& info)
{
	m_dEnvirTsmRefH = info.m_dEnvirTsmRefH;
	m_dEnvirTsmRefL = info.m_dEnvirTsmRefL;
	m_dHeatSinkTsmRefH = info.m_dHeatSinkTsmRefH;
	m_dHeatSinkTsmRefL = info.m_dHeatSinkTsmRefL;
	m_dLidTsmRefH = info.m_dLidTsmRefH;
	m_dLidTsmRefL = info.m_dLidTsmRefL;
	m_dPelt1TsmRefH = info.m_dPelt1TsmRefH;
	m_dPelt1TsmRefL = info.m_dPelt1TsmRefL;
	m_dPelt2TsmRefH = info.m_dPelt2TsmRefH;
	m_dPelt2TsmRefL = info.m_dPelt2TsmRefL;
	m_dPelt3TsmRefH = info.m_dPelt3TsmRefH;
	m_dPelt3TsmRefL = info.m_dPelt3TsmRefL;
	m_dPowerTsmRefH = info.m_dPowerTsmRefH;
	m_dPowerTsmRefL = info.m_dPowerTsmRefL;
	m_dScanTsmRefH = info.m_dScanTsmRefH;
	m_dScanTsmRefL = info.m_dScanTsmRefL;
	m_strChkRst = info.m_strChkRst;
}

BOOL CSensorMCheckParam::IsCheckEnvirTsm()
{
	return m_bCheckEnvirTsm;
}

BOOL CSensorMCheckParam::IsCheckHeatSinkTsm()
{
	return m_bCheckHeatSinkTsm;
}

BOOL CSensorMCheckParam::IsCheckLidTsm()
{
	return m_bCheckLidTsm;
}

BOOL CSensorMCheckParam::IsCheckPelt1Tsm()
{
	return m_bCheckPelt1Tsm;
}

BOOL CSensorMCheckParam::IsCheckPelt2Tsm()
{
	return m_bCheckPelt2Tsm;
}

BOOL CSensorMCheckParam::IsCheckPelt3Tsm()
{
	return m_bCheckPelt3Tsm;
}

BOOL CSensorMCheckParam::IsCheckPowerTsm()
{
	return m_bCheckPowerTsm;
}

BOOL CSensorMCheckParam::IsCheckScanTsm()
{
	return m_bCheckScanTsm;
}

double CSensorMCheckParam::GetEnvirTsmRefH()
{
	return m_dEnvirTsmRefH;
}

double CSensorMCheckParam::GetEnvirTsmRefL()
{
	return m_dEnvirTsmRefL;
}

double CSensorMCheckParam::GetHeatSinkTsmRefH()
{
	return m_dHeatSinkTsmRefH;
}

double CSensorMCheckParam::GetHeatSinkTsmRefL()
{
	return m_dHeatSinkTsmRefL;
}

double CSensorMCheckParam::GetLidTsmRefH()
{
	return m_dLidTsmRefH;
}

double CSensorMCheckParam::GetLidTsmRefL()
{
	return m_dLidTsmRefL;
}

double CSensorMCheckParam::GetPelt1TsmRefH()
{
	return m_dPelt1TsmRefH;
}

double CSensorMCheckParam::GetPelt1TsmRefL()
{
	return m_dPelt1TsmRefL;
}

double CSensorMCheckParam::GetPelt2TsmRefH()
{
	return m_dPelt2TsmRefH;
}

double CSensorMCheckParam::GetPelt2TsmRefL()
{
	return m_dPelt2TsmRefL;
}

double CSensorMCheckParam::GetPelt3TsmRefH()
{
	return m_dPelt3TsmRefH;
}

double CSensorMCheckParam::GetPelt3TsmRefL()
{
	return m_dPelt3TsmRefL;
}

double CSensorMCheckParam::GetPowerTsmRefH()
{
	return m_dPowerTsmRefH;
}

double CSensorMCheckParam::GetPowerTsmRefL()
{
	return m_dPowerTsmRefL;
}

double CSensorMCheckParam::GetScanTsmRefH()
{
	return m_dScanTsmRefH;
}

double CSensorMCheckParam::GetScanTsmRefL()
{
	return m_dScanTsmRefL;
}

void CSensorMCheckParam::SetEnvirTsmRefH(double dData)
{
	m_dEnvirTsmRefH = dData;
}

void CSensorMCheckParam::SetEnvirTsmRefL(double dData)
{
	m_dEnvirTsmRefL = dData;
}

void CSensorMCheckParam::SetHeatSinkTsmRefH(double dData)
{
	m_dHeatSinkTsmRefH = dData;
}

void CSensorMCheckParam::SetHeatSinkTsmRefL(double dData)
{
	m_dHeatSinkTsmRefL = dData;
}

void CSensorMCheckParam::SetLidTsmRefH(double dData)
{
	m_dLidTsmRefH = dData;
}

void CSensorMCheckParam::SetLidTsmRefL(double dData)
{
	m_dLidTsmRefL = dData;
}

void CSensorMCheckParam::SetPelt1TsmRefH(double dData)
{
	m_dPelt1TsmRefH = dData;
}

void CSensorMCheckParam::SetPelt1TsmRefL(double dData)
{
	m_dPelt1TsmRefL = dData;
}

void CSensorMCheckParam::SetPelt2TsmRefH(double dData)
{
	m_dPelt2TsmRefH = dData;
}

void CSensorMCheckParam::SetPelt2TsmRefL(double dData)
{
	m_dPelt2TsmRefL = dData;
}

void CSensorMCheckParam::SetPelt3TsmRefH(double dData)
{
	m_dPelt3TsmRefH = dData;
}

void CSensorMCheckParam::SetPelt3TsmRefL(double dData)
{
	m_dPelt3TsmRefL = dData;
}

void CSensorMCheckParam::SetPowerTsmRefH(double dData)
{
	m_dPowerTsmRefH = dData;
}

void CSensorMCheckParam::SetPowerTsmRefL(double dData)
{
	m_dPowerTsmRefL = dData;
}

void CSensorMCheckParam::SetScanTsmRefH(double dData)
{
	m_dScanTsmRefH = dData;
}

void CSensorMCheckParam::SetScanTsmRefL(double dData)
{
	m_dScanTsmRefL = dData;
}

void CSensorMCheckParam::ResetCheckResult()
{
	m_strChkRst.Empty();
}

void CSensorMCheckParam::AddCheckResult(CString strData)
{
	m_strChkRst += _T("\r\n");
	m_strChkRst += strData;
	return;
}

CString CSensorMCheckParam::GetCheckResult()
{
	return m_strChkRst;
}

BOOL CSensorMCheckParam::IsSuccessful()
{
	return m_strChkRst.IsEmpty();
}



