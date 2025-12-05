
// GeneralPcr.cpp : Defines the class behaviors for the application.

#include "stdafx.h"
#include "GeneralPcr.h"
#include "GeneralPcrDlg.h"

#include "MySamPlateDlg.h"
#include "ConfigureDB.h"
#include "LogManager.h"
#include "InstrumentPCR.h"
#include "ReagentItemManager.h"
#include "PublicInfo.h"
#include "LanguageInfo.h"
#include "UserLoginDlg.h"
#include "StartPageDlg.h"
#include "Ps96IvdDlg.h"
#include "PublicFun.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGeneralPcrApp

BEGIN_MESSAGE_MAP(CGeneralPcrApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CGeneralPcrApp construction

CGeneralPcrApp::CGeneralPcrApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	m_iFileManageDlgH = 44;

#ifdef PS96INSTRUMENT
	m_eSamExportMode = ESEM_VERT;
#else
	m_eSamExportMode = ESEM_HORI;
#endif
	
	m_pLangInfo = NULL;
	m_bInitNormal = TRUE;

	m_gclrSoftTheme = Color(0X06,0X91,0X9D);
	m_gclrSoftThemeEnd = Color(29,151,196);
	m_gclr2LevelTheme = Color(66,166,190);
	m_gclr3LevelTheme = Color(72,174,196);
	m_clrSoftTheme = RGB(0X06,0X91,0X9D);
	m_clr2LevelTheme = RGB(66,166,190);
	m_clr3LevelTheme = RGB(72,174,196);
	m_clrSoftLight = RGB(0XE6,0XF4,0XF5);
	m_clrDarkTx = RGB(0X51,0X60,0XDC);
	
	m_gdiplusToken = 0;
	m_hMutex = NULL;

	m_pcLangId = 2052;
	m_iStdDensityDpd = 8;

	m_iDefChanNum = 4;

	m_ppFCrosstalkPara = NULL;
	m_ppFMeltCrosstalk = NULL;
}

CGeneralPcrApp::~CGeneralPcrApp()
{
}

// The one and only CGeneralPcrApp object

CGeneralPcrApp theApp;

// CGeneralPcrApp initialization

BOOL CGeneralPcrApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	m_pcLangId = GetUserDefaultUILanguage();

	m_pLangInfo = CLanguageInfo::GetInstance();
	if(NULL == m_pLangInfo)
	{
		CString strTip(_T("LanguageInfo.dll初始化失败！"));
		if(2052 != m_pcLangId)
		{
			strTip = _T("Failed to initialize LanguageInfo.dll!");
		}
		MessageBoxExW(NULL,strTip,_T("GeneralPcr"),MB_ICONINFORMATION | MB_OK,m_pcLangId);
		m_bInitNormal = FALSE;
		return FALSE;
	}

	InitLanguageSet();

	AfxEnableControlContainer();

	// 检测是否已经启动一个实例
	CString strMutex(_T(""));
#ifdef PS96INSTRUMENT
	strMutex = _T("GENERAL_PCR_PS96");
#else
	#ifdef N96INSTRUMENT
		strMutex = _T("GENERAL_PCR_N96");
	#else

		#ifdef CAN_YOUNG
			strMutex = _T("GENERAL_PCR_CANYOUNG");
		#else
			#ifdef XINYI_THEME
				strMutex = _T("GENERAL_PCR_M16");
			#else
				strMutex = _T("GENERAL_PCR_N16");
			#endif
		#endif

	#endif
#endif
	m_hMutex = ::CreateMutex(NULL,TRUE,strMutex);
	if(NULL != m_hMutex)
	{
		if(ERROR_ALREADY_EXISTS == GetLastError())
		{
			CString strTip(_T("软件已经打开！"));
			if(GetBeEnglish())
			{
				strTip = _T("The software has been opened!");
			}
		
			MessageBoxExW(NULL,strTip,_T("GeneralPcr"),MB_ICONINFORMATION | MB_OK,m_pcLangId);

			m_bInitNormal = FALSE;
			return FALSE;
		}
	}

	Gdiplus::GdiplusStartupInput  gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken,&gdiplusStartupInput,NULL);

	::SetThreadExecutionState(ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED | ES_CONTINUOUS);  

	InitRatioParam();
	ReadParam();

	InitChanNum();
	InitDefaultDyeName();
	InitFilterParam();
	InitAmpCrossSetValue();
	InitMeltCrossSetValue();
	ReadSystemAnalyseParam();
	ReadDefaultGeneName();

	TCHAR cFileName[255];
	::GetModuleFileName(AfxGetInstanceHandle(),cFileName,255);
	CString strPath;
	strPath.Format(_T("%s"),cFileName);
	int iPos = strPath.ReverseFind('\\');
	strPath = strPath.Left(iPos + 1);

#ifdef _DEBUG
	clock_t ckStart = clock();
#endif
	//初始化语言信息
	CString strLanguageFilePath = strPath + _T("language\\language.xlsx");
	m_pLangInfo->InitLanguageText(strLanguageFilePath);

	ResetTextFont();

#ifdef _DEBUG
	ckStart = clock() - ckStart;
	++ckStart;
#endif

	//初始化断电保护信息
	m_powerOffProtect.InitPowerOffProtect(strPath);

	//初始化模板文件列表信息
	InitTemplateFileInfo();

	CMySamPalteDlg::InitMapSampleTypeToText();
	CMySamPalteDlg::InitMapTextToSampleType();
	CMySamPalteDlg::InitMapSamTypeToHoleClrInfo();
	CMySamPalteDlg::InitMapSamTypeToAbbr();
	InitMapSampleTypeToCbIndex();
	ReadPs96IvdSet();
	ReadCYInfo();

	// 用户和配置数据库实例化
	CConfigureDB* pUserDB = CConfigureDB::GetInstance();
	pUserDB->Open(strPath);
	if (!pUserDB->IsOpen())
	{
		AfxMessageBox(_T("打开数据库失败!"));
		return FALSE;
	}

	// 输入用户和密码
	if(!FindAndInputUser(strPath))
	{
		return FALSE;
	}

	// 系统参数初始化
	CPublicInfo* pPublicInfo = CPublicInfo::GetInstance();
	pPublicInfo->Initialize(strPath,m_curUserInfo.GetName());
	pPublicInfo->SetUserRole(m_curUserInfo.GetPermissionType());

	// 染料目标信息初始化
	BOOL bReturn = pUserDB->SearchDye(&m_listDyeInfo,m_curUserInfo.GetName());
	if (!bReturn)
	{
		AfxMessageBox(_T("读取染料数据库信息失败!"));
		return FALSE;
	}
	if (m_listDyeInfo.GetCount() == 0)
	{
		bReturn = pUserDB->SearchDye(&m_listDyeInfo, _T("SYSTEM"));
		if (!bReturn)
		{
			AfxMessageBox(_T("读取染料数据库信息失败!"));
			return FALSE;
		}
	}

	// 日志数据库实例化
	CLogManager* pLogManager = CLogManager::GetInstance();
	pLogManager->Initialize(strPath);

	// 试剂项目管理器初始化
	CReagentItemManager* pItemManager = CReagentItemManager::GetInstance();
	pItemManager->Initialize(pPublicInfo->GetUserPath(), strPath);

#ifdef USE_STARTPAGE
	//弹出启动页，识别仪器，处理仪器开机自检
	CStartPageDlg startPageDlg;
	startPageDlg.m_strStartPath = strPath;
	startPageDlg.DoModal();
#else 
	//连接仪器判断是否有仪器可连接
	if(!InitilizePCR(strPath))
	{
		return FALSE;
	}
#endif

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	INT_PTR nResponse = 0;
#ifdef PS96INSTRUMENT
	if(GetBeIVDUser())
	{
		CPs96IvdDlg dlg;
		m_pMainWnd = &dlg;
		nResponse = dlg.DoModal();
	}
	else
	{
		CGeneralPcrDlg dlg;
		m_pMainWnd = &dlg;
		nResponse = dlg.DoModal();
	}
#else
	CGeneralPcrDlg dlg;
	m_pMainWnd = &dlg;
	nResponse = dlg.DoModal();
