#pragma once
class CMyPoint
{
public:
	CMyPoint(void);
	~CMyPoint(void);
	CMyPoint(const CMyPoint& pt);

	void operator=(const CMyPoint& pt);
	BOOL operator==(const CMyPoint& pt);
	BOOL operator<(const CMyPoint& pt);
	BOOL operator>(const CMyPoint& pt);
	BOOL operator<=(const CMyPoint& pt);
	BOOL operator>=(const CMyPoint& pt);

public:
	double m_X;
	double m_Y;

	int m_xPixel;	//点在坐标轴中的水平位置
	int m_yPixel;	//点在坐标轴中的垂直位置
};

