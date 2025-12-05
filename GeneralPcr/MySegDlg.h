#pragma once

#include "FontSet.h"
#include "LinkList.h"
#include "MyScrollInfo.h"
#include "SegItemInfo.h"
#include "LinkList.h"
#include "SegDotPosInfo.h"
#include "SegStepRange.h"
#include "MySegDlgParam.h"
#include "MyStepEditDlg.h"
#include "MyNewEdit.h"
#include <GdiPlus.h>

//荧光采集图标标识
typedef enum CollectFluTagType{
	ECFTT_TRIANGLE,ECFTT_CAMERA
}ECOLLECTFLUTAGTYPE;

class CMySegDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CMySegDlg)
	
public:
	CMySegDlg();
	virtual ~CMySegDlg();
	
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();

public:
			int DrawSegment(CDC* pDC);
			int DrawHoriScroll(CDC* pDC);
			int DrawVertScroll(CDC* pDC);
			int DrawHoriScrollStyle2(CDC* pDC);	//绘制水平滚动条样式2，圆角边框模式使用
			int DrawVertScrollStyle2(CDC* pDC);	//绘制垂直滚动条样式2，圆角边框模式使用
			int DrawGradientData(CDC* pDC);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg LRESULT OnMsgFuncEditKillFocus(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgEditEnterPushed(WPARAM wParam,LPARAM lParam);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

			CSegDotPosInfo FindDotPosInfo(CPoint pt); 
			float PixelHeightToTem(int pixelH);

			int UpdateSegStepRange(int segId,int stepId);

			void ProcessHoriSrcoll(UINT nFlags, CPoint point);
			void ProcessVertSrcoll(UINT nFlags, CPoint point);

			void AddSegment();
			void AddStep(int segId,BOOL bMelt = FALSE);
			int SetSegmentStepSelectState(BOOL bCtrlClicked,BOOL bRedraw = FALSE);
			int SetSegmentSelectState(BOOL bCtrlClicked,BOOL bRedraw = FALSE);
			int SetStepSelectState(BOOL bCtrlClicked,BOOL bRedraw = FALSE);
			int SetSegmentFullSelectState(BOOL bSel,BOOL bRedraw = TRUE);

			int SetDefaultSingleSelectStep(int iSegIndex,int iStepIndex,BOOL bRedraw = FALSE);	//没有步骤被选中的时候，设置默认选中的步骤

			void InsertSegment(EINSERTSELECT insetSel);
			void InsertStep(EINSERTSELECT insetSel);
			void DeleteSelectedItem();
			void Clear(BOOL bRedraw = TRUE);

			void DeleteMeltSeg();
			void AddMeltSegIfNoMeltSegExist();
			void SetDefaultAmlProgramForProject();
			void SetDefaultAmlProgramForProject2();
			void SetDefaultMeltProgramForProject();
			void SetDefaultAbsoluteQuantification();	//设置绝对定量默认实验流程
			void SetDefaultStandardMeltingCurve();	//设置标准熔解曲线默认实验流程

			void SetBeMonitorFunc(BOOL beMonitorFunc,BOOL bRedraw = FALSE);
			BOOL GetBeMonitorFunc();
			BOOL SetSegmentInfo(CLinkList<CSegItemInfo>& lnkSegItemInfo,BOOL bRedraw = FALSE);
			BOOL GetSegmentInfo(CLinkList<CSegItemInfo>& lnkSegItemInfo);

			void SetBeEditable(BOOL bEditable);
			BOOL GetBeEditable();

			BOOL UpdateCurSelectStepCycleInfo(int item,int sub,int cycle,BOOL bSelState,BOOL bRedraw = FALSE);
			void ResetCurItemSubCycle();

			void EditStep(int iSegId,int iStepId);
			void EditSegment(int iSegId);

			BOOL GetBeHaveGetDataStep();	//判断所有程序段是否已经存在扩增步骤
			BOOL GetSegmentBeHaveGetDataStep(int iSegId);	//判断单个程序段中是否已经存在扩增步骤

			BOOL GetBeHaveAmlCollectStepExcept(int iSegId,int iStepId);	//判断除了特定步骤，是否已经存在扩增步骤
			BOOL GetBeHaveMeltCollectStepExcept(int iSegId,int iStepId);	//判断除了特定步骤，是否已经存在熔曲步骤

			void SetScrollArrowWH(int iWidth,int iHeight,BOOL bRedraw = FALSE);

			void SetFourAngleBkColor(COLORREF clr,BOOL bRedraw = FALSE);
			COLORREF GetFourAngleBkColor();

			void SetSegStepName(int iStartSegIndex,BOOL bRedraw = FALSE);	//自动设置程序段和步骤名称

			void SetCollectFluTagType(ECOLLECTFLUTAGTYPE eType,BOOL bRedraw = FALSE);
			ECOLLECTFLUTAGTYPE GetCollectFluTagType();

			void SetShieldGradientSetState(BOOL bData);
			BOOL GetShieldGradientSetState();

			void SetTemperatureRateShowState(BOOL bData);
			BOOL GetTemperatureRateShowState();
			
			int GetStepIndexAt(int iSegId,int iStepId);	//获取当前步骤是流程的第几个步骤
			int SetSegmentSetpShow(int iSegId,int iStepId,BOOL bRedraw = FALSE);

			int SetFuncEditCtrlFont(CString strFtName,int nHeight,int nWeight = FW_NORMAL,FONT_UNIT_TYPE fontUnitType = TYPE_PIXEL);

			void SetGradientInfoUseState(BOOL bUse,BOOL bRedraw = FALSE);
			BOOL GetGradientInfoUseState();

			void SetGradientString(CString strData,BOOL bRedraw = FALSE);
			void SetColumnString(CString strData,BOOL bRedraw = FALSE);
			void SetSegmentString(CString strData,BOOL bRedraw = FALSE);
			void SetStepString(CString strData,BOOL bRedraw = FALSE);
			void SetRangeString(CString strData,BOOL bRedraw = FALSE);

			BOOL IsSelectMultiSegment();
			BOOL IsOnlyOneSegmentSelected();
			int GetSelectedSegmentCount();
			int GetFirstSelectedSegmentId();

			CString GetGradientInfoString();

			void CorrectSegmentName();
			void CorrectSegmengIndex();

private:
			int CalcTotalColumnWidth();
			int CalcTotalRowHeight();

			int CalcSegStepToShow();
			int GetStepNum();
			int GetStepNum(int segStart,int segEnd);
			int StartEndShowColToSegStepRange();

			int ScrollCol(int pxHoriShift);
			int ScrollRow(int pxVertShift);

			int ScrollColToFixPos(int pxFixPos);
			int ScrollRowToFixPos(int pxFixPos);
			void CalcScrollPos();

			void InitHoriScrollParam();
			void InitVertScrollParam();

			float GetSegLastStepTem(int segId,int stepId);
			BOOL GetMeltSegStartEndTemperature(double& dStartTem,double& dEndTem);
			void UpdateSegStepScrollPosInfo();
			int EndEdit();

			int GetWidth(int colStart,int colEnd);
			int GetHeight(int rowStart,int rowEnd);

protected:
			BOOL IsExistGradientStep();
			BOOL IsShowGradientData();
			int GradientSetChanged(BOOL bRedraw);

public:
	CMyNewEdit m_edSegInfo;

	//字体变量
	CFontSet m_fontGeneral;
	CFontSet m_fontStepTag;
	CFontSet m_fontSegTag;
	
	int m_priorItem;	//标识前一个segment（相对于当前正在运行的segment），默认为-1
	int m_priorStep;	//标识前一个step（相对于当前正在运行的step），默认为-1
	int m_curRunCycle;	//记录当前运行的是单个segment中第几个循环
	int m_curRunItem;	//记录当前运行的是哪个segment
	int m_curRunStep;	//记录当前运行的是哪个step

	CLinkList<CSegItemInfo> m_lnkSegItemInfo;

	//水平滚动条
	CMyScrollInfo m_horiScroll;
	CMyScrollInfo m_vertScroll;

	int GetHeaderHeight() { return m_pxHeaderH; }
	void SetHeaderHeight(int iHeight) { m_pxHeaderH = iHeight; }

protected:
	double m_dMaxTem;	//可设置最大温度值
	double m_dMinTem;	//可设置的最低温度值
	int m_iMaxCycleNum;	//可设置的最大循环数
	int m_iMinCycleNum;	//可设置的最小循环数
	int m_iMaxDelaySec;	//可设置的最大延时时间
	int m_iMinDelaySec;	//可设置的最小延时时间

	BOOL m_bShieldGradientSet;	//是否屏蔽梯度设置项
	BOOL m_bShowTemRate;	//是否显示升降温速率

private:
	int m_rowNum;
	int m_colNum;

	int m_rowIdStartShow;
	int m_rowIdEndShow;
	int m_ssColLeftIndent;	//起始显示列左侧缩进
	int m_esColRightIndent;	//终止显示列左侧缩进
	int m_ssRowTopIndent;	//起始显示行顶部缩进
	int m_esRowBottomIndent;	//终止显示列底部缩进

	int m_pxRowH;	//固定行高 
	int m_pxColW;	//固定列宽
	int m_pxHeaderH;	//固定标题高度
	BOOL m_bUseFixedHW;	//是否使用固定行高列宽模式

	Color m_clrGraHeadStart;
	Color m_clrGraHeadEnd;
	BOOL m_bEditable;	//是否可编辑

	BOOL m_bShowHoriScroll;	//是否显示水平滚动条
	BOOL m_bShowVertScroll;	//是否显示垂直滚动条

	CRect m_rcLst;
	CRect m_rcHeader;
	CRect m_rcBody;
	CRect m_rcGrad;	//梯度信息显示区域

	int m_totalColW;	//所有列的宽度之和
	int m_totalRowH;	//所有行的高度之和

	int m_cdcW;	//记录显示列的总宽度（包括 左侧、右侧缩进像素） CDC Width
	int m_cdcH;	//记录显示行的总高度（包括 顶部、底部缩进像素） CDC Height

	ESCROLLHVCONFLICTMANAGE m_eScrollHVCM;

	COLORREF m_clrHeaderBk;
	COLORREF m_clrBodyBk;

	int m_colIdStartShow;
	int m_colIdEndShow;

	CSegStepRange m_segStepRange;	//从零开始
	CSegDotPosInfo m_segDotPosInfo;	//实时记录鼠标当前位置信息
	CSegDotPosInfo m_preEditDotInfo;	//记录编辑框弹出时的位置信息,用于在编辑框失去焦点时正确保存编辑框内容

	int m_stepWidth;	//步骤项宽度

	BOOL m_bMonitorFunc;	//标识是否用于监控
	BOOL m_bEditDataProcessed;	//标识编辑框数据是否已经获取，防止（编辑框）Enter和失去焦点响应函数重复处理

	int m_visibleStepNum;	//页面可见的最大步骤个数

	Gdiplus::Image m_image;
	Gdiplus::Image m_imgTriangle;	//三角形标识
	Gdiplus::Image m_imgCamera;	//相机标识
	Gdiplus::Image m_imageMelt;	//熔曲类型背景图标
	Gdiplus::Image m_imgAllPtIcon;	//熔曲段AllPoint模式图标

	int m_roundHeight;	//温度、延时时间背景图中小圆圈高度

	BOOL m_bSelMoveStart;	//标识是否选中温度线段

	CPoint m_ptPre;

	float m_minTem;
	float m_maxTem;
	float m_oneTemH;	//1℃对应的像素高度

	int m_frameW;	//边框宽度

	CMySegDlgParam m_segParam;

	COLORREF m_clrFourAngleBk;	//圆角矩形以外的背景色
	int m_iRoundAngleSize;	//圆角大小
	Color m_clrIndepent;	//独立第三方颜色，用于支持局部透明

	ECOLLECTFLUTAGTYPE m_eCollectFluTagType;	//标识采集荧光或者不采集荧光的 图标类型

	CString m_strSegDefName;	//程序段默认名称
	CString m_strStepDefName;	//步骤默认名称

	int m_iGradInfoW;	//梯度信息显示宽度
	CLinkList<double> m_lnkGradData;
	BOOL m_bHaveGrad;	//是否包含梯度信息
	BOOL m_bUseGradInfo;	//是否使用梯度信息
	CString m_strGrad;
	CString m_strCol;
	CString m_strSeg;
	CString m_strStep;
	CString m_strRange;
};


