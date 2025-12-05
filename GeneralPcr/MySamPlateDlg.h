#pragma once

#include "FontSet.h"
#include "LinkList.h"
#include "TitleInfo.h"
#include "CellInfo.h"
#include "MyScrollInfo.h"
#include "SelfDefType.h"
#include "RowInfo.h"
#include "DotPosInfo.h"
#include "RowColEditInfo.h"
#include "TubeInfo.h"
#include "PlateInfo.h"
#include "MySelWnd.h"
#include "HoleColorInfo.h"
 
#include <GdiPlus.h>
#include <map>

using namespace std;

#define SAMPLATE_DELETEKEY_PRESSED WM_USER + 6034	//通知父窗口Delete按键被按下

class CMySamPalteDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CMySamPalteDlg)

public:
	CMySamPalteDlg();
	virtual ~CMySamPalteDlg();
	
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();

public:
			int DrawTotalHeader(CDC* pDC);
			int DrawRowHeader(CDC* pDC);
			int DrawColHeader(CDC* pDC);
			int DrawBody(CDC* pDC);
			int DrawHoriScroll(CDC* pDC);
			int DrawVertScroll(CDC* pDC);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg LRESULT OnMsgEditEnterPushed(WPARAM wParam,LPARAM lParam);

			void ProcessHoriSrcoll(UINT nFlags, CPoint point);
			void ProcessVertSrcoll(UINT nFlags, CPoint point);

			int GetColStartDrawInMemoryHoriPixel(int colId);
			int GetRowStartDrawInMemoryVertPixel(int rowId);

			int GetWidth(int colStart,int colEnd);
			int GetHeight(int rowStart,int rowEnd);

			CDotPosInfo CalcPtRowCol(CPoint pt);
			
			int InsertColumn(int colId,CString strColTitle,BOOL bRedraw = TRUE);
			int InsertRow(int rowId,BOOL bRedraw = TRUE);
			
			int DeleteColumn(int colId,BOOL bRedraw = TRUE);
			int DeleteRow(int rowId,BOOL bRedraw = TRUE);

			int DeleteMultiRow(vector<int> vecSelectedRow,BOOL bRedraw = TRUE);	//存储行信息的是链表结构，删除多项时，倒着删除,vecSelectedRow记录的下标需要是从小到大的顺序

			void DeleteData(BOOL bRedraw = FALSE);	//清除列表信息，不包括标题栏
			void Clear(BOOL bRedraw = FALSE);	//删除所有的行，不包括标题栏

			vector<int> GetSelectedRowId();	//获取选中行的下标，从小到大的顺序

			int SetColumnTitleInfo(int colId,CTitleInfo titleInfo,BOOL bRedraw = TRUE);
			int GetColumnTitleInfo(int colId,CTitleInfo& titleInfo);

			int SetColumnCheckFunc(int colId,BOOL bHaveCheckFunc,BOOL bRedraw = FALSE);
			int GetColumnCheckFunc(int colId,BOOL& bHaveCheckFunc);

			int SetColumnColorFunc(int colId,BOOL bHaveColorFunc,BOOL bRedraw = FALSE);
			int GetColumnColorFunc(int colId,BOOL& bHaveColorFunc);

			int SetColumnWidth(int colId,int colWidth,BOOL bRedraw = FALSE);
			int GetColumnWidth(int colId);

			int SetItemHeight(int itemH,BOOL bRedraw = FALSE);
			int GetItemHeight();

			int SetFixedRowHeightColumnWidth(int iRowH,int iColW,BOOL bRedraw = FALSE);
			int GetFixedRowHeightColumnWidth(int& iRowH,int& iColW);

			int SetColumnText(int colId,CString strColTitle);
			CString GetColumnText(int colId);

			int SetColumnMaxMinInfo(int colId,CMaxMinInfo maxMinInfo);
			int GetColumnMaxMinInfo(int colId,CMaxMinInfo& maxMinInfo);

			int SetItemText(int rowId,int colId,CString strText,BOOL bRedraw = FALSE);
			CString GetItemText(int rowId,int colId);

			int SetRowSelectState(int rowId,BOOL bSelected,BOOL bSingleSel = FALSE,BOOL bRedraw = FALSE);
			int GetRowSelectState(int rowId,BOOL& bSelected);

			int SetRowDisableState(int rowId,BOOL bDisabled,BOOL bRedraw = FALSE);
			int GetRowDisableState(int rowId,BOOL& bDisabled);

			int SetCellCheckState(int rowId,int colId,BOOL bChecked,BOOL bRedraw = FALSE);
			int GetCellCheckState(int rowId,int colId,BOOL& bChecked);

			int SetCellDisableState(int rowId,int colId,BOOL bDisabled,BOOL bRedraw = FALSE);
			int GetCellDisableState(int rowId,int colId,BOOL& bDisabled);

			int SetCellSelfCbInfo(int rowId,int colId,BOOL bCbable,BOOL bUseCellLnkCbStr,CLinkList<CString>& lnkCbStr,BOOL bEditableCb,BOOL bRedraw = FALSE);	//设置单元格自定义的下拉框信息
			int GetCellSelfCbInfo(int rowId,int colId,BOOL& bCbable,BOOL& bUseCellLnkCbStr,CLinkList<CString>& lnkCbStr,BOOL& bEditableCb);

			int GetRowNum();
			int GetColNum();

			void SetBeGradientHeader(BOOL bGradientHeader,BOOL bRedraw = FALSE);
			BOOL GetBeGradientHeader();

			void SetBkColor(COLORREF bkClr,BOOL bRedraw = FALSE);
			COLORREF GetBkColor();

			void SetHeaderBkColor(COLORREF headerBkClr,BOOL bRedraw = FALSE);
			Color GetHeaderBkColor();

			void SetBodyBkColor(COLORREF bodyBkClr,BOOL bRedraw = FALSE);
			COLORREF GetBodyBkColor();

			void SetRowHeaderWidth(int iRowW,BOOL bRedraw = FALSE);
			int GetRowHeaderWidth();

			void SetColumnHeaderHeight(int iColH,BOOL bRedraw = FALSE);
			int GetColumnHeaderHeight();

			void SetFrameLineWidth(int frameLineWidth,BOOL bRedraw = FALSE);
			int GetFrameLineWidth();

			void SetFrameLineColor(COLORREF frameLineColor,BOOL bRedraw = FALSE);
			COLORREF GetFrameLineColor();

			void SetRowColLineWidth(int rowColLineWidth,BOOL bRedraw = FALSE);
			int GetRowColLineWidth();

			void SetRowColLineColor(COLORREF rowColLineColor,BOOL bRedraw = FALSE);
			COLORREF GetRowColLineColor();

			void SetBeShowFrameLine(int bShowFrameLine,BOOL bRedraw = FALSE);
			BOOL GetBeShowFrameLine();

			void SetBeShowRowColLine(int bShowRowColLine,BOOL bRedraw = FALSE);
			BOOL GetBeShowRowColLine();

			void SetHeaderTextColor(COLORREF headTextColor,BOOL bRedraw = FALSE);
			COLORREF GetHeaderTextColor();

			void SetBeSingleSelect(BOOL bSingleSelect,BOOL bRedraw = FALSE);
			BOOL GetBeSingleSelect();

			void SetTitle(CString strTitle,BOOL bRedraw = FALSE);
			CString GetTitle();

			void SetBeTellParentRowSelChange(BOOL beTellParentSiteSelChange);
			BOOL GetBeTellParentRowSelChange();

			void InitFontHeader(int lfHeight);
			void SetMyHeaderFont(CString fontName,int nHeight,int nWeight = FW_NORMAL,FONT_UNIT_TYPE fontUnitType = TYPE_PIXEL); //nHeight单位为 磅，即1/72英寸;
			void SetMyHeaderFont();

			void InitFontTitle(int lfHeight);
			void SetMyTitleFont(CString fontName,int nHeight,int nWeight = FW_NORMAL,FONT_UNIT_TYPE fontUnitType = TYPE_PIXEL); //nHeight单位为 磅，即1/72英寸;
			void SetMyTitleFont();

			void InitFontAbbr(int lfHeight);
			void SetMyFontAbbr(CString fontName,int nHeight,int nWeight = FW_NORMAL,FONT_UNIT_TYPE fontUnitType = TYPE_PIXEL); //nHeight单位为 磅，即1/72英寸;
			void SetMyFontAbbr();

			void SetBeEditable(BOOL bEditable,BOOL bRedraw = FALSE);
			BOOL GetBeEditable();

			int GetRowTag(int iRowId,CString& strTag);
			int GetColTag(int iColId,CString& strTag);

			int GetSiteTag(int iSiteIndex,CString& strTag);
			int GetSiteTag(int iRowId,int iColId,CString& strTag);

			int SetRowColNum(int iRowNum,int iColNum,BOOL bRedraw = FALSE);
			BOOL GetBeValidPlate();	//判断孔板是否合法

			int UpdateRowSelectState(int iRowId,BOOL bRedraw);
			int UpdateColSelectState(int iColId,BOOL bRedraw);

			int UpdateRowSelectState(BOOL bRedraw);
			int UpdateColSelectState(BOOL bRedraw);

			void SetAllSelectState(BOOL bSelected,BOOL bRedraw = FALSE);
			void SetAllSelectStateImmediately(BOOL bSelected,BOOL bRedraw = FALSE);	//让父窗口立即响应
			BOOL GetBeAllSelected();

			int SetCurSelectSite(vector<int>& vecSelSite,BOOL bRedraw = FALSE);
			int GetCurSelectSite(vector<int>& vecSelSite);

			int GetSamInfo(int iSiteIndex,CTubeInfo& samInfo);
			int GetSamInfo(int iRowId,int iColId,CTubeInfo& samInfo);

			int GetTubeInfoPtr(int iSiteIndex,CTubeInfo** ppTubeInfo);
			CTubeInfo* GetTubeInfoPtr();
			int GetSamInfo(vector<CTubeInfo>& vecSamInfo);

			int SetSamInfo(vector<CTubeInfo>& vecSamInfo,BOOL bRedraw = FALSE);
			int SetTubeInfo(CPtrArray* pTubeAryPtr,BOOL bRedraw = FALSE);
			int SetTubeInfo(CTubeInfo* pTubeInfo,int iSiteNum,BOOL bRedraw = FALSE);
			int SetTubeInfo(vector<CTubeInfo*>& vecPTubeInfo,BOOL bRedraw = FALSE);

			int SetSamInfo(int iSiteIndex,CTubeInfo& samInfo,BOOL bRedraw = FALSE);
			int SetSamInfo(int iRowId,int iColId,CTubeInfo& samInfo,BOOL bRedraw = FALSE);

			int SetSamInfo(int iSiteIndex,EITEMINFOTYPE eItemInfoType,CString strData,int intData = 0,double dData = 0,BOOL bRedraw = FALSE);
			int SetSamInfo(int iRowId,int iColId,EITEMINFOTYPE eItemInfoType,CString strData,int intData = 0,double dData = 0,BOOL bRedraw = FALSE);

			int SetRectWidth(int iRectW,BOOL bRedraw = FALSE);
			int SetRectHeight(int iRectH,BOOL bRedraw = FALSE);

			void SetBeShowTopText(BOOL bShowTopTx,BOOL bRedraw = FALSE);
			BOOL GetBeShowTopText();

			void SetBeShowBottomText(BOOL bShowBottomTx,BOOL bRedraw = FALSE);
			BOOL GetBeShowBottomText();

			int GetPlateSelectState(vector<BOOL>& vecSelState);
			int GetPlateSelectedSite(vector<BOOL>& vecSelectedSite);

			void SetFourAngleBkColor(COLORREF clrFourAngleBk,BOOL bRedraw = FALSE);
			COLORREF GetFourAngleBkColor();

			void SetInterval(int iHori,int iVert,BOOL bRedraw = FALSE);
			int GetHoriInterval();
			int GetVertInterval();

			void SetBeShieldDeleteKey(BOOL bShieldDeleteKey);
			BOOL GetBeShieldDeleteKey();

			void SetBeParentProcessDeleteKey(BOOL bParentProcessDeleteKey);
			BOOL GetBeParentProcessDeleteKey();

			void SetSiteIdShowState(BOOL bShow,BOOL bRedraw = FALSE);
			BOOL GetSiteIdShowState();

			void SetLastChanBlockShowState(BOOL bShow,BOOL bRedraw = FALSE);
			BOOL GetLastChanBlockShowState();

			int DeleteSelectedTubeInfo(BOOL bTemplateMode);	//删除选中的孔位信息
			int DeleteTubeInfo(vector<int> vecId,BOOL bTemplateMode);	//删除选中孔位的所有信息
			int DeleteTubeInfo(vector<int> vecId,EDELETETYPE eDeletType,BOOL bTemplateMode);	//删除选中孔位的所有信息
			int DeleteAllTubeInfo();	//删除所有的孔位信息

			int GetSameTubeInfo(CTubeInfo& tubeInfo);	//选中多个孔位时，提取相同的信息项

			int SetAllTubeChanInfo(int iChanId,BOOL bSelected,CString strDyeName,CString strTargetName);

			CString GetOneChannelDyeNameFromTubeInfo(int iChanId);

			int GetAllSampleType(vector<eSampleType>& vecSamType);	//按下标顺序获取孔板的样本类型信息

	 static CString SampleTypeToText(eSampleType eType);
	 static eSampleType TextToSampleType(CString strText);

	 static int InitMapSampleTypeToText();
	 static int InitMapTextToSampleType(); 
	 static int InitMapSamTypeToHoleClrInfo(); 
	 static int InitMapSamTypeToAbbr();
	 static CString GetSampleTypeAbbr(eSampleType eType);

