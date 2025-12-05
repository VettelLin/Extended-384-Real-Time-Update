#pragma once
class CTubeChanEditInfo
{
public:
	CTubeChanEditInfo(void);
	~CTubeChanEditInfo(void);

	CTubeChanEditInfo(const CTubeChanEditInfo& info);
	void operator=(const CTubeChanEditInfo& info);

	BOOL GetBeAllTubeSameSet();	//判断是否是所有孔位在某些参数上要保持一致（通道的染料名和颜色标识）

public:
	int m_iRowId;
	int m_iColId;
	BOOL m_bChecked;
	double m_dData;
	int m_iDpd;	//m_dData的小数点位数
	CString m_strData;
	COLORREF m_clrData;
};

