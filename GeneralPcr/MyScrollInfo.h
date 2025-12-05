
#pragma once

#include "ArrowInfo.h"

//滚动条类型(水平滚动条、垂直滚动条、未初始化)
typedef enum ScrollType{
	EST_HORI = 1,EST_VERT,EST_NONE
}ESCROLLTYPE;

typedef enum SliderShape{
	ESS_RECTANGLE,ESS_ROUNDRECTANGLE
}ESLIDERSHAPE;

class CMyScrollInfo
{
public:
	CMyScrollInfo();
	~CMyScrollInfo();

	CMyScrollInfo(const CMyScrollInfo& info);
	void operator=(const CMyScrollInfo& info);

	void CalcSliderPos();
	void CalcCurPixelPos();

	void CalcCurPointInPane(CPoint point);
	BOOL IsInScrollPane();

public:
	CRect m_rcScroll;
	CRect m_rcStartArrow;
	CRect m_rcEndArrow;
	CRect m_rcSlider;

	COLORREF m_clrBk;
	COLORREF m_clrScrollFrame;	//滚动条边框颜色
	COLORREF m_clrStartArrowRegionFrame;	//是否显示左侧、顶部箭头区域边框
	COLORREF m_clrEndArrowRegionFrame;	//是否显示右侧、底部箭头区域边框
	Color m_clrSliderBk;
	Color m_clrSliderFrame;

	BOOL m_bInSlider;	//标识鼠标是否在滑块内
	BOOL m_bLeftBtDown;	//标识鼠标左键是否被按下
	BOOL m_bStartDragSlider;	//是否开始拖拽滑块
	BOOL m_bSliderDragMove;	//滑块是否正在被拖动

	int m_arrowWH;	//箭头高度或者宽度

	int m_scrollW;	//滚动条宽度
	int m_scrollH;	//滚动条高度

	int m_pixelForShow;	//控件可显示宽度或者高度（该容量内不需要滚动条）
	int m_pixelTotal;	//控件当前显示所有数据需要的宽度或者高度
	int m_curPixelPos;	//滑块当前位置在body中对应的像素位置
	int m_curSliderPos;	//滑块在滚动条有效区域中的像素位置

	int m_validScope;	//滑块在滚动条中最大可移动距离

	BOOL m_bScrollNeedShow;	//标识滚动条是否需要显示

	double m_dSliderRatio;	//滑块占滚动条有效区域（不包括箭头区域）比例值
	double m_dScrollRatio;	//滚动条中的1个像素对应body中的像素数

	CPoint m_prePt;	//记录鼠标移动过程中前一个点

	ESCROLLTYPE m_eScrollType;

	BOOL m_bShowStartArrowRegion;	//是否绘制左侧或者顶部箭头区域
	BOOL m_bShowEndArrowRegion;	//是否绘制右侧或者底部箭头区域
	BOOL m_bShowSliderFrame;	//是否显示滑块边框
	BOOL m_bShowScrollFrame;	//是否显示滚动条边框
	BOOL m_bShowStartArrowRegionFrame;	//是否显示左侧、顶部箭头区域边框
	BOOL m_bShowEndArrowRegionFrame;	//是否显示右侧、底部箭头区域边框

	ESLIDERSHAPE m_eSliderShape;	//滑块外形

	double m_dHoriRatio;
	double m_dVertRatio;

	CArrowInfo m_arrowInfo1;
	CArrowInfo m_arrowInfo2;

	BOOL m_bInStartArrow;
	BOOL m_bInStartPane;
	BOOL m_bInEndPane;
	BOOL m_bInEndArrow;
};




