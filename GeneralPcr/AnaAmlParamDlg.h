
#pragma once

#include "MyButton.h"
#include "MyLstCtrl.h"
#include "ExperimentData.h"

class CAnaAmlParamDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CAnaAmlParamDlg)

public:
	CAnaAmlParamDlg(CDataAnalysisPara* pAnalysisPara,CWnd* pParent = NULL);   // standard constructor
	virtual ~CAnaAmlParamDlg();

	enum { IDD = IDD_POPUPDLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
			void CreateCtrl();
			void CreateBt();
			void CreateLst();
			void InitDlg();
			void InitCtrl();
			void InitBt();
			void InitLst();
			void SetCtrlPos();
			void ShowCtrl();

			void SetCtrlText();

	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedAddTarget();
	afx_msg void OnBnClickedDeleteTarget();
	afx_msg LRESULT OnMsgRowSelectedChange(WPARAM wParam,LPARAM lParam);

			BOOL SetRoundRect(int x = 5,int y = 5);

			int SetLstAmlAnaBaseParam(CDataAnalysisPara* pAnalysisPara);

protected:
	void SetOneAmlBaseParam(int iRowIndex, tagAnaAmplifyPara* pInputParam);

public:
	BOOL GetChanged() { return m_bChanged; }

protected:
	CMyLstCtrl m_lstAmlParam;
	CMyButton m_btnAddTarget; // 增加一个基因参数
	CMyButton m_btnDeleteTarget; // 删除一个基因参数

	CMyButton m_btSave;
	CMyButton m_btCancel;

	CDataAnalysisPara* m_pAnalysisPara;

	BOOL m_bChanged;	//标识分析参数是否改变，临时使用，20220408，孙文郎
	int m_iChannelCount;
	CLinkList<CString> m_lnkCbStrChannel;

private:
	int m_roundValue;	//圆角数值
	int m_frameWidth;	//边框宽度

	int m_iWndWidth;	//窗口宽度
	int m_iWndHeight;	//窗口高度
	int m_iCaptionHeight;	//标题栏高度

	//CString m_strHandSetThre[PBCHANNUM];	//记录列表框初始化时手动阈值的字符串格式，防止浮点数对比出错
};



