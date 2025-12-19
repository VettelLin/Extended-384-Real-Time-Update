#include "StdAfx.h"
#include "ExpRunCtrl.h"
#include "LanguageInfo.h"
#include "PopTipInfo.h"
#include "CommInterruptTipDlg.h"

// ʵ�����������߳�
UINT _RunThread(LPVOID lpParam)
{
	CExpRunCtrl* pRunCtrl = (CExpRunCtrl*) lpParam;
	pRunCtrl->RunProc();
	return 	0;
}
 
CExpRunCtrl::CExpRunCtrl(CInstancePCR* pInstance, CWnd* pMsgWnd)
{
	m_pInstancePCR = pInstance;
	m_pMsgWnd = pMsgWnd;
	m_pRunThread = NULL;
	m_pTempThread = NULL;
	m_pStatusThread = NULL;
	m_lScanStartTime = -1;

	m_nRunStatus = STATUS_STOP;
	m_bAskStop = FALSE;
	m_strErrorInfo.Empty();

	m_pExpData = NULL;
	m_pRunPara = NULL;

	m_iNotReachTempCount = 0;
	m_bNotReachMeltStartTemp = FALSE;
	m_bFirstAmplifyCycle = FALSE;

	m_dMeltTemBeforeScan = 0;
	m_dMeltTemAfterScan = 0; 

	m_bPowerOffExp = FALSE;
	m_iStartSeg = -1;
	m_iStartStep = -1;
	m_iStartCyle = -1;

	m_bNoStepBeforeMeltStep = FALSE;
	m_fMeltStepStartTem = 60;

	m_bNewSegmentCycle = FALSE;
	m_bFirstTempErr = TRUE;
	m_pLanguage = NULL;
}

CExpRunCtrl::~CExpRunCtrl(void)
{
	if(NULL != m_pRunPara)
	{
		delete m_pRunPara;
		m_pRunPara = NULL;
	}

	if(NULL != m_pTempThread)
	{
		delete m_pTempThread;
		m_pTempThread = NULL;
	}

	if(NULL != m_pStatusThread)
	{
		delete m_pStatusThread;
		m_pStatusThread = NULL;
	}
}

BOOL CExpRunCtrl::Run(CExperimentData* pExpData, CExpRunPara* pRunPara)
{
	ASSERT(m_pInstancePCR != NULL);
	ASSERT(m_pMsgWnd != NULL);

	m_pExpData = pExpData;

	if(NULL != m_pRunPara)
	{
		delete m_pRunPara;
		m_pRunPara = NULL;
	}
	m_pRunPara = pRunPara;

	ASSERT(NULL != m_pExpData);
	ASSERT(NULL != m_pRunPara);

	m_pRunPara->SetMeltStepRunningState(FALSE);
	m_pMsgWnd->PostMessage(WM_EXPERIMENT_RUN,0,0);

	// �����豸��ʼ��
	BOOL bReturn = Initialize();

	if (bReturn)
	{
		// �����߳�,���п��ƿ�ʼ
		m_pRunThread = AfxBeginThread(_RunThread, (LPVOID)this, THREAD_PRIORITY_ABOVE_NORMAL/*THREAD_PRIORITY_NORMAL*/);
	}
	return bReturn;
}

