#pragma once


class AFX_EXT_CLASS CReportTemplateData
{
public:
	CReportTemplateData(void);
	~CReportTemplateData(void);

	enum eDataFlag
	{
		DATA_NAME = 0,
		DATA_LAB_NAME,
		DATA_ADDRESS,
		DATA_CONTACT_URL,
		DATA_CONTACT_EMAIL,
		DATA_CONTACT_PHONE,
		DATA_TEST_NAME,
		DATA_DISCLAIMER,
	};

	struct tagTemplate
	{
		CString		strName;
		CString		strLabName;
		CString		strLabAddress;
		CString		strContactUrl;
		CString		strContactEmail;
		CString		strContactPhone;
		CString		strTestName; //
		CString		strDisclaimer; // 免责声明
	};

public:
	void SetStartupPath(CString strPath);
	void DeleteAll();

	BOOL SaveFile();
	BOOL OpenFile();
	BOOL SaveFile(CString strFilePath);
	BOOL OpenFile(CString strFilePath);

	int GetTemplateCount() { return m_arrayTemplate.GetCount(); }

	int AddTemplate(eDataFlag nFlag, CString string);
	void DeleteTemplate(int iIndex);
	int AddTemplate(tagTemplate* pInputTemplate);
	tagTemplate* GetTemplate(int iIndex);

	CString GetStringBy(eDataFlag nFlag, int iIndex=-1);
	void SetStringTo(eDataFlag nFlag, CString string, int iIndex=-1);

	int GetCurSel() { return m_iUsedIndex; }
	void SetCurSel(int iIndex) { m_iUsedIndex = iIndex; }
	void SetCurSel(CString strName);

	void SetChanged(BOOL bChanged) { m_bChanged = bChanged; }
	BOOL GetChanged() { return m_bChanged;}

	void CopyTo(CReportTemplateData* pTargetData);
	void CopyFrom(CReportTemplateData* pSourceData);

	BOOL FindSameName(CString strName);

protected:
	CString GetStringBy(tagTemplate* pTemplate, eDataFlag nFlag);
	void SetStringTo(tagTemplate* pTemplate, eDataFlag nFlag, CString string);

protected:
	WORD	m_wFileFlag; // 文件标识码
	WORD	m_wFileVersion; // 文件版本

	CString m_strFilePath;

	int	m_iUsedIndex;
	CPtrArray	m_arrayTemplate;

	BOOL m_bChanged;
};

