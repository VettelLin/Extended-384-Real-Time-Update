#pragma once

#define WM_EDIT_VALUE_CHANGED		WM_USER + 2101 // 数值改变，根据消息用户完成与改变相关的处理

// 增强性编辑框控件，提供文件选择、路径选择等操作

class AFX_EXT_CLASS CBioEdit : public CEdit
{
public:
	CBioEdit();
	virtual ~CBioEdit();

// Attributes
public:
	enum BioEditMode
	{
		EditMode_Normal = 0, ///< 通常编辑框
		EditMode_Default = 1, ///< 默认按钮
		EditMode_File = 2, ///< 文件选择按钮
		EditMode_Folder = 3, ///< 路径选择按钮
		EditMode_Number = 4, ///< 数字编辑框
		EditMode_Spin = 5 ///< 带spin按钮的编辑框 
	};

protected:
	BioEditMode	m_nMode; ///< 编辑框的操作模式
	int			m_nButtonWidth;
	CRect		m_rectButton;
	CImageList	m_ImageButton;
	CSize		m_sizeImage;

	BOOL		m_bIsMinus; ///< 是否支持负数，默认为正数
	BOOL		m_bIsButtonPressed;
	BOOL		m_bIsButtonHighlighted;
	BOOL		m_bIsButtonCaptured;
	CString		m_strLabel;
	CString		m_strFileExt;
	CString		m_strFileFilter;
	CString		m_strFolderTitle; ///< 浏览文件夹时的标题
	LPCTSTR		m_lpszInitPath; ///< 浏览文件夹时初始化路径

	// 处理数字编辑框
	CString m_strValue;
	double m_dStepValue; ///< 步长
	double m_dMinValue; ///< 最小值
	double m_dMaxValue; ///< 最大值
	UINT m_nPrecision; ///< 浮点数的精度
	// 处理spin编辑框
	BOOL m_bIsInTop;///< 上按钮是否按下
	CRect m_rectTop; ///< 上按钮大小
	CRect m_rectBottom; ///< 下按钮大小

	BOOL	m_bAutoCheck; ///< 是否自动检测数据的合法性
	UINT	m_nTimer; ///< 计时器ID
	UINT	m_nSleepTime; ///< 计数器间隔时间
	
	BOOL	m_bWantAllKeys;

// Operations
public:
	void EnableMinus(){m_bIsMinus=TRUE;}
	void EnableDefaultButton(CString strLabel = _T("..."));
	void EnableFileBrowseButton(CString strFileExt = _T(""), CString strFilter = _T(""));
	void EnableFolderBrowseButton(CString strFolderTitle = _T(""));
	void EnableSpinNumButton();
	void EnableNumberEdit();
	void EnableAutoCheck(BOOL bEnable=TRUE){m_bAutoCheck=bEnable;}
	void EnableWantAllKeys(BOOL bEnable=TRUE){ m_bWantAllKeys = bEnable; }

	void GetValue(double &dOutput);
	void SetValue(double dInput);
	void GetValue(long &lOutput);
	void SetValue(long lInput);
	void SetRange(double dMinValue , double dMaxValue);
	void GetRange(double& dMinValue, double& dMaxValue);
	void SetStepValue(double dStep);
	double GetStepValue(void);
	void SetPrecision(UINT nInPrecision);
	
	void SetTimerSleepTime(UINT nTime){m_nSleepTime=nTime;}
	

protected:
	void SetIntenalImage();
	void DrawButtonFrame(CDC* pDC, CRect rect, BOOL bIsPressed);
	void OnClickFileButton();
	void OnClickFolderButton();
	static int CALLBACK BrowseFolderCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	
	void DrawDefaultButtonLabel(CDC* pDC, CRect rect, BOOL bIsButtonPressed);
	void JudgeNumberIsLegal(void);

// Overrides
protected:
	virtual void OnClickButton();
	virtual void DrawButton(CDC* pDC, CRect rect, BOOL bIsButtonPressed, BOOL bIsButtonHot);
	virtual void ChangeLayout();

// Implementation
protected:
	afx_msg void OnNcPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	//afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();

	DECLARE_MESSAGE_MAP()
};