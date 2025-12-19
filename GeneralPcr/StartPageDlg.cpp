#include "StdAfx.h"
#include "StartPageDlg.h"
#include "GeneralPcr.h"
#include "InstrumentPCR.h"
#include "InstancePCR.h"

IMPLEMENT_DYNAMIC(CStartPageDlg,CDialogEx)

#define WM_DISCERNINSTRUMENT_RESULT	WM_USER + 6035	//通知仪器识别结果
																							
CStartPageDlg::CStartPageDlg(CWnd* pParent /*= NULL*/)
	: CDialogEx(CStartPageDlg::IDD, pParent),m_stTipInfo(TRUE)
{
	m_iWidth = 800;
	m_iHeight = 600;
	m_iFrameW = 1;
	m_iRoundAngle = 10;
	m_iHeaderH = 40;
	m_strStartPath.Empty();
	m_strWarningTip.Empty();
}

CStartPageDlg::~CStartPageDlg(void)
{

}

void CStartPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStartPageDlg,CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_MESSAGE(WM_DISCERNINSTRUMENT_RESULT,OnMsgDiscernInstrumentRst)  
END_MESSAGE_MAP()

BOOL CStartPageDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	int iTempInt1 = 150 * m_dHoriRatio;
	int iTempInt2 = 146 * m_dHoriRatio;

	Graphics graphics(memDc.m_hDC);
	ImageAttributes ImgAtt;
	ImgAtt.SetWrapMode(WrapModeTileFlipXY);

	Image image(_T(".\\BkPic\\StartBk.png"));
	Rect rcDest(0,0,rect.Width(),rect.Height());
	graphics.DrawImage(&image,rcDest,0,0,image.GetWidth(),image.GetHeight(),UnitPixel,&ImgAtt);
//
//#ifndef NOLOGO_STARTUP_VERSION
//
//	Image image2(_T(".\\BkPic\\StartPagelogo.png"));
//#ifdef ODREI
//	Rect rcDest2(iTempInt1,122 * m_dVertRatio,175 * m_dHoriRatio,87 * m_dVertRatio);
//#else
//	Rect rcDest2(iTempInt1,122 * m_dVertRatio,image2.GetWidth(),image2.GetHeight());
//#endif
//	graphics.DrawImage(&image2,rcDest2,0,0,image2.GetWidth(),image2.GetHeight(),UnitPixel,&ImgAtt);
//
//	Image image3(_T(".\\BkPic\\Welcome.png"));
//	Rect rcDest3(iTempInt2,413 * m_dVertRatio,image3.GetWidth(),image3.GetHeight());
//	graphics.DrawImage(&image3,rcDest3,0,0,image3.GetWidth(),image3.GetHeight(),UnitPixel,&ImgAtt);
//
//#ifndef XINYI_THEME
//	Image image4(_T(".\\BkPic\\right.png"));
//	Rect rcDest4(iTempInt1,987 * m_dVertRatio,image4.GetWidth(),image4.GetHeight());
//	graphics.DrawImage(&image4,rcDest4,0,0,image4.GetWidth(),image4.GetHeight(),UnitPixel,&ImgAtt);
//	int iTxStartX = iTempInt1 + image4.GetWidth() + 2;
//
//	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
//	memDc.SetBkMode(TRANSPARENT);
//	memDc.SetTextColor(CLR_STARTPAGE_TX);
//	memDc.DrawText(RIGHT_STRING,CRect(iTxStartX,980 * m_dVertRatio,600 * m_dHoriRatio,1010 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
//	memDc.SelectObject(pOldFont);
//#else
//	Image image5(_T(".\\BkPic\\RightInfo.png"));
//	Rect rcDest5(iTempInt1,987 * m_dVertRatio,image5.GetWidth(),image5.GetHeight());
//	graphics.DrawImage(&image5,rcDest5,0,0,image5.GetWidth(),image5.GetHeight(),UnitPixel,&ImgAtt);
//#endif
//
//#endif

	graphics.ReleaseHDC(memDc.m_hDC);

	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();

	return TRUE;
}

void CStartPageDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonDown(nFlags, point);

	return;
}

void CStartPageDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CStartPageDlg::OnMsgDiscernInstrumentRst(WPARAM wParam,LPARAM lParam)
{
	if(MSGTEXTTAG_INT == wParam)
	{
		int iTempInt1 = 0;
		CString strTemp(_T(""));
		CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
		if(0 == lParam)	//未识别到仪器
		{
			strTemp = pLangInfo->GetText(340);
		}
		else if(1 == lParam)	//未识别到仪器
		{
			strTemp = pLangInfo->GetText(331);
		}
		else if(2 == lParam)	//仪器已正常识别
		{
			strTemp = pLangInfo->GetText(332);
			for(int i = 0;i < m_vecInstruSerial.size();++i)
			{
				strTemp += m_vecInstruSerial[i];
				if(i + 1 < m_vecInstruSerial.size())
				{
					strTemp += ",";
				}
			}
			strTemp += pLangInfo->GetText(341);
		}
		else if(3 == lParam)	//运动模块自检中
		{
			m_stTipInfo.GetWindowText(strTemp);
			strTemp += _T("\n");
			strTemp += pLangInfo->GetText(333);
			strTemp += pLangInfo->GetText(334);
			m_stTipInfo.SetWindowText(strTemp);
		}
		else if(4 == lParam)	//运动模块自检失败
		{
			m_stTipInfo.GetWindowText(strTemp);
			iTempInt1 = strTemp.Find(pLangInfo->GetText(334));
			if(-1 != iTempInt1)
			{
				strTemp = strTemp.Left(iTempInt1);
			}
			strTemp += pLangInfo->GetText(335); 
		}
		else if(5 == lParam)	//运动模块自检成功
		{
			m_stTipInfo.GetWindowText(strTemp);
			iTempInt1 = strTemp.Find(pLangInfo->GetText(334));
			if(-1 != iTempInt1)
			{
				strTemp = strTemp.Left(iTempInt1);
			}
			strTemp += pLangInfo->GetText(336); 
		}
		else if(6 == lParam)	//传感器模块自检中
		{
			m_stTipInfo.GetWindowText(strTemp);
			strTemp += _T("\n");
			strTemp += pLangInfo->GetText(337);
			strTemp += pLangInfo->GetText(334);
			m_stTipInfo.SetWindowText(strTemp);
		}
		else if(7 == lParam)	//传感器模块自检失败
		{
			m_stTipInfo.GetWindowText(strTemp);
			iTempInt1 = strTemp.Find(pLangInfo->GetText(334));
			if(-1 != iTempInt1)
			{
				strTemp = strTemp.Left(iTempInt1);
			}
			strTemp += pLangInfo->GetText(335); 
		}
		else if(8 == lParam)	//传感器模块自检成功
		{
			m_stTipInfo.GetWindowText(strTemp);
			iTempInt1 = strTemp.Find(pLangInfo->GetText(334));
			if(-1 != iTempInt1)
			{
				strTemp = strTemp.Left(iTempInt1);
			}
			strTemp += pLangInfo->GetText(336); 
		}
		else if(9 == lParam)	//加热模块自检中
		{
			m_stTipInfo.GetWindowText(strTemp);
			strTemp += _T("\n");
			strTemp += pLangInfo->GetText(338);
			strTemp += pLangInfo->GetText(334);
			m_stTipInfo.SetWindowText(strTemp);
		}
		else if(10 == lParam)	//加热模块自检失败
		{
			m_stTipInfo.GetWindowText(strTemp);
			iTempInt1 = strTemp.Find(pLangInfo->GetText(334));
			if(-1 != iTempInt1)
			{
				strTemp = strTemp.Left(iTempInt1);
			}
			strTemp += pLangInfo->GetText(335); 
		}
		else if(11 == lParam)	//加热模块自检成功
		{
			m_stTipInfo.GetWindowText(strTemp);
			iTempInt1 = strTemp.Find(pLangInfo->GetText(334));
			if(-1 != iTempInt1)
			{
				strTemp = strTemp.Left(iTempInt1);
			}
			strTemp += pLangInfo->GetText(336); 
		}
		else if(12 == lParam)	//光学模块自检中
		{
			m_stTipInfo.GetWindowText(strTemp);
			strTemp += _T("\n");
			strTemp += pLangInfo->GetText(339);
			strTemp += pLangInfo->GetText(334);
			m_stTipInfo.SetWindowText(strTemp);
		}
		else if(13 == lParam)	//光学模块自检失败
		{
			m_stTipInfo.GetWindowText(strTemp);
			iTempInt1 = strTemp.Find(pLangInfo->GetText(334));
			if(-1 != iTempInt1)
			{
				strTemp = strTemp.Left(iTempInt1);
			}
			strTemp += pLangInfo->GetText(335); 
		}
		else if(14 == lParam)	//光学模块自检成功
		{
			m_stTipInfo.GetWindowText(strTemp);
			iTempInt1 = strTemp.Find(pLangInfo->GetText(334));
			if(-1 != iTempInt1)
			{
				strTemp = strTemp.Left(iTempInt1);
			}
			strTemp += pLangInfo->GetText(336); 
		}
		m_stTipInfo.SetWindowText(strTemp);
	}
	else 
	{
		if(0 == wParam)
		{
			if(0 == lParam)	//自检成功
			{

			}
			else if(1 == lParam)	//自检成功，有警告信息
			{
				PopWarningTipInfo(m_strWarningTip);
			}
		}
		else if(1 == wParam)	//自检失败（通信失败）
		{
			PopWarningTipInfo(theApp.m_pLangInfo->GetText(342));
		}
		else if(2 == wParam)	//不自检（仪器未连接或者用户选择不自检，可通过lParam判断）
		{

		}
		else if(3 == wParam)	//仪器屏蔽自检
		{

		}
		else if(4 == wParam)	//运行上一个未完成的实验（断电保护）
		{
			int iLnkIndex = lParam;
			theApp.m_lnkLnkIndex.AddTail(iLnkIndex);
		}
		else if(5 == wParam)	//不自检（仪器未连接或者用户选择不自检，可通过lParam判断）
		{
			m_gifHanding.Stop();
			PopWarningTipInfo(theApp.m_pLangInfo->GetText(436));
		}

		if(5 != wParam)
		{
			m_gifHanding.Stop();
		}
		
		OnOK();
	}

	return 0;
}

