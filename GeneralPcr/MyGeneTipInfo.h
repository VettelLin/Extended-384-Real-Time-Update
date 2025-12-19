#pragma once

typedef enum TipType
{
	EGTT_NORMAL = 1,EGTT_QUERY,EGTT_WARNING
}EGENETIPTYPE;

class CMyGeneTipInfo
{
public:
	CMyGeneTipInfo(void);
	~CMyGeneTipInfo(void);

	CMyGeneTipInfo(const CMyGeneTipInfo& geneTipInfo);
	void operator=(const CMyGeneTipInfo& geneTipInfo);

public:
	CWnd* m_pParent;
	CString m_strTipInfo;
	EGENETIPTYPE m_eGeneTipType;
};