#endif
	
	if(IDOK == nResponse)	
	{
		
	}
	else if(IDCANCEL == nResponse)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	
	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CGeneralPcrApp::ExitInstance()
{
	CLanguageInfo::CleanUp();

	if(!m_bInitNormal)
	{
		return CWinApp::ExitInstance();
	}

	::SetThreadExecutionState(ES_CONTINUOUS);

	CPublicInfo* pPublicInfo = CPublicInfo::GetInstance();
	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	pInstrument->SaveInstanceParaForDemo(pPublicInfo->GetIniFilePath(CPublicInfo::INI_FILE_SYSTEM, FALSE));

	CConfigureDB::DestroyInstance();
	CLogManager::DestroyInstance();
	CInstrumentPCR::CleanUp();
	CPublicInfo::CleanUp();
	CReagentItemManager::DestroyInstance();
	DeleteDyeInfo();

	DeleteSystemAnaParam();

	if(NULL != m_hMutex)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}

	Gdiplus::GdiplusShutdown(m_gdiplusToken);

	WriteCYInfo();

	_CrtDumpMemoryLeaks();

	return CWinApp::ExitInstance();
}

BOOL CGeneralPcrApp::InitilizePCR(CString strStartupPath)
{
	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	BOOL bReturn = pInstrument->Initialize(strStartupPath);
	if (!bReturn) // 没有仪器可连接
	{
		// 提示用户联机错误，是否继续进行？
		while(!bReturn && IDYES == PopQueryTipInfo(m_pLangInfo->GetText(153)))
		{
			::Sleep(1000);
			bReturn = pInstrument->Initialize(strStartupPath);
		}
	}
	else //断电保护处理，20220825，孙文郎
	{
		int iModelCount = pInstrument->GetInstanceCount();
		for (int i=0; i<iModelCount; i++)
		{
			CInstancePCR* pInstance = pInstrument->GetInstancePCR(i);
			if(NULL != pInstance)
			{
				int iLnkIndex = theApp.m_powerOffProtect.SearchPowerOffProtect(pInstance->GetSN());
				if(-1 != iLnkIndex)
				{
					CString strTip(_T(""));
					strTip.Format(m_pLangInfo->GetText(343),pInstance->GetSN());
					if(IDYES == PopQueryTipInfo(strTip))
					{
						theApp.m_lnkLnkIndex.AddTail(iLnkIndex);
					}
					else
					{
						theApp.m_powerOffProtect.DeleteOldProgress(iLnkIndex);
					}
				}
			}
		}
	}

	return TRUE;
}

BOOL CGeneralPcrApp::InitilizePCRStartPage(CString strStartupPath)
{
	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	BOOL bReturn = pInstrument->Initialize(strStartupPath);
	
	return bReturn;
}

void CGeneralPcrApp::ResetTextFont()
{
	if(m_pLangInfo->GetBeEnglish())
	{
		CFontSet::m_stTxFont = TEXTFONT_ENGLISH;
	}
	else
	{
		CFontSet::m_stTxFont = TEXTFONT_CHINESE;
	}

	return;
}

void CGeneralPcrApp::InitChanNum()
{
#ifdef PS96INSTRUMENT
	m_iDefChanNum = 6;
#else
	m_iDefChanNum = 4;
#endif

	m_ppFCrosstalkPara = new float*[m_iDefChanNum];
	for (int t = 0;t < m_iDefChanNum;++t)
	{
		m_ppFCrosstalkPara[t] = new float[m_iDefChanNum];
		for (int x = 0;x < m_iDefChanNum;++x)
		{
			m_ppFCrosstalkPara[t][x] = 0;
		}
	}	

	m_ppFMeltCrosstalk = new float*[m_iDefChanNum];
	for (int t = 0;t < m_iDefChanNum;++t)
	{
		m_ppFMeltCrosstalk[t] = new float[m_iDefChanNum];
		for (int x = 0;x < m_iDefChanNum;++x)
		{
			m_ppFMeltCrosstalk[t][x] = 0;
		}
	}

	tagAnaAmplifyPara anaAmp;
	tagAnaMeltCurvePara anaMelt;
	for(int i = 0;i < m_iDefChanNum;++i)
	{
		anaAmp.iChannelNo = i + 1;
		anaMelt.iChannelNo = i + 1;
		m_vecAmplifyPara.push_back(anaAmp);
		m_vecMeltCurvePara.push_back(anaMelt);
	}

	return;
}

void CGeneralPcrApp::InitDefaultDyeName()
{
	m_vecDefDyeName.clear();

	for(int i = 0;i < m_iDefChanNum;++i)
	{
		m_vecDefDyeName.push_back(_T(""));
	}

	BOOL bPs96 = FALSE;
#ifdef PS96INSTRUMENT
	bPs96 = TRUE;
#endif

	if(bPs96)
	{
		for(int i = 0;i < m_iDefChanNum;++i)
		{
			if(0 == i)
			{
				m_vecDefDyeName[i] = _T("FAM");
			}
			else if(1 == i)
			{
				m_vecDefDyeName[i] = _T("Cy5");
			}
			else if(2 == i)
			{
				m_vecDefDyeName[i] = _T("VIC");
			}
			else if(3 == i)
			{
				m_vecDefDyeName[i] = _T("Cy5.5");
			}
			else if(4 == i)
			{
				m_vecDefDyeName[i] = _T("ROX");
			}
			else if(5 == i)
			{
				m_vecDefDyeName[i] = _T("MOT");
			}
		}
	}
	else
	{
		for(int i = 0;i < m_iDefChanNum;++i)
		{
			if(0 == i)
			{
				m_vecDefDyeName[i] = _T("FAM");
			}
			else if(1 == i)
			{
				m_vecDefDyeName[i] = _T("Cy5");
			}
			else if(2 == i)
			{
				m_vecDefDyeName[i] = _T("VIC");
			}
			else if(3 == i)
			{
				m_vecDefDyeName[i] = _T("ROX");
			}
			else if(4 == i)
			{
				m_vecDefDyeName[i] = _T("Cy5.5");
			}
		}
	}

	for(int i = 0;i < m_iDefChanNum;++i)
	{
		m_vecAmplifyPara[i].strDye = m_vecDefDyeName[i];
		m_vecMeltCurvePara[i].strDye = m_vecDefDyeName[i];
	}

	return;
}

void CGeneralPcrApp::InitFilterParam()
{
	tagFilterItem fltItem;
	fltItem.iFilterUser = 1;
	fltItem.nType = FILTER_MEDIAN;
	fltItem.iTimes = 1;
	fltItem.iPara1 = 5;
	fltItem.iPara2 = 0;
	m_vecFlt.push_back(fltItem);

	fltItem.iFilterUser = 1;
	fltItem.nType = FILTER_FM_BN;
	fltItem.iTimes = 3;
	fltItem.iPara1 = 1;
	fltItem.iPara2 = 3;
	m_vecFlt.push_back(fltItem);

	fltItem.iFilterUser = 0;
	m_vecFlt.push_back(fltItem);
	m_vecFlt.push_back(fltItem);
	m_vecFlt.push_back(fltItem);
	m_vecFlt.push_back(fltItem);

	return;
}

void CGeneralPcrApp::InitAmpCrossSetValue()
{
	if(NULL != m_ppFCrosstalkPara)
	{
#ifdef PS96INSTRUMENT
		m_ppFCrosstalkPara[0][2] = 0.14;
		m_ppFCrosstalkPara[1][3] = 0.15;
		m_ppFCrosstalkPara[2][4] = 0.03;
#else
		m_ppFCrosstalkPara[0][2] = 0.18;
		m_ppFCrosstalkPara[2][0] = 0.01;
#endif
	}

	return;
}

void CGeneralPcrApp::InitMeltCrossSetValue()
{
	if(NULL != m_ppFCrosstalkPara)
	{
#ifdef PS96INSTRUMENT
		m_ppFMeltCrosstalk[0][2] = 0.14;
		m_ppFMeltCrosstalk[1][3] = 0.15;
		m_ppFMeltCrosstalk[2][4] = 0.03;
#else
		m_ppFMeltCrosstalk[0][2] = 0.18;
		m_ppFMeltCrosstalk[2][0] = 0.01;
#endif
	}

	return;
}

