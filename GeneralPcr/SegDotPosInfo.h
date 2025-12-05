#pragma once

typedef enum DotPosType{
	EDP_CYCLE = 1,EDP_TEM,EDP_TIME,EDP_COLLECTFLU,EDP_NONE,EDP_LINE,EDP_SEGNAME,EDP_HORISCROLL,EDP_VERTSCROLL,EDP_SEGEMPTY
}EDOTPOSTYPE;

class CSegDotPosInfo
{
public:
	CSegDotPosInfo(void);
	~CSegDotPosInfo(void);

	CSegDotPosInfo(const CSegDotPosInfo& segDotPosInfo);
	void operator=(const CSegDotPosInfo& segDotPosInfo);

	BOOL BeValidStepEmptyPos();	//是否是Step空白区域，相对于空白区域、标题区域和Step可编辑区域
	BOOL BeValidSegmentEmptyPos();	//是否是Segment空白区域，相对于Segment名称和循环数区域

public:
	int m_seg;
	int m_step;
	EDOTPOSTYPE m_eDotPosType;

	CRect m_rcItem;
	CPoint m_pt;
};

