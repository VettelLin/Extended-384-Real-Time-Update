#pragma once

enum eLogType
{
	LOG_TYPE_RECORD = 0, // 一般日志，为了记录保存
	LOG_TYPE_TIP, // 提示
	LOG_TYPE_WARM, // 警告
	LOG_TYPE_ERROR, // 错误
};

// 数据库中系统运行日志信息
class AFX_EXT_CLASS CLogRunInfo
{
public:
	CLogRunInfo() {
		m_lIndex = -1;
		m_strDate.Empty();
		m_strContent.Empty();
		m_strUser.Empty();
		m_strInstrument.Empty();
		m_nLogType = LOG_TYPE_RECORD;
		m_strErrorIndex.Empty();
		m_nStatus = 0; 
	}
	virtual ~CLogRunInfo() {}

// Attributes
public:
	long GetIndex()  const { return m_lIndex; }
	void SetIndex(long lIndex) { m_lIndex = lIndex; }
	CString GetDate() const  { return m_strDate; }
	void SetDate(CString strDate){m_strDate = strDate;}
	CString GetContent() const  { return m_strContent; }
	void SetContent(CString strContent){m_strContent = strContent;}
	CString GetUser() const  { return m_strUser; }
	void SetUser(CString strUser){m_strUser = strUser;}
	CString GetInstrument() const  { return m_strInstrument; }
	void SetInstrument(CString strContent){m_strInstrument = strContent;}
	eLogType GetLogType() { return m_nLogType; }
	void SetLogType(eLogType nType) { m_nLogType = nType; }
	CString GetErrorIndex() const  { return m_strErrorIndex; }
	void SetErrorIndex(CString strIndex){m_strErrorIndex = strIndex;}
	int GetStatus() { return m_nStatus; }
	void SetStatus(int nStatus) { m_nStatus = nStatus; }

protected:
	long		m_lIndex;
	CString		m_strDate; // 日志时间，按照年月日时分秒
	CString		m_strContent; // 日志内容，限制100字符
	CString		m_strUser;
	CString		m_strInstrument;
	eLogType	m_nLogType; // 日志类型,0:正常 1:提示 2:警告 3:错误
	CString		m_strErrorIndex; // 保存警告或错误日志的原因序号数组，限制50字符
	int			m_nStatus; // 日志状态，0:正常，1: 删除不显示
};
typedef CTypedPtrList<CPtrList, CLogRunInfo*> CLogRunInfoList;

// 数据库中系统错误和警告日志信息
class AFX_EXT_CLASS CLogErrorInfo
{
public:
	CLogErrorInfo() {
		m_lIndex = -1;
		m_strDate.Empty();
		m_strError.Empty();
		m_strErrPara1.Empty();
		m_strErrPara2.Empty();
		m_strUser.Empty();
		m_strInstrument.Empty();
		m_nLogType = LOG_TYPE_ERROR;
	}
	virtual ~CLogErrorInfo() {}

public:
	void SetErrorAppend(CString strAppend) { m_strError =+ strAppend; }

// Attributes
public:
	long GetIndex()  const { return m_lIndex; }
	void SetIndex(long lIndex) { m_lIndex = lIndex; }
	CString GetDate() const  { return m_strDate; }
	void SetDate(CString strDate){m_strDate = strDate;}
	CString GetError() const  { return m_strError; }
	void SetError(CString strError){m_strError = strError;}
	CString GetErrPara1() const  { return m_strErrPara1; }
	void SetErrPara1(CString strError){m_strErrPara1 = strError;}
	CString GetErrPara2() const  { return m_strErrPara2; }
	void SetErrPara2(CString strError){m_strErrPara2 = strError;}
	CString GetUser() const  { return m_strUser; }
	void SetUser(CString strUser){m_strUser = strUser;}
	CString GetInstrument() const  { return m_strInstrument; }
	void SetInstrument(CString strContent){m_strInstrument = strContent;}
	eLogType GetLogType() { return m_nLogType; }
	void SetLogType(eLogType nType) { m_nLogType = nType; }

protected:
	long		m_lIndex;
	CString		m_strDate; // 日志时间，按照年月日时分秒
	CString		m_strError;  // 日志内容，限制100字符
	CString		m_strErrPara1; // 日志内容补充说明1，限制50字符
	CString		m_strErrPara2; // 日志内容补充说明2，限制50字符
	CString		m_strUser;
	CString		m_strInstrument;
	eLogType	m_nLogType; // 日志类型, 2:警告 3:错误
};
typedef CTypedPtrList<CPtrList, CLogErrorInfo*> CLogErrorInfoList;


// 数据库中通讯交互记录日志信息，方便查找问题
class AFX_EXT_CLASS CLogCommInfo
{
public:
	enum eCommType	{
		COMM_TYPE_SEND = 0,
		COMM_TYPE_RECEIVE,
	};

	CLogCommInfo() {
		m_strDate.Empty();
		m_strInstrument.Empty();
		m_strComment.Empty();
		m_nCommType = COMM_TYPE_SEND;
		m_nResult = 0;
		m_nSendCount = 0;
		m_strContent.Empty();
	}
	virtual ~CLogCommInfo() {}


// Attributes
public:
	CString GetDate() const  { return m_strDate; }
	void SetDate(CString strDate){m_strDate = strDate;}
	CString GetInstrument() const  { return m_strInstrument; }
	void SetInstrument(CString strContent){m_strInstrument = strContent;}
	CString GetComment() const  { return m_strComment; }
	void SetComment(CString strComment){m_strComment = strComment;}
	eCommType GetCommType() { return m_nCommType; }
	void SetCommType(eCommType nType) { m_nCommType = nType; }
	int GetResult() { return m_nResult; }
	void SetResult(int nResult) { m_nResult = nResult; }
	int GetSendCount() { return m_nSendCount; }
	void SetSendCount(int nCount) { m_nSendCount = nCount; }
	CString GetContent() const  { return m_strContent; }
	void SetContent(CString strContent){m_strContent = strContent;}

protected:
	CString		m_strDate; // 日志时间，按照年月日时分秒
	CString		m_strInstrument;
	CString		m_strComment;  // 通讯注释，方便查找，限制20字符
	eCommType	m_nCommType; // 通讯类型 1:发送 2:接受
	int			m_nResult; // 结果 0:成功 1:应答失败 2:超时 3:超出规定次数失败 4:收到数据校验错误 5:发送失败
	int			m_nSendCount; // 发送次数 1/2/3，0:接受
	CString		m_strContent; // 通讯内容
};
typedef CTypedPtrList<CPtrList, CLogCommInfo*> CLogCommInfoList;
