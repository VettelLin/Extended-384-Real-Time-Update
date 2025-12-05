#include "StdAfx.h"
#include "ExperimentInfo.h"
#include "PublicFun.h"

CExperimentInfo::CExperimentInfo()
{
	m_strExpName = CreateExperimentName(); 
	m_strFilePath.Empty(); 

	m_nExpType = EXP_TYPE_ABSOLUTE;
	m_strOperator.Empty(); 
	m_strVerifier.Empty(); 
	m_strComments.Empty();
	m_strStartTime.Empty();
	m_strEndTime.Empty();

	m_strInstrument.Empty();
	m_strInstrumentSN.Empty();
	m_nTubeType = TUBE_TRANSPARENT;

	m_bLidHeater = TRUE;
	m_fLidTopTemp = 90;
	m_nTempMode = TEMP_MODE_CONSTANT;
	m_nTempCtrlMode = TEMP_CTRL_TUBE;
	m_nBlockHeater = 0;
	m_ScanMode = CHANNEL_SCAN_ALL;
	m_nMeltScanMode = MELT_SCAN_STANDARD;
	m_nPMTGear = MPPC_VOLGEAR_MIDDLE;

	m_strCYReportNo.Empty();

	m_iTotalSec = 0;
	m_iRemainSec = 0;
	m_iHasRunSec = 0;
	m_timeStart = CTime::GetCurrentTime();
}

void CExperimentInfo::CopyExperiment(CExperimentInfo& expInfo)
{
	if(NULL == this)
	{
		return;
	}

	m_strExpName = expInfo.m_strExpName;
	m_strFilePath = expInfo.m_strFilePath;

	m_nExpType = expInfo.m_nExpType;
	m_strOperator = expInfo.m_strOperator;
	m_strVerifier = expInfo.m_strVerifier;
	m_strComments = expInfo.m_strComments;
	m_strStartTime = expInfo.m_strStartTime;
	m_strEndTime = expInfo.m_strEndTime;
	m_strRunDuration = expInfo.m_strRunDuration;

	m_strInstrument = expInfo.m_strInstrument;
	m_strInstrumentSN = expInfo.m_strInstrumentSN;
	m_strFirmwareVersion = expInfo.m_strFirmwareVersion;
	m_strSoftwareVersion = expInfo.m_strSoftwareVersion;

	m_nTubeType = expInfo.m_nTubeType;
	m_bLidHeater = expInfo.m_bLidHeater;
	m_fLidTopTemp = expInfo.m_fLidTopTemp;
	m_nTempMode = expInfo.m_nTempMode;
	m_nTempCtrlMode = expInfo.m_nTempCtrlMode;
	m_nBlockHeater = expInfo.m_nBlockHeater;
	m_ScanMode = expInfo.m_ScanMode;
	m_nMeltScanMode = expInfo.m_nMeltScanMode;
	m_nPMTGear = expInfo.m_nPMTGear;

	m_strCYReportNo = expInfo.m_strCYReportNo;

	m_iTotalSec = expInfo.m_iTotalSec;
	m_iRemainSec = expInfo.m_iRemainSec;
	m_iHasRunSec = expInfo.m_iHasRunSec;
	m_timeStart = expInfo.m_timeStart;

	return;
}

BOOL CExperimentInfo::WriteFile(CFile* pFile,BOOL bTemplate)
{
	CPublicFun::WriteStringToFile(pFile, m_strExpName);
	CPublicFun::WriteStringToFile(pFile, m_strFilePath);

	int nTempInt = m_nExpType;
	pFile->Write((const char *)&nTempInt, sizeof(int));
	CPublicFun::WriteStringToFile(pFile, m_strOperator);
	CPublicFun::WriteStringToFile(pFile, m_strVerifier);
	CPublicFun::WriteStringToFile(pFile, m_strComments);

	//保存模板文件时，不保留相关状态信息
	if(bTemplate)
	{
		CPublicFun::WriteStringToFile(pFile,_T(""));
		CPublicFun::WriteStringToFile(pFile,_T(""));
		CPublicFun::WriteStringToFile(pFile,_T(""));
	}
	else
	{
		CPublicFun::WriteStringToFile(pFile, m_strStartTime);
		CPublicFun::WriteStringToFile(pFile, m_strEndTime);
		CPublicFun::WriteStringToFile(pFile, m_strRunDuration);
	}

	CPublicFun::WriteStringToFile(pFile, m_strInstrument);
	CPublicFun::WriteStringToFile(pFile, m_strInstrumentSN);
	CPublicFun::WriteStringToFile(pFile, m_strFirmwareVersion);
	CPublicFun::WriteStringToFile(pFile, m_strSoftwareVersion);

	nTempInt = m_nTubeType;
	pFile->Write((const char *)&nTempInt, sizeof(int));
	pFile->Write((const char *)&m_bLidHeater, sizeof(BOOL));
	pFile->Write((const char *)&m_fLidTopTemp, sizeof(float));
	nTempInt = m_nTempMode;
	pFile->Write((const char *)&nTempInt, sizeof(int));
	nTempInt = m_nTempCtrlMode;
	pFile->Write((const char *)&nTempInt, sizeof(int));
	pFile->Write((const char *)&m_nBlockHeater, sizeof(UINT));
	nTempInt = m_ScanMode;
	pFile->Write((const char *)&nTempInt, sizeof(int));
	nTempInt = m_nMeltScanMode;
	pFile->Write((const char *)&nTempInt, sizeof(int));
	nTempInt = m_nPMTGear;
	pFile->Write((const char *)&nTempInt, sizeof(int));

	if(bTemplate)
	{
		int iTotalSec = 0,iRemainSec = 0;
		pFile->Write((const char *)&iTotalSec,sizeof(int));
		pFile->Write((const char *)&iRemainSec,sizeof(int));
	}
	else
	{
		pFile->Write((const char *)&m_iTotalSec, sizeof(int));
		pFile->Write((const char *)&m_iRemainSec, sizeof(int));
	}

	return TRUE;
}