private:
			int CalcTotalColumnWidth();
			int CalcTotalRowHeight();
			int CalcRowColToShow();	//计算显示的 行列起始、终止序号,上下左右缩进,当增删行、列并且影响到显示行列时调用该函数
			int CalcRowColToShowOnNoRowData();	//没有行数据时，按照固定行高（28）计算显示分割线显示区域
			int CalcColToShow(int newStartCol);	//删除列时使用
			int CalcRowToShow(int newStartRow);	//删除行时使用
			int ScrollCol(int pxHoriShift);
			int ScrollRow(int pxVertShift);
			int ScrollColToFixPos(int pxFixPos);
			int ScrollRowToFixPos(int pxFixPos);
			void CalcScrollPos();

			void CheckMaxMinSet(CString& strTemp,CMaxMinInfo& maxMinInfo);	//结束编辑后，验证最大最小值设置是否在正确范围内，不在正确范围内则自动修正

			int GetHoleColorInfo(int iRowId,int iColId,CHoleColorInfo& holeClrInfo);

public:
	CLinkList<CTitleInfo> m_lnkTitleInfo;	//保存标题信息
	CLinkList<CLinkList<CCellInfo>> m_2LnkCellInfo;	//保存行列内容信息
	CLinkList<CRowInfo> m_lnkRowInfo;	//保存行公共信息

