#pragma once

#include "FontSet.h"
#include "Resource.h"
#include "MyNewCb.h"
#include "MyNewEdit.h"
#include "MyLstCtrl.h"
#include "TubeInfo.h"
#include "InterfaceA.h"
#include "EditModeAbs.h"
#include "RectManage.h"

//class CProcSetDlg;

class CSamBaseInfoEditDlg: public CDialogEx,public CFontSet,public CEditModeAbs
{
	DECLARE_DYNAMIC(CSamBaseInfoEditDlg)

public:
	CSamBaseInfoEditDlg(CWnd* pParent = NULL,CInterfaceA* pInterfaceA = NULL);
	~CSamBaseInfoEditDlg(void);

	enum {IDD = IDD_POPUPDLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMsgTellParentTextChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgTellParentSetNewText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgTellParentLstCellChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgListDeleteComboRowString(WPARAM wParam, LPARAM lParam);

	virtual BOOL OnInitDialog();

			void InitDlg();
			void CreateCtrl();
			void CreateBt();
			void CreateSt();
			void CreateCb();
			void CreateEd();
			void CreateLst();
			void InitCtrl();
			void InitBt();
			void InitSt(); 
			void InitCb();
			void InitEd();
			void InitLst();
			void InitLstSamBaseInfo();
			void SetCtrlPos();
			void ShowCtrl();

			void AddChannel(tagDyeChannelInfo* pDyeChanInfo);

			void SetDefaultGeneName();	//设置默认的基因选择集合
			void SetDefaultColor();	//初始化页面时，设置列表框行的默认颜色
			void SetDefaultDyeName(vector<CString>& vecDyeName);	//初始化页面时，设置默认的染料名

			void Clear();

			void SetEditableState(BOOL bEditable);
			BOOL GetEditableState();

			void SetTubeInfo(CTubeInfo& info,BOOL bSetDensityEditState);
			void SetDensityEditableState(BOOL bEditable);

			int LstRowHeightAutoFit(int iChanCount);	//打开文件后，根据文件中的通道数，自动设置行高

			void ResetLstTitle();

			int RefreshLanguageDisplay();

	virtual	int SetEditMode(EEXPEDITMODE eExpEditMode);	

public:
	CString m_strTitle;

	CMyNewCb m_cbSamType;
	CMyNewCb m_cbRepeatTubeNumber;
	CMyNewEdit m_edSamId;

	CMyLstCtrl m_lstSamBaseInfo;

private:
	int m_iWidth;	//窗口宽度
	int m_iHeight;	//窗口高度
	int m_iHeaderH;	//标题高度

	int m_roundValue;	//圆角数值
	int m_frameWidth;	//边框宽度

	BOOL m_bEditabe;	//窗口是否可编辑状态

	CInterfaceA* m_pInterfaceA;

	BOOL m_bDensityEditable;	//标识列表框的第五列是否处于可编辑状态

	CRectManage m_rcManage;

	BOOL m_bCtrlCreated;
};

