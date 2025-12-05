#pragma once

typedef enum MySelWndDotPosType{
	EMSWDPT_FRAME,EMSWDPT_BODY,EMSWDPT_VERTSCROLL,EMSWDPT_NONE
}EMYSELWNDDOTPOSTYPE;

class CMySelWndDotPosInfo
{
public:
	CMySelWndDotPosInfo(void);
	~CMySelWndDotPosInfo(void);

	CMySelWndDotPosInfo(const CMySelWndDotPosInfo& dotPosInfo);
	void operator=(const CMySelWndDotPosInfo& dotPosInfo);
	BOOL operator==(const CMySelWndDotPosInfo& dotPosInfo);

	BOOL BeSameRow(CMySelWndDotPosInfo& dotPosInfo);
	
	void Clear();

public:
	EMYSELWNDDOTPOSTYPE m_eDotPosType;
	int m_rowId;
	CPoint m_pt;
};