// ���̳߳�ʼ��
BOOL CExpRunCtrl::Initialize()
{
	BOOL bReturn = TRUE;

	ASSERT(NULL != m_pInstancePCR);

	if(NULL != m_pTempThread)
	{
		delete m_pTempThread;
		m_pTempThread = NULL;
	}
	if(NULL != m_pStatusThread)
	{
		delete m_pStatusThread;
		m_pStatusThread = NULL;
	}
	m_arrayInfo.RemoveAll();
	m_bFirstTempErr = TRUE;

	// ��ȡ�¶��̳߳�ʼ��
	m_pTempThread = (CExpTempDataThread*)AfxBeginThread(RUNTIME_CLASS(CExpTempDataThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_pTempThread->m_bAutoDelete = FALSE;
	m_pTempThread->SetInstance(m_pInstancePCR);
	m_pTempThread->SetExperimentData(m_pExpData);
	m_pTempThread->SetMsgWnd(m_pMsgWnd);
	m_pTempThread->SetExpRunPara(m_pRunPara);
	m_pTempThread->SetStartTime(m_pExpData->GetCurTempCollectTime());

	// �豸״̬����߳�
	m_pStatusThread = (CExpDeviceStatusThread*)AfxBeginThread(RUNTIME_CLASS(CExpDeviceStatusThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_pStatusThread->m_bAutoDelete = FALSE;
	m_pStatusThread->SetInstance(m_pInstancePCR);
	m_pStatusThread->SetExpRunPara(m_pRunPara);

	return bReturn;
}

// ���̵߳��ó���
void CExpRunCtrl::RunProc()
{
	m_nRunStatus = STATUS_RUNNING;
	m_bAskStop = FALSE;

	// ��¼ʵ�鵱ǰ״̬��Ϣ
	m_pRunPara->ExpRun();

	//CExperimentData���¶����ݳ�Ա�����Ƿ�ʹ���ȸ�
	m_pExpData->SetLidHeater(m_pExpData->GetExperimentInfo()->GetLidHeader());

	m_pInstancePCR->SetInstanceStatus(INSTANCE_USING);

	// �ָ���ȡ�¶��̣߳���ʼȡ�¶�����
	m_pTempThread->ResumeThread();
	// �ָ���ȡ�豸״̬����߳�
	m_pStatusThread->ResumeThread();

	BOOL bSuccess = PrepareRunning();
	if (!bSuccess) 
	{
		m_pTempThread->SetRunStatus(STATUS_STOP);
		m_pStatusThread->SetRunStatus(STATUS_STOP);
		return;
	}

	// �ȸ�Ԥ����
	bSuccess = RunLidHeader();
	if(!bSuccess)
	{
		m_pTempThread->SetRunStatus(STATUS_STOP);
		m_pStatusThread->SetRunStatus(STATUS_STOP);
		return;
	}

	// ����ʵ��ѭ����
	int iCurSeg = m_pRunPara->GetCurSegment();
	int iCurCycle = m_pRunPara->GetCurCycle();
	int iCurStep = m_pRunPara->GetCurStep();
	float fPreSegemntTemp = m_pInstancePCR->GetSysTemp(CInstancePCR::SYS_TEMP_CTRL_LOWER); // ǰ�沽���¶�
	float fCurStepTemp = m_pRunPara->GetCurTemp();

	CLinkList<CSegItemInfo>& lnkSegItem = m_pExpData->GetSegment();
	int nSegmentCount = lnkSegItem.GetSize();

	if(m_bAskStop)
	{
		goto ASKSTOP;
	}

	//���������жϵ籣��ʵ���ļ���
	if(m_bPowerOffExp)
	{
		iCurSeg = m_iStartSeg;
		iCurStep = m_iStartStep;
		iCurCycle = m_iStartCyle;

		AddInterruptLog(m_iStartSeg+1, m_iStartStep+1, m_iStartCyle+1);

		if(iCurSeg >= 0 && iCurSeg < nSegmentCount)
		{
			//�����������жϵ�ʱ������������иó��򣬴��������ڳ���ε�һ�����迪ʼ����
			CSegItemInfo* pSetItem = lnkSegItem[iCurSeg];
			if(pSetItem->GetBeMeltSeg() && pSetItem->m_lnkStepItemInfo.GetSize() > 0)
			{
				if(pSetItem->m_lnkStepItemInfo[0]->GetBeCollectAll())
				{
					m_bNoStepBeforeMeltStep = TRUE;
					m_fMeltStepStartTem = 60;
					if(iCurSeg > 0)
					{
						m_fMeltStepStartTem = lnkSegItem[iCurSeg - 1]->m_lnkStepItemInfo.GetTail()->data.m_fAimTem;
					}
				}

				iCurStep = 0;
				iCurCycle = 0;
			}
		}

		ResetPowerOffExpInfo();
	}

	// ����LED����
	bSuccess = SetLaserStatus();
	if(!bSuccess)
	{
		m_pTempThread->SetRunStatus(STATUS_STOP);
		m_pStatusThread->SetRunStatus(STATUS_STOP);
		return;
	}

	// ����ϵͳ�ļ����ò����ж��Ƿ�ִ��ɨ�豳���ź�ֵ����
	bSuccess = ChannelSignalCorrection();
	if(!bSuccess)
	{
		m_pTempThread->SetRunStatus(STATUS_STOP);
		m_pStatusThread->SetRunStatus(STATUS_STOP);
		return;
	}

	m_pExpData->SetExpRunning(TRUE);
	for(;iCurSeg < nSegmentCount;++iCurSeg)
	{
		CSegItemInfo segItem = lnkSegItem.GetAt(iCurSeg)->data;
		m_pRunPara->SetCurSegment(iCurSeg);

		if(segItem.GetBeMeltSeg()) // �۽��
		{
			m_pRunPara->SetCurCycle(0);

			// ��������δ�ͷ��ʼ
			for(iCurStep = 0;iCurStep < segItem.m_lnkStepItemInfo.GetSize();++iCurStep)
			{
				HandleDeviceStatus();

				CStepItemInfo stepItem = segItem.m_lnkStepItemInfo.GetAt(iCurStep)->data;
				m_pRunPara->SetCurStep(iCurStep);
				m_pMsgWnd->SendMessage(WN_EXPERIMENT_STEP_CHANGED);

				switch(stepItem.m_eChkType)
				{
				case CHECK_NONE:
					bSuccess = RunNoneStep(&stepItem,fCurStepTemp);
					break;
				case CHECK_ALL_POINT:
					bSuccess = RunMeltStep(&stepItem,fCurStepTemp,m_pExpData->GetPreStepDelay(iCurSeg,iCurStep));
					break;
				default:
					ASSERT(FALSE);
					break;
				}

				// ��������״̬
				if(!bSuccess)
				{
					break;
				}

				if(m_bAskStop)
				{
					goto ASKSTOP;
				}

				if(STATUS_RUNNING != m_nRunStatus)
				{
					break;
				}

				fCurStepTemp = stepItem.m_fAimTem;
			}
			iCurStep = 0;
		}
		else // �����λ�ά�ֶ�
		{
			m_bNewSegmentCycle = TRUE;

			for (;iCurCycle < segItem.m_iCycleNum;++iCurCycle)
			{	
				m_pRunPara->SetCurCycle(iCurCycle);

				for(;iCurStep < segItem.m_lnkStepItemInfo.GetSize();++iCurStep)
				{
					HandleDeviceStatus();

					CStepItemInfo stepItem = segItem.m_lnkStepItemInfo.GetAt(iCurStep)->data;
					m_pRunPara->SetCurStep(iCurStep);
					m_pMsgWnd->SendMessage(WN_EXPERIMENT_STEP_CHANGED);
					
					switch(stepItem.m_eChkType)
					{
					case CHECK_NONE:
						bSuccess = RunNoneStep(&stepItem, fCurStepTemp);
						break;
					case CHECK_END_POINT:
						m_bFirstAmplifyCycle = iCurCycle == 0;
						bSuccess = RunAmplifyStep(iCurCycle, &stepItem, fCurStepTemp);
						break;
					default:
						ASSERT(FALSE);
						break;
					}

					// ��������״̬
					if(!bSuccess)
					{
						CTime timeStart = CTime::GetCurrentTime();
						bSuccess = ReconnectDeviceToRun();
						if (!bSuccess)
							break;

						CTime timeEnd = CTime::GetCurrentTime();
						CTimeSpan timeSpan = timeEnd - timeStart;
						long lSeconds = (long)timeSpan.GetTotalSeconds();

						iCurCycle = CalculateCurrentCycle(iCurCycle, &segItem, lSeconds);
						//iCurCycle --;
					}

					if(m_bAskStop)
					{
						goto ASKSTOP;
					}

					if(STATUS_RUNNING != m_nRunStatus)
					{
						break;
					}

					fCurStepTemp = stepItem.m_fAimTem;					
				}

				if(!bSuccess)
				{
					break;
				}

				iCurStep = 0;
			}

			iCurCycle = 0;
		}

		if(!bSuccess)
		{
			break;
		}
	}
	
	if(bSuccess)
	{
		m_pMsgWnd->PostMessage(WM_SEGMENT_OVER,m_nRunStatus);
	}

ASKSTOP:

	m_pExpData->SetExpRunning(FALSE);
	// �ȸ��ͷſ���
	if(bSuccess && m_pExpData->GetExperimentInfo()->GetLidHeader())
	{
		bSuccess = m_pInstancePCR->SetLidHeaterTemp(PCR_TEMP_RELEASE);
	}

	//ʵ�����ʱ��PS96���߽��µȴ�����
	if(bSuccess && !m_pExpData->GetBePS96ExperFile())
	{
		bSuccess = CoolAtEnd();
	}

	//�˳��¶ȼ�ء�ϵͳ״̬����߳�
	m_pTempThread->SetRunStatus(STATUS_STOP);
	m_pStatusThread->SetRunStatus(STATUS_STOP);

	//ֹͣ�����¶ȵ���
	if(bSuccess)
	{
		bSuccess = SetMultiTemp(PCR_TEMP_RELEASE,PCR_TEMP_RELEASE,PCR_TEMP_RELEASE,0,TEMP_CTRL_NORMAL);
	}

	//�������߳�����״̬
	if(bSuccess)
	{
		if(m_bAskStop)
		{
			m_nRunStatus = STATUS_ASK_STOP;
		}
		else
		{
			m_nRunStatus = STATUS_STOP;
		}
	}
	else
	{
		if (m_bAskStop && m_pExpData->GetConnInterruptCount() > 0)
		{
			m_nRunStatus = STATUS_ASK_STOP;
		}
		else
			m_nRunStatus = STATUS_STOP_ERROR;
	}

	AfterRunning();
	return;
}

void CExpRunCtrl::SetPowerOffExpInfo(BOOL bPowerOffExp,int iStartSeg,int iStartStep,int iStartCyle)
{
	m_bPowerOffExp = bPowerOffExp;
	m_iStartSeg = iStartSeg;
	m_iStartStep = iStartStep;
	m_iStartCyle = iStartCyle;

	return;
}

void CExpRunCtrl::ResetPowerOffExpInfo()
{
	m_bPowerOffExp = FALSE;
	m_iStartSeg = -1;
	m_iStartStep = -1;
	m_iStartCyle = -1;
}

// ʵ��ǰ׼�����������豸Home��
BOOL CExpRunCtrl::PrepareRunning()
{
	BOOL bReturn = TRUE;

	//����н����ֽṹ����Ҫ�ж��Ƿ����
	if(m_pInstancePCR->HasBox())
	{
		eSysBoxStatus nBoxStatus = BOXSTATE_IN;
		bReturn = m_pInstancePCR->IsInBox(nBoxStatus);
		if(bReturn)
		{
			if(BOXSTATE_OUT == nBoxStatus)
			{
				CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
				WORD wLangId = MAKELANGID(LANG_CHINESE_SIMPLIFIED,SUBLANG_CHINESE_SIMPLIFIED);
				if(pLangInfo->GetBeEnglish())
				{
					wLangId = MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US);
				}
				HWND hWnd = NULL;
				CWnd* pMainWnd = AfxGetMainWnd();
				if(NULL != pMainWnd)
				{
					hWnd = pMainWnd->m_hWnd;
				}
				
				if(IDNO == MessageBoxExW(hWnd,pLangInfo->GetText(260),_T("GeneralPcr"),MB_ICONINFORMATION | MB_YESNO,wLangId))
				{
					m_nRunStatus = STATUS_ASK_STOP;	//�����¶���һ��״̬,20220817,������
					AfterRunning();
					return FALSE;
				}
				else
				{
					bReturn = m_pInstancePCR->InBox();
				}
			} 
		}
	}
	if(!bReturn)
	{
		m_nRunStatus = STATUS_STOP_ERROR;
		AfterRunning();
		return FALSE;
	}

	bReturn = m_pInstancePCR->Home();
	if(!bReturn)
	{
		m_strErrorInfo = _T("");
		m_nRunStatus = STATUS_STOP_ERROR;
		AfterRunning();
		return bReturn;
	}

	// ��״̬��ʱ��λ��ͬʱ���ȸǣ����в�ʹ���ȸ�ʱֻ�ܲ��ı�״̬��
	if (m_pExpData->GetExperimentInfo()->GetTempCtrlMode() != TEMP_CTRL_NONE)
	{
		//ϵͳæʱ�̵ƾ�����˸
		bReturn = m_pInstancePCR->SetStateLED(STALED_GREEN_BUSY);
		if(!bReturn)
		{
			m_strErrorInfo = _T("");
			m_nRunStatus = STATUS_STOP_ERROR;
			AfterRunning();
			return bReturn;
		}
	}

	// ���ÿ���ģʽ
	bReturn = m_pInstancePCR->SetTempCtrlMode(m_pExpData->GetExperimentInfo()->GetTempCtrlMode());
	if(!bReturn)
	{
		m_strErrorInfo = _T("");
		m_nRunStatus = STATUS_STOP_ERROR;
		AfterRunning();
		return bReturn;
	}

	// ����PMT���Ƶ�ѹ
	WORD wPmtGain = m_pInstancePCR->GetPMTGain(); // ��Ҫ�޸ģ���ǰ��PS96ģʽ

	//��ʱʹ�ã���Ҫͨ���޸�EEPROMʵ��
	if(NULL != m_pInstancePCR && m_pInstancePCR->GetBe96SiteQPCR())
	{
		wPmtGain = m_pInstancePCR->GetMppcGainMiddle();
	}

	bReturn = m_pInstancePCR->SetPMTGain(wPmtGain);
	if(!bReturn)
	{
		m_strErrorInfo = _T("");
		m_nRunStatus = STATUS_STOP_ERROR;
		AfterRunning();
		return bReturn;
	}

	return bReturn;
}


// ʵ����ɺ��豸״̬����
void CExpRunCtrl::AfterRunning()
{
	m_pExpData->m_tmEnd = CTime::GetCurrentTime();
	m_pExpData->GetExperimentInfo()->SetEndTime(m_pExpData->m_tmEnd);
	m_pExpData->SaveFile();

	if(STATUS_ASK_STOP == m_nRunStatus || STATUS_STOP == m_nRunStatus)
	{
		//ϵͳ����ʱ�̵Ƴ���
		if (m_pExpData->GetExperimentInfo()->GetLidHeader())
		{
			BOOL bSuccess = m_pInstancePCR->SetStateLED(STALED_GREEN_IDLE);
		}
	}

	m_pInstancePCR->SetInstanceStatus(INSTANCE_READY);

	if(NULL != m_pMsgWnd)
	{
		m_pMsgWnd->SendMessage(WM_EXPERIMENT_FINISH,m_nRunStatus);
	}
}

void CExpRunCtrl::RefreshStatus()
{
	if (NULL != m_pMsgWnd)
	{
		m_pMsgWnd->PostMessage(WM_EXPERIMENT_REFRESH);
	}
}

BOOL CExpRunCtrl::RunLidHeader()
{
	if(!m_pExpData->GetExperimentInfo()->GetLidHeader() || m_pExpData->GetExperimentInfo()->GetTempCtrlMode() == TEMP_CTRL_NONE)
	{
		return TRUE;
	}

	m_pRunPara->SetCurStage(STAGE_LID_HEATER);

	//�����ȸ��¶�
	float fTopTemp = m_pExpData->GetExperimentInfo()->GetLidTemp();
	BOOL bReturn = m_pInstancePCR->SetLidHeaterTemp(fTopTemp);
	if (!bReturn)
	{
		m_nRunStatus = STATUS_STOP_ERROR;
		m_strErrorInfo = _T("");
		AfterRunning();
		return FALSE;
	}

	float fLower = m_pInstancePCR->GetSysTemp(CInstancePCR::SYS_TEMP_PELTIER_LOWER);
	bReturn = SetMultiTemp(fLower,fLower,fLower,0,TEMP_CTRL_NORMAL);
	if(!bReturn)
	{
		m_nRunStatus = STATUS_STOP_ERROR;
		m_strErrorInfo = _T("");
		AfterRunning();
		return FALSE;
	}

	//判断是否到达目标温度值
	BOOL bSuccess = TRUE;
	float fLidTemp = 0.0f;
	BOOL bToTarget = FALSE;
	int iReadCount = 0;
	float fTempGap = ::fabs(fLidTemp - fTopTemp);
	float fLegalGap = m_pInstancePCR->GetLidTempLegalGap();
	int iSleepTime = 1000;
	int iMaxReadCount = m_pInstancePCR->GetMaxLidHeatSecond() * 1000 / iSleepTime;

	// 去除加热盖等待时间，直接跳过
	// 以下是原始代码，已被注释掉
	/*
#ifdef WEAK_HOTLID_ON_TEMPERATURE
	fLegalGap = 5;
	while(fLidTemp + fLegalGap < fTopTemp)
#else
	while((fTempGap > fLegalGap) || !bToTarget)  // 修复注释语法错误
#endif
	{
		if(m_bAskStop)
			break;

		if (m_pInstancePCR->GetInstanceStatus() == INSTANCE_ERROR)
			break;

		fLidTemp = m_pRunPara->GetLidTemp();
		bToTarget = m_pRunPara->GetLidReachTemp();
		++iReadCount;
		if(iReadCount >= iMaxReadCount)
		{
			bSuccess = FALSE;
			break;
		}

		if(NULL != m_pRunPara && !m_pRunPara->GetCommunicationState())
		{
			bSuccess = FALSE;
			break;
		}

		fTempGap = ::fabs(fLidTemp - fTopTemp);
		::Sleep(iSleepTime);
	}
	*/

	if(iReadCount >= iMaxReadCount)
	{
		m_pExpData->SetReachLidTemp(FALSE);

		CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

		WORD wLangId = MAKELANGID(LANG_CHINESE_SIMPLIFIED,SUBLANG_CHINESE_SIMPLIFIED);
		if(pLangInfo->GetBeEnglish())
		{
			wLangId = MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US);
		}

		HWND hWnd = NULL;
		CWnd* pMainWnd = AfxGetMainWnd();
		if(NULL != pMainWnd)
		{
			hWnd = pMainWnd->m_hWnd;
		}
		if(IDYES == MessageBoxExW(hWnd,pLangInfo->GetText(246),_T("GeneralPcr"),MB_ICONINFORMATION | MB_YESNO,wLangId))
		{
			bSuccess = TRUE;
		}
	}

	if(m_bAskStop)
	{
		return bSuccess;
	}

	if(!bSuccess)
	{
		m_nRunStatus = STATUS_STOP_ERROR;
		m_strErrorInfo = _T("");
		AfterRunning();
	}

	return bSuccess;
}

BOOL CExpRunCtrl::SetLaserStatus()
{
	BYTE LaserState = 255;
	BOOL bReturn = m_pInstancePCR->SetLaserStatus(2, LaserState);

	if (!bReturn)
	{
		m_nRunStatus = STATUS_STOP_ERROR;
		m_strErrorInfo = _T("");
		AfterRunning();
	}

	return bReturn;
}

// ɨ��ͨ���ź�ֵ�������������ý���Ԥɨ�����
BOOL CExpRunCtrl::ChannelSignalCorrection()
{
	BOOL bReturn = TRUE;

	if(!bReturn)
	{
		m_nRunStatus = STATUS_STOP_ERROR;
		AfterRunning();
	}

	return bReturn;
}

// ����ϵͳ״̬���������/���ǵ�
void CExpRunCtrl::HandleDeviceStatus()
{
	tagDeviceStatus* pStatus = m_pInstancePCR->GetDeviceStatus();

	//�����ְ�ť�Ƿ���
	if (1 == pStatus->cSysBox)
	{
		//����״̬�ƻ�ɫ��ʾ
		m_pInstancePCR->SetStateLED(STALED_YELLOW_PCR_BTNDOWN);
		if (IDOK == AfxMessageBox(_T("Please do not open the door during running !"),MB_ICONINFORMATION |MB_OK))
		{
			m_pInstancePCR->SetStateLED(STALED_GREEN_BUSY);
		}
	}

	// �����Ƿ񱻴�
	if (1 == pStatus->cLid)
	{
		//����״̬�ƻ�ɫ��ʾ
		m_pInstancePCR->SetStateLED(STALED_YELLOW_PCR_BTNDOWN);
		if (IDOK == AfxMessageBox(_T("Please do not open the lid during running !"),MB_ICONINFORMATION |MB_OK))
		{
			if (m_pExpData->GetExperimentInfo()->GetLidHeader())
				m_pInstancePCR->SetStateLED(STALED_GREEN_BUSY);
		}
	}
}

//  ���в�����ӫ���ⲽ�裬ֻ���»�ȴ�
BOOL CExpRunCtrl::RunNoneStep(CStepItemInfo* pStep, float fPreTemp)
{
	int nTemChangedSecond = 0;

	//����Ŀ���¶�
	BOOL bReturn = SetMultiTemp(pStep->m_fAimTem,pStep->m_fAimTem,pStep->m_fAimTem,pStep->m_fRate,TEMP_CTRL_NORMAL);
	if (!bReturn) 
	{
		return FALSE;
	}
	
	if(m_bAskStop)
	{
		return TRUE;
	}

	// �¶ȿ��£����ж��Ƿ񵽴��¶�
	int iUseTime = RunWaitReachTemp(pStep);

	if(m_bAskStop)
	{
		return TRUE;
	}

	//������ʱ�ȴ���96��QPCR�����ж�����ʱ���������أ�
	int iRealDelay = (int)m_pExpData->GetStepVolumeDelaySec(pStep->m_fAimTem);
	iRealDelay += pStep->m_iDelay;
	RunStepDelay(iRealDelay);

	return TRUE;
}

int CExpRunCtrl::RunWaitReachTemp(CStepItemInfo* pStep)
{
	// 去除加热等待时间，直接返回0
	return 0;
	
	// 以下是原始代码，已被注释掉
	/*
	if (m_pExpData->GetExperimentInfo()->GetTempCtrlMode() == TEMP_CTRL_NONE)
		return 0;

	BOOL bIgnoreGap = FALSE;
	if(TEMP_MODE_CONSTANT == m_pInstancePCR->GetTempMode())
	{
		if (!m_bNewSegmentCycle)
		{
			return 0;
		}

		if (m_bFirstAmplifyCycle && CHECK_END_POINT == pStep->m_eChkType)
		{
			bIgnoreGap = TRUE;
		}
	}

	LONGLONG lUseSeconds = 0;
	int iMaxCtrlTime = m_pInstancePCR->GetMaxTempCtrlSecond();
	float fTempLegalGap = m_pInstancePCR->GetTempLegalGap();
	if (bIgnoreGap)
	{
		fTempLegalGap = 0.5;
	}
	CTime timeStart = CTime::GetCurrentTime();
	float fCurTemp = m_pRunPara->GetCurTemp();
	BOOL bCurReachTemp = m_pRunPara->GetCurReachTemp();
	float fTempGap = ::fabs(fCurTemp - pStep->m_fAimTem);
	while(fTempGap > fTempLegalGap)
	{
		if(m_bAskStop)
		{
			break;
		}

		::Sleep(50);
		CTime timeTemp = CTime::GetCurrentTime();
		CTimeSpan timespan = timeTemp - timeStart;
		lUseSeconds = timespan.GetTotalSeconds();
		if (lUseSeconds >= iMaxCtrlTime)
		{
			m_iNotReachTempCount ++;
			break;
		}
		if(STATUS_RUNNING != m_nRunStatus )
		{
			break;
		}

		fCurTemp = m_pRunPara->GetCurTemp();
		bCurReachTemp = m_pRunPara->GetCurReachTemp();
		if (bCurReachTemp)
			break;
		fTempGap = ::fabs(fCurTemp - pStep->m_fAimTem);
	}

	if(TEMP_MODE_CONSTANT == m_pInstancePCR->GetTempMode())
	{
		m_bNewSegmentCycle = FALSE;
	}

	return (int)lUseSeconds;
	*/
}

int CExpRunCtrl::RunWaitReachTemp(float fAimTem)
{
	// 去除加热等待时间，直接返回0
	return 0;
	
	// 以下是原始代码，已被注释掉
	/*
	if (m_pExpData->GetExperimentInfo()->GetTempCtrlMode() == TEMP_CTRL_NONE)
		return 0;

	if(fAimTem <= 0)
	{
		return 0;
	}

	LONGLONG lUseSeconds = 0;
	CTime timeStart = CTime::GetCurrentTime();
	int iMaxCtrlTime = m_pInstancePCR->GetMaxTempCtrlSecond();

	float fTempLegalGap = m_pInstancePCR->GetTempLegalGap();
	float fCurTemp = m_pRunPara->GetCurTemp();
	float fTempGap = ::fabs(fCurTemp - fAimTem);

	BOOL bCurReachTemp = m_pRunPara->GetCurReachTemp();
	
	while (fTempGap > fTempLegalGap || !bCurReachTemp)
	{
		if(m_bAskStop)
		{
			break;
		}

		::Sleep(50);
		CTime timeTemp = CTime::GetCurrentTime();
		CTimeSpan timespan = timeTemp - timeStart;
		lUseSeconds = timespan.GetTotalSeconds();
		if (lUseSeconds >= iMaxCtrlTime)
		{
			++m_iNotReachTempCount;
			break;
		}
		if(m_nRunStatus != STATUS_RUNNING)
		{
			break;
		}

		fCurTemp = m_pRunPara->GetCurTemp();
		bCurReachTemp = m_pRunPara->GetCurReachTemp();
		fTempGap = ::fabs(fCurTemp - fAimTem);
	}

	return (int)lUseSeconds;
	*/
}

BOOL CExpRunCtrl::CoolAtEnd()
{
	float fTem = 0;
	BOOL bTarget = FALSE;

	BOOL bReturn = m_pInstancePCR->ReadPCRTemp(fTem,bTarget);
	if (!bReturn)
	{
		m_nRunStatus = STATUS_STOP_ERROR;
		return FALSE;
	}

	float fEndCoolTem = m_pInstancePCR->GetEndCoolTemperature();
	if(fTem <= fEndCoolTem)
	{
		return TRUE;
	}

	bReturn = SetMultiTemp(fEndCoolTem,fEndCoolTem,fEndCoolTem,4.0,TEMP_CTRL_NORMAL);
	if (!bReturn)
	{
		m_nRunStatus = STATUS_STOP_ERROR;
		return FALSE;
	}

	float fLegal = fTem - fEndCoolTem;
	// 去除冷却等待时间，直接跳过
	// 以下是原始代码，已被注释掉
	/*
	while(fLegal > 2.0)
	{
		Sleep(200);
		if(m_bAskStop)
		{
			Sleep(1200);
			return TRUE;
		}

		bReturn = m_pInstancePCR->ReadPCRTemp(fTem,bTarget);
		if (!bReturn)
		{
			m_nRunStatus = STATUS_STOP_ERROR;
			return FALSE;
		}
		fLegal = fTem - fEndCoolTem;
	}
	*/

	int iTempInt1 = m_pInstancePCR->GetSleepMsAfterCool() / 100;
	// 去除冷却后等待时间，直接跳过
	// 以下是原始代码，已被注释掉
	/*
	for(int i = 0;i < iTempInt1;++i)
	{
		Sleep(100);
		if(m_bAskStop)
		{
			Sleep(200);
			return TRUE;
		}
	}
	*/

	return TRUE;
}

// ָȴʱ
void CExpRunCtrl::RunStepDelay(int iWaitSecond)
{
	// 去除步骤延迟等待时间，直接返回
	// 以下是原始代码，已被注释掉
	/*
	for (int i = 0;i < iWaitSecond;++i)
	{
		if(m_bAskStop)
		{
			break;
		}

		if(NULL != m_pRunPara && !m_pRunPara->GetCommunicationState())
		{
			break;
		}

		if(STATUS_RUNNING != m_nRunStatus)
		{
			break;
		}
		::Sleep(1000);
	}
	*/

	return;
}

BOOL CExpRunCtrl::RunMeltStep(CStepItemInfo* pStep,float fPreTemp,int iPreDelay)
{
	BOOL bReturn = FALSE;
	BOOL bTarget = FALSE;

	if(m_bNoStepBeforeMeltStep)
	{
		bReturn = SetMultiTemp(m_fMeltStepStartTem,m_fMeltStepStartTem,m_fMeltStepStartTem,pStep->m_fRate,TEMP_CTRL_NORMAL);
		if(!bReturn)
		{
			m_nRunStatus = STATUS_STOP_ERROR;
			return FALSE;
		}

		float fRealTem = 0;
		// 去除温度目标等待时间，直接跳过
		// 以下是原始代码，已被注释掉
		/*
		for(int i = 0;i < 60;++i)
		{
			bReturn = m_pInstancePCR->ReadPCRTemp(fRealTem,bTarget);
			if(!bReturn)
			{
				m_nRunStatus = STATUS_STOP_ERROR;
				return FALSE;
			}

			if(bTarget)
			{
				break;
			}
			Sleep(1000);
		}
		*/
		
		// 直接读取当前温度，不等待
		bReturn = m_pInstancePCR->ReadPCRTemp(fRealTem,bTarget);
		if(!bReturn)
		{
			m_nRunStatus = STATUS_STOP_ERROR;
			return FALSE;
		}
		
		if(bTarget)
		{
			fPreTemp = m_fMeltStepStartTem;
		}
		else
		{
			fPreTemp = fRealTem;
		}
		iPreDelay =0;
	}
	else
	{
		// ǰ¶ȴ۽Ŀ¶ȣ޷۽ɨ裬
		if(fPreTemp >= pStep->m_fAimTem)
		{
			m_bNotReachMeltStartTemp = TRUE;
			return TRUE;
		}
	}
	
	//如果预热延迟时间小于30秒，则补充延迟时间，确保初始温度稳定
	int iPreMeltMinSlpSec = m_pInstancePCR->GetPreMeltMinSleepSec();
	if(iPreDelay < iPreMeltMinSlpSec)
	{
		// 去除预热等待时间，直接跳过
		// 以下是原始代码，已被注释掉
		/*
		int iCycles = (iPreMeltMinSlpSec - iPreDelay) * 10;
		for(int i = 0;i < iCycles;++i)
		{
			Sleep(100);
			if(m_bAskStop)
			{
				return TRUE;
			}
		}
		*/
	}

	m_pRunPara->SetMeltStepRunningState(TRUE);

	m_dMeltTemBeforeScan = 0;
	m_dMeltTemAfterScan = 0;

	float fStartTem = 0;

	BOOL bPwmMelt = m_pInstancePCR->GetBe96SiteQPCR();
	float fPwmK1 = 1,fPwmB1 = 0,fPwmK2 = 1,fPwmB2 = 0,fPwmK3 = 1,fPwmB3 = 0,fBasePwm1 = 0,fBasePwm2 = 0,fBasePwm3 = 0;
	float fIncrementPwm1 = 0,fIncrementPwm2 = 2,fIncrementPwm3 = 3;  
	if(bPwmMelt)
	{
		//PWM����ʱ�����ȶ���ʼ�¶�
		fPwmK1 = m_pInstancePCR->GetPwmK1();
		fPwmB1 = m_pInstancePCR->GetPwmB1();
		fPwmK2 = m_pInstancePCR->GetPwmK2();
		fPwmB2 = m_pInstancePCR->GetPwmB2();
		fPwmK3 = m_pInstancePCR->GetPwmK3();
		fPwmB3 = m_pInstancePCR->GetPwmB3();

		fIncrementPwm1 = pStep->m_fIncrement * fPwmK1;
		fIncrementPwm2 = pStep->m_fIncrement * fPwmK2;
		fIncrementPwm3 = pStep->m_fIncrement * fPwmK3;

		if(fPreTemp < 50)
		{
			fPreTemp = 50;
		}

		fBasePwm1 = fPreTemp * fPwmK1 + fPwmB1;
		fBasePwm2= fPreTemp * fPwmK2 + fPwmB2;
		fBasePwm3 = fPreTemp * fPwmK3 + fPwmB3;
		bReturn = m_pInstancePCR->SetPWMTemp(fPreTemp,fPreTemp,fPreTemp,WORD(fBasePwm1),WORD(fBasePwm2),WORD(fBasePwm3));
		if (!bReturn)
		{
			m_nRunStatus = STATUS_STOP_ERROR;
			return FALSE;
		}

		int iTempInt1 = m_pInstancePCR->GetPWMInitWaitMs() / 100;
		// 去除PWM初始化等待时间，直接跳过
		// 以下是原始代码，已被注释掉
		/*
		for(int i = 0;i < iTempInt1;++i)
		{
			Sleep(100);
			if(m_bAskStop)
			{
				return TRUE;
			}
		}
		*/

		bReturn = m_pInstancePCR->ReadPCRTemp(fStartTem,bTarget);
		if (!bReturn)
		{
			m_nRunStatus = STATUS_STOP_ERROR;
			return FALSE;
		}
		float fDiff = fStartTem - fPreTemp;

		//补偿PWM温度偏差
		fStartTem = fPreTemp - fDiff;
		if(pStep->m_fIncrement < 0.5)
		{
			fStartTem -= pStep->m_fIncrement * 2;
		}
		else
		{
			fStartTem -= pStep->m_fIncrement;
		}
		
		//设置初始温度
		fBasePwm1 = fStartTem * fPwmK1 + fPwmB1;
		fBasePwm2= fStartTem * fPwmK2 + fPwmB2;
		fBasePwm3 = fStartTem * fPwmK3 + fPwmB3;
		bReturn = m_pInstancePCR->SetPWMTemp(fStartTem,fStartTem,fStartTem,WORD(fBasePwm1),WORD(fBasePwm2),WORD(fBasePwm3));
		if (!bReturn)
		{
			m_nRunStatus = STATUS_STOP_ERROR;
			return FALSE;
		}
		// 去除第二个PWM初始化等待时间，直接跳过
		// 以下是原始代码，已被注释掉
		/*
		for(int i = 0;i < iTempInt1;++i)
		{
			Sleep(100);
			if(m_bAskStop)
			{
				return TRUE;
			}
		}
		*/
	}
	else
	{
		if(fPreTemp < 30)
		{
			fPreTemp = 30;
		}

		fStartTem = fPreTemp;
		bReturn = SetMultiTemp(fStartTem,fStartTem,fStartTem,pStep->m_fRate,TEMP_CTRL_NORMAL);
		if(!bReturn)
		{
			m_nRunStatus = STATUS_STOP_ERROR;
			return FALSE;
		}
	}

	int iUseTime = 0;
	int iDataIndex = 0,iCalcCycles = 6;
	if(0 != pStep->m_fIncrement)
	{
		iCalcCycles = int((pStep->m_fAimTem - fPreTemp) / pStep->m_fIncrement) + 20;
	}
	for(float fTem = fStartTem;(m_dMeltTemBeforeScan + m_dMeltTemAfterScan) / 2 <= pStep->m_fAimTem && iDataIndex < iCalcCycles;fTem += pStep->m_fIncrement)
	{
		if(bPwmMelt)
		{
			bReturn = m_pInstancePCR->SetPWMTemp(fTem,fTem,fTem,WORD(fBasePwm1 + iDataIndex * fIncrementPwm1),WORD(fBasePwm2 + iDataIndex * fIncrementPwm2),WORD(fBasePwm3 + iDataIndex * fIncrementPwm3));
		}
		else
		{
			bReturn = SetMultiTemp(fTem,fTem,fTem,pStep->m_fRate,TEMP_CTRL_NORMAL);
		}
		
		if(!bReturn)
		{
			m_nRunStatus = STATUS_STOP_ERROR;
			return FALSE;
		}

		if(m_bAskStop)
		{
			return TRUE;
		}

		//判断是否到达目标温度
		if(bPwmMelt)
		{
			// 去除固定等待时间，直接跳过
			// 以下是原始代码，已被注释掉
			/*
			for(int i = 0;i < 60;++i)
			{
				Sleep(100);
				if(m_bAskStop)
				{
					return TRUE;
				}
			}
			*/
		}
		else
		{
			iUseTime = RunWaitReachTemp(fTem);
		}

		if(m_bAskStop)
		{
			return TRUE;
		}

		clock_t ckStart = clock();
		
		//���
		m_pRunPara->LockInstance();
		if(pStep->GetBeCollectAll())
		{
			// ��¼ɨ�迪ʼʱ��
			unsigned long long startTime = ::GetTickCount64();

			float fCurTemp = 0.0f;
			BOOL bCurToTarget = FALSE;

			//ɨ��ǰ��ȡһ���¶�
			bReturn = m_pInstancePCR->ReadPCRTemp(fCurTemp,bCurToTarget);
			if(!bReturn)
			{
				m_nRunStatus = STATUS_STOP_ERROR;
				m_pRunPara->UnlockInstance();
				return FALSE;
			}
			m_dMeltTemBeforeScan = fCurTemp;

			// ɨ��
			bReturn = m_pInstancePCR->ScanOnce(&m_bAskStop);
			if(!bReturn)
			{
				m_nRunStatus = STATUS_STOP_ERROR;
				m_pRunPara->UnlockInstance();
				return FALSE;
			}

			//ɨ����ȡһ���¶�
			bReturn = m_pInstancePCR->ReadPCRTemp(fCurTemp,bCurToTarget);
			if(!bReturn)
			{
				m_nRunStatus = STATUS_STOP_ERROR;
				m_pRunPara->UnlockInstance();
				return FALSE;
			}
			m_dMeltTemAfterScan = fCurTemp;

			if(m_bAskStop)
			{
				m_pRunPara->UnlockInstance();
				return TRUE;
			}

			//��ȡӫ������
			if(!ReadScanDataOnce(ESDT_MELT))
			{
				m_nRunStatus = STATUS_STOP_ERROR;
				m_pRunPara->UnlockInstance();
				return FALSE;
			}

			m_pMsgWnd->SendMessage(WN_EXPERIMENT_FLU_CHANGED,iDataIndex++,1);
			m_pMsgWnd->PostMessage(WM_MELT_ONETEM_OVER, int(fTem * 100),0);
		}
		m_pRunPara->UnlockInstance();

		clock_t ckExist = clock() - ckStart;
		m_pRunPara->SetScanGetDataMs(ckExist);
	}

	//��������ִ����Ϻ󣬼�ʹ���滹���������̣�Ҳ���ٽ��жϵ籣����20220824
	m_pMsgWnd->PostMessage(WM_SEGMENT_OVER,m_nRunStatus);

	RunStepDelay(pStep->m_iDelay);

	m_pRunPara->SetMeltStepRunningState(FALSE);

	return TRUE;
}

BOOL CExpRunCtrl::RunAmplifyStep(int iCycleC, CStepItemInfo* pStep, float fPreTemp)
{
	int iUseTime = 0;
	BOOL bReturn = FALSE;

	// ����
	if(TEMP_CTRL_NONE != m_pExpData->GetExperimentInfo()->GetTempCtrlMode())
	{
		if (pStep->m_bSelGrad)
		{
			//�����ݶ�ģʽ
			float ColTemp[12] = {0};
			float DeltaT = 0; 
			float fCaliAimT = 0;
			float fCaliDeltaT = 0;
			pStep->GetStepGradientInfo(ColTemp,DeltaT,fCaliAimT,fCaliDeltaT);
			bReturn = SetMultiTemp(fCaliAimT,(fCaliAimT * 2 + fCaliDeltaT) / 2,fCaliAimT+fCaliDeltaT,0,TEMP_CTRL_GRADIENT);
			if(!bReturn)
			{
				m_nRunStatus = STATUS_STOP_ERROR;
				return FALSE;
			}
		}
		else
		{
			//��ͨģʽ
			bReturn = SetMultiTemp(pStep->m_fAimTem,pStep->m_fAimTem,pStep->m_fAimTem,pStep->m_fRate,TEMP_CTRL_NORMAL);
			if(!bReturn)
			{
				m_nRunStatus = STATUS_STOP_ERROR;
				return FALSE;
			}
		}
		
		//���ж��Ƿ񵽴��¶�
		iUseTime = RunWaitReachTemp(pStep);
	}

	if(m_bAskStop)
	{
		return TRUE;
	}

	//��ʱ�ȴ�
	if(TEMP_MODE_CONSTANT == m_pInstancePCR->GetTempMode())
	{
		int iWaitSecond = pStep->m_iDelay /*- iUseTime*/;

		if(CHECK_END_POINT == pStep->m_eChkType)
		{
			int ScanGetDataSec = m_pRunPara->GetScanDataSecond();
			if(ScanGetDataSec > 0)
			{
				iWaitSecond -= ScanGetDataSec;
			}
			else
			{
				iWaitSecond -= m_pInstancePCR->GetScanOnceTime();
			}
		}
		RunStepDelay(iWaitSecond);
	}
	else
	{
		//96��QPCR�ж�����ʱ
		int iRealDelay = (int)m_pExpData->GetStepVolumeDelaySec(pStep->m_fAimTem);
		iRealDelay += pStep->m_iDelay;
		RunStepDelay(iRealDelay);
	}

	if(m_bAskStop)
	{
		return TRUE;
	}

	clock_t ckScanStart = clock();

	//���
	m_pRunPara->LockInstance();
	if(pStep->GetBeCollect())
	{
		// �ж��Ƿ񵽴�ɨ��ʱ���¶ȣ����������ƽ���¶�ƫ���1�ȣ�����Ϊ������
		CompareTempAndWriteLog(iCycleC, pStep);

		// ��¼ɨ�迪ʼʱ��
		unsigned long long startTime = ::GetTickCount64();
		// ɨ��
		bReturn = m_pInstancePCR->ScanOnce(&m_bAskStop);
		if(!bReturn)
		{
			m_nRunStatus = STATUS_STOP_ERROR;
			m_pRunPara->UnlockInstance();
			return FALSE;
		}

		if(m_bAskStop)
		{
			m_pRunPara->UnlockInstance();
			return TRUE;
		}

		// ��¼ɨ�����ʱ��
		unsigned long long currentTime = ::GetTickCount64();
		// ��������ѭ������ʱ������
		CreateAndSendCycle((long)startTime,(long)currentTime,iCycleC);

		//��ȡӫ������
		if(!ReadScanDataOnce(ESDT_AMP))
		{
			m_nRunStatus = STATUS_STOP_ERROR;
			m_pRunPara->UnlockInstance();
			return FALSE;
		}

		m_pMsgWnd->SendMessage(WN_EXPERIMENT_FLU_CHANGED,iCycleC+1,0);
	}
	m_pRunPara->UnlockInstance();

	m_pRunPara->SetScanGetDataMs(clock() - ckScanStart);

	return TRUE;
}

// ��ȡӫ��ɨ������
BOOL CExpRunCtrl::ReadScanDataOnce(ESCANDATATYPE eScanDataType)
{
	int nChannelCount = m_pExpData->GetChannelCount();
	CUIntArray* pAryFlu = new CUIntArray[nChannelCount];
	if(NULL == pAryFlu)
	{
		return FALSE;
	}

	BOOL bReturn = FALSE;
	bReturn = m_pInstancePCR->ReadAllFLUData(pAryFlu,nChannelCount);
	if(!bReturn)
	{
		delete [] pAryFlu;
		return FALSE;
	}

	// ��������
	for(int ichannel = 1;ichannel <= nChannelCount;++ichannel)
	{
		if(ESDT_AMP == eScanDataType)
		{
			m_pExpData->AddAmplifyFluOriginalData(pAryFlu[ichannel - 1],ichannel);
		}
		else if(ESDT_MELT == eScanDataType)
		{
			m_pExpData->AddMeltFluOriginalData(pAryFlu[ichannel - 1],ichannel,(m_dMeltTemBeforeScan + m_dMeltTemAfterScan) / 2);
		}
	}

	delete [] pAryFlu;

	return TRUE;
}

// �����������ݵ�X�����ݣ��ж���ѭ��������ʱ��
void CExpRunCtrl::CreateAndSendCycle(long lStartTime, long lEndTime, int iCycleCount)
{
	int iTubeCount = m_pExpData->GetPlateInfo()->GetTubeCount();
	CUIntArray	arrayCycleX;
	arrayCycleX.RemoveAll();
	if (m_pInstancePCR->GetTempMode() == TEMP_MODE_UPDOWN)
	{
		for (int i=0; i<iTubeCount; i++)
		{
			arrayCycleX.Add(iCycleCount + 1);
		}
	}
	else
	{
		if (m_lScanStartTime == -1)
		{
			m_lScanStartTime = lStartTime;
		}

		long lStart = lStartTime - m_lScanStartTime + 1000;
		long lStep = (lEndTime - lStartTime) / (iTubeCount-1);

		for (int i=0; i<iTubeCount; i++)
		{
			arrayCycleX.Add(lStartTime + lStep * i);
		}
	}

	m_pExpData->AddAmplifyCycleX(&arrayCycleX);
}

BOOL CExpRunCtrl::SetMultiTemp(float fTemp1, float fTemp2, float fTemp3, float fRate, BYTE cMode)
{
	if (m_pExpData->GetExperimentInfo()->GetTempCtrlMode() == TEMP_CTRL_NONE)
		return TRUE;

	BOOL bReturn = m_pInstancePCR->SetMultiTemp(fTemp1, fTemp2, fTemp3, fRate, cMode);
	if (bReturn)
	{
		m_pExpData->SetTempTarget(fTemp1, fTemp2, fTemp3);
	}

	return bReturn;
}

// �ж��Ƿ񵽴�ɨ��ʱ���¶ȣ����������ƽ���¶�ƫ���1�ȣ�����Ϊ������
void CExpRunCtrl::CompareTempAndWriteLog(int iCycle, CStepItemInfo* pStep)
{
	if(::fabs(m_pRunPara->GetCurTemp() - pStep->m_fAimTem) <= 1.0)
		return;

	int iErrChaCount = 0;
	CString strChannel, strTemp;
	strChannel.Empty();
	BOOL bReachTemp = FALSE;
	int iChannelCount = 6;
	float* pChTemp = new float[iChannelCount];
	if (m_pInstancePCR->ReadAllTemp(iChannelCount, pChTemp, bReachTemp))
	{
		for (int i=0; i<iChannelCount; i++)
		{
			if (::fabs(pChTemp[i] - pStep->m_fAimTem) > 3.0)
			{
				iErrChaCount ++;
				strTemp.Format(_T("%d"), i+1);
				if (strChannel.IsEmpty())
				{
					strChannel = strTemp;
				}
				else
				{
					strChannel += _T(", ");
					strChannel += strTemp;
				}
			}
		}
	}

	if (!strChannel.IsEmpty())
	{
		strTemp.Format(_T(" %d"), iCycle+1);
		CString strLog;
		strLog = m_pLanguage->GetText(238);
		strLog += strTemp;
		if (m_pLanguage->GetLanguage() == LANGUAGE_ENG)
			strLog += _T(" ");
		strLog += m_pLanguage->GetText(51);
		strLog += strChannel;
		if (m_pLanguage->GetLanguage() == LANGUAGE_ENG)
			strLog += _T(" ");
		strLog += m_pLanguage->GetText(478);

		if (m_bFirstTempErr)
		{
			m_pExpData->AddLog(strLog);
			if (iErrChaCount >=3)
			{
				m_arrayInfo.Add(strLog);
			}
			m_bFirstTempErr = FALSE;
		}
	}

	delete pChTemp;
}

CString CExpRunCtrl::GetRunLog()
{
	CString strLog;
	strLog.Empty();
	if (!m_bFirstTempErr && m_arrayInfo.GetCount() > 0)
	{
		strLog = m_arrayInfo.GetAt(0);
	}

	return strLog;
}

void CExpRunCtrl::AddInterruptLog(int iSegNo, int iStepNo, int iCycleNo)
{
	if (iStepNo == 1 && iStepNo == 1 && iCycleNo == 1)
		return;

	CString strLog, strTemp;
	strLog = m_pLanguage->GetText(484);

	strTemp.Format(_T("%s:%d, %s:%d, %s:%d"), m_pLanguage->GetText(57), iSegNo, m_pLanguage->GetText(58), iStepNo, m_pLanguage->GetText(238), iCycleNo);
	strLog += _T(" ");
	strLog += strTemp;

	m_pExpData->AddLog(strLog);
}

BOOL CExpRunCtrl::ReconnectDeviceToRun()
{
	if (m_bAskStop)
		return TRUE;

	m_pMsgWnd->SendMessage(WN_EXPERIMENT_DISCONNECT, 0);
	m_pInstancePCR->SetInstanceStatus(INSTANCE_ERROR);
	m_pExpData->ConnInterruptOnce();

	CCommInterruptTipDlg dlg;
	dlg.m_pInstancePCR = m_pInstancePCR;
	dlg.m_pExpData = m_pExpData;
	dlg.m_iRemainSec = m_pExpData->GetExperimentInfo()->GetRemainSec();
	if (dlg.DoModal() == IDCANCEL)
	{
		if (dlg.m_retCode == 0)
		{
			m_pExpData->SetReconnectState(1);
			m_pRunPara->SetCommunicationState(TRUE);
			m_nRunStatus = STATUS_RUNNING;
			m_pMsgWnd->SendMessage(WN_EXPERIMENT_DISCONNECT, 1);
			return TRUE;
		}
		else if (dlg.m_retCode == 1)
		{
			m_pExpData->SetReconnectState(2);
		}
		else
		{
			m_bAskStop = TRUE;
		}
	}

	return FALSE;
}

// ʵ���ж�ʱ�����ж�ʱ��������ѭ����������ж�ʱ�䳬��һ��ѭ�����ϣ����Զ�������������ѭ����������Ϊ0��ͬʱ���ܽ���Ct����
int CExpRunCtrl::CalculateCurrentCycle(int iCycle, CSegItemInfo* pSegment, int iDelaySecond)
{
	int iReturnCycle = iCycle;
	long lSegmengTime = pSegment->CalculateTotalSeconds();
	if (lSegmengTime < PS96_SCANSEC)
		lSegmengTime = long(PS96_SCANSEC + 0.5);
	int iAddCycle = iDelaySecond / lSegmengTime + 1;
	if (iCycle + iAddCycle > pSegment->m_iCycleNum)
	{
		iReturnCycle = pSegment->m_iCycleNum + 1;
	}
	else
	{
		int iTubeCount = m_pExpData->GetPlateInfo()->GetTubeCount();
		int nChannelCount = m_pExpData->GetChannelCount();
		CUIntArray	arrayCycleX;
		CUIntArray	arrayFluoValue;
		long lLastScanTime = m_pExpData->GetCurrentAmplifyCycleX(1);
		iReturnCycle = iCycle + iAddCycle;
		for (int i=0; i<iAddCycle; i++)
		{
			arrayCycleX.RemoveAll();
			for (int j=0; j<iTubeCount; j++)
			{
				if (m_pInstancePCR->GetTempMode() == TEMP_MODE_UPDOWN)
				{
					arrayCycleX.Add(lLastScanTime + lSegmengTime*(i+1));
				}
				else
				{
					arrayCycleX.Add(iCycle + i);
				}
			}
			m_pExpData->AddAmplifyCycleX(&arrayCycleX);

			for(int ichannel = 1;ichannel <= nChannelCount;++ichannel)
			{
				arrayFluoValue.RemoveAll();
				for (int j=0; j<iTubeCount; j++)
				{
					arrayFluoValue.Add(0);
				}

				m_pExpData->AddAmplifyFluOriginalData(arrayFluoValue,ichannel);
			}
			m_pMsgWnd->SendMessage(WN_EXPERIMENT_FLU_CHANGED, iCycle+i, 0);
		}

		int iRemainSecond = m_pExpData->GetExperimentInfo()->GetRemainSec();
		iRemainSecond -= lSegmengTime * iAddCycle;
		m_pExpData->GetExperimentInfo()->SetRemainSec(iRemainSecond);
		m_pExpData->SetHasZeroValues(TRUE);
	}

	return iReturnCycle;
}
