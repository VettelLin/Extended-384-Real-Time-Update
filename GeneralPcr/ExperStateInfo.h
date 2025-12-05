#pragma once

typedef enum InstruConnectState{
	EICS_CONNECT_SUCCESS,EICS_CONNECT_FAIL,EICS_NONE
}EINSTRUCONNECTSTATE;

typedef enum ExperType{
	EET_ABSOLUTE,EET_MELT,EET_NONE
}EEXPERTYPE;

class CExperStateInfo
{
public:
	CExperStateInfo(void);
	~CExperStateInfo(void);

	CExperStateInfo(const CExperStateInfo& info);
	void operator=(const CExperStateInfo& info);

public:
	EINSTRUCONNECTSTATE m_eInstruConnectState;	//仪器连接状态
	EEXPERTYPE m_eExperType;	//实验类型

	CString m_strModel;	//仪器型号
	CString m_strExperName;	//实验名称
	CString m_strRemainTime;	//剩余时间
	CString m_strExperStartTime;	//实验开始时间
	double m_dProgress;	//实验进度
	double m_dExperTem;	//实验运行过程中的实时温度
};

