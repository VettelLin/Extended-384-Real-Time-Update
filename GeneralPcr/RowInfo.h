#pragma once
class CRowInfo
{
public:
	CRowInfo(void);
	~CRowInfo(void);

	CRowInfo(const CRowInfo& info);
	void operator=(const CRowInfo& info);

public:
	int m_rowH;	//行高，不同于CCellInfo中的单元格高度
	BOOL m_bSelected;	//行是否选中
	BOOL m_bDisabled;	//是否禁用整行
};

