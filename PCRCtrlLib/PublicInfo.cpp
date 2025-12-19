#include "StdAfx.h"
#include "PublicInfo.h"
#include "LanguageInfo.h"

CPublicInfo* CPublicInfo::m_pInstance = NULL;


CPublicInfo::CPublicInfo(void)
{
	m_strStartupPath.Empty();
	m_strUserPath.Empty();
	m_strUserDataPath.Empty();

	m_strUserName.Empty();

	m_nAnaDataType = DEFAULT_DATA_NORMALIZE;
	m_ePermissionType = EPT_ORDINARY;

	m_pReportData = new CReportTemplateData;

	m_bFluoDataAutoAdjust = TRUE;
	m_iFluoDataMaxLimited = 60000;
	m_iFluoDataAdjustLimited = 20000;
}

CPublicInfo::~CPublicInfo(void)
{
	SAFE_DELETE(m_pReportData);
}

CPublicInfo* CPublicInfo::GetInstance(void)
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CPublicInfo;
	}
	return m_pInstance;
}

void CPublicInfo::CleanUp()
{
	SAFE_DELETE(m_pInstance);
	m_pInstance = NULL;
}

void CPublicInfo::Initialize(CString strStartupPath, CString strUserName)
{
	m_strStartupPath = strStartupPath;
	m_strUserName = strUserName;

	ReadAndCreateUserPath();
	ReadParameters();

	m_pReportData->SetStartupPath(m_strStartupPath);
	m_pReportData->OpenFile();
}

// 根据类型得到配置文件全路径
CString CPublicInfo::GetIniFilePath(eIniFileType nIniType, BOOL bRead)
{
	CString strIniFileName;
	switch(nIniType)
	{
	case INI_FILE_SYSTEM:
		strIniFileName = _T("system.ini");
		break;
	case INI_FILE_ALGORITHM:
		strIniFileName = _T("AlgorithmPara.ini");
		break;
	case INI_FILE_CALIBRATE:
		strIniFileName = _T("CalibratePara.ini");
		break;
	case INI_FILE_CROSSTALK:
		strIniFileName = _T("CrosstalkPara.ini");
		break;
	case INI_FILE_FILTER:
		strIniFileName = _T("FilterPara.ini");
		break;
	default:
		strIniFileName.Empty();
		ASSERT(FALSE);
		break;
	}

	CString strIniFilePath = m_strUserPath + _T("\\System\\") + strIniFileName;
	if(bRead && !PathFileExists(strIniFilePath))
	{
		strIniFilePath = m_strStartupPath + _T("\\System\\") + strIniFileName;
	}

	return strIniFilePath;
}

// 读取和创建用户文件路径
BOOL CPublicInfo::ReadAndCreateUserPath()
{
	// 创建用户配置参数及数据路径
	m_strUserPath = m_strStartupPath + _T("Users");
	if(!PathFileExists(m_strUserPath))
	{
		BOOL bRtn = -1;
		bRtn = CreateDirectory(m_strUserPath,NULL);
		if (0 == bRtn)
		{
			return FALSE;
		}
	}

	m_strUserPath += _T("\\") + m_strUserName;
	if(!PathFileExists(m_strUserPath))
	{
		BOOL bRtn = -1;
		bRtn = CreateDirectory(m_strUserPath,NULL);
		if (0 == bRtn)
		{
			return FALSE;
		}
	}

	// 创建用户保存Ini文件路径
	CString strUserSystemPath = GetUserSystemPath();
	if(!PathFileExists(strUserSystemPath))
	{
		BOOL bRtn = -1;
		bRtn = CreateDirectory(strUserSystemPath, NULL);
		if (0 == bRtn)
		{
			return FALSE;
		}
	}
	
	CString strUserDataPath = ReadStringFromIniFile(m_strUserName, _T("SavePath"), _T(""), GetIniFilePath(INI_FILE_SYSTEM, TRUE));
	if (!strUserDataPath.IsEmpty())
	{
		if(!PathFileExists(strUserDataPath))
		{
			strUserDataPath.Empty();
		}
		else
		{
			m_strUserDataPath = strUserDataPath;
		}
	}

	if (strUserDataPath.IsEmpty())
	{
		//获取存储路径
		strUserDataPath = m_strUserPath + _T("\\Data");
		if(!PathFileExists(strUserDataPath))
		{
			BOOL bRtn = -1;
			bRtn = CreateDirectory(strUserDataPath, NULL);
			if (0 == bRtn)
			{
				return FALSE;
			}
		}

		SetUserDataSavePath(strUserDataPath);// 保存路径
	}

	return TRUE;
}

CString CPublicInfo::ReadStringFromIniFile(CString strAppName, CString strKeyName, CString strDefault, CString strIniFile)
{
	CString strData;
	TCHAR c1[250];
	::GetPrivateProfileString(strAppName, strKeyName, strDefault, c1, 250, strIniFile);
	strData = c1;
	strData.TrimLeft();
	return strData;
}


// 读取公共配置参数
void CPublicInfo::ReadParameters()
{
	ASSERT(!m_strUserPath.IsEmpty());
	//CString strIniFile = GetIniFilePath(INI_FILE_SYSTEM, TRUE);
	CString strIniFile = m_strStartupPath + _T("\\System\\") + _T("system.ini");

	m_nAnaDataType = (eAnalysisDataType)::GetPrivateProfileInt(_T("Analysis"), _T("DataType"), 0, strIniFile);

	m_bFluoDataAutoAdjust = (BOOL)::GetPrivateProfileInt(_T("FluoDataAdjust"), _T("AutoAdjust"), 0, strIniFile);
	m_iFluoDataMaxLimited = (BOOL)::GetPrivateProfileInt(_T("FluoDataAdjust"), _T("MaxLimited"), 0, strIniFile);
	m_iFluoDataAdjustLimited = (BOOL)::GetPrivateProfileInt(_T("FluoDataAdjust"), _T("AdjustLimited"), 0, strIniFile);

}

