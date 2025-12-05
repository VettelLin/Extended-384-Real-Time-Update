#pragma once
class CMySegEditInfo
{
public:
	CMySegEditInfo(void);
	~CMySegEditInfo(void);

	CMySegEditInfo(const CMySegEditInfo& segEditInfo);
	void operator=(const CMySegEditInfo& segEditInfo);

public:
	CString m_strSegName;	//程序段名称
	int m_cycleNum;	//循环数
	BOOL m_bMeltSeg;	//标识Segment是否包含熔解步骤
};

