#pragma once

#include <GdiPlus.h>

class CIconBtInfo
{
public:
	CIconBtInfo(void);
	~CIconBtInfo(void);

	CIconBtInfo(const CIconBtInfo& info);
	void operator=(const CIconBtInfo& info);

public:
	BOOL m_bUseIcon;
	CString m_strIconPath;
	int m_iSonIconNum;
	BOOL m_bSonIconPosOrder;	//子图标是否按照正序排列

	Rect m_rcIcon;
	CRect m_rcText;

	BOOL m_bRoundRectBk;
	int m_iRoundRadius;
	int m_iBoderLineW;

	Color m_clrFrameNormal;
	Color m_clrFrameHover;
	Color m_clrFrameSelected;
	Color m_clrFrameDisabled;

	Color m_clrNormal;
	Color m_clrHover;
	Color m_clrSelected;
	Color m_clrDisabled;

	BOOL m_bNormalNotFill;	//普通状态下不填充圆角矩形
	BOOL m_bHoverNotFill;	//悬浮状态下不填充圆角矩形
};

