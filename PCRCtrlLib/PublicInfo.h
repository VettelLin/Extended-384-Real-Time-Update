#pragma once

#include "DeclaresPCR.h"
#include "ReportTemplateData.h"

// 界面相关公共信息
class AFX_EXT_CLASS CPublicInfo
{
public:
	CPublicInfo(void);
	~CPublicInfo(void);

	enum eIniFileType
	{
		INI_FILE_SYSTEM = 0, // System.ini 
		INI_FILE_ALGORITHM, // AlgorithmPara.ini
		INI_FILE_CALIBRATE, // CalibratePara.ini
		INI_FILE_CROSSTALK, // CrosstalkPara.ini
		INI_FILE_FILTER, // FilterPara.ini
	};

// Operations
public:
	static CPublicInfo* GetInstance(void);
	static void CleanUp();
	void Initialize(CString strStartupPath, CString strUserName);

	void ReadParameters();
	void SaveParameters();

	CString GetIniFilePath(eIniFileType nIniType , BOOL bRead=TRUE); // 根据类型得到配置文件全路径

protected:
	BOOL ReadAndCreateUserPath();
	CString ReadStringFromIniFile(CString strAppName, CString strKeyName, CString strDefault, CString strIniFile);

// Attributes
public:

	void SetAnalysisDataType(eAnalysisDataType nType) { m_nAnaDataType = nType; }
	eAnalysisDataType GetAnalysisDataType() { return m_nAnaDataType; }

	//vector<COLORREF> GetChannelColor() { return m_vecChannelColor; }

	CString GetStartupPath() { return m_strStartupPath; }
	CString GetUserPath() { return m_strUserPath; } // 得到用户路径
	CString GetUserSystemPath() { return m_strUserPath + _T("\\System"); }
	void SetUserDataSavePath(CString strPath); // 设置用户数据保存路径
	CString GetUserDataSavePath() { return m_strUserDataPath; } // 取得用户数据保存路径

	CString GetUserName() { return m_strUserName; }
	EPERMISSIONTYPE GetUserRole() { return m_ePermissionType; }
	void SetUserRole(EPERMISSIONTYPE eType) { m_ePermissionType = eType; }

	CString SampleTypeToText(eSampleType eType);
	CString GetSampleTypeAbbrText(eSampleType eType);

	eSampleType TextToSampleType(CString strText);

	CReportTemplateData* GetReportTemplateData() { return m_pReportData; }

	BOOL GetFluoDataAutoAdjust() { return m_bFluoDataAutoAdjust; }
	int GetFluoDataMaxLimited() { return m_iFluoDataMaxLimited; } 
	int GetFluoDataAdjustLimited() { return m_iFluoDataAdjustLimited; }

protected:
	static CPublicInfo* m_pInstance;

	CString		m_strStartupPath;
	CString		m_strUserPath; // 用户路径
	CString		m_strUserDataPath; // 用户数据保存路径

	CString		m_strUserName; // 用户名

	eAnalysisDataType	m_nAnaDataType; // 扩增分析默认使用的数据类型
	EPERMISSIONTYPE m_ePermissionType;

	CReportTemplateData* m_pReportData; // 报告模板数据

	BOOL	m_bFluoDataAutoAdjust; // 检测荧光值过限自动调整
	int		m_iFluoDataMaxLimited; // 检测荧光值最大限制值
	int		m_iFluoDataAdjustLimited; // 需要补偿限制最大值
};

