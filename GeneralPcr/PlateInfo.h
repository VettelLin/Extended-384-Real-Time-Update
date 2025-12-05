#pragma once

//保存孔板孔位绘制参数

class CPlateInfo
{
public:
	CPlateInfo(void);
	~CPlateInfo(void);

	CPlateInfo(const CPlateInfo& info);
	void operator=(const CPlateInfo& info);

public:
	BOOL m_bSelected;	//孔位是否选中
};

