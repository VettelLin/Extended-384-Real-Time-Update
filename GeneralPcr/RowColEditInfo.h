#pragma once

//记录正在编辑的行列信息

class CRowColEditInfo
{
public:
	CRowColEditInfo(void);
	~CRowColEditInfo(void);

	CRowColEditInfo(const CRowColEditInfo& rowColEditInfo);
	void operator=(const CRowColEditInfo& rowColEditInfo);
	BOOL operator==(const CRowColEditInfo& rowColEditInfo);

	BOOL BeValidRowCol();
	void Clear();

public:
	int m_rowId;
	int m_colId;

	CString m_strText;	//修改之前的文本
	CString m_strNewText;	//修改之后的文本
};

