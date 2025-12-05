#pragma once

typedef enum MyLstDotPosType{
	EDT_FRAME,EDT_HEADER,EDT_TOTALHEADER,EDT_ROWHEADER,EDT_COLHEADER,EDT_BODY,EDT_HORISCROLL,EDT_VERTSCROLL,EDT_NONE
}EMYLSTDOTPOSTYPE;

class CDotPosInfo
{
public:
	CDotPosInfo(void);
	~CDotPosInfo(void);

	CDotPosInfo(const CDotPosInfo& dotPosInfo);
	void operator=(const CDotPosInfo& dotPosInfo);
	BOOL operator==(const CDotPosInfo& dotPosInfo);

	BOOL BeSameRowCol(CDotPosInfo& dotPosInfo);
	BOOL BeSameRow(CDotPosInfo& dotPosInfo);
	BOOL BeSameCol(CDotPosInfo& dotPosInfo);

	BOOL BeValidCellPos();	//标识是否在EDT_BODY已经存在的单元格中

public:
	EMYLSTDOTPOSTYPE m_eDotPosType;
	int m_rowId;
	int m_colId;
	CPoint m_pt;
};

