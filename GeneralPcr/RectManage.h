#pragma once

#include "LinkList.h"

class CRectManage
{
public:
	CRectManage(void);
	~CRectManage(void);

	CRectManage(const CRectManage& info);
	void operator=(const CRectManage& info);

	int RectMinusRect(CRect rcSource,CRect rect,CLinkList<CRect>& rcRst);
	int CalcRectResult();
	int AddRectToDelete(CRect rect);
	int AddRectToDelete(int iLeft,int iTop,int iWidth,int iHeight);
	int ClearRectToDelete();
	int AddRect(CRect rect);
	int AddRect(int iLeft,int iTop,int iWidth,int iHeight);
	int DeleteRect(CRect rect);

	int AddRectRoundFrame(CRect rect,int iRoundH,int iFrameW);	//添加圆角矩形的四角和边框矩形
	int AddRectRoundFrame(int iLeft,int iTop,int iWidth,int iHeight,int iRoundH,int iFrameW);	//添加圆角矩形的四角和边框矩形

public:
	CRect m_rcInit;

	CLinkList<CRect> m_lnkRcToDelete;
	CLinkList<CRect> m_lnkRcRst;
};

