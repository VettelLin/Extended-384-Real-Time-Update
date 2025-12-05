#pragma once
class CCaretInfo
{
public:
	CCaretInfo(void);
	~CCaretInfo(void);

	CCaretInfo(const CCaretInfo& caretInfo);
	void operator=(const CCaretInfo& caretInfo);

public:
	int m_pxXStart;	//插入符水平方向起始位置
	int m_pxYStart;	//插入符垂直方向起始位置
	int m_pxWidth;	//插入符宽度
	int m_pxHeight;	//插入符高度

	int m_iCurCharIndex;	//记录插入符当前处于哪个字符的（紧邻）后面
};

