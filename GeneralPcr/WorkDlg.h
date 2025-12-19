#pragma once

#include "MiddleDlg.h"
#include "FileManageDlg.h"
#include "LinkList.h"
#include "RectManage.h"
#include "FontSet.h"
// CWorkDlg dialog

class CWorkDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CWorkDlg)

public:
	CWorkDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWorkDlg();

// Dialog Data
	enum { IDD = IDD_WORKDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow,UINT nStatus);

	virtual BOOL OnInitDialog();
	virtual	void CreateCtrl();
			void CreateCkbt();
	virtual	void InitCtrl();
			void InitCkbt();
	virtual	void SetCtrlPos();
	virtual	void ShowCtrl();
			void InitSonDlg();
	virtual	void InitFileManageDlg();

	virtual	int SetPageSelected(int pageId);

			int AddNewPage(int pageId);
			int DeletePage(int pageId);

			int OpenFile();	//打开工程文件
	virtual int OpenPowerOffProtectFile(CString strFilePath);
			int OpenOriginalData();	//打开原始扩增数据文件

			int SavePrj(int iIndex,BOOL bNew);
			int DeletePrj(int iIndex);

			CMiddleDlg* GetMiddleDlg(int iPageId);

			int Delete();

			int NewTemplateAdded(int iIndex);	//新的模板文件成功添加

			int GetCurrentPageId();	//获取当前选中页面的ID

			BOOL IsRunningProcessExist(); //获取是否有正在运行的实验

			int RefreshLanguageDisplay();

			int SetDefaultGeneName();
			void SetInstrumentConnectState(CString strModel, CString strSN, BOOL bConnected, BOOL bReshowList);
			afx_msg LRESULT OnMsgInstrumentReinitialize(WPARAM wParam,LPARAM lParam);

public:
	CLinkList<CMiddleDlg*> m_lnkPageDlgPtr;
	CMiddleDlg* m_pCurSelPageDlg;
	CFileManageDlg m_fileManageDlg;

	CRectManage m_rcManage;

private:
	BOOL m_bCtrlCreated;
};
