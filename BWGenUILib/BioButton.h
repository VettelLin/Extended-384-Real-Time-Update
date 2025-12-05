
#pragma once

//////////////////////////////////////////////////////////////////////
// CBioButton is a CButton derived class.

class AFX_EXT_CLASS CBioButton : public CButton
{
	DECLARE_DYNAMIC(CBioButton)

public:
	CBioButton();
    virtual ~CBioButton();
	
	enum eButtonStyle
	{
		BTN_STYLE_FLAT = 0, 
		BTN_STYLE_SEMIFLAT = 1,
		BTN_STYLE_NORMAL = 2,
		BTN_STYLE_NOBORDER = 3,
		BTN_STYLE_CIRCLE = 4,
		BTN_STYLE_THIN_EDGE = 5,
	};

	enum eButtonIconAlign
	{
		BTN_ALIGN_HORZ = 0,
		BTN_ALIGN_VERT = 1,
		BTN_ALIGN_HORZ_RIGHT = 2
	};

	enum eButtonIconType
	{
		BTN_TYPE_NORMAL = 0,
		BTN_TYPE_PRESSED,
		BTN_TYPE_DISABLED,
		BTN_TYPE_HILITED,

		BTN_MAX_TYPES
	};

	enum eButtonColor
	{
		BTN_COLOR_BKGIN = 0, ///< 鼠标在内的背景色
		BTN_COLOR_TXTIN, ///< 鼠标在内的文本色
		BTN_COLOR_BKGOUT,
		BTN_COLOR_TXTOUT,
		BTN_COLOR_BKGFOCUS,
		BTN_COLOR_TXTFOCUS,
		
		BTN_MAX_COLORS
	};

protected:
	CFont	m_fontNormal;
	int		m_nStyle; ///< 按钮风格
	int		m_nAlign; ///< 按钮图标位置
	CToolTipCtrl*	m_pToolTip;
	CFont*	m_pFontText; ///< 文本的字体
	int		m_nCunstomNumber;	// 主要用于Tip、Tube数量的显示，初始化是-1，-1就是不显示
	
	HICON	m_hIcons[BTN_MAX_TYPES];
	CSize	m_sizeImage; // Initial size of the button.
	int		m_nBorderGap; // Gap between the button edge and the image.
	int		m_nImageGap; // Gap between the button text and the image.
	
	COLORREF	m_clrColors[BTN_MAX_COLORS];

	BOOL	m_bHasFlatBorder; ///< 扁平风格鼠标在上时是否绘制边框
	BOOL	m_bIsHilited; ///< 是否高亮，鼠标在按钮上
	BOOL	m_bLBtnDown; // TRUE if the left mouse button is pressed.
	BOOL	m_bPainted;	// Used during paint operations.
	BOOL	m_bIsChecked; ///< TRUE：被选中
	BOOL	m_bDefault; ///< 是否是默认按钮

	BOOL	m_bTransparent; ///< TRUE:按钮是透明的
	BOOL	m_bAutoDisabled; ///< 自动创建非激活图标，使用灰度图
	BOOL	m_bAutoBigger; ///< 在高亮时图标是否变大
	CSize	m_sizeBigImage; ///< 图标在高亮时的大小
	float	m_fBigPercent; ///< 变大比例

	BOOL	m_bGradualColor;
	COLORREF	m_clrTop;
	COLORREF	m_clrBottom;

	BOOL	m_bUser3DColor;
	COLORREF	m_clrUser3DTopLeft;
	COLORREF	m_clrUser3DRightBottom;

	COLORREF	m_clrThinEDGE;
	BOOL		m_bOnlyIcon;
	int			m_nCornerRadius;
	COLORREF	m_clrParentBgk;
	
public:
	BOOL IsChecked(){return m_bIsChecked;}
	void SetOnlyIcon(BOOL bOnlyIcon) { m_bOnlyIcon = bOnlyIcon; }

	void SetButtonFlat(BOOL bRedraw=TRUE);
	void SetButtonSemiflat(BOOL bRedraw=TRUE);
	void SetButtonNormal(BOOL bRedraw=TRUE);
	void SetButtonNoBorder(BOOL bRedraw=TRUE);
	void SetButtonDefault(BOOL bRedraw=TRUE);
	void SetButtonCircle(BOOL bRedraw=TRUE);
	void SetButtonThinEDGE(BOOL bRedraw=TRUE);

