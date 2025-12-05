#pragma once

#include <GdiPlus.h>
using namespace Gdiplus;

class CHoleColorInfo
{
public:
	CHoleColorInfo(void);
	~CHoleColorInfo(void);

	CHoleColorInfo(const CHoleColorInfo& info);
	void operator=(const CHoleColorInfo& info);

public:
	Color m_clrFrame;	//边框颜色
	Color m_clrFill;	//填充颜色
	COLORREF m_clrTxUnSelected;	//未选中状态文字颜色
	COLORREF m_clrTxSelected;	//选中状态文字颜色
};

