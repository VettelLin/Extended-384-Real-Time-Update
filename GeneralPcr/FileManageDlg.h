#pragma once

#include "MyPageSwitchBt.h"

#include "resource.h"
#include "RectManage.h"
#include "ExperimentData.h"

#include <GdiPlus.h>
#include <vector>

using namespace std;

// CFileManageDlg dialog

class CWorkDlg;

#define BT_X_INDENT	1	//从第二个按钮开始，页面按钮左移像素数
#define BT_TOP_START	0	//页面切换按钮顶部

class CFileManageDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CFileManageDlg)

public:
	CFileManageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFileManageDlg();

// Dialog Data
	enum {IDD = IDD_MIDDLEDLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	virtual BOOL OnInitDialog();

			void InitDlg();

	virtual	void SetCtrlPos();
	virtual	void SetPageSwitchBtPos(int iStart);
			void ShowCtrl();

public:
	afx_msg void OnPageSelected(UINT nID);
	afx_msg LRESULT OnMsgClosePage(WPARAM wParam,LPARAM lParam);

			int AddPage();	//添加新页面，外部使用
	virtual	int AddNewPage(CExperimentData* pExpData,CString strFileName = _T(""));
	virtual	int DeletePage(int pageId);
			void SetNewFileSavePath(CString strNewFileSavePath);	//实验开始后，更新实验路径信息到实验切换按钮中
			int SetPageFirstColumn(int pageId);	//设置页面按钮为第一列位置
			int SetPageSwitchBtState(int pageId,BOOL bRunning,BOOL bRedraw = TRUE);	//设置按钮为非运行状态（普通状态）
			int SetPageSelected(int pageId);
			int SetCurrentSelectPageString(CString strFileName,CString strFilePath);
			void SetExperName(int pageId,CString strExpeName);
			BOOL CheckFileExistOrNot(CString strFilePath);	//判断文件是否已经打开
			int SetPageFilePath(int iPageId,CString strFilePath);
			BOOL CheckFileNameExistOrNot(CString strFileName);	//判断是否存在 相同文件名 的实验已被打开

	virtual	BOOL PreTranslateMessage(MSG* pMsg);

			void InitPageSwitchBt(CMyPageSwitchBt* pPageSwitchBt);

public:
	CLinkList<CMyPageSwitchBt*> m_lnkPageSwitchBtPtr;

	Image m_imgLogo;
	Image m_imgLogo2;

public:
	int m_iBtHeight;	//按钮默认高度

	CMyPageSwitchBt* m_pCurSelBt;

	CWorkDlg* m_pWorkDlg;

	BOOL m_bCtrlCreated;

	int m_iLeftIndent;	//标识第一个按钮到窗口左侧的距离
	int m_iRightIndent;	//标识最后一个按钮到窗口左侧的距离
	int m_iBtDefWidth;	//按钮默认宽度
	int m_iBtRealWidth;	//实际使用的按钮宽度
	int m_iBtPreRealW;	//记录上一次使用的按钮宽度
	int m_iBtRegionW;	//按钮区域宽度

	CRectManage m_rcManage;
};