int CGeneralPcrApp::DeleteSystemAnaParam()
{
	for(int t = 0;t < m_iDefChanNum;++t)
	{
		delete [] m_ppFCrosstalkPara[t];
	}
	delete [] m_ppFCrosstalkPara;
	m_ppFCrosstalkPara = NULL;

	for(int t = 0;t < m_iDefChanNum;++t)
	{
		delete [] m_ppFMeltCrosstalk[t];
	}
	delete [] m_ppFMeltCrosstalk;
	m_ppFMeltCrosstalk = NULL;

	m_iDefChanNum = 0;

	m_vecDefDyeName.clear();
	m_vecAmplifyPara.clear();
	m_vecMeltCurvePara.clear();

	return 0;
}

int CGeneralPcrApp::AddDefaultGeneName(CString strData,BOOL bSave)
{
	if(strData.IsEmpty())
	{
		return -1;
	}

	Node<CString>* pNode = m_lnkDefGN.GetHead();
	while(NULL != pNode)
	{
		if(pNode->data == strData)
		{
			return 1;
		}
		pNode = pNode->pNext;
	}

	int iIndex = 0;

	pNode = m_lnkDefGN.GetHead();
	if(NULL == pNode)
	{
		m_lnkDefGN.AddTail(strData);
		goto SUCCEDSS_END;
	}

	while(NULL != pNode)
	{
		if(strData < pNode->data)
		{
			m_lnkDefGN.InsertAtPre(iIndex,strData);
			goto SUCCEDSS_END;;
		}
		pNode = pNode->pNext;
		++iIndex;
	}
	m_lnkDefGN.AddTail(strData);

SUCCEDSS_END:
	if(bSave)
	{
		WriteDefaultGeneName();
	}
	return 0;
}

int CGeneralPcrApp::DeleteDefaultGeneName(int iIndex, BOOL bSave)
{
	if(iIndex < 0 || iIndex >= m_lnkDefGN.GetSize())
	{
		return -1;
	}

	m_lnkDefGN.DeleteAt(iIndex);
	if(bSave)
	{
		WriteDefaultGeneName();
	}

	return 0;
}


// 根据用户定义染料和仪器通道初始化可用扫描通道
void CGeneralPcrApp::GetChannelDyeInfo()
{
	/*POSITION pos = theApp.m_listDyeInfo.GetHeadPosition();
	while(NULL != pos)
	{
	CDBDyeInfo* pDyeDB = theApp.m_listDyeInfo.GetNext(pos);

	tagDyeChannelInfo* pDye = new tagDyeChannelInfo;
	pDye->iChannel = pChannel->iExPosition;
	pDye->strTarget = pDyeDB->GetTarget();
	pDye->strDye = pDyeDB->GetDye();
	pDye->strQuencher = pDyeDB->GetQuencher();
	pDye->clrTarget = CPublicFun::ConverToColor(pDyeDB->GetColor());
	pDye->bDefault = pDyeDB->GetDefault();
	}*/

	return;
}

BOOL CGeneralPcrApp::FindAndInputUser(CString strStartupPath)
{
	CString strUserName(_T(""));

	CConfigureDB* pUserDB = CConfigureDB::GetInstance();
	CUserInfoList listUser;
	if(NULL != pUserDB)
	{
		pUserDB->SearchUser(&listUser);
	}

	CUserLoginDlg dlgUserLogin(NULL,&listUser);
	if(IDOK != dlgUserLogin.DoModal())
	{
		POSITION pos = listUser.GetHeadPosition();
		while (pos != NULL)
		{
			CDBUserInfo* pUser = listUser.GetNext(pos);
			SAFE_DELETE(pUser);
		}

		return FALSE;
	}

	m_curUserInfo = dlgUserLogin.m_userInfo;

	//释放内存空间
	POSITION pos = listUser.GetHeadPosition();
	while (pos != NULL)
	{
		CDBUserInfo* pUser = listUser.GetNext(pos);
		SAFE_DELETE(pUser);
	}
	listUser.RemoveAll();

	return TRUE;
}

void CGeneralPcrApp::DeleteDyeInfo()
{
	// 删除染料信息
	POSITION pos = m_listDyeInfo.GetHeadPosition();
	while (pos != NULL)
	{
		CDBDyeInfo* pUser = m_listDyeInfo.GetNext(pos);
		SAFE_DELETE(pUser);
	}
	m_listDyeInfo.RemoveAll();
}

BOOL CGeneralPcrApp::IsTemplateFile(CString strFilePath)
{
	if(strFilePath.IsEmpty())
	{
		return FALSE;
	}

	int iDotIndex = strFilePath.ReverseFind(_T('.'));
	if(iDotIndex < 0)
	{
		return FALSE;
	}

	return PCR_TEMPLATE_FILE_SUFFIX == strFilePath.Mid(iDotIndex + 1);
}


void CGeneralPcrApp::InitRatioParam()
{
	CFontSet::GeneScreenXWidth = GetSystemMetrics(SM_CXSCREEN);
	CFontSet::GeneScreenYHeight = GetSystemMetrics(SM_CYSCREEN);

	SystemParametersInfo(SPI_GETWORKAREA,0,(PVOID)(&CFontSet::GeneCRDlg),0); 
	m_rcWorkArea = CFontSet::GeneCRDlg;
	m_rcScreen = CRect(0,0,CFontSet::GeneScreenXWidth,CFontSet::GeneScreenYHeight);

	CFontSet::m_dHoriRatio = double(CFontSet::GeneScreenXWidth) / CFontSet::m_uiGeneDesWidth;
	CFontSet::m_dVertRatio = double(CFontSet::GeneCRDlg.Height()) / CFontSet::m_uiGeneDesHeight;

	CFontSet::m_dTxRatio = CFontSet::m_dHoriRatio * 0.5 + CFontSet::m_dVertRatio * 0.5;

	return;
}

void CGeneralPcrApp::InitMapSampleTypeToCbIndex()
{
#ifdef N16USENEWSAMPLETYPE
	m_mapSamTypeToCbIndex[SAMPLE_TYPE_MULLTI_DIFFERENT] = -1;
	m_mapSamTypeToCbIndex[SAMPLE_TYPE_NONE] = 5;
	m_mapSamTypeToCbIndex[SAMPLE_TYPE_UNKNOWN] = 0;
	m_mapSamTypeToCbIndex[SAMPLE_TYPE_STANDARD] = 1;
	m_mapSamTypeToCbIndex[SAMPLE_TYPE_POSITIVE] = 2;
	m_mapSamTypeToCbIndex[SAMPLE_TYPE_NEGATIVE] = 3;
	m_mapSamTypeToCbIndex[SAMPLE_TYPE_NTC] = 4;

    #ifdef CAN_YOUNG
	    m_mapSamTypeToCbIndex[SAMPLE_TYPE_IC] = 5;
    #endif

#else
	m_mapSamTypeToCbIndex[SAMPLE_TYPE_MULLTI_DIFFERENT] = -1;
	m_mapSamTypeToCbIndex[SAMPLE_TYPE_NONE] = -1;
	m_mapSamTypeToCbIndex[SAMPLE_TYPE_UNKNOWN] = 0;
	m_mapSamTypeToCbIndex[SAMPLE_TYPE_STANDARD] = 1;
	m_mapSamTypeToCbIndex[SAMPLE_TYPE_NTC] = 2;
	m_mapSamTypeToCbIndex[SAMPLE_TYPE_NEGATIVE] = 3;
	m_mapSamTypeToCbIndex[SAMPLE_TYPE_POSITIVE] = 4;
	m_mapSamTypeToCbIndex[SAMPLE_TYPE_POS_CRITICAL] = 5;
	m_mapSamTypeToCbIndex[SAMPLE_TYPE_CALIBRATOR] = 6;
	m_mapSamTypeToCbIndex[SAMPLE_TYPE_BUFFER] = 7;
#endif
	return;
}