BOOL CExperimentInfo::ReadFile(CFile* pFile, WORD wVersion)
{
	m_strExpName = CPublicFun::ReadStringFromFile(pFile);
	m_strFilePath = CPublicFun::ReadStringFromFile(pFile);

	int tempInt = 0;
	pFile->Read(&tempInt, sizeof(int));
	m_nExpType = (eExperimentType)tempInt;
	m_strOperator = CPublicFun::ReadStringFromFile(pFile);
	m_strVerifier = CPublicFun::ReadStringFromFile(pFile);
	m_strComments = CPublicFun::ReadStringFromFile(pFile);
	m_strStartTime = CPublicFun::ReadStringFromFile(pFile);
	m_strEndTime = CPublicFun::ReadStringFromFile(pFile);
	m_strRunDuration = CPublicFun::ReadStringFromFile(pFile);

	m_strInstrument = CPublicFun::ReadStringFromFile(pFile);
	m_strInstrumentSN = CPublicFun::ReadStringFromFile(pFile);
	m_strFirmwareVersion = CPublicFun::ReadStringFromFile(pFile);
	m_strSoftwareVersion = CPublicFun::ReadStringFromFile(pFile);

	pFile->Read(&tempInt, sizeof(int));
	m_nTubeType = (eTubeType)tempInt;
	pFile->Read(&m_bLidHeater, sizeof(BOOL));
	pFile->Read(&m_fLidTopTemp, sizeof(float));
	pFile->Read(&tempInt, sizeof(int));
	m_nTempMode = (ePCRTempMode)tempInt;
	pFile->Read(&tempInt, sizeof(int));
	m_nTempCtrlMode = (eTempCtrlMode)tempInt;
	pFile->Read(&m_nBlockHeater, sizeof(UINT));
	pFile->Read(&tempInt, sizeof(int));
	m_ScanMode = (eChannelScanlMode)tempInt;
	pFile->Read(&tempInt, sizeof(int));
	m_nMeltScanMode = (eMeltScanMode)tempInt;
	pFile->Read(&tempInt, sizeof(int));
	m_nPMTGear = (eMPPCVoltageGear)tempInt;

	pFile->Read(&m_iTotalSec, sizeof(int));
	pFile->Read(&m_iRemainSec, sizeof(int));

	m_iHasRunSec = m_iTotalSec - m_iRemainSec;	//添加于20220823，孙文郎
	if(m_iHasRunSec > m_iTotalSec)
	{
		m_iHasRunSec = m_iTotalSec;
		m_iRemainSec = 0;
	}

	return TRUE;
}

CString CExperimentInfo::CreateExperimentName()
{
	CString strDate;
	SYSTEMTIME st;
	GetLocalTime(&st);
	strDate.Format(_T("%4d-%02d-%02d_%02d-%02d-%02d"),st.wYear, st.wMonth, st.wDay, st.wHour,st.wMinute,st.wSecond);
	return strDate;
}

BOOL CExperimentInfo::GetBePS96ExperimentFile()
{
	return _T("iAMP-PS96") == m_strInstrument.Left(9);
}

CString CExperimentInfo::GetSaveDataFileName()
{
	CString strName;

	strName = GetFilePath() + _T("\\") + GetExpName();

	return strName;
}

void CExperimentInfo::SetCYReportNo(CString strData)
{
	m_strCYReportNo = strData;
	return;
}

CString CExperimentInfo::GetCYReportNo()
{
	return m_strCYReportNo;
}

void CExperimentInfo::SetTotalSec(int iTotalSec)
{
	m_iTotalSec = iTotalSec;
	return;
}

int CExperimentInfo::GetTotalSec()
{
	return m_iTotalSec;
}

CString CExperimentInfo::GetTotalTimeString()
{
	int iHour = m_iTotalSec / 3600;
	int iTotalSec = m_iTotalSec % 3600;
	int iMinute = iTotalSec / 60;
	int iSec = iTotalSec % 60;
	CString strTemp(_T(""));

	strTemp.Format(_T("%02d:%02d:%02d"),iHour,iMinute,iSec);

	return strTemp;
}

void CExperimentInfo::SetHasRunSec(int iHasRunSec)
{
	m_iHasRunSec = iHasRunSec;
	if(m_iHasRunSec < 0)
	{
		m_iHasRunSec = 0;
	}
}

int CExperimentInfo::GetHasRunSec()
{
	return m_iHasRunSec;
}

void CExperimentInfo::SetRemainSec(int iRemainSec)
{
	m_iRemainSec = iRemainSec;
	return;
}

int CExperimentInfo::GetRemainSec()
{
	return m_iRemainSec;
}

void CExperimentInfo::SetStartTime(CTime time)
{
	m_timeStart = time;
	m_strStartTime = time.Format(_T("%Y-%m-%d %H:%M:%S"));
}

void CExperimentInfo::SetEndTime(CTime time)
{
	m_strEndTime = time.Format(_T("%Y-%m-%d %H:%M:%S"));
	CTimeSpan timeSpan = time - m_timeStart;
	m_strRunDuration.Format(_T("%d minutes %d seconds"), timeSpan.GetMinutes(), timeSpan.GetSeconds());
}