// 保存公共配置参数
void CPublicInfo::SaveParameters()
{
	ASSERT(!m_strUserPath.IsEmpty());
	//CString strIniFile = GetIniFilePath(INI_FILE_SYSTEM, FALSE);
	CString strIniFile = m_strStartupPath + _T("\\System\\") + _T("system.ini");

	CString strValue;

	strValue.Format(_T("%d"), m_nAnaDataType);
	::WritePrivateProfileString(_T("Analysis"), _T("DataType"), strValue, strIniFile);

	strValue.Format(_T("%d"), m_bFluoDataAutoAdjust);
	::WritePrivateProfileString(_T("FluoDataAdjust"), _T("AutoAdjust"), strValue, strIniFile);
	strValue.Format(_T("%d"), m_iFluoDataMaxLimited);
	::WritePrivateProfileString(_T("FluoDataAdjust"), _T("MaxLimited"), strValue, strIniFile);
	strValue.Format(_T("%d"), m_iFluoDataAdjustLimited);
	::WritePrivateProfileString(_T("FluoDataAdjust"), _T("AdjustLimited"), strValue, strIniFile);

}

void CPublicInfo::SetUserDataSavePath(CString strPath)
{
	if (strPath != m_strUserDataPath)
	{
		m_strUserDataPath = strPath;
		CString strIniFile = GetIniFilePath(INI_FILE_SYSTEM, FALSE);
		::WritePrivateProfileString(m_strUserName, _T("SavePath"), m_strUserDataPath, strIniFile);
	}
}

CString CPublicInfo::SampleTypeToText(eSampleType eType)
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	switch(eType)
	{
	case SAMPLE_TYPE_NONE:	
		return pLangInfo->GetText(292);
	case SAMPLE_TYPE_UNKNOWN:	
		return pLangInfo->GetText(211);
	case SAMPLE_TYPE_STANDARD:	
		return pLangInfo->GetText(212);
	case SAMPLE_TYPE_NTC:	
		return pLangInfo->GetText(213);
	case SAMPLE_TYPE_NEGATIVE:	
		return pLangInfo->GetText(214);
	case SAMPLE_TYPE_POSITIVE:	
		return pLangInfo->GetText(215);
	case SAMPLE_TYPE_POS_CRITICAL:	
		return pLangInfo->GetText(216);
	case SAMPLE_TYPE_CALIBRATOR:	
		return pLangInfo->GetText(217);
	case SAMPLE_TYPE_BUFFER:	
		return pLangInfo->GetText(218);
	case SAMPLE_TYPE_IC:	
		return pLangInfo->GetText(457);
	default: break;
	}
	
	return _T("");
}

CString CPublicInfo::GetSampleTypeAbbrText(eSampleType eType)
{
	switch(eType)
	{
	case SAMPLE_TYPE_UNKNOWN:	
		return _T("UN");
	case SAMPLE_TYPE_STANDARD:	
		return _T("ST");
	case SAMPLE_TYPE_POSITIVE:	
		return _T("PC");
	case SAMPLE_TYPE_NEGATIVE:	
		return _T("NC");
	case SAMPLE_TYPE_NTC:	
		return _T("NT");
	case SAMPLE_TYPE_IC:
		return _T("IC");
	
	default: break;
	}

	return _T("");
}

eSampleType CPublicInfo::TextToSampleType(CString strText)
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	if(strText == _T("UN"))
	{
		return SAMPLE_TYPE_UNKNOWN;
	}
	else if(_T("ST") == strText)
	{
		return SAMPLE_TYPE_STANDARD;
	}
	else if(_T("PC") == strText)
	{
		return SAMPLE_TYPE_POSITIVE;
	}
	else if(_T("NC") == strText)
	{
		return SAMPLE_TYPE_NEGATIVE;
	}
	else if(_T("NT") == strText)
	{
		return SAMPLE_TYPE_NTC;
	}
	else if(pLangInfo->GetText(292) == strText)
	{
		return SAMPLE_TYPE_NONE;
	}
	else if(pLangInfo->GetText(211) == strText)
	{
		return SAMPLE_TYPE_UNKNOWN;
	}
	else if(pLangInfo->GetText(212) == strText)
	{
		return SAMPLE_TYPE_STANDARD;
	}
	else if(pLangInfo->GetText(213) == strText)
	{
		return SAMPLE_TYPE_NTC;
	}
	else if(pLangInfo->GetText(214) == strText)
	{
		return SAMPLE_TYPE_NEGATIVE;
	}
	else if(pLangInfo->GetText(215) == strText)
	{
		return SAMPLE_TYPE_POSITIVE;
	}
	else if(pLangInfo->GetText(216) == strText)
	{
		return SAMPLE_TYPE_POS_CRITICAL;
	}
	else if(pLangInfo->GetText(217) == strText)
	{
		return SAMPLE_TYPE_CALIBRATOR;
	}
	else if(pLangInfo->GetText(218) == strText)
	{
		return SAMPLE_TYPE_BUFFER;
	}

	return SAMPLE_TYPE_NONE;
}