int CGeneralPcrApp::GetSampleTypeCbIndex(eSampleType eType)
{
	map<eSampleType,int>::iterator iter = m_mapSamTypeToCbIndex.find(eType);
	if(m_mapSamTypeToCbIndex.end() == iter)
	{
		return -1;
	}

	return iter->second;
}

int CGeneralPcrApp::ReadParam()
{
	CString strFilePath(GetExeFileDirectory() + _T("temporary.set"));
	
	ifstream ifs(strFilePath,ios::in | ios::binary);
	if(!ifs.is_open())
	{
		return -1;
	}
	ifs.read((char *)&m_eSamExportMode,sizeof(ESAMINFOEXPORTMODE));
	ifs.close();

	//读取LIS信息
	wchar_t buf[1024] = {0};

	strFilePath = GetExeFileDirectory() + _T("System\\LisSet.ini");
	GetPrivateProfileString(_T("LIS"),_T("IpAddr"),_T("127.0.0.1"),buf,sizeof(buf),strFilePath);
	m_lisSetInfo.m_strIpAddress = buf;

	GetPrivateProfileString(_T("LIS"),_T("Port"),_T("21110"),buf,sizeof(buf),strFilePath);
	m_lisSetInfo.m_strPort = buf;

	return 0;
}

int CGeneralPcrApp::WriteParam()
{
	CString strFilePath(GetExeFileDirectory() + _T("temporary.set"));

	ofstream ofs(strFilePath,ios::out | ios::trunc | ios::binary);
	if(!ofs.is_open())
	{
		return -1;
	}

	ofs.write((const char *)&theApp.m_eSamExportMode,sizeof(ESAMINFOEXPORTMODE));

	ofs.close();
	return 0;
}

int CGeneralPcrApp::InitLanguageSet()
{
	CString strFilePath(GetExeFileDirectory() + _T("language.set"));

	eLanguageUI eLang = LANGUAGE_CHS;

	ifstream ifs(strFilePath,ios::in | ios::binary);
	if(ifs.is_open())
	{
		ifs.read((char *)&eLang,sizeof(eLanguageUI));
		ifs.close();

		m_pLangInfo->SetLanguage(eLang);
		return 0;
	}

	//PS96默认英文
#ifdef PS96INSTRUMENT
	SetLanguage(LANGUAGE_ENG);
	return 1;
#endif
	
	//语言设置文件不存在，读取PC的语言环境作为默认语言
	LANGID langId = GetUserDefaultUILanguage();
	if(2052 == langId)
	{
		eLang = LANGUAGE_CHS;
	}
	else
	{
		eLang = LANGUAGE_ENG;
	}

	SetLanguage(eLang);

	return 2;
}

int CGeneralPcrApp::WriteLanguageSet()
{
	CString strFilePath(GetExeFileDirectory() + _T("language.set"));

	ofstream ofs(strFilePath,ios::out | ios::trunc | ios::binary);
	if(!ofs.is_open())
	{
		return -1;
	}

	eLanguageUI eLang = m_pLangInfo->GetLanguage();
	ofs.write((const char *)&eLang,sizeof(eLanguageUI));
	ofs.close();

	return 0;
}

int CGeneralPcrApp::ReadCYInfo()
{
	CString strFilePath(GetExeFileDirectory() + CYINFO_FILENAME);

	ifstream ifs(strFilePath,ios::in | ios::binary);
	if(!ifs.is_open())
	{
		return -1;
	}

	int iFileTag = 0;

	ifs.read((char *)&iFileTag,sizeof(int));
	if(CYINFO_FILETAG != iFileTag)
	{
		ifs.close();
		return -2;
	}

	int iLen = 0;
	TCHAR lpTStr[1024] = {0};

	ifs.read((char*)&iLen,sizeof(int));
	if(iLen > 0)
	{
		ifs.read((char*)lpTStr,sizeof(TCHAR) * iLen);
		lpTStr[iLen] = 0; 
		m_cyInfo.m_strClient = lpTStr;
	}

	iLen = 0;
	ifs.read((char*)&iLen,sizeof(int));
	if(iLen > 0)
	{
		ifs.read((char*)lpTStr,sizeof(TCHAR) * iLen);
		lpTStr[iLen] = 0; 
		m_cyInfo.m_strCpAdr = lpTStr;
	}

	ifs.close();

	return 0;
}

int CGeneralPcrApp::WriteCYInfo()
{
	CString strFilePath(GetExeFileDirectory() + CYINFO_FILENAME);

	ofstream ofs(strFilePath,ios::out | ios::trunc | ios::binary);
	if(!ofs.is_open())
	{
		return -1;
	}

	int iFileTag = CYINFO_FILETAG;

	ofs.write((const char *)&iFileTag,sizeof(int));

	int iLen = m_cyInfo.m_strClient.GetLength();
	ofs.write((const char*)&iLen,sizeof(int));
	if(iLen > 0)
	{
		ofs.write((const char*)m_cyInfo.m_strClient.GetBuffer(),sizeof(TCHAR) * iLen);
	}

	iLen = m_cyInfo.m_strCpAdr.GetLength();
	ofs.write((const char*)&iLen,sizeof(int));
	if(iLen > 0)
	{
		ofs.write((const char*)m_cyInfo.m_strCpAdr.GetBuffer(),sizeof(TCHAR) * iLen);
	}

	ofs.close();

	return 0;
}

int CGeneralPcrApp::ReadPs96IvdSet()
{
	CString strFilePath(GetExeFileDirectory() + _T("ivd.set"));

	ifstream ifs(strFilePath,ios::in | ios::binary);
	if(!ifs.is_open())
	{
		return -1;
	}

	int iVerionId = 1;
	ifs.read((char*)&iVerionId,sizeof(int));

	ifs.read((char*)&m_ps96IvdSet.m_eRstExportOrder,sizeof(ERSTEXPORTORDER));

	int iLen = 0;
	ifs.read((char*)&iLen,sizeof(int));
	if(iLen > 0)
	{
		TCHAR lpTStr[1024] = {0};
		ifs.read((char*)lpTStr,sizeof(TCHAR) * iLen);
		lpTStr[iLen] = 0; 
		m_ps96IvdSet.m_strHospitalName = lpTStr;
	}

	ifs.close();

	return 0;
}

int CGeneralPcrApp::WritePs96IvdSet()
{
	CString strFilePath(GetExeFileDirectory() + _T("ivd.set"));

	ofstream ofs(strFilePath,ios::out | ios::trunc | ios::binary);
	if(!ofs.is_open())
	{
		return -1;
	}

	int iVerionId = 1;
	ofs.write((const char*)&iVerionId,sizeof(int));

	ofs.write((const char*)&m_ps96IvdSet.m_eRstExportOrder,sizeof(ERSTEXPORTORDER));

	int iLen = m_ps96IvdSet.m_strHospitalName.GetLength();
	ofs.write((const char*)&iLen,sizeof(int));
	if(iLen > 0)
	{
		ofs.write((const char*)m_ps96IvdSet.m_strHospitalName.GetBuffer(),sizeof(TCHAR) * iLen);
	}
	
	ofs.close();

	return 0;
}

