#pragma once

#include <afxmt.h>

/**
 * \ingroup PublicLib
 * 程序运行实例检测，保证只有一个程序运行
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 2005-09-20
 *
 * \author 
 *
 * \par license
 * 
 * \todo 
 *
 * \bug 
 *
 */
class AFX_EXT_CLASS CBioInstanceChecker : public CObject
{
public:
  //Constructors / Destructors
  CBioInstanceChecker();
  ~CBioInstanceChecker();

  //General functions
  //查找第一个正在运行的实例
  BOOL TrackFirstInstanceRunning();
  //查找前一个正在运行的实例
  BOOL PreviousInstanceRunning();
  //激活前一个实例
  HWND ActivatePreviousInstance(); 
  
protected:
    //内存映射文件名
    CString MakeMMFFilename();
	//势力互斥对象
	CMutex m_instanceDataMutex;
	//前一实例句柄
	HANDLE m_hPrevInstance;
};
