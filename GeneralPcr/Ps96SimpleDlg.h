#pragma once

#include "FontSet.h"
#include "MiddleDlg.h"
#include "AssaySelectDlg.h"
#include "LoadSamDlg.h"
#include "MyStatic.h"

typedef enum PS96Simple_Son_Page_Type
{
	EPS96SSPT_NONE,EPS96SSPT_ASSAYSELECT,EPS96SSPT_LOADSAM
}EPS96SIMPLE_SON_PAGE_TYPE;

class CPs96SimpleDlg: public CMiddleDlg
{
public:
	CPs96SimpleDlg(void);
	~CPs96SimpleDlg(void);

	DECLARE_MESSAGE_MAP()

public:
	virtual afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnBnClickedRunStop();
	afx_msg void OnBnClickedSet();
	afx_msg void OnBnClickedSwtAssaySelect();
	afx_msg void OnBnClickedSwtSamInfo();
		
	virtual BOOL OnInitDialog();

			void InitDlg();
	virtual	void InitSonDlg();

	virtual	void CreateCtrl();
	virtual	void CreateBt();
	virtual	void CreateSt();
	virtual void InitCtrl();
	virtual void InitBt();
			void InitOneBt(CMyButton* pBt);
	virtual void InitSt();
	virtual void InitCb();
	virtual	void SetCtrlPos();
	virtual	void ShowCtrl();

	virtual	void SetCtrlText();

	virtual void SetExpDataPtr(CExperimentData* pExpData);
	virtual void EmbodyBaseSetInfo();

	virtual int RefreshExpDlgRunState(BOOL bOccupy);

	virtual	int Run();	//运行
	virtual	int RunPowerOffProtectExperiment();	//运行断电保护实验
	virtual	int Stop();	//停止

	afx_msg LRESULT OnMsgExperFluChanged(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgExperRun(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgExperFinished(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgSegmentOver(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgMeltOneTemOver(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgExperTempChanged(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgExperStepChanged(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgRunPowerOffProtect(WPARAM wParam,LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

			void SetPageSelected(EPS96SIMPLE_SON_PAGE_TYPE eSonPageType);

			void RefreshExpNameAndSavePath();

	virtual	void RefreshExpEmbody();
			void ClearExpEmbody();

	virtual void SetTemplateListSelectedRow(int iIndex);
	virtual int NewTemplateAdded(int iIndex);
			
			int DeleteTemplate(int iIndex);
	virtual	void DeleteTemplateListRow(int iIndex);
	virtual int SetEditMode(EEXPEDITMODE eExpEditMode);

			int ImportId();

	virtual int ExportResult();
	virtual void SetConnectState(BOOL bRunExp);
	afx_msg void OnBnClickedFindInstrument();
	afx_msg LRESULT OnMsgTellParentSetNewText(WPARAM wParam, LPARAM lParam);
	virtual void ReshowInstrumentList();
	virtual void SetSelectedInstrument(CString strModel, CString strSN);

	virtual void InitExperimentData(CExperimentData* pExpData);

protected:
	void ExportPS96ResultSheetFormat(libxl::Book* pBook, libxl::Sheet* pSheetId);
	void ExportPS96ResultSheetRectBorder(libxl::Book* pBook, libxl::Sheet* pSheetId, int iStartRow, int iStartCol, int iEndRow, int iEndCol);
	void ExportPS96ResultWriteStrBold(libxl::Book* pBook, libxl::Sheet* pSheetId, int iRow, int iCol, CString strText, int iFontSize=-1);
	void ExportPS96ResultWriteStrUnderline(libxl::Book* pBook, libxl::Sheet* pSheetId, int iRow, int iCol, CString strText);

	int DataProcess(BOOL bNewOver,int iAmlFlag = 5);

	void InitInstrument();
	CString CombineModelAndSN(CString strModel,CString strSN);
		
public:
	CMyStatic m_stRemainTm;
	CMyStatic m_stAssayName;
	CMyStatic m_stFileSaveFolder;

	CMyNewCb m_cbInstrumentType;
	CMyButton m_btnFindInstrument; // 查找设备

	CMyButton m_btRunStop;
	CMyButton m_btSet;
	CMyButton m_swtBtAssaySel;
	CMyButton m_swtBtSamInfo;

	CAssaySelDlg m_assaySelDlg;
	CLoadSamDlg m_loadSamDlg;

protected:
	EPS96SIMPLE_SON_PAGE_TYPE m_eCurSelPage;

};

