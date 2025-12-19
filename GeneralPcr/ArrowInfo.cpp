#include "StdAfx.h"
#include "ArrowInfo.h"


CArrowInfo::CArrowInfo(void)
{
	m_arrowW = 9;
	m_arrowH = 7;
	m_arrowRightIndent = 11;
	m_clrArrow = Color(128,128,128);
	m_clrLineNormal = Color(0X33,0X33,0X33);
	m_clrLineDropDown = Color(0X33,0X33,0X33);

	for(int i = 0;i < 3;++i)
	{
		m_aryPtTriangle[i].X = 0;
		m_aryPtTriangle[i].Y = 0;
		m_aryPtTriangleDropDown[i].X = 0;
		m_aryPtTriangleDropDown[i].Y = 0;
		m_aryPtTriangleLeft[i].X = 0;
		m_aryPtTriangleLeft[i].Y = 0;
		m_aryPtTriangleRight[i].X = 0;
		m_aryPtTriangleRight[i].Y = 0;
	}

	m_eArrowStyle = EAS_FILL;
	m_iLineW = 2;
}

CArrowInfo::~CArrowInfo(void)
{

}

CArrowInfo::CArrowInfo(const CArrowInfo& arrowInfo)
{
	m_arrowW = arrowInfo.m_arrowW;
	m_arrowH = arrowInfo.m_arrowH;
	m_arrowRightIndent = arrowInfo.m_arrowRightIndent;
	m_clrArrow = arrowInfo.m_clrArrow;
	m_clrLineNormal = arrowInfo.m_clrLineNormal;
	m_clrLineDropDown = arrowInfo.m_clrLineDropDown;

	for(int i = 0;i < 3;++i)
	{
		m_aryPtTriangle[i] = arrowInfo.m_aryPtTriangle[i];
		m_aryPtTriangleDropDown[i] = arrowInfo.m_aryPtTriangleDropDown[i];
		m_aryPtTriangleLeft[i] = arrowInfo.m_aryPtTriangleLeft[i];
		m_aryPtTriangleRight[i] = arrowInfo.m_aryPtTriangleRight[i];
	}
	m_eArrowStyle = arrowInfo.m_eArrowStyle;
	m_iLineW = arrowInfo.m_iLineW;
}

void CArrowInfo::operator=(const CArrowInfo& arrowInfo)
{
	m_arrowW = arrowInfo.m_arrowW;
	m_arrowH = arrowInfo.m_arrowH;
	m_arrowRightIndent = arrowInfo.m_arrowRightIndent;
	m_clrArrow = arrowInfo.m_clrArrow;
	m_clrLineNormal = arrowInfo.m_clrLineNormal;
	m_clrLineDropDown = arrowInfo.m_clrLineDropDown;

	for(int i = 0;i < 3;++i)
	{
		m_aryPtTriangle[i] = arrowInfo.m_aryPtTriangle[i];
		m_aryPtTriangleDropDown[i] = arrowInfo.m_aryPtTriangleDropDown[i];
		m_aryPtTriangleLeft[i] = arrowInfo.m_aryPtTriangleLeft[i];
		m_aryPtTriangleRight[i] = arrowInfo.m_aryPtTriangleRight[i];
	}
	m_eArrowStyle = arrowInfo.m_eArrowStyle;
	m_iLineW = arrowInfo.m_iLineW;
}

