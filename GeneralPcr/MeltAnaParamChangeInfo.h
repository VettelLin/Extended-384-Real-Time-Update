#pragma once

#include<vector>
#include "ChanMeltAnaParamChangeInfo.h"

using namespace std;

class CMeltAnaParamChangeInfo
{
public:
	CMeltAnaParamChangeInfo(void);
	~CMeltAnaParamChangeInfo(void);

	CMeltAnaParamChangeInfo(const CMeltAnaParamChangeInfo& info);
	void operator=(const CMeltAnaParamChangeInfo& info);

	int SetChannelNum(int iChanNum);

	BOOL IsAnaParamChanged();
	BOOL IsChannelAnaParamChanged(int iChanId);

public:
	int m_iChanNum;
	vector<CChanMeltAnaParamChangeInfo> m_vecChanMeltChangeInfo; 
};