int CGeneralPcrApp::ReadSystemAnalyseParam()
{
	if(NULL == m_ppFCrosstalkPara)
	{
		return -3;
	}
	if(NULL == m_ppFMeltCrosstalk)
	{
		return -4;
	}

	CString strFilePath(GetExeFileDirectory() + SYSANAPARAMFILENAME);

	ifstream ifs(strFilePath,ios::in | ios::binary);
	if(!ifs.is_open())
	{
		return -1;
	}

	int iFileTag = 0;

	ifs.read((char *)&iFileTag,sizeof(int));
	if(SYSANAPARAMFILETAG != iFileTag)
	{
		ifs.close();
		return -2;
	}

	int iFVer = 1;
	ifs.read((char *)&iFVer,sizeof(int));

	int iTmpInt1 = 0;
	ifs.read((char *)&iTmpInt1,sizeof(int));
	if(iTmpInt1 != m_iDefChanNum)
	{
		ifs.close();
		return -5;
	}
	for(int i = 0;i < iTmpInt1;++i)
	{
		for(int j = 0;j < iTmpInt1;++j)
		{
			ifs.read((char *)&(m_ppFCrosstalkPara[i][j]),sizeof(float));
		}
	}

	ifs.read((char *)&iTmpInt1,sizeof(int));
	if(iTmpInt1 != m_iDefChanNum)
	{
		ifs.close();
		return -6;
	}
	for(int i = 0;i < iTmpInt1;++i)
	{
		for(int j = 0;j < iTmpInt1;++j)
		{
			ifs.read((char *)&(m_ppFMeltCrosstalk[i][j]),sizeof(float));
		}
	}

	m_vecFlt.clear();
	ifs.read((char *)&iTmpInt1,sizeof(int));
	for(int i = 0;i < iTmpInt1;++i)
	{
		tagFilterItem flt;
		ifs.read((char *)&flt.iFilterUser,sizeof(int));
		ifs.read((char *)&flt.nType,sizeof(eFilterType));
		ifs.read((char *)&flt.iTimes,sizeof(int));
		ifs.read((char *)&flt.iPara1,sizeof(int));
		ifs.read((char *)&flt.iPara2,sizeof(int));
		m_vecFlt.push_back(flt);
	}

	int iLen = 0;
	TCHAR lpTStr[1024] = {0};

	ifs.read((char *)&iTmpInt1,sizeof(int));
	if(iTmpInt1 != m_iDefChanNum)
	{
		ifs.close();
		return -7;
	}
	for(int i = 0;i < iTmpInt1;++i)
	{
		ifs.read((char*)&iLen,sizeof(int));
		if(iLen > 0)
		{
			ifs.read((char*)lpTStr,sizeof(TCHAR) * iLen);
			lpTStr[iLen] = 0; 
			m_vecAmplifyPara[i].strTarget = lpTStr;
		}

		ifs.read((char*)&iLen,sizeof(int));
		if(iLen > 0)
		{
			ifs.read((char*)lpTStr,sizeof(TCHAR) * iLen);
			lpTStr[iLen] = 0; 
			m_vecAmplifyPara[i].strTubeFlag = lpTStr;
		}

		ifs.read((char*)&m_vecAmplifyPara[i].iChannelNo,sizeof(int));

		ifs.read((char*)&iLen,sizeof(int));
		if(iLen > 0)
		{
			ifs.read((char*)lpTStr,sizeof(TCHAR) * iLen);
			lpTStr[iLen] = 0; 
			m_vecAmplifyPara[i].strDye = lpTStr;
		}

		ifs.read((char*)&m_vecAmplifyPara[i].nAnaType,sizeof(eAnalysisType));

		ifs.read((char*)&m_vecAmplifyPara[i].bAutoBaseline,sizeof(BOOL));
		ifs.read((char*)&m_vecAmplifyPara[i].iBaselineStart,sizeof(int));
		ifs.read((char*)&m_vecAmplifyPara[i].iBaselineEnd,sizeof(int));

		ifs.read((char*)&m_vecAmplifyPara[i].nMethodCt,sizeof(eMethodCalculateCt));
		ifs.read((char*)&m_vecAmplifyPara[i].nDefaultDataType,sizeof(eAnalysisDataType));
		ifs.read((char*)&m_vecAmplifyPara[i].nThresholdType,sizeof(eSplineThresholdType));

		ifs.read((char*)&m_vecAmplifyPara[i].dCalcNormalizeThreshold,sizeof(double));
		ifs.read((char*)&m_vecAmplifyPara[i].dCalcDeltaRnThreshold,sizeof(double));
		ifs.read((char*)&m_vecAmplifyPara[i].dNormalizeThreshold,sizeof(double));
		ifs.read((char*)&m_vecAmplifyPara[i].dDeltaRnThreshold,sizeof(double));

		ifs.read((char*)&m_vecAmplifyPara[i].bUseFilter,sizeof(BOOL));
	}

	ifs.read((char *)&iTmpInt1,sizeof(int));
	if(iTmpInt1 != m_iDefChanNum)
	{
		ifs.close();
		return -8;
	}
	for(int i = 0;i < iTmpInt1;++i)
	{
		ifs.read((char*)&iLen,sizeof(int));
		if(iLen > 0)
		{
			ifs.read((char*)lpTStr,sizeof(TCHAR) * iLen);
			lpTStr[iLen] = 0; 
			m_vecMeltCurvePara[i].strTarget = lpTStr;
		}

		ifs.read((char*)&iLen,sizeof(int));
		if(iLen > 0)
		{
			ifs.read((char*)lpTStr,sizeof(TCHAR) * iLen);
			lpTStr[iLen] = 0; 
			m_vecMeltCurvePara[i].strTubeFlag = lpTStr;
		}

		ifs.read((char*)&m_vecMeltCurvePara[i].iChannelNo,sizeof(int));

		ifs.read((char*)&iLen,sizeof(int));
		if(iLen > 0)
		{
			ifs.read((char*)lpTStr,sizeof(TCHAR) * iLen);
			lpTStr[iLen] = 0; 
			m_vecMeltCurvePara[i].strDye = lpTStr;
		}

		ifs.read((char*)&m_vecMeltCurvePara[i].dMinTemp,sizeof(double));
		ifs.read((char*)&m_vecMeltCurvePara[i].dMaxTemp,sizeof(double));
		ifs.read((char*)&m_vecMeltCurvePara[i].dThreshold,sizeof(double));
	}
	
	ifs.close();

	return 2;
}