BOOL CStartPageDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

	SetWindowPos(NULL,theApp.m_rcWorkArea.left,theApp.m_rcWorkArea.top,theApp.m_rcWorkArea.Width(),theApp.m_rcWorkArea.Height(),SWP_NOZORDER);
	m_iWidth = theApp.m_rcWorkArea.Width();
	m_iHeight = theApp.m_rcWorkArea.Height();

	InitDlg();

	CreateCtrl();
	InitCtrl();
	SetCtrlPos();
	ShowCtrl();

	AfxBeginThread(DiscernInstrumentTd,this);

	return TRUE;
}

void CStartPageDlg::InitDlg()
{
	SetMyFont(m_stTxFont,30 * m_dTxRatio);
	return;
}

void CStartPageDlg::CreateCtrl()
{
	CreateBt();
	CreateSt();
	CreateCb();
	CreateEd();
	CreateLst();
}

void CStartPageDlg::CreateBt()
{

}

void CStartPageDlg::CreateSt()
{
	m_stTipInfo.Create(_T(""),WS_CHILD,CRect(0,0,0,0),this,ID_STARTPAGEDLG_ST_TIPINFO);
	m_gifHanding.Create(_T(""),WS_CHILD,CRect(0,0,0,0),this,ID_STARTPAGEDLG_GIF_HANDING);
}