private:
	int m_rowNum;
	int m_colNum;

	int m_colIdStartShow;
	int m_colIdEndShow;
	int m_rowIdStartShow;
	int m_rowIdEndShow;
	int m_ssColLeftIndent;	//起始显示列左侧缩进
	int m_esColRightIndent;	//终止显示列左侧缩进
	int m_ssRowTopIndent;	//起始显示行顶部缩进
	int m_esRowBottomIndent;	//终止显示列底部缩进

	int m_rowIdSeparatorStart;	//分割线显示起始行，不需要垂直滚动时使用
	int m_rowIdSeparatorEnd;	//分割线显示终止行，不需要垂直滚动时使用
	int m_esSeparatorBottomIndent;	//分割线显示底部缩进，不需要垂直滚动时使用

	int m_pxRowH;	//固定行高 
	int m_pxColW;	//固定列宽
	int m_pxColHeaderH;	//列标题标题高度
	int m_pxRowHeaderW; //行标题宽度
	int m_pxFrame;	//边框宽度
	int m_pxRowColLine;	//行列分割线宽度

	BOOL m_bUseFixedHW;	//是否使用固定行高列宽模式
	BOOL m_bGradientHeader;	//标题栏是否使用渐变色背景

	COLORREF m_clrBk;	//主窗口背景色
	Color m_clrHeaderBk;	//标题栏背景颜色
	Color m_clrHeaderSelectedBk;	//标题栏背景颜色
	COLORREF m_clrBodyBk;	//body背景色
	COLORREF m_clrFrame;	//边框颜色
	COLORREF m_clrRowColLine;	//行列分割线颜色
	COLORREF m_clrHeaderText;	//标题栏文字颜色

	Color m_clrGraHeadStart;
	Color m_clrGraHeadEnd;
	BOOL m_bEditable;	//是否可编辑

	COLORREF m_clrHeadBodySepa;	//Header和Body分隔线

	BOOL m_bShowFrameLine;	//是否显示列表框边框线
	BOOL m_bShowRowColLine;	//是否显示水平、垂直分割线
	BOOL m_bSingleSelect;	//标识是否是单选功能

	BOOL m_bTellParentSiteSelChange;	//类表框选中孔位改变是否通知父窗口

	BOOL m_bShowSplitLine;	//是否显示分割线

	CRect m_rcLst;
	CRect m_rcColHeader;	//行标题
	CRect m_rcRowHeader;	//列标题	
	CRect m_rcTotalHeader;	//总标题
	CRect m_rcBody;

	int m_totalColW;	//所有列的宽度之和
	int m_totalRowH;	//所有行的高度之和

	int m_cdcW;	//记录显示列的总宽度（包括 左侧、右侧缩进像素） CDC Width
	int m_cdcH;	//记录显示行的总高度（包括 顶部、底部缩进像素） CDC Height

	CDotPosInfo m_curDotPosInfo;	//记录最近一次点击的行列信息，需要重新梳理
	CDotPosInfo m_preDotPosInfo;	//记录前一个点击的行列信息，需要重新梳理
	CLinkList<CDotPosInfo> m_lnkHistoryDotPosInfo;	//记录最近N个点击的行列信息

	CDotPosInfo m_dotPosLeftBtDown;	//记录鼠标左键按下时位置
	CDotPosInfo m_dotPosLeftBtUp;	//记录鼠标左键弹起时位置

	CFont m_fontSetHeader; 
	CFont m_fontSetTitle;
	CFont m_fontAbbr;	//显示样本类型缩写使用
	LOGFONT m_logFontHeader;
	LOGFONT m_logFontTitle;
	LOGFONT m_logFontAbbr;

	CString m_strTotalTitle;	//总标题

	int m_iRowWidth;

	BOOL m_bDraggableStart;	//标识可以触发拖动鼠标选中区域
	BOOL m_bDragMove;	//标识是否正在拖动鼠标选中区域
	CDotPosInfo m_dragStartDotPosInfo;	//记录拖动鼠标选中区域起点信息
	CPoint m_ptPreOnMove;	//记录鼠标移动时上一个坐标点

	int m_iRectW;	//圆角矩形宽度
	int m_iRectH;	//圆角矩形高度

	BOOL m_bShowTopTx;	//孔位顶端是否显示文本
	BOOL m_bShowBottomTx;	//孔位底端是否显示文本

	BOOL m_bLeftBtDown;	//记录左键按下、弹起是否成对发生在控件内部

	int m_iRoundAngleLT;	//左上角圆角大小
	int m_iRoundAngleRT;	//右上角圆角大小
	int m_iRoundAngleRB;	//右下角圆角大小
	int m_iRoundAngleLB;	//左下角圆角大小