int CGeneralPcrApp::WriteSystemAnalyseParam()
{
	CString strFilePath(GetExeFileDirectory() + SYSANAPARAMFILENAME);

	ofstream ofs(strFilePath,ios::out | ios::trunc | ios::binary);
	if(!ofs.is_open())
	{
		return -1;
	}

	int iFileTag = SYSANAPARAMFILETAG;
	ofs.write((const char *)&iFileTag,sizeof(int));

	int iFVer = 1;
	ofs.write((const char *)&iFVer,sizeof(int));

	int iTmpInt1 = m_iDefChanNum;
	if(NULL == m_ppFCrosstalkPara)
	{
		iTmpInt1 = 0;
	}
	ofs.write((const char *)&iTmpInt1,sizeof(int));
	for(int i = 0;i < iTmpInt1;++i)
	{
		for(int j = 0;j < iTmpInt1;++j)
		{
			ofs.write((const char *)&(m_ppFCrosstalkPara[i][j]),sizeof(float));
		}
	}

	iTmpInt1 = m_iDefChanNum;
	if(NULL == m_ppFMeltCrosstalk)
	{
		iTmpInt1 = 0;
	}
	ofs.write((const char *)&iTmpInt1,sizeof(int));
	for(int i = 0;i < iTmpInt1;++i)
	{
		for(int j = 0;j < iTmpInt1;++j)
		{
			ofs.write((const char *)&(m_ppFMeltCrosstalk[i][j]),sizeof(float));
		}
	}

	iTmpInt1 = m_vecFlt.size();
	ofs.write((const char *)&iTmpInt1,sizeof(int));
	for(int i = 0;i < iTmpInt1;++i)
	{
		ofs.write((const char *)&m_vecFlt[i].iFilterUser,sizeof(int));
		ofs.write((const char *)&m_vecFlt[i].nType,sizeof(eFilterType));
		ofs.write((const char *)&m_vecFlt[i].iTimes,sizeof(int));
		ofs.write((const char *)&m_vecFlt[i].iPara1,sizeof(int));
		ofs.write((const char *)&m_vecFlt[i].iPara2,sizeof(int));
	}

	int iLen = 0;
	iTmpInt1 = m_vecAmplifyPara.size();
	ofs.write((const char *)&iTmpInt1,sizeof(int));
	for(int i = 0;i < iTmpInt1;++i)
	{
		iLen = m_vecAmplifyPara[i].strTarget.GetLength();
		ofs.write((const char*)&iLen,sizeof(int));
		if(iLen > 0)
		{
			ofs.write((const char*)m_vecAmplifyPara[i].strTarget.GetBuffer(),sizeof(TCHAR) * iLen);
		}

		iLen = m_vecAmplifyPara[i].strTubeFlag.GetLength();
		ofs.write((const char*)&iLen,sizeof(int));
		if(iLen > 0)
		{
			ofs.write((const char*)m_vecAmplifyPara[i].strTubeFlag.GetBuffer(),sizeof(TCHAR) * iLen);
		}

		ofs.write((const char *)&m_vecAmplifyPara[i].iChannelNo,sizeof(int));

		iLen = m_vecAmplifyPara[i].strDye.GetLength();
		ofs.write((const char*)&iLen,sizeof(int));
		if(iLen > 0)
		{
			ofs.write((const char*)m_vecAmplifyPara[i].strDye.GetBuffer(),sizeof(TCHAR) * iLen);
		}

		ofs.write((const char *)&m_vecAmplifyPara[i].nAnaType,sizeof(eAnalysisType));

		ofs.write((const char *)&m_vecAmplifyPara[i].bAutoBaseline,sizeof(BOOL));
		ofs.write((const char *)&m_vecAmplifyPara[i].iBaselineStart,sizeof(int));
		ofs.write((const char *)&m_vecAmplifyPara[i].iBaselineEnd,sizeof(int));

		ofs.write((const char *)&m_vecAmplifyPara[i].nMethodCt,sizeof(eMethodCalculateCt));
		ofs.write((const char *)&m_vecAmplifyPara[i].nDefaultDataType,sizeof(eAnalysisDataType));
		ofs.write((const char *)&m_vecAmplifyPara[i].nThresholdType,sizeof(eSplineThresholdType));

		ofs.write((const char *)&m_vecAmplifyPara[i].dCalcNormalizeThreshold,sizeof(double));
		ofs.write((const char *)&m_vecAmplifyPara[i].dCalcDeltaRnThreshold,sizeof(double));
		ofs.write((const char *)&m_vecAmplifyPara[i].dNormalizeThreshold,sizeof(double));
		ofs.write((const char *)&m_vecAmplifyPara[i].dDeltaRnThreshold,sizeof(double));

		ofs.write((const char *)&m_vecAmplifyPara[i].bUseFilter,sizeof(BOOL));
	}

	iTmpInt1 = m_vecMeltCurvePara.size();
	ofs.write((const char *)&iTmpInt1,sizeof(int));
	for(int i = 0;i < iTmpInt1;++i)
	{
		iLen = m_vecMeltCurvePara[i].strTarget.GetLength();
		ofs.write((const char*)&iLen,sizeof(int));
		if(iLen > 0)
		{
			ofs.write((const char*)m_vecMeltCurvePara[i].strTarget.GetBuffer(),sizeof(TCHAR) * iLen);
		}

		iLen = m_vecMeltCurvePara[i].strTubeFlag.GetLength();
		ofs.write((const char*)&iLen,sizeof(int));
		if(iLen > 0)
		{
			ofs.write((const char*)m_vecMeltCurvePara[i].strTubeFlag.GetBuffer(),sizeof(TCHAR) * iLen);
		}

		ofs.write((const char *)&m_vecMeltCurvePara[i].iChannelNo,sizeof(int));

		iLen = m_vecMeltCurvePara[i].strDye.GetLength();
		ofs.write((const char*)&iLen,sizeof(int));
		if(iLen > 0)
		{
			ofs.write((const char*)m_vecMeltCurvePara[i].strDye.GetBuffer(),sizeof(TCHAR) * iLen);
		}

		ofs.write((const char *)&m_vecMeltCurvePara[i].dMinTemp,sizeof(double));
		ofs.write((const char *)&m_vecMeltCurvePara[i].dMaxTemp,sizeof(double));
		ofs.write((const char *)&m_vecMeltCurvePara[i].dThreshold,sizeof(double));
	}

	ofs.close();

	return 0;
}

int CGeneralPcrApp::ReadDefaultGeneName()
{
	m_lnkDefGN.Clear();

	CString strFP(GetExeFileDirectory() + DEFGENE_FN);

	ifstream ifs(strFP,ios::in | ios::binary);
	if(!ifs.is_open())
	{
		return -1;
	}

	int iFileTag = 0;
	ifs.read((char *)&iFileTag,sizeof(int));
	if(DEFGENE_FT != iFileTag)
	{
		ifs.close();
		return -2;
	}

	int iNum = 0;
	ifs.read((char *)&iNum,sizeof(int));

	int iLen = 0;
	TCHAR lpTStr[1024] = {0};
	CString strTemp(_T(""));

	for(int i = 0;i < iNum;++i)
	{
		ifs.read((char*)&iLen,sizeof(int));
		if(iLen > 0)
		{
			ifs.read((char*)lpTStr,sizeof(TCHAR) * iLen);
			lpTStr[iLen] = 0; 
			strTemp = lpTStr;
		}
		else
		{
			strTemp.Empty();
		}
		m_lnkDefGN.AddTail(strTemp);
	}

	ifs.close();

	return 0;
}

int CGeneralPcrApp::WriteDefaultGeneName()
{
	CString strFP(GetExeFileDirectory() + DEFGENE_FN);

	ofstream ofs(strFP,ios::out | ios::trunc | ios::binary);
	if(!ofs.is_open())
	{
		return -1;
	}

	int iFileTag = DEFGENE_FT;
	ofs.write((const char *)&iFileTag,sizeof(int));

	int iNum = m_lnkDefGN.GetSize();
	ofs.write((const char *)&iNum,sizeof(int));

	int iLen = 0;
	Node<CString>* pNode = m_lnkDefGN.GetHead();
	while(NULL != pNode)
	{
		iLen = pNode->data.GetLength();
		ofs.write((const char*)&iLen,sizeof(int));
		if(iLen > 0)
		{
			ofs.write((const char*)pNode->data.GetBuffer(),sizeof(TCHAR) * iLen);
		}

		pNode = pNode->pNext;
	}

	ofs.close();

	return 0;
}

BOOL CGeneralPcrApp::GetBeIVDUser()
{
	return m_curUserInfo.GetBeIVDUser();
}

BOOL CGeneralPcrApp::GetBeEnglish()
{
	if(NULL == m_pLangInfo)
	{
		m_pLangInfo = CLanguageInfo::GetInstance();
		if(NULL == m_pLangInfo)
		{
			return FALSE;
		}
	}

	return LANGUAGE_ENG == m_pLangInfo->GetLanguage();
}

void CGeneralPcrApp::SetLanguage(eLanguageUI eLang,BOOL bSaveToFile /*= TRUE*/)
{
	if(NULL == m_pLangInfo)
	{
		m_pLangInfo = CLanguageInfo::GetInstance();
		if(NULL == m_pLangInfo)
		{
			return;
		}
	}

	m_pLangInfo->SetLanguage(eLang);

	if(bSaveToFile)
	{
		WriteLanguageSet();
	}

	return;
}

BOOL CGeneralPcrApp::GetBeAdminPermission()
{
	return m_curUserInfo.GetBeAdminPermission();
}

BOOL CGeneralPcrApp::GetBeOrdinaryPermission()
{
	return m_curUserInfo.GetBeOrdinaryPermission();
}