	void SetButtonChecked(BOOL bChecked, BOOL bRedraw=TRUE);

	void SetIconRight(BOOL bRedraw=TRUE);
	void SetIconVert(BOOL bRedraw=TRUE);

	void SetBorderGap(int nGap){m_nBorderGap=nGap;}
	void SetImageGap(int nGap){m_nImageGap=nGap;}

	void SetTextColor(COLORREF clrIn, COLORREF clrOut=NULL, COLORREF clrFocus=NULL);
	void SetBkgInColor(COLORREF clr){m_clrColors[BTN_COLOR_BKGIN] = clr;}
	void SetTextInColor(COLORREF clr){m_clrColors[BTN_COLOR_TXTIN] = clr;}
	void SetBkgOutColor(COLORREF clr){m_clrColors[BTN_COLOR_BKGOUT] = clr;}
	void SetTextOutColor(COLORREF clr){m_clrColors[BTN_COLOR_TXTOUT] = clr;}
	void SetBkgFocusColor(COLORREF clr){m_clrColors[BTN_COLOR_BKGFOCUS] = clr;}
	void SetTextFocusColor(COLORREF clr){m_clrColors[BTN_COLOR_TXTFOCUS] = clr;}

	void SetGradualColor(COLORREF clrTop, COLORREF clrBottom);
	void SetUser3DColor(COLORREF clrTopLeft, COLORREF clrRightBottom);
	void SetThinEDGEColor(COLORREF clrThinEdge) { m_clrThinEDGE = clrThinEdge; }
	void SetParentBkg(COLORREF clrParentBgk) { m_clrParentBgk = clrParentBgk; }
	
	void SetHasFlatBorder(BOOL bHas){m_bHasFlatBorder = bHas;}

	virtual BOOL SetIcon(CSize size, HICON hIcon, HICON hIconPushed=NULL, HICON hIconHilited=NULL, BOOL bRedraw=TRUE);
	virtual BOOL SetIcon(CSize size, UINT nID, UINT nPushedID=0, UINT nHilitedID=0, BOOL bRedraw=TRUE);
	virtual BOOL SetIcon(CSize size, LPCTSTR lpszID, LPCTSTR lpszPushedID=NULL, LPCTSTR lpszHilitedID=NULL, BOOL bRedraw=TRUE);
	virtual BOOL SetDisabledIcon(HICON hIcon, BOOL bRedraw=FALSE);
	virtual BOOL SetDisabledIcon(UINT nID, BOOL bRedraw=FALSE);
	virtual BOOL SetDisabledIcon(LPCTSTR lpszID, BOOL bRedraw=FALSE);

	void SetTransparent(BOOL bFlag){m_bTransparent=bFlag;}
	void SetAutoDisabled(BOOL bFlag){m_bAutoDisabled=bFlag;}
	void SetBigPercent(float fPercent){m_fBigPercent=fPercent;} ///< 必须在设置图标之前设置

	void SetToolTip(CString strTip);

	void SetFontSize(long lHeight, long lWeight=FW_NORMAL);
	void SetFontPoint(int nPoint, CString strFont);
	void SetCustomNumber(int nNumber);

protected:
	void OffsetPoint( CPoint& point, CSize size);

	virtual void CleanUpGDI();
	virtual void DrawButtonIcon(CDC* pDC, UINT nState, CRect& rcItem);

	virtual void DrawButtonText(CDC* pDC, UINT nState, CRect& rcItem, COLORREF clrText);
	void DrawCustomNumber(CDC* pDC, UINT nState, CRect& rcItem, COLORREF clrText);
	void DrawCustomText(CDC* pDC, UINT nState, CRect& rcItem, COLORREF clrText, CString& strText);

	virtual CString GetButtonText(BOOL bRemoveAmpersand);
    virtual CPoint CalculateImagePosition(CDC* pDC, UINT nState, CRect& rcItem, BOOL bHasPushedImage);
    void NoteButtonDepressed(BOOL bDepressed);
	void DeferInitialUpdate();

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

private:
	void Initialize(void);
	void StripMnemonics(CString& strIn);
	BOOL DrawParentBackground (CWnd* pWnd, CDC* pDC);
	HICON CreateGrayscaleIcon(HICON hIcon);

	void DrawGradualBackground(CDC* pDC, CRect& rcItem);
};