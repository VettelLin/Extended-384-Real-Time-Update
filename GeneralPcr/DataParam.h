#pragma once

typedef enum DataType{
	EDT2_NONE,EDT2_INT,EDT2_DOUBLE,EDT2_STRING
}EDATATYPE;

class CDataParam
{
public:
	CDataParam(void);
	CDataParam(EDATATYPE eDtType,int iDpd,BOOL bData);
	~CDataParam(void);

	CDataParam(const CDataParam& info);
	void operator=(const CDataParam& info);

	int DataFilter(CString& strData);

public:
	EDATATYPE m_eDataType;
	int m_iDpd;	//小数点位数
	BOOL m_bUseDataFilter;	//是否使用数据过滤功能
};

