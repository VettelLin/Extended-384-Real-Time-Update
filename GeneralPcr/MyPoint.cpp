#include "StdAfx.h"
#include "MyPoint.h"


CMyPoint::CMyPoint(void)
{
	m_X = 0;
	m_Y = 0;
	m_xPixel = 0;
	m_yPixel = 0;
}


CMyPoint::~CMyPoint(void)
{

}

CMyPoint::CMyPoint(const CMyPoint& pt)
{
	m_X = pt.m_X;
	m_Y = pt.m_Y;
	m_xPixel = pt.m_X;
	m_yPixel = pt.m_Y;
}

void CMyPoint::operator=(const CMyPoint& pt)
{
	m_X = pt.m_X;
	m_Y = pt.m_Y;
	m_xPixel = pt.m_X;
	m_yPixel = pt.m_Y;
}

BOOL CMyPoint::operator==(const CMyPoint& pt)
{
	return (m_X == pt.m_X && m_Y == pt.m_Y);
}

BOOL CMyPoint::operator<(const CMyPoint& pt)
{
	return m_X < pt.m_X;
}

BOOL CMyPoint::operator>(const CMyPoint& pt)
{
	return m_X > pt.m_X;
}

BOOL CMyPoint::operator<=(const CMyPoint& pt)
{
	return m_X <= pt.m_X;
}

BOOL CMyPoint::operator>=(const CMyPoint& pt)
{
	return m_X >= pt.m_X;
}
