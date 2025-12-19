#pragma once

#include "FontSet.h"
#include "MyNewEdit.h"
#include "LinkList.h"
#include "TitleInfo.h"
#include "CellInfo.h"
#include "MyScrollInfo.h"
#include "SelfDefType.h"
#include "RowInfo.h"
#include "DotPosInfo.h"
#include "RowColEditInfo.h"
#include "MyInputCb.h"
#include "ColorSelectDlg.h"

#include <GdiPlus.h>
#include <map>

using namespace std;

#define WM_TELLPARENTCELLCHANGED	WM_USER + 6031	//编辑框单元格信息改变，通知父窗口

class CMyLstCtrl : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CMyLstCtrl)

public:
	CMyLstCtrl();
	virtual ~CMyLstCtrl();
	
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();

public:
			int DrawHeader(CDC* pDC);
	virtual	int DrawBody(CDC* pDC);
			int DrawHoriScroll(CDC* pDC);
			int DrawVertScroll(CDC* pDC);
			int DrawHoriScrollStyle2(CDC* pDC);	//绘制水平滚动条样式2，圆角边框模式使用
			int DrawVertScrollStyle2(CDC* pDC);	//绘制垂直滚动条样式2，圆角边框模式使用

	//virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnMsgFuncEditKillFocus(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgEditEnterPushed(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgTellParentTextChanged(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgTellParentSetNewText(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgTouch(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgGesture(WPARAM wParam,LPARAM lParam);

	afx_msg LRESULT OnMsgHorizonScroll(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgVerticalScroll(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgSelectNewColor(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgDeleteComboRowString(WPARAM wParam,LPARAM lParam);

			void ProcessHoriSrcoll(UINT nFlags, CPoint point);
			void ProcessVertSrcoll(UINT nFlags, CPoint point);

			int GetColStartDrawInMemoryHoriPixel(int colId);
			int GetRowStartDrawInMemoryVertPixel(int rowId);
			 
			int GetWidth(int colStart,int colEnd);
			int GetHeight(int rowStart,int rowEnd);

			void SetScrollArrowWH(int iWidth,int iHeight,BOOL bRedraw = FALSE);

			CDotPosInfo CalcPtRowCol(CPoint pt);

			int InsertColumn(int colId,CString strColTitle,BOOL bRedraw = FALSE);
			int InsertRow(int rowId,BOOL bRedraw = FALSE);
			int InsertRowSetHeight(int rowId,int iRowH,BOOL bRedraw = FALSE);

			int DeleteColumn(int colId,BOOL bRedraw = TRUE);
			int DeleteAllColumn(BOOL bRedraw = TRUE);
			int DeleteRow(int rowId,BOOL bRedraw = TRUE);

			int DeleteMultiRow(vector<int> vecSelectedRow,BOOL bRedraw = TRUE);	//存储行信息的是链表结构，删除多项时，倒着删除,vecSelectedRow记录的下标需要是从小到大的顺序

			void DeleteData(BOOL bRedraw = FALSE);	//清除列表信息，不包括标题栏
			void Clear(BOOL bRedraw = FALSE);	//删除所有的行，不包括标题栏

			int SetSelectedRowId(vector<int>& vecSelRowId,BOOL bRedraw = FALSE);	//获取选中行的下标，从小到大的顺序
			vector<int> GetSelectedRowId();	//获取选中行的下标，从小到大的顺序

			int SetColumnTitleInfo(int colId,CTitleInfo titleInfo,BOOL bRedraw = TRUE);
			int GetColumnTitleInfo(int colId,CTitleInfo& titleInfo);

			int SetColumnCheckFunc(int colId,BOOL bHaveCheckFunc,BOOL bRedraw = FALSE);
			int GetColumnCheckFunc(int colId,BOOL& bHaveCheckFunc);

			int SetCellCheckFunc(int rowId,int colId,BOOL bCheckable,BOOL bRedraw = FALSE);
			int GetCellCheckFunc(int rowId,int colId,BOOL& bCheckable);

			int SetColumnColorFunc(int colId,BOOL bHaveColorFunc,BOOL bRedraw = FALSE);
			int GetColumnColorFunc(int colId,BOOL& bHaveColorFunc);

			int SetColumnColorEditable(int colId,BOOL bClrEditable,BOOL bRedraw = FALSE);
			int GetColumnColorEditable(int colId,BOOL& bClrEditable);

			int SetColumnEditable(int colId,BOOL bEditable,BOOL bRedraw = FALSE);
			int GetColumnEditable(int colId,BOOL& bEditable);

			int SetColumnDisableState(int colId,BOOL bDisabled,BOOL bRedraw = FALSE);
			int GetColumnDisableState(int colId,BOOL& bDisabled);

			int SetColumnTextAlignFormat(int colId,DWORD dwTxAlign,BOOL bRedraw = FALSE);
			int GetColumnTextAlignFormat(int colId,DWORD& dwTxAlign);

			int SetColumnMultiDensityState(int colId,BOOL bData,BOOL bRedraw = FALSE);
			int GetColumnMultiDensityState(int colId,BOOL& bData);

			int SetCellEditable(int rowId,int colId,BOOL bEditable,BOOL bRedraw = FALSE);
			int GetCellEditable(int rowId,int colId,BOOL& bEditable);

			int SetColumnCbtable(int colId,BOOL bCbtable,BOOL bCbEditable = FALSE,BOOL bRedraw = FALSE);
			int GetColumnCbtable(int colId,BOOL& bCbtable);

			int SetColumnBeTellParentCellChanged(int colId,BOOL bTellParentCellChanged);
			int GetColumnBeTellParentCellChanged(int colId,BOOL& bTellParentCellChanged);

			int SetColumnCbString(int colId,CLinkList<CString>& lnkCbStr,BOOL bRedraw = FALSE);
			int GetColumnCbString(int colId,CLinkList<CString>& lnkCbStr);

			int SetColumnWidth(int colId,int colWidth,BOOL bRedraw = FALSE);
			int GetColumnWidth(int colId);

			int SetColumnDataParam(int iColId,CDataParam& dtParam,BOOL bRedraw = FALSE);
			int GetColumnDataParam(int iColId,CDataParam& dtParam);

			int SetItemHeight(int itemH,BOOL bRedraw = FALSE);
			int GetItemHeight();

			int SetColumnText(int colId,CString strColTitle,BOOL bRedraw = FALSE);
			CString GetColumnText(int colId);

			int SetColumnMaxMinInfo(int colId,CMaxMinInfo maxMinInfo);
			int GetColumnMaxMinInfo(int colId,CMaxMinInfo& maxMinInfo);

			int SetItemText(int rowId,int colId,CString strText,BOOL bRedraw = FALSE);
			CString GetItemText(int rowId,int colId);

			int SetItemColorParam(int rowId,int colId,COLORREF clr,int clrW,int clrH,int iRoundAngle,BOOL bRedraw = FALSE);
			int GetItemColorParam(int rowId,int colId,COLORREF& clr,int& clrW,int& clrH,int& iRoundAngle);
			int SetItemColor(int rowId,int colId,COLORREF clr,BOOL bRedraw = FALSE);
			int GetItemColor(int rowId,int colId,COLORREF& clr);

			int SetRowSelectState(int rowId,BOOL bSelected,BOOL bSingleSel = FALSE,BOOL bRedraw = FALSE);
			int GetRowSelectState(int rowId,BOOL& bSelected);

			int SetRowDisableState(int rowId,BOOL bDisabled,BOOL bRedraw = FALSE);
			int GetRowDisableState(int rowId,BOOL& bDisabled);

			int SetCellCheckState(int rowId,int colId,BOOL bChecked,BOOL bRedraw = FALSE);
			int GetCellCheckState(int rowId,int colId,BOOL& bChecked);

			int SetCellDisableState(int rowId,int colId,BOOL bDisabled,BOOL bRedraw = FALSE);
			int GetCellDisableState(int rowId,int colId,BOOL& bDisabled);

			int SetCellSelfCbInfo(int rowId,int colId,BOOL bCbable,BOOL bUseCellLnkCbStr,CLinkList<CString>& lnkCbStr,BOOL bEditableCb,BOOL bRedraw = FALSE);	//设置单元格自定义的下拉框信息
			int SetCellSelfCbInfo(int rowId,int colId,BOOL bCbable,BOOL bUseCellLnkCbStr,BOOL bEditableCb,BOOL bRedraw = FALSE);	//设置单元格自定义的下拉框信息
			int SetCellSelfCbInfo(int rowId,int colId,CLinkList<CString>& lnkCbStr,BOOL bRedraw = FALSE);
			int GetCellSelfCbInfo(int rowId,int colId,BOOL& bCbable,BOOL& bUseCellLnkCbStr,CLinkList<CString>& lnkCbStr,BOOL& bEditableCb);
			int GetCellSelfCbInfo(int rowId,int colId,CLinkList<CString>& lnkCbStr);
			int SetCellSelCbRowDelete(int rowId, int colId, BOOL bEnableDelete);

			int GetRowNum();
			int GetColNum();

			void SetBeGradientHeader(BOOL bGradientHeader,BOOL bRedraw = FALSE);
			BOOL GetBeGradientHeader();

			void SetBkColor(Color bkClr,BOOL bRedraw = FALSE);
			Color GetBkColor();

			void SetHeaderBkColor(Color headerBkClr,BOOL bRedraw = FALSE);
			Color GetHeaderBkColor();

			void SetBodyBkColor(Color bodyBkClr,BOOL bRedraw = FALSE);
			Color GetBodyBkColor();

			void SetHeaderHeight(COLORREF headerHeight,BOOL bRedraw = FALSE);
			COLORREF GetHeaderHeight();

			void SetFrameLineWidth(int frameLineWidth,BOOL bRedraw = FALSE);
			int GetFrameLineWidth();

			void SetFrameLineColor(Color frameLineColor,BOOL bRedraw = FALSE);
			Color GetFrameLineColor();

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

			void SetBeAllowCancelRowSelect(BOOL bValue,BOOL bRedraw = FALSE);
			BOOL GetBeAllowCancelRowSelect();

			void SetTitle(CString strTitle,BOOL bRedraw = FALSE);
			CString GetTitle();

			void SetBeTellParentRowSelChange(BOOL beTellParentRowSelChange);
			BOOL GetBeTellParentRowSelChange();

			void SetFourAngleBkColor(COLORREF clr,BOOL bRedraw = FALSE);
			COLORREF GetFourAngleBkColor();

			void SetLargeCheckIconUseState(BOOL bData,BOOL bRedraw = FALSE);
			BOOL GetLargeCheckIconUseState();

			void InitFontHeader(int lfHeight);
			void SetMyHeaderFont(CString fontName,int nHeight,int nWeight = FW_NORMAL,FONT_UNIT_TYPE fontUnitType = TYPE_PIXEL); //nHeight单位为 磅，即1/72英寸;
			void SetMyHeaderFont();

			void InitFontTitle(int lfHeight);
			void SetMyTitleFont(CString fontName,int nHeight,int nWeight = FW_NORMAL,FONT_UNIT_TYPE fontUnitType = TYPE_PIXEL); //nHeight单位为 磅，即1/72英寸;
			void SetMyTitleFont();

			int SetMapColumnDisableControl(int colId,CLinkList<int>& lnkDisabledCol);
			int GetMapColumnDisableControl(int colId,CLinkList<int>& lnkDisabledCol);
			int DeleteColumnDisableControl(int colId,BOOL bRedraw = FALSE);
			void ClearMapColumnDisableControl();

			int SetMapRowOutsideControl(int rowId,CLinkList<int>& lnkDisabledRow);
			int GetMapRowOutsideControl(int rowId,CLinkList<int>& lnkDisabledRow);
			int DeleteRowOutsideControl(int rowId,BOOL bRedraw = FALSE);
			void ClearMapRowOutsideControl();

			int UpdateRowColumnDisableState(int rowId,int colId,CString strText,BOOL bRedraw = FALSE);	//列表框内部列控制关系更新
			int TellParentSomething(int rowId,int colId,CString strText,BOOL bRedraw = FALSE);	//通知父窗口行信息改变，由父窗口做出其它操作,目前只对 可使用编辑框、下拉框修改的单元格进行该处理
			int TellParentCellChanged(int rowId,int colId);	//通知父窗口单元格信息改变，由父窗口做出其它操作,目前只对 可使用编辑框、下拉框修改的单元格进行该处理

			int AddPairToMapDisabledStateKeyString(CString strKey,BOOL bDisabled,BOOL bRedraw = FALSE);
			int GetKeyStringDisabledState(CString strKey,BOOL& bDisabled);
			int DeleteKeyStringDisabledState(CString strKey,BOOL bRedraw = FALSE);
			void ClearMapDisabledStateKeyString();

			void SetBeEditable(BOOL bEditable,BOOL bRedraw = FALSE);
			BOOL GetBeEditable();

			void EndEDit();	//结束编辑
			void EndEDitForFuncCtrlKillFocus(int iCtrlTag);

			void CbTextChanged();

			void SetBeShowRowSelectState(BOOL bShowRowSelState,BOOL bRedraw = FALSE);
			BOOL GetBeShowRowSelectState();

			void SetBeScrollInsideFrame(BOOL bScrollInsideFrame,BOOL bRedraw = FALSE);
			BOOL GetBeScrollInsideFrame();

			CRect GetColorSelectDlgRect(CRect rcBase,int iWndW,int iWndH);

			void SetRowBkSelectedColor(COLORREF clrRowBkSel);
			COLORREF GetRowBkSelectedColor();

			void SetRowBkUnSelectedColor(COLORREF clrRowBkUnSel);
			COLORREF GetRowBkUnSelectedColor();

			int CalculateFitRowHeight();	//计算自动适配的行高
			int SetAllRowHeight(int iRowH,BOOL bRedraw = FALSE);	//设置所有的行高

			int BottomBlankSetAutoFitHeight(BOOL bLastRowUseRemainder = TRUE,BOOL bRedraw = FALSE);	//如果总的行高小于m_rcBody的高度，自动计算行高使m_rcBody完全填充，bLastRowUseRemainder最后一行是否加上余数

			int SetFuncEditCtrlFont(CString strFtName,int nHeight,int nWeight = FW_NORMAL,FONT_UNIT_TYPE fontUnitType = TYPE_PIXEL);

			int RefreshSerialNumber(int iColId,int iIndex,BOOL bRedraw = FALSE);

			BOOL IsCbEndEdit();
			void SetCbEndEditState(BOOL bData);
			int SetCbSelectItem(int iRowId,int iColId,CLinkList<CString>& lnkStr);

protected:
			int RenewHoriScrollPos();	//刷新水平滚动条位置
			int RenewVertScrollPos();	//刷新垂直滚动条位置

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
			void CalcScrollPosStyle2();

			void CheckMaxMinSet(CString& strTemp,CMaxMinInfo& maxMinInfo);	//结束编辑后，验证最大最小值设置是否在正确范围内，不在正确范围内则自动修正

public:
	CLinkList<CTitleInfo> m_lnkTitleInfo;	//保存标题信息
	CLinkList<CLinkList<CCellInfo>> m_2LnkCellInfo;	//保存行列内容信息
	CLinkList<CRowInfo> m_lnkRowInfo;	//保存行公共信息

	//水平滚动条
	CMyScrollInfo m_horiScroll;
	CMyScrollInfo m_vertScroll;
	ESCROLLHVCONFLICTMANAGE m_eScrollHVCM;

	CMyNewEdit m_edInput;	//用于编辑单元格信息
	CMyInputCb m_cbCtrl;	//用于下拉选择

	int m_iInputEdTag;	//编辑框标识号
	int m_iInputCbTag;	//下拉选择框标识号

	map<int,CLinkList<int>> m_mapColDisableCtrl;	//记录列之间的禁用控制关系，键为“列下标”,值为“列控制的其它列下标集合"
	map<int,CLinkList<int>> m_mapRowOutsideCtrl;	//记录行对外部关联控件的控制关系，键为“行下标”,值为“行控制的其它控件（列表框）行下标集合"
	map<CString,BOOL> m_mapDisabledStateKeyString;	//记录“获取禁用状态的索引词，键为“关键词”,值为“TRUE/FALSE"

	BOOL m_bCbEndEdit;	//标识下拉选择控件是否终止编辑

protected:
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
	int m_pxHeaderH;	//固定标题高度
	int m_pxFrame;	//边框宽度
	int m_pxRowColLine;	//行列分割线宽度

	BOOL m_bUseFixedHW;	//是否使用固定行高列宽模式
	BOOL m_bGradientHeader;	//标题栏是否使用渐变色背景

	Color m_clrBk;	//主窗口背景色
	Color m_clrHeaderBk;	//标题栏背景颜色
	Color m_clrBodyBk;	//body背景色
	Color m_clrFrame;	//边框颜色
	COLORREF m_clrRowColLine;	//行列分割线颜色
	COLORREF m_clrHeaderText;	//标题栏文字颜色

	Color m_clrGraHeadStart;
	Color m_clrGraHeadEnd;
	BOOL m_bEditable;	//是否可编辑

	COLORREF m_clrHeadBodySepa;	//Header和Body分隔线

	BOOL m_bShowHoriScroll;	//是否显示水平滚动条
	BOOL m_bShowVertScroll;	//是否显示垂直滚动条
	BOOL m_bShowFrameLine;	//是否显示列表框边框线
	BOOL m_bShowRowColLine;	//是否显示水平、垂直分割线
	BOOL m_bSingleSelect;	//标识是否是单选功能
	BOOL m_bAllowCancelRowSelect;	//标识是否允许取消行选中

	BOOL m_bTellParentRowSelChange;	//类表框选中行改变是否通知父窗口

	CRect m_rcLst;
	CRect m_rcHeader;
	CRect m_rcBody;

	int m_totalColW;	//所有列的宽度之和
	int m_totalRowH;	//所有行的高度之和

	int m_cdcW;	//记录显示列的总宽度（包括 左侧、右侧缩进像素） CDC Width
	int m_cdcH;	//记录显示行的总高度（包括 顶部、底部缩进像素） CDC Height

	CDotPosInfo m_curDotPosInfo;	//记录最近一次点击的行列信息
	CDotPosInfo m_preDotPosInfo;	//记录前一个点击的行列信息
	CLinkList<CDotPosInfo> m_lnkHistoryDotPosInfo;	//记录最近N个点击的行列信息

	CFont m_fontSetHeader;
	CFont m_fontSetTitle;
	LOGFONT m_logFontHeader;
	LOGFONT m_logFontTitle;

	Image m_image;
	Image m_img2;

	CRowColEditInfo m_rowColEditInfo;	//记录当前的编辑行列信息
	CRowColEditInfo m_preRowColEditInfo;	//记录上一次的编辑行列信息

	CString m_strLstTitle;	//列表框标题

	//圆角矩形样式参数
	BOOL m_bRoundRectStyle;	//标识是否使用圆角矩形样式
	COLORREF m_clrFourAngleBk;	//圆角矩形以外的背景色
	int m_iRoundAngleSize;	//圆角大小
	Color m_clrIndepent;	//独立第三方颜色，用于支持局部透明

	CColorSelectDlg m_clrSelDlg;

	BOOL m_bShowRowSelState;	//标识是否显示行选中状态

	BOOL m_bScrollInsideFrame;	//标识滚动条是否在边框之内

	COLORREF m_clrRowBkSel;	//选中行背景色
	COLORREF m_clrRowBkUnSel;	//未选中行背景色

	BOOL m_bLeftBtDown;	//记录左键按下、弹起是否成对发生在控件内部

	BOOL m_bUseLargeChkIcon;	//是否使用放大的CheckBox图标

	BOOL m_bUseFixedIdpd;	//浮点数是否使用固定小数位数
};