int CGeneralPcrApp::InitTemplateFileInfo()
{
	int iRet1 = ReadDefaultTemplateFileInfo();
	int iRet2 = ReadUserOpenedTemplateFileInfo();

	m_lnkTotalTemFileInfo.Clear();
	if(0 != iRet1 && 0 != iRet2)
	{
		return -1;
	}

	CStringArray	arrayOrder;
	ReadTemplateOrder(&arrayOrder);

	int iTempIndex = -1;
	CTemplateFileInfo templateInfo;
	for (int i=0; i<arrayOrder.GetCount(); i++)
	{
		iTempIndex = IsUserOpenTemplateExist(arrayOrder.GetAt(i));
		if (iTempIndex != -1)
		{
			Node<CTemplateFileInfo>* pNodeTemp = m_lnkUserOpenedTemFileInfo.GetAt(iTempIndex);
			templateInfo.m_strFileName = pNodeTemp->data.m_strFileName;
			templateInfo.m_strFilePath = pNodeTemp->data.m_strFilePath;
			templateInfo.m_strTitle = pNodeTemp->data.m_strTitle;
			m_lnkTotalTemFileInfo.AddTail(templateInfo);
			continue;
		}

		iTempIndex = IsDefaultTemplateExist(arrayOrder.GetAt(i));
		if (iTempIndex != -1)
		{
			Node<CTemplateFileInfo>* pNodeTemp = m_lnkDefTemFileInfo.GetAt(iTempIndex);
			templateInfo.m_strFileName = pNodeTemp->data.m_strFileName;
			templateInfo.m_strFilePath = pNodeTemp->data.m_strFilePath;
			templateInfo.m_strTitle = pNodeTemp->data.m_strTitle;
			m_lnkTotalTemFileInfo.AddTail(templateInfo);
			continue;
		}
	}

	Node<CTemplateFileInfo>* pNodeTotal = m_lnkUserOpenedTemFileInfo.GetHead();
	while(NULL != pNodeTotal)
	{
		if (IsTemplateExist(pNodeTotal->data.m_strTitle))
			continue;

		templateInfo.m_strFileName = pNodeTotal->data.m_strFileName;
		templateInfo.m_strFilePath = pNodeTotal->data.m_strFilePath;
		templateInfo.m_strTitle = pNodeTotal->data.m_strTitle;
		m_lnkTotalTemFileInfo.AddTail(templateInfo);

		pNodeTotal = pNodeTotal->pNext;
	}

	pNodeTotal = m_lnkDefTemFileInfo.GetHead();
	while(NULL != pNodeTotal)
	{
		if (!IsTemplateExist(pNodeTotal->data.m_strTitle))
		{
			templateInfo.m_strFileName = pNodeTotal->data.m_strFileName;
			templateInfo.m_strFilePath = pNodeTotal->data.m_strFilePath;
			templateInfo.m_strTitle = pNodeTotal->data.m_strTitle;
			m_lnkTotalTemFileInfo.AddTail(templateInfo);
		}

		pNodeTotal = pNodeTotal->pNext;
	}

	return 0;
}

int CGeneralPcrApp::IsUserOpenTemplateExist(CString strTempalteName)
{
	int iReturn = -1;
	int iIndex = 0;
	Node<CTemplateFileInfo>* pNodeTotal = m_lnkUserOpenedTemFileInfo.GetHead();
	while(NULL != pNodeTotal)
	{
		if(pNodeTotal->data.m_strTitle == strTempalteName)
		{
			iReturn = iIndex;
			break;
		}
		pNodeTotal = pNodeTotal->pNext;
		++iIndex;
	}

	return iReturn;
}

int CGeneralPcrApp::IsDefaultTemplateExist(CString strTempalteName)
{
	int iReturn = -1;
	int iIndex = 0;
	Node<CTemplateFileInfo>* pNodeTotal = m_lnkDefTemFileInfo.GetHead();
	while(NULL != pNodeTotal)
	{
		if(pNodeTotal->data.m_strTitle == strTempalteName)
		{
			iReturn = iIndex;
			break;
		}
		pNodeTotal = pNodeTotal->pNext;
		++iIndex;
	}

	return iReturn;
}


int CGeneralPcrApp::SaveTemplateFileInfo()
{
	int iRet1 = SaveDefaultTemplateFileInfo();
	int iRet2 = SaveUserOpenedTemplateFileInfo();

	if(0 != iRet1 && 0 != iRet2)
	{
		return -1;
	}

	return 0;
}

int CGeneralPcrApp::ReadDefaultTemplateFileInfo()
{
	CString strFilePath(GetExeFileDirectory() + _T("Template\\"));
	if(!PathIsDirectory(strFilePath))
	{
		return -1;
	}
	strFilePath += _T("*.*");

	CFileFind fileFind;
	BOOL bFind = fileFind.FindFile(strFilePath); 
	if(!bFind)
	{
		return -2;
	}

	CString strTemp(_T(""));
	CTemplateFileInfo temFileInfo;

	while(bFind)
	{
		bFind = fileFind.FindNextFile();
		if(fileFind.IsDots() || fileFind.IsDirectory())
		{
			continue;
		}
		else 
		{
			temFileInfo.m_strFilePath = fileFind.GetFilePath(); 
			if(IsTemplateFile(temFileInfo.m_strFilePath))
			{
				temFileInfo.m_strFileName = fileFind.GetFileName();
				temFileInfo.m_strTitle = fileFind.GetFileTitle();
				m_lnkDefTemFileInfo.AddTail(temFileInfo);
			}
		}
	}

	return 0;
}

int CGeneralPcrApp::SaveDefaultTemplateFileInfo()
{
	CString strFilePath(GetExeFileDirectory() + _T("Template\\"));
	strFilePath += DEFAULTTEMPLATEFILENAME;

	ofstream ofs(strFilePath,ios::out | ios::trunc | ios::binary);
	if(!ofs.is_open())
	{
		return -1;
	}

	int iFileTag = DEFAULTTEMPLATEFILTAG;
	ofs.write((const char*)&iFileTag,sizeof(int));

	int iTemNum = m_lnkDefTemFileInfo.GetSize();
	ofs.write((const char*)&iTemNum,sizeof(int));

	int strLen = 0;
	Node<CTemplateFileInfo>* pNode = m_lnkDefTemFileInfo.GetHead();
	while(NULL != pNode)
	{
		strLen = pNode->data.m_strFileName.GetLength();
		ofs.write((const char *)&strLen,sizeof(int));
		if(strLen > 0)
		{
			ofs.write((const char *)(pNode->data.m_strFileName.GetBuffer()),sizeof(TCHAR) * strLen);
		}

		strLen = pNode->data.m_strFilePath.GetLength();
		ofs.write((const char *)&strLen,sizeof(int));
		if(strLen > 0)
		{
			ofs.write((const char *)(pNode->data.m_strFilePath.GetBuffer()),sizeof(TCHAR) * strLen);
		}

		strLen = pNode->data.m_strTitle.GetLength();
		ofs.write((const char *)&strLen,sizeof(int));
		if(strLen > 0)
		{
			ofs.write((const char *)(pNode->data.m_strTitle.GetBuffer()),sizeof(TCHAR) * strLen);
		}

		pNode = pNode->pNext;
	}

	ofs.close();
	return 0;
}

int CGeneralPcrApp::ReadUserOpenedTemplateFileInfo()
{
	CString strFilePath(GetExeFileDirectory() + _T("Template\\"));
	strFilePath += USEROPENEDTEMPLATEFILENAME;

	ifstream ifs(strFilePath,ios::in | ios::binary);
	if(!ifs.is_open())
	{
		return -1;
	}

	int iFileTag = 0;
	ifs.read((char *)&iFileTag,sizeof(int));
	if(USEROPENEDTEMPLATEFILTAG != iFileTag)
	{
		ifs.close();
		return -2;
	}

	m_lnkUserOpenedTemFileInfo.Clear();

	TCHAR lpTStr[1024] = {0};
	int strLen = 0;

	int iTemNum = 0;
	ifs.read((char*)&iTemNum,sizeof(int));

	for(int i = 0;i < iTemNum;++i)
	{
		CTemplateFileInfo temFileInfo;

		ifs.read((char *)&strLen,sizeof(int));
		if(strLen > 0)
		{
			ifs.read((char*)lpTStr,sizeof(TCHAR) * strLen);
			lpTStr[strLen] = 0;
			temFileInfo.m_strFileName = lpTStr;
		}

		ifs.read((char *)&strLen,sizeof(int));
		if(strLen > 0)
		{
			ifs.read((char*)lpTStr,sizeof(TCHAR) * strLen);
			lpTStr[strLen] = 0;
			temFileInfo.m_strFilePath = lpTStr;
		}

		ifs.read((char *)&strLen,sizeof(int));
		if(strLen > 0)
		{
			ifs.read((char*)lpTStr,sizeof(TCHAR) * strLen);
			lpTStr[strLen] = 0;
			temFileInfo.m_strTitle = lpTStr;
		}

		m_lnkUserOpenedTemFileInfo.AddTail(temFileInfo);
	}

	ifs.close();

	return 0;
}