void CStartPageDlg::CreateCb()
{

}

void CStartPageDlg::CreateEd()
{

}

void CStartPageDlg::CreateLst()
{

}

void CStartPageDlg::InitCtrl()
{
	InitBt();
	InitSt();
	InitCb();
	InitEd();
	InitLst();
}

void CStartPageDlg::InitBt()
{

}

void CStartPageDlg::InitSt()
{
	int iFontH = 36 * m_dTxRatio;
	
	m_stTipInfo.SetTextColor(CLR_STARTPAGE_TX);
	m_stTipInfo.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_stTipInfo.SetWordStyle(DT_LEFT | DT_END_ELLIPSIS);
	m_stTipInfo.SetSingleLineState(FALSE);

	m_gifHanding.SetGifFilePath(_T(".\\BkPic\\Loading.gif"));
}

void CStartPageDlg::InitCb()
{

}

void CStartPageDlg::InitEd()
{

}

void CStartPageDlg::InitLst()
{

}

void CStartPageDlg::SetCtrlPos()
{
	m_stTipInfo.SetWindowPos(this,159 * m_dHoriRatio,378 * m_dVertRatio,420 * m_dHoriRatio,200 * m_dVertRatio,SWP_NOZORDER);
	m_gifHanding.SetWindowPos(this,429 * m_dHoriRatio,719 * m_dVertRatio,80 * m_dHoriRatio,80 * m_dHoriRatio,SWP_NOZORDER);
}

void CStartPageDlg::ShowCtrl()
{
	m_stTipInfo.ShowWindow(SW_SHOW);
	m_gifHanding.ShowWindow(SW_SHOW);
	m_gifHanding.Start();
}

