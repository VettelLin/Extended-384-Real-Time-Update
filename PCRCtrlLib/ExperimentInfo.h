#pragma once

#include "DeclaresPCR.h"

//  实验数据文件基本信息
class AFX_EXT_CLASS CExperimentInfo
{
public:
	CExperimentInfo();
	~CExperimentInfo(){}

	void CopyExperiment(CExperimentInfo& expInfo);

// Operations
public:
	BOOL WriteFile(CFile* pFile,BOOL bTemplate);
	BOOL ReadFile(CFile* pFile, WORD wVersion);

	CString GetSaveDataFileName();
	CString CreateExperimentName();

// Attributes
public:
	CString GetExpName() { return m_strExpName; }
	void SetExpName(CString string) { m_strExpName = string; }
	CString GetFilePath() { return m_strFilePath; }
	void SetFilePath(CString string) { m_strFilePath = string; }

	eExperimentType GetExpType() { return m_nExpType; }
	void SetExpType(eExperimentType nType) { m_nExpType = nType; }
	CString GetOperator() { return m_strOperator; }
	void SetOperator(CString string) { m_strOperator = string; }
	CString GetVerifier() { return m_strVerifier; }
	void SetVerifier(CString string) { m_strVerifier = string; }
	CString GetComments() { return m_strComments; }
	void SetComments(CString string) { m_strComments = string; }

	CString GetStartTime() { return m_strStartTime;}
	void SetStartTime(CString string) { m_strStartTime = string; }
	void SetStartTime(CTime time);
	CString GetEndTime() { return m_strEndTime;}
	void SetEndTime(CString string) { m_strEndTime = string; }
	void SetEndTime(CTime time);
	CString GetRunDuration() { return m_strRunDuration;}
	void SetRunDuration(CString strDuration) { m_strRunDuration = strDuration; }

	CString GetInstrument() { return m_strInstrument; }
	void SetInstrument(CString string) { m_strInstrument = string; }
	CString GetInstrumentSN() { return m_strInstrumentSN; }
	void SetInstrumentSN(CString string) { m_strInstrumentSN = string;}
	CString GetFirmwareVersion() { return m_strFirmwareVersion; }
	void SetFirmwareVersion(CString strVersion) { m_strFirmwareVersion = strVersion; }
	CString GetSoftwareVersion() { return m_strSoftwareVersion; }
	void SetSoftwareVersion(CString strVersion) { m_strSoftwareVersion = strVersion; }

	BOOL GetBePS96ExperimentFile();	//判断是否是PS96实验文件，添加这个函数是为了兼容PS96早期实验文件的识别,20220624

	eTubeType GetTubeType() { return m_nTubeType; }
	void SetTubeType(eTubeType nType) { m_nTubeType = nType; }
	BOOL GetLidHeader() { return m_bLidHeater; }
	void SetLidHeader(BOOL bUse) { m_bLidHeater = bUse; }
	float GetLidTemp() { return m_fLidTopTemp; }
	void SetLidTemp(float fValue) { m_fLidTopTemp = fValue; }

	ePCRTempMode GetTempMode() { return m_nTempMode; }
	void SetTempMode(ePCRTempMode nMode) { m_nTempMode = nMode; }
	eTempCtrlMode GetTempCtrlMode() { return m_nTempCtrlMode; }
	void SetTempCtrlMode(eTempCtrlMode nMode) { m_nTempCtrlMode = nMode; }
	UINT GetBlockHeater() { return m_nBlockHeater; }
	void SetBlockHeader(UINT nBlock) { m_nBlockHeater = nBlock; }
	eChannelScanlMode GetChannelScanMode() { return m_ScanMode; }
	void SetChannelScanMode(eChannelScanlMode nMode) { m_ScanMode = nMode; }
	eMeltScanMode GetMeltScanMode() { return m_nMeltScanMode; }
	void SetMeltScanMode(eMeltScanMode nMode) { m_nMeltScanMode = nMode; }
	eMPPCVoltageGear GetPMTVoltageGear() { return m_nPMTGear; }
	void SetPMTVoltageGear(eMPPCVoltageGear nGear) { m_nPMTGear = nGear; }

	void SetCYReportNo(CString strData);
	CString GetCYReportNo();

	void SetTotalSec(int iTotalSec);
	int GetTotalSec();
	CString GetTotalTimeString();

	void SetHasRunSec(int iHasRunSec);
	int GetHasRunSec();

	void SetRemainSec(int iRemainSec);
	int GetRemainSec();

protected:
	CString		m_strExpName; // 实验名称，文件名
	CString		m_strFilePath; // 文件保存路径
	
	eExperimentType m_nExpType; // 实验类型
	CString		m_strOperator; // 操作者
	CString		m_strVerifier; // 审核者
	CString		m_strComments; // 备注
	CString		m_strStartTime; // 实验开始时间
	CString		m_strEndTime; // 实验结束时间
	CString		m_strRunDuration; // 时间耗时

	CString		m_strInstrument; // 仪器型号
	CString		m_strInstrumentSN; // 仪器编码SN
	CString		m_strFirmwareVersion; // 硬件版本
	CString		m_strSoftwareVersion; // 软件版本

	eTubeType	m_nTubeType; // 反应管类型
	BOOL		m_bLidHeater; // 是否使用热盖
	float		m_fLidTopTemp; // 热盖最高温度
	ePCRTempMode  	m_nTempMode; // 加热模式，升降温/恒温
	eTempCtrlMode	m_nTempCtrlMode; // 控温模式 模块控温/试管控温
	UINT			m_nBlockHeater; // 在模块加热时选择的反应模块，使用位操作
	eChannelScanlMode m_ScanMode; // 扫描方式 全通道/选择通道
	eMeltScanMode m_nMeltScanMode; // 熔曲扫描方式
	eMPPCVoltageGear m_nPMTGear; // 扫描模块控制电压

	CString m_strCYReportNo;	//报告编号，康研葆使用

	//实验时间参数，添加于20220320，孙文郎
	int m_iTotalSec;	//实验总时间
	int m_iRemainSec;	//实验剩余时间
	int m_iHasRunSec;	//实验已经运行的时间
	CTime	m_timeStart; // 开始时间，为了计算实验耗时使用，不保存
};

