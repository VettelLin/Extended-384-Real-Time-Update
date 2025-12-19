#pragma once

#include "Resource.h"
#include "CaretInfo.h"
#include "LinkList.h"
#include "FontSet.h"
#include "MaxMinInfo.h"
#include "SelfDefType.h"

#define WM_EDITENTERPUSHED	WM_USER + 6025	//编辑框Enter按键被按下

typedef enum SelectMoveType{
	ESMT_NONE,ESMT_OUTLEFT,ESMT_OUTRIGHT,ESMT_NORMAL
}ESELECTMOVETYPE;

//正常模式、只读模式
typedef enum WorkState{
	EWS_NORMAL,EWS_READONLY
}EWORKSTATE;

typedef enum RegionType{
	ERT_FRAME,ERT_LEFTINDENT,ERT_EDIT,ERT_RIGHTINDENT,ERT_TOPINDENT,ERT_BOTTOMINDENT,ERT_EMPTY,ERT_OUTSIDE
}EREGIONTYPE;

//边框样式，矩形边框、圆角边框
typedef enum FrameStyle{
	EFS_RECTANGLE,EFS_ROUNDRECT
}EFRAMESTYLE;

typedef enum TextMode{
	ETM_NORMAL,ETM_PASSWORDDOT
}ETEXTMODE;

#define WM_TELLPARENTTEXTCHANGED	WM_USER + 6029	//手动编辑字符导致编辑框文本改变，通知父窗口
#define WM_TELLPARENTSETNEWTEXT	WM_USER + 6030	//SetText导致编辑框文本改变，通知父窗口

class CMyNewEdit : public CEdit,public CFontSet
{
	DECLARE_DYNAMIC(CMyNewEdit)

public:
	CMyNewEdit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMyNewEdit();

	enum { IDD = IDD_MYNEWEDIT};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);

			int SetCharFullShow(int iIndex);
			int CalcCaretPos(int iIndex,int& iCaretXPos);

			void ClearSelectInfo();

			void SetBeEditable(BOOL bEditable,BOOL bRedraw = FALSE);
			BOOL GetBeEditable();

			int SetAllSelected();
			int DeleteAll(BOOL bRedraw = TRUE);

			int SetFrameColor(Color clrNormal,Color clrHover,Color clrSelected,Color clrDisabled,BOOL bRedraw = FALSE);
			int SetMainWndBkColor(Color clrNormal,Color clrHover,Color clrSelected,Color clrDisabled,BOOL bRedraw = FALSE);
			int SetTextColor(COLORREF clrNormal,COLORREF clrHover,COLORREF clrSelected,COLORREF clrDisabled,BOOL bRedraw = FALSE);
			int SetSelectedTextColor(COLORREF clrNormal,COLORREF clrHover,COLORREF clrSelected,COLORREF clrDisabled,BOOL bRedraw = FALSE);
			int SetSelectedBkColor(COLORREF clrNormal,COLORREF clrHover,COLORREF clrSelected,COLORREF clrDisabled,BOOL bRedraw = FALSE);

	virtual int SetText(CString strText,BOOL bRedraw = TRUE,BOOL bForbidTellParent = FALSE);
			int SetText(double dData,BOOL bRedraw = TRUE,BOOL bForbidTellParent = FALSE);
			CString GetText();
			
			void ClearMaxMinLimit();

			void SetMaxMinInfo(CMaxMinInfo maxMinInfo,BOOL bRedraw = FALSE);
			CMaxMinInfo GetMaxMinInfo();

			void SetBeEnterTellParent(BOOL bEnterTellParent);
			BOOL GetBeEnterTellParent();

			void SetTellParentTxChangedParam(BOOL bTellParentTxChanged,int iTextTypeTag);
			void GetTellParentTxChangedParam(BOOL& bTellParentTxChanged,int& iTextTypeTag);

			void SetTellParentSetNewText(BOOL bTellParentSetNewText,int iTextTypeTag);
			void GetTellParentSetNewText(BOOL& bTellParentSetNewText,int& iTextTypeTag);

			void SetWorkState(EWORKSTATE eWorkState);
			EWORKSTATE GetWorkState();
			
			void SetFrameStyle(EFRAMESTYLE eFrameStyle,BOOL bRedraw = FALSE);
			EFRAMESTYLE GetFrameStyle();
			 
			void SetFrameRoundAngle(int iFrameRoundAngle,BOOL bRedraw = FALSE);
			int GetFrameRoundAngle();

			void SetLeftImageParam(BOOL bShowImg,int iSonPicNum,CString strFilePath,BOOL bRedraw = FALSE);
			BOOL GetBeShowLeftImage();
			int GetLeftImageSonPicNum();
			CString GetLeftImagePath();

			void SetPasswordParam(ETEXTMODE eData,int iDotRadius,int iDotInterval);

			int TextParamChanged(BOOL bRedraw = FALSE);	//字体属性（字体名称、大小）改变了

	virtual int LBtDownSpecialProc();
			