//void CArrowInfo::CalcPtData(CRect rect)
//{
//	int tempInt1 = rect.right - m_arrowW - m_arrowRightIndent;
//
//	m_aryPtTriangle[0].X = tempInt1;
//	m_aryPtTriangle[0].Y = rect.top + (rect.Height() - m_arrowH) / 2;
//	m_aryPtTriangle[1].X = tempInt1 + m_arrowW / 2;
//	m_aryPtTriangle[1].Y = rect.top + (rect.Height() - m_arrowH) / 2 + m_arrowH;
//	m_aryPtTriangle[2].X = tempInt1 + m_arrowW;
//	m_aryPtTriangle[2].Y = rect.top + (rect.Height() - m_arrowH) / 2;
//
//	m_aryPtTriangleDropDown[0].X = tempInt1;
//	m_aryPtTriangleDropDown[0].Y = rect.top + (rect.Height() - m_arrowH) / 2 + m_arrowH;
//	m_aryPtTriangleDropDown[1].X = tempInt1 + m_arrowW / 2;
//	m_aryPtTriangleDropDown[1].Y = rect.top + (rect.Height() - m_arrowH) / 2;
//	m_aryPtTriangleDropDown[2].X = tempInt1 + m_arrowW;
//	m_aryPtTriangleDropDown[2].Y = rect.top + (rect.Height() - m_arrowH) / 2 + m_arrowH;
//
//	int tempInt2 = rect.top + (rect.Height() - m_arrowH) / 2;
//	int tempInt3 = rect.left + (rect.Width() - m_arrowW) / 2;
//
//	m_aryPtTriangleLeft[0].X = rect.right - m_arrowRightIndent;
//	m_aryPtTriangleLeft[0].Y = tempInt2;
//	m_aryPtTriangleLeft[1].X = tempInt3;
//	m_aryPtTriangleLeft[1].Y = tempInt2 + m_arrowH / 2;
//	m_aryPtTriangleLeft[2].X = rect.right - m_arrowRightIndent;
//	m_aryPtTriangleLeft[2].Y = tempInt2 + m_arrowH;
//
//	m_aryPtTriangleRight[0].X = rect.left + m_arrowRightIndent;	//右向箭头和左向箭头对称
//	m_aryPtTriangleRight[0].Y = tempInt2;
//	m_aryPtTriangleRight[1].X = rect.right - (rect.Width() - m_arrowW) / 2;
//	m_aryPtTriangleRight[1].Y = tempInt2 + m_arrowH / 2;
//	m_aryPtTriangleRight[2].X = m_aryPtTriangleRight[0].X;
//	m_aryPtTriangleRight[2].Y = tempInt2 + m_arrowH;
//}


void CArrowInfo::CalcPtData(CRect rect)
{
	int tempInt1 = rect.left + (rect.Width() - m_arrowW) / 2;

	m_aryPtTriangle[0].X = tempInt1;
	m_aryPtTriangle[0].Y = rect.top + (rect.Height() - m_arrowH) / 2;
	m_aryPtTriangle[1].X = tempInt1 + m_arrowW / 2;
	m_aryPtTriangle[1].Y = rect.top + (rect.Height() - m_arrowH) / 2 + m_arrowH;
	m_aryPtTriangle[2].X = tempInt1 + m_arrowW;
	m_aryPtTriangle[2].Y = rect.top + (rect.Height() - m_arrowH) / 2;

	m_aryPtTriangleDropDown[0].X = tempInt1;
	m_aryPtTriangleDropDown[0].Y = rect.top + (rect.Height() - m_arrowH) / 2 + m_arrowH;
	m_aryPtTriangleDropDown[1].X = tempInt1 + m_arrowW / 2;
	m_aryPtTriangleDropDown[1].Y = rect.top + (rect.Height() - m_arrowH) / 2;
	m_aryPtTriangleDropDown[2].X = tempInt1 + m_arrowW;
	m_aryPtTriangleDropDown[2].Y = rect.top + (rect.Height() - m_arrowH) / 2 + m_arrowH;

	int tempInt2 = rect.top + (rect.Height() - m_arrowH) / 2;
	int tempInt3 = rect.left + (rect.Width() - m_arrowW) / 2;

	m_aryPtTriangleLeft[0].X = tempInt3 + m_arrowW;
	m_aryPtTriangleLeft[0].Y = tempInt2;
	m_aryPtTriangleLeft[1].X = tempInt3;
	m_aryPtTriangleLeft[1].Y = tempInt2 + m_arrowH / 2;
	m_aryPtTriangleLeft[2].X = tempInt3 + m_arrowW;
	m_aryPtTriangleLeft[2].Y = tempInt2 + m_arrowH;

	m_aryPtTriangleRight[0].X = tempInt3;	//右向箭头和左向箭头对称
	m_aryPtTriangleRight[0].Y = tempInt2;
	m_aryPtTriangleRight[1].X = tempInt3 + m_arrowW;
	m_aryPtTriangleRight[1].Y = tempInt2 + m_arrowH / 2;
	m_aryPtTriangleRight[2].X = m_aryPtTriangleRight[0].X;
	m_aryPtTriangleRight[2].Y = tempInt2 + m_arrowH;
}