int CGeneralPcrApp::SaveUserOpenedTemplateFileInfo()
{
	CString strFilePath(GetExeFileDirectory() + _T("Template\\"));
	strFilePath += USEROPENEDTEMPLATEFILENAME;

	ofstream ofs(strFilePath,ios::out | ios::trunc | ios::binary);
	if(!ofs.is_open())
	{
		return -1;
	}

	int iFileTag = USEROPENEDTEMPLATEFILTAG;
	ofs.write((const char*)&iFileTag,sizeof(int));

	int iTemNum = m_lnkUserOpenedTemFileInfo.GetSize();
	ofs.write((const char*)&iTemNum,sizeof(int));

	int strLen = 0;
	Node<CTemplateFileInfo>* pNode = m_lnkUserOpenedTemFileInfo.GetHead();
	while(NULL != pNode)
	{
		strLen = pNode->data.m_strFileName.GetLength();
		ofs.write((const char *)&strLen,sizeof(int));
		if(strLen > 0)
		{
			ofs.write((const char *)(pNode->data.m_strFileName.GetBuffer()),sizeof(TCHAR) * strLen);
		}

		strLen = pNode->data.m_strFilePath.GetLength();
		ofs.write((const char *)&strLen,sizeof(int));
		if(strLen > 0)
		{
			ofs.write((const char *)(pNode->data.m_strFilePath.GetBuffer()),sizeof(TCHAR) * strLen);
		}

		strLen = pNode->data.m_strTitle.GetLength();
		ofs.write((const char *)&strLen,sizeof(int));
		if(strLen > 0)
		{
			ofs.write((const char *)(pNode->data.m_strTitle.GetBuffer()),sizeof(TCHAR) * strLen);
		}

		pNode = pNode->pNext;
	}

	ofs.close();
	return 0;
}

int CGeneralPcrApp::IsTemplateExist(CString strTemplateTitle)
{
	Node<CTemplateFileInfo>* pNodeTotal = m_lnkTotalTemFileInfo.GetHead();
	while(NULL != pNodeTotal)
	{
		if(pNodeTotal->data.m_strTitle == strTemplateTitle)
		{
			return TRUE;
		}
		pNodeTotal = pNodeTotal->pNext;
	}

	return FALSE;
}

int CGeneralPcrApp::IsTemplateExist(CString strTemplateTitle,int& iIndex)
{
	iIndex = 0;
	Node<CTemplateFileInfo>* pNodeTotal = m_lnkTotalTemFileInfo.GetHead();
	while(NULL != pNodeTotal)
	{
		if(pNodeTotal->data.m_strTitle == strTemplateTitle)
		{
			return TRUE;
		}
		pNodeTotal = pNodeTotal->pNext;
		++iIndex;
	}

	return FALSE;
}

int CGeneralPcrApp::AddNewTemplate(CTemplateFileInfo& temFileInfo)
{
	if(IsTemplateExist(temFileInfo.m_strTitle))
	{
		return 1;
	}

	m_lnkUserOpenedTemFileInfo.AddTail(temFileInfo);
	if(SaveUserOpenedTemplateFileInfo() < 0)
	{
		m_lnkUserOpenedTemFileInfo.DeleteTail();
		return -1;
	}

	m_lnkTotalTemFileInfo.AddTail(temFileInfo);

	return 0;
}

int CGeneralPcrApp::AddNewTemplate(CTemplateFileInfo& temFileInfo,int& iIndex)
{
	if(IsTemplateExist(temFileInfo.m_strTitle))
	{
		return 1;
	}

	m_lnkUserOpenedTemFileInfo.AddTail(temFileInfo);
	if(SaveUserOpenedTemplateFileInfo() < 0)
	{
		m_lnkUserOpenedTemFileInfo.DeleteTail();
		return -1;
	}

	m_lnkTotalTemFileInfo.AddTail(temFileInfo);
	iIndex = m_lnkTotalTemFileInfo.GetSize() - 1;

	return 0;
}

int CGeneralPcrApp::DeleteTemplate(int iIndex)
{
	if(iIndex < 0)
	{
		return -1;
	}

	int iTotalTemNum = m_lnkTotalTemFileInfo.GetSize();
	if(iIndex >= iTotalTemNum)
	{
		return -2;
	}

	m_lnkTotalTemFileInfo.DeleteAt(iIndex);
	
	if(iIndex < m_lnkDefTemFileInfo.GetSize())
	{
		m_lnkDefTemFileInfo.DeleteAt(iIndex);
	}
	else
	{
		iIndex -= m_lnkDefTemFileInfo.GetSize();
		m_lnkUserOpenedTemFileInfo.DeleteAt(iIndex);
	}

	SaveTemplateFileInfo();

	return 0;
}

// 初始化实验名是否被使用，避免自动命名时同名
BOOL CGeneralPcrApp::HasExpNameExist(CString strExpName)
{
	BOOL bFind = FALSE;
	for (int i=0; i<m_arrayExpInitName.GetCount(); i++)
	{
		if (m_arrayExpInitName.GetAt(i) == strExpName)
		{
			bFind = TRUE;
			break;
		}
	}
	if (!bFind)
	{
		m_arrayExpInitName.Add(strExpName);
	}

	return bFind;
}

// 是否当前保存的文件正在进行实验，避免实验文件保存时覆盖
BOOL CGeneralPcrApp::HasExpSaveFileUsed(CString strExpSavePath)
{
	BOOL bFind = FALSE;
	for (int i=0; i<m_arrayExpSaveFileUsed.GetCount(); i++)
	{
		if (m_arrayExpSaveFileUsed.GetAt(i) == strExpSavePath)
		{
			bFind = TRUE;
			break;
		}
	}
	if (!bFind)
	{
		m_arrayExpSaveFileUsed.Add(strExpSavePath);
	}

	return bFind;
}

void CGeneralPcrApp::DeleteSavePathName(CString strPathName)
{
	for (int i=0; i<m_arrayExpSaveFileUsed.GetCount(); i++)
	{
		if (m_arrayExpSaveFileUsed.GetAt(i) == strPathName)
		{
			m_arrayExpSaveFileUsed.RemoveAt(i);
			break;
		}
	}
}

BOOL CGeneralPcrApp::HasExpSaveFileExist(CString strExpSavePath)
{
	CFileFind fileFind;
	BOOL bFind = fileFind.FindFile(strExpSavePath);
	return bFind;
}

void CGeneralPcrApp::SaveTempalteOrder()
{
	if (m_lnkTotalTemFileInfo.GetSize() < 1)
		return;

	CString strSaveFile(GetExeFileDirectory() + _T("Template\\TemplateOrder.tl"));
	CFile file;       
	CFileException e;
	if (!file.Open(strSaveFile,CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &e))
	{
		if (e.m_cause == CFileException::fileNotFound)
		{
			if (!file.Open(strSaveFile, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone, &e))
			{
				return;
			}
		}
		else
		{
			return;
		} 
	}

	int iTemplateCount = m_lnkTotalTemFileInfo.GetSize();
	file.Write((const char*)&iTemplateCount, sizeof(int));

	Node<CTemplateFileInfo>* pNodeTotal = m_lnkTotalTemFileInfo.GetHead();
	while(NULL != pNodeTotal)
	{
		CPublicFun::WriteStringToFile(&file,pNodeTotal->data.m_strTitle);
		pNodeTotal = pNodeTotal->pNext;
	}

	file.Close();
}

void CGeneralPcrApp::ReadTemplateOrder(CStringArray* pArrayOrder)
{
	pArrayOrder->RemoveAll();

	CString strSaveFile(GetExeFileDirectory() + _T("Template\\TemplateOrder.tl"));

	CFile file;       
	if (!file.Open(strSaveFile, CFile::modeRead | CFile::shareDenyNone))
	{
		return;
	}

	int iCount = 0;
	//读取扩增使用的交叉干扰系数
	file.Read(&iCount, sizeof(int));
	for(int i=0; i<iCount; ++i)
	{
		CString strTemp = CPublicFun::ReadStringFromFile(&file);
		pArrayOrder->Add(strTemp);
	}

	file.Close();
}