public:
	int CalcCharStartEndShowIndex();	//正序计算
	int CalcCharStartEndShowIndex(int iStartIndex);	//正序计算
	int CalcCharEndStartShowIndex();	//倒序计算
	int CalcCharEndStartShowIndex(int iEndIndex);	//倒序计算
	int HoriScrollPixel(int iPxShift,BOOL bRedraw = FALSE,BOOL bSetCaretPos = FALSE);
	int HoriScrollChar(int iCharShift,BOOL bRedraw = FALSE);
	int TransformXPosToCharInex(int pxX);	//把横坐标转换为字符串下标位置，改变m_caretInfo
	int TransformXPosToCharInex(int pxX,int& iIndex);	//把横坐标转换为字符串下标位置并单独输出，不改变m_caretInfo
	int GetLeftUnShowCharPixelLength();	//计算左侧未显示字符的总宽度
	int GetRightUnShowCharPixelLength();	//计算右侧未显示字符的总宽度
	
	int GetWidth(int iStart,int iEnd);

	int ArrowKeyProcess(UINT nChar);
	int DeleteKeyProcess(UINT nChar);
	int MouseLeftBtPushedMoveProcess(CPoint pt);

	int DeleteSelectedChar(BOOL bRedraw = TRUE);
	int DeleteChar();	//以下标作为参考标准
	int DeleteCharAfter(); 	//以下标作为参考标准
	int DeleteChar(int iIndex);
	int DeleteCharAfter(int iIndex); 

	int CharFilter(UINT nChar,UINT nFlags);
	int MsgFilter(MSG* pMsg);

	int CopySelectedTextToClipBoard();
	int CtrlAndVProcess();
	
	int CheckLnkCharWidth();

	EREGIONTYPE GetRegionType(CPoint pt);

	int TellParentTxChangedProcess();	//文字变更后，处理“是否通知父窗口”
	int TellParentSetNewTextProcess();	//SetText导致文字变更后，处理“是否通知父窗口”
	
	void SetBeFuncEdit(BOOL bFuncEdit);
	BOOL GetBeFuncEdit();

	void SetDefaultIndent();

	void SetCtrlTag(int iCtrlTag);
	int GetCtrlTag();

public:
	CCaretInfo m_caretInfo;
	CString m_strText;

	CLinkList<int> m_lnkCharW;	//记录每个字符宽度

	int m_iLeftIndent;	//文字显示区域左侧空白区域宽度
	int m_iRightIndent;	//文字显示区域右侧空白区域宽度
	int m_iTopIndent;	//文字显示区域顶部空白区域高度
	int m_iBottomIndent;	//文字显示区域底部空白区域高度
	int m_iCharShowLI;	//绘制文字时左侧缩进
	int m_iCharShowRI;	//绘制文字时右侧缩进
	int m_iCharShowW;	//文字显示区域宽度

	int m_pxFrameW;	//边框宽度
	
	int m_iCharStartShowIndex;	//显示的第一个字符在总体字符串中的下标
	int m_iCharEndShowIndex;	//显示的最后一个字符在总体字符串中的下标

