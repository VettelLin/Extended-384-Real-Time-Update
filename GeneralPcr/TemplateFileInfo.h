#pragma once
class CTemplateFileInfo
{
public:
	CTemplateFileInfo(void);
	~CTemplateFileInfo(void);

	CTemplateFileInfo(const CTemplateFileInfo& info);
	void operator=(const CTemplateFileInfo& info);

	BOOL IsFileExist();

public:
	CString m_strFilePath;
	CString m_strFileName;
	CString m_strTitle;
};