UINT CStartPageDlg::DiscernInstrumentTd(LPVOID pParam)
{
	int iErrCode = 0,iTempInt1 = 0,iRet1 = 0,iRet2 = 0,iRet3 = 0,iRet4 = 0;
	
	CStartPageDlg* pStartPageDlg = (CStartPageDlg*)pParam;
	pStartPageDlg->PostMessageW(WM_DISCERNINSTRUMENT_RESULT,MSGTEXTTAG_INT,0);
	//Sleep(1000);

	BOOL bConnected = theApp.InitilizePCRStartPage(pStartPageDlg->m_strStartPath);
	if(!bConnected)
	{
		pStartPageDlg->PostMessageW(WM_DISCERNINSTRUMENT_RESULT,MSGTEXTTAG_INT,1);
		iErrCode = 1;
		goto INSTRUNOTCONNECTED;
	}

	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	
	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	if(NULL == pInstrument)
	{
		iErrCode = 2;
		goto NOTSELFCHECK;
	}
	CInstancePCR* pInstance = pInstrument->GetAvailableInstance();
	if(NULL == pInstance)
	{
		iErrCode = 3;
		goto NOTSELFCHECK;
	}
	else
	{
		pStartPageDlg->m_vecInstruSerial.push_back(pInstance->GetSN());
		pStartPageDlg->PostMessageW(WM_DISCERNINSTRUMENT_RESULT,MSGTEXTTAG_INT,2);
	}

	int iLnkIndex = theApp.m_powerOffProtect.SearchPowerOffProtect(pInstance->GetSN());
	if(-1 != iLnkIndex)
	{
		//goto CONTINUE_OLD_EXP;
		CString strTip(_T(""));
		strTip.Format(pLangInfo->GetText(343),pInstance->GetSN());
		if(IDYES == PopQueryTipInfo(strTip))
		{
			//用户选择继续运行上一次实验，不再处理开机自检相关信息
			goto CONTINUE_OLD_EXP;
		}
		else
		{
			theApp.m_powerOffProtect.DeleteOldProgress(iLnkIndex);
		}
	}

	//N96才自检
#ifndef N96INSTRUMENT
	goto SHIELD_SELFCHECK;
#endif

	if(IDYES != PopQueryTipInfo(pLangInfo->GetText(330)))
	{
		iErrCode = 4;
		goto NOTSELFCHECK;
	}

	pStartPageDlg->PostMessageW(WM_DISCERNINSTRUMENT_RESULT,MSGTEXTTAG_INT,3);
	iRet1 = pInstance->SportModuleCheck();
	if(iRet1 < 0)
	{
		iTempInt1 = 4;
	}
	else 
	{
		iTempInt1 = 5;
	}
	pStartPageDlg->PostMessageW(WM_DISCERNINSTRUMENT_RESULT,MSGTEXTTAG_INT,iTempInt1);
	if(4 == iTempInt1)
	{
		goto COMMU_FAILED;
	}

	pStartPageDlg->PostMessageW(WM_DISCERNINSTRUMENT_RESULT,MSGTEXTTAG_INT,6);
	iRet2 = pInstance->SensorModuleCheck();
	if(iRet2 < 0)
	{
		iTempInt1 = 7;
	}
	else
	{
		iTempInt1 = 8;
	}
	pStartPageDlg->PostMessageW(WM_DISCERNINSTRUMENT_RESULT,MSGTEXTTAG_INT,iTempInt1);
	if(7 == iTempInt1)
	{
		goto COMMU_FAILED;
	}

	pStartPageDlg->PostMessageW(WM_DISCERNINSTRUMENT_RESULT,MSGTEXTTAG_INT,9);
	iRet3 = pInstance->HeatModeduleCheck();
	if(iRet3 < 0)
	{
		iTempInt1 = 10;
	}
	else
	{
		iTempInt1 = 11;
	}
	pStartPageDlg->PostMessageW(WM_DISCERNINSTRUMENT_RESULT,MSGTEXTTAG_INT,iTempInt1);
	if(10 == iTempInt1)
	{
		goto COMMU_FAILED;
	}

	pStartPageDlg->PostMessageW(WM_DISCERNINSTRUMENT_RESULT,MSGTEXTTAG_INT,12);
	iRet4 = pInstance->OpticsModuleCheck();
	if(iRet4 < 0)
	{
		iTempInt1 = 13;
	}
	else
	{
		iTempInt1 = 14;
	}
	pStartPageDlg->PostMessageW(WM_DISCERNINSTRUMENT_RESULT,MSGTEXTTAG_INT,iTempInt1);
	if(13 == iTempInt1)
	{
		goto COMMU_FAILED;
	}

	//自检结束，通信正常，如果有警告，则弹窗提示
	pStartPageDlg->m_strWarningTip = pInstance->GetCheckResultString();
	if(pStartPageDlg->m_strWarningTip.IsEmpty())
	{
		Sleep(1200);
		pStartPageDlg->PostMessageW(WM_DISCERNINSTRUMENT_RESULT,0,0);
	}
	else
	{
		pStartPageDlg->PostMessageW(WM_DISCERNINSTRUMENT_RESULT,0,1);
	}
	
	return 0;

COMMU_FAILED:
	//Sleep(16000);
	pStartPageDlg->PostMessageW(WM_DISCERNINSTRUMENT_RESULT,1,0);
	return iErrCode;

INSTRUNOTCONNECTED:
	Sleep(600);
	pStartPageDlg->PostMessageW(WM_DISCERNINSTRUMENT_RESULT,5,iErrCode);
	return iErrCode;

NOTSELFCHECK:
	if(4 != iErrCode)
	{
		//Sleep(16000);
	}
	pStartPageDlg->PostMessageW(WM_DISCERNINSTRUMENT_RESULT,2,iErrCode);
	return iErrCode;

SHIELD_SELFCHECK:
	pStartPageDlg->PostMessageW(WM_DISCERNINSTRUMENT_RESULT,3,0);
	return 0;

CONTINUE_OLD_EXP:
	pStartPageDlg->PostMessageW(WM_DISCERNINSTRUMENT_RESULT,4,iLnkIndex);
	return 0;
}