public:
	//水平滚动条
	CMyScrollInfo m_horiScroll;
	CMyScrollInfo m_vertScroll;
	ESCROLLHVCONFLICTMANAGE m_eScrollHVCM;

	int m_iTotalSiteNum;	//总的孔位数
	BOOL m_bAllSelected;	//是否全部选中

	CMySelWnd m_sonWnd;

	//圆角矩形样式参数
	BOOL m_bRoundRectStyle;	//标识是否使用圆角矩形样式
	COLORREF m_clrFourAngleBk;	//圆角矩形以外的背景色
	int m_iRoundAngleSize;	//圆角大小
	Color m_clrIndepent;	//独立第三方颜色，用于支持局部透明

	static map<eSampleType,CString> m_mapSamTypeToTx;	//样本类型--文本映射
	static map<CString,eSampleType> m_mapTxToSamType;	//文本--样本类型映射
	static map<eSampleType,CHoleColorInfo> m_mapSamTypeToHoleClrInfo;	//样本类型--孔位颜色信息映射
	static map<eSampleType,CString> m_mapSamTypeToAbbr;	//样本类型--文本缩写映射

protected:
	CTubeInfo* m_pTubeInfo;	//样本信息
	CPlateInfo* m_pPlateInfo;	//孔板孔位参数信息

	//备用参数
	int m_iLeftIndent;	//左侧缩进	
	int m_iRightIndent;	//右侧缩进
	int m_iTopIndent;	//顶部缩进
	int m_iBottomIndent;	//底部缩进
	
	int m_iHoriInterval;	//孔位间水平间隔
	int m_iVertInterval;	//孔位间垂直间隔

	BOOL m_bShieldDeleteKey;	//是否屏蔽Delete按键删除功能

	BOOL m_bParentProcessDeleteKey;	//标识是否由父窗口处理Delete按键删除功能

	BOOL m_bShowSiteId;	//是否显示孔位序号标识
	BOOL m_bShowLastChanBlock;	//是否显示最后一个通道的颜色块（PS96的校准通道）
};






