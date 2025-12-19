#pragma once

#include "MyPoint.h"
#include "MyButton.h"
#include "MyStatic.h"
#include "LinkList.h"
#include "LineInfo.h"
#include "TubeInfo.h"
#include "ChanThreInfo.h"
#include "LinearFitData.h"
#include "FloatDlg.h"
#include "resource.h"
#include<vector>

using namespace std;
// CMyDrawCurveDlg dialog 


#define WM_THRELINESTOPMOVE	WM_USER + 6009	//释放阈值线，通知父窗口
#define WM_THRECHANGED	WM_USER + 6010	//阈值线被拖动，通知父窗口

typedef enum CurveType{
	STDLINE = 1,GENERALCURVE
}ECURVETYPE;

typedef enum XAxisMode{
	NORMALXAXIS = 1,TIMERXAXIS
}EXAXISMODE;

class CMyDrawCurveDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CMyDrawCurveDlg)

public:
	CMyDrawCurveDlg(CWnd* pParent = NULL,BOOL beFilter = FALSE);   // standard constructor
	virtual ~CMyDrawCurveDlg();

// Dialog Data
	enum { IDD = IDD_MIDDLEDLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

private:
	afx_msg void OnPaint();
			int DrawThreLine(CDC* pDC);

public:
			BOOL CurveToPic(vector<BOOL>& vecCurMonitorChan,CString strPicName);
			int CurveToPic(CString strPicName);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags,CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags,CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags,short zDelta,CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
			
			void SetHoriStepNums(double horiStepNums,BOOL bRedraw = FALSE);
			double GetHoriStepNums();
			void SetVertStepNums(double vertStepNums,BOOL bRedraw = FALSE);
			double GetVertStepNums();
			void SetHoriStep(double horiStep,BOOL bRedraw = FALSE);
			double GetHoriStep();
			void SetVertStep(double vertStep,BOOL bRedraw = FALSE);
			double GetVertStep();

			int SetHoriAxisParam(int iStart,int iEnd,int iHoriStep,BOOL bRedraw = FALSE);	//设置X轴参数，防止横坐标标识不明确

			void SetHoriStartValue(double horiStartValue,BOOL bRedraw = FALSE);
			double GetHoriStartValue();
			void SetHoriEndValue(double horiEndValue,BOOL bRedraw = FALSE);
			double GetHoriEndValue();
			void SetVertStartValue(double vertStartValue,BOOL bRedraw = FALSE);
			double GetVertStartValue();
			void SetVertEndValue(double vertEndValue,BOOL bRedraw = FALSE,BOOL bUseMinOfMax = TRUE);
			double GetVertEndValue();
			
			void SetHoriStrColor(COLORREF horiStrClr,BOOL bRedraw = FALSE);
			COLORREF GetHoriStrColor();
			void SetHoriLineColor(COLORREF horiLineClr,BOOL bRedraw = FALSE);
			COLORREF GetHoriLineColor();
			void SetVertStrColor(COLORREF vertStrClr,BOOL bRedraw = FALSE);
			COLORREF GetVertStrColor();
			void SetVertLineColor(COLORREF vertLineClr,BOOL bRedraw = FALSE);
			COLORREF GetVertLineColor();
			void SetHoriBaseLineColor(COLORREF horiBaseLineClr,BOOL bRedraw = FALSE);
			COLORREF GetHoriBaseLineColor();

			void SetVertBaseLineColor(COLORREF vertBaseLineClr,BOOL bRedraw = FALSE);
			COLORREF GetVertBaseLineColor();

			void SetXSeparatorColor(COLORREF xSptClr,BOOL bRedraw = FALSE);
			COLORREF GetXSeparatorColor();
			void SetRoundTagInnerColor(COLORREF roundTagInnerClr,BOOL bRedraw = FALSE);
			COLORREF GetRoundTagInnerColor();
			BOOL SetRoundTagLineWidth(int roundTagLineWidth,BOOL bRedraw = FALSE);
			UINT GetRoundTagLineWidth();
			BOOL SetRoundTagLineRadius(int roundTagLineRadius,BOOL bRedraw = FALSE);
			UINT GetRoundTagLineRadius();
			void SetHoriLineWidth(int horiLineWidth,BOOL bRedraw = FALSE);
			int GetHoriLineWidth();
			void SetVertLineWidth(int vertLineWidth,BOOL bRedraw = FALSE);
			int GetVertLineWidth();
			void SetHoriSptWidth(int horiSptWidth,BOOL bRedraw = FALSE);
			int GetHoriSptWidth();
			void SetCurveWidth(int curveWidth,BOOL bRedraw = FALSE);
			int GetCurveWidth();
			void SetHoriBaseLineWidth(int horiBaseLineWidth,BOOL bRedraw = FALSE);
			int GetHoriBaseLineWidth();
			void SetVertBaseLineWidth(int vertBaseLineWidth,BOOL bRedraw = FALSE);
			int GetVertBaseLineWidth();
			void SetShowVertLine(BOOL bShowVertLine,BOOL bRedraw = FALSE);
			BOOL GetShowVertLine();
			void SetShowHoriLine(BOOL bShowHoriLine,BOOL bRedraw = FALSE);
			BOOL GetShowHoriLine();
			void SetShowHoriSeparator(BOOL bShowHoriSpt,BOOL bRedraw = FALSE);
			BOOL GetShowHoriSeparator();
			void SetShowRoundTag(BOOL bShowRoundTag,BOOL bRedraw = FALSE);
			BOOL GetShowRoundTag();

			void SetShowVertBaseLine(BOOL bShowVertBaseLine,BOOL bRedraw = FALSE);
			BOOL GetShowVertBaseLine();

			void SetShowHoriBaseLine(BOOL bShowHoriBaseLine,BOOL bRedraw = FALSE);
			BOOL GetShowHoriBaseLine();
			BOOL SetHoriStringOutStep(int horiStrOutStep,BOOL bRedraw = FALSE);
			int  GetHoriStringOutStep();
			BOOL SetVertStringOutStep(int vertStrOutStep,BOOL bRedraw = FALSE);
			int  GetVertStringOutStep();
			BOOL SetHoriDecimalPointLength(UINT horiDecPtLen,BOOL bRedraw = FALSE);
			UINT GetHoriDecimalPointLength();
			BOOL SetVertDecimalPointLength(UINT horiDecPtLen,BOOL bRedraw = FALSE);
			UINT GetVertDecimalPointLength();

	static  COLORREF GetChannelCurveColor(int chanId);

			void SetCaptionHeight(int capHeight,BOOL bRedraw = FALSE);
			int  GetCaptionHeght();
			BOOL SetRoundRectParam(int roundX,int roundY,BOOL bRedraw = FALSE);
			void GetRoundRectParam(int& roundX,int& roundY);
			void SetDlgSize(int dlgWidth,int dlgHeight,BOOL bRedraw = FALSE);
			void GetDlgSize(int& dlgWidth,int& dlgHeight);

			void SetLeftTopPosition(int left,int top,BOOL bRedraw = FALSE);
			void GetLeftTopPosition(int& left,int& top);

			void SetLegendHeight(int legendHeight,BOOL bRedraw = FALSE);
			void GetLegendHeight(int& legendHeight);
			void SetLeftIndent(int leftIndent,BOOL bRedraw = FALSE);
			void GetLeftIndent(int& leftIndent);
			void SetRightIndent(int rightIndent,BOOL bRedraw = FALSE);
			void GetRightIndent(int& rightIndent);
			void SetTopIndent(int topIndent,BOOL bRedraw = FALSE);
			void GetTopIndent(int& topIndent);
			void SetBottomIndent(int bottomIndent,BOOL bRedraw = FALSE);
			void GetBottomIndent(int& bottomIndent);
			void SetHaveCaptionOrNot(BOOL bHaveCap,BOOL bRedraw = FALSE);
			BOOL GetHaveCaptionOrNot();
			void SetHaveLegendOrNot(BOOL bHaveLgd,BOOL bRedraw = FALSE);
			BOOL GetHaveLegendOrNot();
			void SetLeanXOrNot(BOOL bLeanX);
			BOOL GetLeanXOrNot();
			void SetBackgrounColor(BOOL bkClr,BOOL bRedraw = FALSE);
			COLORREF GetBackgrounColor();

			void SetXAxisMode(EXAXISMODE eXAxisMode,BOOL bRedraw = FALSE);
			EXAXISMODE GetXAxisMode();

			void SetBeHaveData(BOOL bHaveData);
			BOOL GetBeHaveData();

			int SetExpressionShowState(int lineId,BOOL bShowExpression,BOOL bRedraw = FALSE);
			int GetExpressionShowState(int lineId,BOOL& bShowExpression);
			void SetXAxisExplainShowState(BOOL bShowXAxisExplain,BOOL bRedraw = FALSE);
			BOOL GetXAxisExplainShowState();
			void SetXAxisExplain(CString strXAxisExplain,BOOL bRedraw = FALSE);
			CString GetXAxisExplain();
			void SetExpressionAutoXYMinMax(BOOL bExpressionAutoXYMinMax,BOOL bRedraw = FALSE);
			BOOL GetExpressionAutoXYMinMax();

			void SetBeHaveThreLineFunc(BOOL bHaveThreLineFunc,BOOL bRedraw = FALSE);
			BOOL GetBeHaveThreLineFunc();
			void SetBeUseThreLineFunc(BOOL bUseThreLineFunc,BOOL bRedraw = FALSE);
			BOOL GetBeUseThreLineFunc();

			void SetXAxisTag(CString xAxisTag,BOOL bRedraw = FALSE);
			CString GetXAxisTag();
			void SetYAxisTag(CString yAxisTag,BOOL bRedraw = FALSE);
			CString GetYAxisTag();

			void SetBeStdCrv(BOOL bStdCrv,BOOL bRedraw = FALSE);
			BOOL GetBeStdCrv();

			void SetBeUseFloatFunc(BOOL bUseFloatFunc);
			BOOL GetBeUseFloatFunc();

			void SetYAxisMinOfMax(double dMinOfMax);
			double GetYAxisMinOfMax();

			void SetBeShieldYAxisMinOfMax(BOOL bShield);
			BOOL GetBeShieldYAxisMinOfMax();

			int SetLinearFitData(int lineId,LINEARFITDATA* pLinearFitData,BOOL bRedraw = FALSE);
			int SetLinearFitData(int iLineId,CLinearFitData* pLinearFitData,BOOL bRedraw = FALSE);

			void SetMinYDecrementParam(BOOL bUse,double dThre,double dDecre);
			BOOL IsUseMinYDecrement();
			double GetMinYDecrementThreshold();
			double GetMinYDecrement();

			int AddNewDot(int lineIndex,CMyPoint& myPoint,BOOL bRedraw = FALSE);	//添加数据点，自动比较X轴最大值、最小值
			int AddNewDotAutoCompareXYMinMax(int lineIndex,CMyPoint& myPoint,BOOL bRedraw = FALSE,double dZoomRatio = 1.3);	//添加数据点，自动比较X轴、Y轴最大值、最小值，dZoomRatio应该是正数

			void ClearLine(BOOL bRedraw = FALSE);	//清空原始数据点
			void DeleteLineData(BOOL bRedraw);
			int DeleteOneLineData(int lineIndex,BOOL bRedraw);

			int SetLineString(int lineIndex,int iChanIndex,CString strTag,CString strDyeName,CString strTarget);

			void HandSetExpressionXYAxisMinMax(BOOL bRedraw = FALSE);
			void HandSetGeneralLineXYAxisMinMax(double dMultiplier = 1.1,BOOL bRedraw = FALSE);	//计算X/Y轴最大最小值
			void HandSetGeneralLineYAxisMinMax(double dMultiplier = 1.1,BOOL bRedraw = FALSE);	//只计算Y轴最大最小值

			BOOL SetLineNum(int lineNum,BOOL bRedraw = FALSE);
			int SetLineParam(int lineIndex,COLORREF lineClr,int chanId,BOOL bShow,BOOL bRedraw = FALSE);
			int SetLineParam(int iChanId,CString strDyeName,BOOL bRedraw = FALSE);
			int SetLineShow(int lineIndex,BOOL bShow,BOOL bRedraw = FALSE);
			int SetLineXAxisMillisecondTime(int lineIndex,BOOL bXAxisMsTime,BOOL bRedraw = FALSE);

			void SetCurveType(ECURVETYPE eCurveType,BOOL bRedraw = FALSE);
			ECURVETYPE GetCurveType();

			void SetLineColorMode(ELINECLRMODE eLineClrMode,BOOL bRedraw = FALSE);
			ELINECLRMODE GetLineColorMode();
			
			void SetThreLineDataType(ELINEDATATYPE eLineDataType,BOOL bRedraw = FALSE);	//多通道统一设置
			int GetThreLineDataType(int iChanId,ELINEDATATYPE& eLineDataType);	//单通道单独获取

			int SetThreLineShowForamt(ELINESHOWFORMAT eLineShowFormat,BOOL bRedraw = FALSE);	//多通道统一设置
			int GetThreLineShowForamt(int iChanId,ELINESHOWFORMAT& eLineShowFormat);	//单通道单独获取

			int SetBeShowThreLine(int iChanId,BOOL bShowThreLine,BOOL bRedraw = FALSE);	//单通道单独设置，所有阈值线显示状态相同
			int GetBeShowThreLine(int iChanId,BOOL& bShowThreLine);	//单通道单独获取

			int SetThreLineColor(int iChanId,Color clrThreLine,BOOL bRedraw = FALSE);	//单通道单独设置
			int GetThreLineColor(int iChanId,Color& clrThreLine);	//单通道单独获取

			int SetBeShowExpression(BOOL bShowExpression,BOOL bRedraw = FALSE);	//设置是否显示表达式
			BOOL GetBeShowExpression();	

			double PixelHeightToYValue(int pixelH);

			void AddHighlightIndex(int index,BOOL bUnique,BOOL bRedraw = FALSE);
			void GetHighlightIndex(vector<int>& vecHighlightIndex);

			CRect GetFloatWndRegion(CRect rcOrigin);

			int SetThrelineFstShow(BOOL bRedraw = FALSE);	//设置阈值线为“第一次绘制”，重新计算阈值线在控件中的高度

			CString GetExpressionString();

			BOOL IsExistVisibleExpression();	//获取是否存在可见的关系式，不受m_bShowExpressoin影响
			int GetVisibleExpressionCount();	//获取可见的关系式个数，不受m_bShowExpressoin影响
			int GetVisibleExpressionHeight();	//获取可见的关系式占用的高度

			void SetExpressionTag(CString strData,BOOL bRedraw = FALSE);
			CString GetExpressionTag();

			void CreateCtrl();
			void InitCtrl();
			void SetCtrlPos();
			void ShowCtrl();
			void CreateBt();
			void CreateSt();
			void InitBt();
			void InitSt();

	afx_msg void OnBnClickedClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnUpdateCurve(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCurveMenuExportPicture();

protected:
	void DrawLineGDI(CDC* pDC, double dHoriRatio, double dVertRatio);
	void DrawLineGraphics(CDC* pDC, double dHoriRatio, double dVertRatio);

private:
			BOOL SetRoundRect(int x = 5,int y = 5);

public:
	void SetRealUsedHoriDotLen(int iDotLen) { m_realUsedHoriDotLen = iDotLen; }

private:
	int  m_horiStepNums;	//水平方向上基本单元的个数
	int  m_vertStepNums;	//垂直方向上基本单元的个数
	int  m_horiStrOutStep;	//水平方向上文字输出间隔（隔几个Step输出一次文字）
	int  m_vertStrOutStep;	//垂直方向上文字输出间隔（隔几个Step输出一次文字）
	double m_horiStep;	//水平方向上基本单元的对应的数据大小（线段对应的数据值）
	int m_intHoriStep;	//m_horiStep的整型数值
	double m_vertStep;	//垂直方向上基本单元的对应的数据大小（线段对应的数据值）
	int m_intVertStep;	//m_vertStep的整型数值

	double m_horiStartValue;
	double m_horiEndValue;
	double m_vertStartValue;
	double m_vertEndValue;

	COLORREF m_horiStrClr;
	COLORREF m_horiLineClr;
	COLORREF m_vertStrClr;
	COLORREF m_vertLineClr;
	COLORREF m_horiBaseLineClr;
	COLORREF m_vertBaseLineClr;
	COLORREF m_xSptClr;	//X轴分割线Spt=Separator
	COLORREF m_roundTagInnerClr;	//圆形标识点
	int m_horiLineWidth;
	int m_vertLineWidth;
	int m_horiSptWidth;
	int m_horiBaseLineWidth;
	int m_vertBaseLineWidth;
	int m_curveWidth;
	BOOL m_bShowVertBaseLine;	//是否显示垂直基线（不包括Y轴）
	int m_iVertLineInterval;	//垂直分割线间隔步骤数

	BOOL m_bShowHoriBaseLine;//是否显示水平基线（不包括X轴）
	BOOL m_bShowVertLine;	//是否显示Y轴
	BOOL m_bShowHoriLine;	//是否显示X轴
	BOOL m_bShowHoriSpt;	//暂时没有用到该成员变量
	BOOL m_bShowRoundTag;
	UINT m_roundTagLineWidth;
	UINT m_roundTagRadius;
	UINT m_horiDecPtLen;	//不参与计算，水平方向坐标轴文字标识小数点后位数,最大值为20
	UINT m_vertDecPtLen;	//不参与计算，垂直方向坐标轴文字标识小数点后位数，最大值为20
	UINT m_realUsedHoriDotLen;	//真正参与计算的水平方向坐标轴文字标识小数点后位数,最大值为20
	UINT m_realUsedVertDotLen;	//真正参与计算的垂直方向坐标轴文字标识小数点后位数，最大值为20
	int m_figureStartX;		//图形区域在窗口中的水平方向起始位置，下面三个成员变量意义相近
	int m_figureEndX;
	int m_figureStartY;
	int m_figureEndY;

	int	m_roundX;	//窗口圆角X值
	int	m_roundY;	//窗口圆角Y值

	int m_capHeight;//标题栏高度
	CSize m_dlgSize;	//窗口尺寸
	int m_left;		//窗口左上角X坐标
	int m_top;		//窗口左上角Y坐标
	int m_lgdHeight;	//图例区域高度
	int m_leftIndent;	//图形区域左侧缩进
	int m_rightIndent;	//图形区域右侧缩进
	int m_topIndent;	//图形区域顶部缩进
	int m_bottomIndent;	//图形区域底部缩进

	BOOL m_bHaveCap;	//是否显示标题栏
	BOOL m_bHaveLgd;	//是否显示图例
	BOOL m_bLeanX;		//是否垂直显示X轴文字
	
	BOOL m_bExpressionAutoXYMinMax;	//表达式模式（STDLINE）下，是否自动扩展（相对于手动计算）X/Y轴最大值、最小值
	BOOL m_bShowXAxisExplain;	//是否显示X轴内容说明
	CString m_strXAxisExplain;

	COLORREF m_rgbBkClr;//除标题栏外的背景颜色

	int m_lineNum;	//曲线条数

	ECURVETYPE m_eCurveType;
	EXAXISMODE m_eXAxisMode;

	double m_dAxisHoriRt;
	double m_dAxisVertRt;

	BOOL m_bHaveThreLineFunc;	//标识是否包含阈值线功能
	BOOL m_bUseThreLineFunc;	//标识是否使用阈值线功能

	BOOL m_bHaveData;	//标识图形控件是否有数据存在

	CDC* m_pMemDc;	//加速图形绘制
	HBITMAP m_hBitmap;
	BOOL m_bFstPaint;
	int  m_fstPaintWidth;
	int  m_fstPaintHeight;

	BOOL m_bThreMove;	//标识是否是阈值线导致的重绘

	ELINECLRMODE m_eLineClrMode;	//曲线颜色选型

	vector<int> m_vecHighLightIndex;	//保存高亮显示曲线下标

	CString m_strXAxisTag;
	CString m_strYAxisTag;

	BOOL m_bstdCrv;	//标识是否是标曲功能
	int m_stdFrameH;	//标曲标题栏高度

	//圆角矩形样式参数
	BOOL m_bRoundRectStyle;	//标识是否使用圆角矩形样式
	COLORREF m_clrFourAngleBk;	//圆角矩形以外的背景色
	int m_iRoundAngleSize;	//圆角大小
	Color m_clrIndepent;	//独立第三方颜色，用于支持局部透明

	CFloatDlg m_floatDlg;	//悬浮窗

	BOOL m_bUseFloatFunc;	//标识是否使用悬浮提示窗口

	double m_dYAxisMinOfMax;	//纵坐标最大值的最小值，默认值为1000
	BOOL m_bShieldYAxisMinOfMax;	//是否屏蔽m_dYAxisMinOfMax功能

	int m_iHoriDecPtLenSet;	//用于临时保存最新设置的水平文字小数点位数，在类的内部使用

	BOOL m_bShowExpressoin;	//标识“是否显示关系式信息(对于整个图形控件，而不是单条曲线)”

	int m_iEprVertInterval;	//标曲关系式之间的垂直间隔
	int m_iEprXStart;
	int m_iEprYStart;
	int m_iEprEndY;	//记录 整体的标曲关系式 底部Y坐标

	CString m_strEprTag;	//关系式标识
	CRect m_rcEprTag;	//关系式标识所在区域

	BOOL m_bUseMinYDecre;	//是否使用纵坐标最小值减量
	double m_dMinYDecreThre;	//使用纵坐标最小值减量的阈值条件
	double m_dMinYDecrement;	//纵坐标最小值减量

public:
	CMyButton m_btClose;
	CMyStatic m_stCapInfo;

	CLineInfo* m_pLineInfo;	//图形原始数据

	vector<CChanThreInfo> m_vecChanThreInfo;

	CFontSet m_fontExpression;	//表达式字体
};