public:
	int m_iCdcW;	//m_iCharStartShowIndex与m_iCharEndShowIndex之间的字符内存宽度
	int m_iSelDrawCDcW;	//选中区域需绘制字符的宽度

	int m_iSelStart;	//选中区域起始下标
	int m_iSelEnd;	//选中区域终止下标
	int m_iSelCharNum;	//选中字符数

	BOOL m_bLBtPushedMove;	//标识是否是左键按下移动
	BOOL m_bSelMoveCaretHid;	//标识选中滑动时插入符是否已隐藏
	ESELECTMOVETYPE m_eCurSMT;	//标识选中滑动时鼠标位置类型
	ESELECTMOVETYPE m_ePreSMT;	//标识选中滑动时上一次的鼠标位置类型
	CPoint m_ptPreSelMove;	//记录选中滑动时鼠标的上一个位置

	CRect m_rcCtrl;	//控件显示区域
	CRect m_rcText;	//文本显示区域
	CRect m_rcLeftIndent;	//左侧缩进区域
	CRect m_rcRightIndent;	//右侧缩进区域
	CRect m_rcTopIndent;	//顶部缩进区域
	CRect m_rcBottomIndent;	//底部缩进区域

	BOOL m_bEidtable;	//标识是否可编辑

	BOOL m_bMouseFstInCtrl;	//标识鼠标是否初次在控件中
	BOOL m_bEnterTellParent;	//Enter按键按下时是否通知父窗口

	ECTRLSTATE m_eCtrlState;
	ECTRLSTATE m_ePreCtrlState;

	Color m_clrFrameNormal;		//边框颜色
	Color m_clrFrameHover;
	Color m_clrFrameSelected;
	Color m_clrFrameDisabled;

	Color m_clrMainWndNormal;	//主窗口背景色
	Color m_clrMainWndHover;
	Color m_clrMainWndSelected;
	Color m_clrMainWndDisabled;

	COLORREF m_clrTxNormal;		//正常文字颜色
	COLORREF m_clrTxHover;
	COLORREF m_clrTxSelected;
	COLORREF m_clrTxDisabled;

	COLORREF m_clrTxSelNormal;	//选中文字颜色
	COLORREF m_clrTxSelHover;
	COLORREF m_clrTxSelSelected;
	COLORREF m_clrTxSelDisabled;

	COLORREF m_clrBkSelNormal;	//选中区域背景色
	COLORREF m_clrBkSelHover;
	COLORREF m_clrBkSelSelected;
	COLORREF m_clrBkSelDisabled;

	CMaxMinInfo m_maxMinInfo;

	EWORKSTATE m_eWorkState;
	EREGIONTYPE m_eRgnType;

protected:
	EFRAMESTYLE m_eFrameStyle;	//边框样式
	int m_iFrameRoundAngle;	//圆角边框的圆角大小

	BOOL m_bFuncEdit;	//是否是功能控件，用于列表框或者特别控件的内容编辑,失去焦点时自动隐藏
	int m_iCtrlTag;	//用于单一标识功能控件，失去焦点时发送给父窗口，让父窗口能够识别到是哪一个控件

	BOOL m_bShowLeftImg;	//左侧缩进区域是否显示图标
	int m_iLeftImgSonPicNum;	//左侧缩进区域显示图形的子图个数
	CString m_strLeftImgPath;	//左侧缩进图形路径

	int m_iTxH;	//记录字体文字的高度

private:
	BOOL m_bTellParentTxChanged;	//通知父窗口文本已变更，由鼠标按键操作触发
	int m_iTextTypeTag;	//文本功能标识（标识当前编辑框用于编辑哪种类型的信息）

	BOOL m_bTellParentSetNewText;	//通知父窗口文本已变更，由SetText触发
	
	//密码圆点字符格式参数
	int m_iPWDotRadius;	//密码字符圆点半径
	int m_iPWDotDiam;	//密码字符圆点直径
	int m_iPWDotInterval;	//密码字符圆点间隔
	int m_iPWCharW;
	ETEXTMODE m_eTxMode;
};
