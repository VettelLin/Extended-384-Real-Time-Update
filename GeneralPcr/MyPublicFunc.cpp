
#include "stdafx.h"
#include <string>
#include <xstring>
#include <tchar.h>
#include <fstream>
#include <sstream>
#include <io.h>
#include "Define.h"
#include "MyPublicFunc.h"

using namespace std;

#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

CString arrstrRowLable[8] = {_T("A"),_T("B"),_T("C"),_T("D"),_T("E"),_T("F"),_T("G"),_T("H")};
CString arrstrColLable[12] = {_T("1"),_T("2"),_T("3"),_T("4"),_T("5"),_T("6"),_T("7"),_T("8"),_T("9"),_T("10"),_T("11"),_T("12")};

void GetCurrentTmTime(string& tmString)
{
	time_t timer = time(NULL);
	struct tm* localTime = localtime(&timer);
	char standardFormatTime[20];
	memset(standardFormatTime,0,sizeof(standardFormatTime));
	sprintf(standardFormatTime,"%d-%02d-%02d-%02d-%02d-%02d",localTime->tm_year + 1900,localTime->tm_mon + 1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);
	tmString += standardFormatTime;
}

CString GetExeFileDirectory()
{
	TCHAR filePath[MAX_PATH];
	CString fileDirectory(_T(""));
	memset(filePath,0,MAX_PATH);
	::GetModuleFileName(NULL,filePath,MAX_PATH);
	(_tcsrchr(filePath,_T('\\')))[1] = 0;
	fileDirectory += filePath;
	return fileDirectory;  
}

void GetCurrentTmTime(CString& tmString)
{
	time_t timer = time(NULL);
	struct tm* localTime = localtime(&timer);
	TCHAR standardFormatTime[20];
	memset(&standardFormatTime,0,sizeof(standardFormatTime));
#ifdef _UNICODE
	wsprintf(standardFormatTime,_T("%d-%02d-%02d-%02d-%02d-%02d"),localTime->tm_year + 1900,localTime->tm_mon + 1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);
	tmString += standardFormatTime;
#else
	sprintf(standardFormatTime,_T("%d-%02d-%02d-%02d-%02d-%02d"),localTime->tm_year + 1900,localTime->tm_mon + 1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);
	tmString += standardFormatTime;
#endif
}

wstring GetCurrentTmTimeW()
{
	time_t timer = time(NULL);
	struct tm* localTime = localtime(&timer);
	wchar_t standardFormatTime[20];
	memset(&standardFormatTime,0,sizeof(standardFormatTime));
	wsprintf(standardFormatTime,_T("%d-%02d-%02d-%02d-%02d-%02d"),localTime->tm_year + 1900,localTime->tm_mon + 1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);

	return standardFormatTime;
}

string GetCurrentTmTimeA()
{
	time_t timer = time(NULL);
	struct tm* localTime = localtime(&timer);
	char standardFormatTime[20];
	memset(&standardFormatTime,0,sizeof(standardFormatTime));
	sprintf(standardFormatTime,"%d-%02d-%02d-%02d-%02d-%02d",localTime->tm_year + 1900,localTime->tm_mon + 1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);

	return standardFormatTime;
}

CStringA GetLisCurrentTmTimeA()
{
	time_t timer = time(NULL);
	struct tm* localTime = localtime(&timer);
	
	CStringA strTmA(_T(""));
	strTmA.Format("%d%02d%02d%02d%02d%02d",localTime->tm_year + 1900,localTime->tm_mon + 1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);

	return strTmA;
}

time_t GetCurrentTmTime(CString& strTmTime,int format/* = 1*/,CString yMDSeparator /*= _T("/")*/,CString hMSSeparator /*= _T(":")*/,CString dHSeparator /*= _T(" ")*/)
{
	time_t timer = time(NULL);
	struct tm* localTime = localtime(&timer);
	TCHAR stdFormatTime[20];
	memset(&stdFormatTime,0,sizeof(stdFormatTime));
	if(2 == format)
	{
		wsprintf(stdFormatTime,_T("%02d%s%02d%s%02d"),localTime->tm_year + 1900,yMDSeparator,localTime->tm_mon + 1,yMDSeparator,localTime->tm_mday);
	}
	else if(3 == format)
	{
		wsprintf(stdFormatTime,_T("%d%s%02d%s%02d%s%02d%s%02d%s%02d"),localTime->tm_year + 1900,yMDSeparator,localTime->tm_mon + 1,yMDSeparator,localTime->tm_mday,dHSeparator,localTime->tm_hour,hMSSeparator,localTime->tm_min,hMSSeparator,localTime->tm_sec);
	}
	else if(4 == format)
	{
		wsprintf(stdFormatTime,_T("%d%s%02d%s%02d%s%02d%s%02d"),localTime->tm_year + 1900,yMDSeparator,localTime->tm_mon + 1,yMDSeparator,localTime->tm_mday,dHSeparator,localTime->tm_hour,hMSSeparator,localTime->tm_min);
	}
	else
	{
		wsprintf(stdFormatTime,_T("%02d%s%02d%s%02d"),localTime->tm_hour,hMSSeparator,localTime->tm_min,hMSSeparator,localTime->tm_sec);
	}

	strTmTime = stdFormatTime;

	return timer;
}

CString GetCurrentTmTime(int format /*= 1*/,CString yMDSeparator /*= _T("/")*/,CString hMSSeparator /*= _T(":")*/,CString dHSeparator /*= _T(" ")*/)
{
	time_t timer = time(NULL);
	struct tm* localTime = localtime(&timer);
	TCHAR stdFormatTime[20];
	memset(&stdFormatTime,0,sizeof(stdFormatTime));
	if(2 == format)
	{
		wsprintf(stdFormatTime,_T("%02d%s%02d%s%02d"),localTime->tm_year + 1900,yMDSeparator,localTime->tm_mon + 1,yMDSeparator,localTime->tm_mday);
	}
	else if(3 == format)
	{
		wsprintf(stdFormatTime,_T("%d%s%02d%s%02d%s%02d%s%02d%s%02d"),localTime->tm_year + 1900,yMDSeparator,localTime->tm_mon + 1,yMDSeparator,localTime->tm_mday,dHSeparator,localTime->tm_hour,hMSSeparator,localTime->tm_min,hMSSeparator,localTime->tm_sec);
	}
	else if(4 == format)
	{
		wsprintf(stdFormatTime,_T("%d%s%02d%s%02d%s%02d%s%02d"),localTime->tm_year + 1900,yMDSeparator,localTime->tm_mon + 1,yMDSeparator,localTime->tm_mday,dHSeparator,localTime->tm_hour,hMSSeparator,localTime->tm_min);
	}
	else
	{
		wsprintf(stdFormatTime,_T("%02d%s%02d%s%02d"),localTime->tm_hour,hMSSeparator,localTime->tm_min,hMSSeparator,localTime->tm_sec);
	}

	return stdFormatTime;
}

CString TotalSecondToHourMinSec(int totalSec)
{
	int hour = totalSec / 3600;
	totalSec -= hour * 3600;
	int minute = totalSec / 60;
	int second = totalSec - minute * 60;

	CString strInfo(_T(""));
	strInfo.Format(_T("%02d:%02d:%02d"),hour,minute,second);
	return strInfo;
}

void DrawRoundRectangle(CDC* pDC,float x,float y,float width,float height,float arcSize,Color lineColor,float lineWidth,BOOL bFillPath,Color fillColor,int rountType /*= 1*/)
{
	if(NULL == pDC->GetSafeHdc())
	{
		return;
	}

	float halfWidth = arcSize;
	float halfHeight = arcSize;

	Graphics graphics(pDC->GetSafeHdc());
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	//»æÍ¼Â·¾¶
	GraphicsPath roundRectPath;

	//±£´æ»æÍ¼Â·¾¶
	if(2 == rountType)	//ÉÏÏÂÔ²½Ç
	{
		roundRectPath.AddLine(x + width,y + halfHeight,x + width,y + height - halfHeight);	//ÓÒ²àÊúÏß
		roundRectPath.AddArc(x,y + height - 2 * halfHeight,halfWidth,2 * halfHeight,0,90); //ÓÒÏÂÔ²½Ç
		roundRectPath.AddArc(x,y + height - 2 * halfHeight,halfWidth,2 * halfHeight,90,90); //ÓÒÏÂÔ²½Ç

		roundRectPath.AddLine(x,y + height - halfHeight,x,y + halfHeight);	//×ó²àÊúÏß
		roundRectPath.AddArc(x,y,halfWidth,2 * halfHeight,180,90);	//×óÉÏÔ²½Ç
		roundRectPath.AddArc(x,y,halfWidth,2 * halfHeight,180,180);	//×óÉÏÔ²½Ç
	}
	else if(3 == rountType)	//×óÓÒÔ²½Ç 
	{
		roundRectPath.AddLine(x + halfWidth,y,x + width - halfWidth,y);	//¶¥²¿ºáÏß
		roundRectPath.AddArc(x + width - 2 * halfWidth,y,2 * halfWidth,halfHeight,270,90); //ÓÒÉÏÔ²½Ç
		roundRectPath.AddArc(x + width - 2 * halfWidth,y,2 * halfWidth,halfHeight,270,270); //ÓÒÉÏÔ²½Ç

		roundRectPath.AddLine(x + width - halfWidth,y + height,x + halfWidth,y + height);	//µ×²¿ºáÏß
		roundRectPath.AddArc(x,y,2 * halfWidth,halfHeight,0,90);	//×óÏÂÔ²½Ç
		roundRectPath.AddArc(x,y,2 * halfWidth,halfHeight,90,90);	//×óÏÂÔ²½Ç
	}
	else if(4 == rountType)	//×óÓÒ°ëÔ²½Ç
	{
		roundRectPath.AddLine(x + halfWidth,y,x + width - halfWidth,y);	//¶¥²¿ºáÏß
		roundRectPath.AddArc(x + width - 2 * halfWidth,y,2 * halfWidth,height,270,180);	//ÓÒ²à°ëÔ²½Ç
		roundRectPath.AddLine(x + width - halfWidth,y + height,x + halfWidth,y + height);	//µ×²¿ºáÏß
		roundRectPath.AddArc(x,y,2 * halfWidth,height,270,-180); //×ó²à°ëÔ²½Ç
	}
	else
	{
		roundRectPath.AddLine(x + halfWidth,y,x + width - halfWidth,y);	//¶¥²¿ºáÏß
		roundRectPath.AddArc(x + width - 2 * halfWidth,y,2 * halfWidth,2 * halfHeight,270,90); //ÓÒÉÏÔ²½Ç

		roundRectPath.AddLine(x + width,y + halfHeight,x + width,y + height - halfHeight);	//ÓÒ²àÊúÏß
		roundRectPath.AddArc(x + width - 2 * halfWidth,y + height - 2 * halfHeight,2 * halfWidth,2 * halfHeight,0,90); //ÓÒÏÂÔ²½Ç

		roundRectPath.AddLine(x + width - halfWidth,y + height,x + halfWidth,y + height);	//µ×²¿ºáÏß
		roundRectPath.AddArc(x,y + height - 2 * halfHeight,2 * halfWidth,2 * halfHeight,90,90);	//×óÏÂÔ²½Ç

		roundRectPath.AddLine(x,y + height - halfHeight,x,y + halfHeight);	//×ó²àÊúÏß
		roundRectPath.AddArc(x,y,2 * halfWidth,2 * halfHeight,180,90);	//×óÉÏÔ²½Ç
	}
	Pen pen(lineColor,lineWidth);
	graphics.DrawPath(&pen,&roundRectPath);
	if(!bFillPath)
	{
		graphics.ReleaseHDC(pDC->m_hDC);
		return;
	}
	else if(fillColor.GetAlpha() == 0)
	{
		fillColor = lineColor;	//ÈôÎ´Ö¸¶¨Ìî³äÉ«£¬ÔòÓÃÏßÌõÉ«Ìî³ä
	}

	SolidBrush brush(fillColor);
	graphics.FillPath(&brush,&roundRectPath);

	graphics.ReleaseHDC(pDC->m_hDC);
	return;
}

void DrawRoundRectangleNew(CDC* pDC,float x,float y,float width,float height,float arcSizeLT,float arcSizeRT,float arcSizeRB,float arcSizeLB,BOOL bShowLine,Color lineColor,float lineWidth,BOOL bFillPath,Color fillColor,int rountType /*= 1*/)
{
	if(NULL == pDC->GetSafeHdc())
	{
		return;
	}

	float halfWidth = arcSizeLT;
	float halfHeight = arcSizeLT;

	Graphics graphics(pDC->GetSafeHdc());
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	//»æÍ¼Â·¾¶
	GraphicsPath roundRectPath;

	//±£´æ»æÍ¼Â·¾¶
	if(2 == rountType)	//ÉÏÏÂÔ²½Ç
	{
		roundRectPath.AddLine(x + width,y + halfHeight,x + width,y + height - halfHeight);	//ÓÒ²àÊúÏß
		roundRectPath.AddArc(x,y + height - 2 * halfHeight,halfWidth,2 * halfHeight,0,90); //ÓÒÏÂÔ²½Ç
		roundRectPath.AddArc(x,y + height - 2 * halfHeight,halfWidth,2 * halfHeight,90,90); //ÓÒÏÂÔ²½Ç

		roundRectPath.AddLine(x,y + height - halfHeight,x,y + halfHeight);	//×ó²àÊúÏß
		roundRectPath.AddArc(x,y,halfWidth,2 * halfHeight,180,90);	//×óÉÏÔ²½Ç
		roundRectPath.AddArc(x,y,halfWidth,2 * halfHeight,180,180);	//×óÉÏÔ²½Ç
	}
	else if(3 == rountType)	//×óÓÒÔ²½Ç 
	{
		roundRectPath.AddLine(x + halfWidth,y,x + width - halfWidth,y);	//¶¥²¿ºáÏß
		roundRectPath.AddArc(x + width - 2 * halfWidth,y,2 * halfWidth,halfHeight,270,90); //ÓÒÉÏÔ²½Ç
		roundRectPath.AddArc(x + width - 2 * halfWidth,y,2 * halfWidth,halfHeight,270,270); //ÓÒÉÏÔ²½Ç

		roundRectPath.AddLine(x + width - halfWidth,y + height,x + halfWidth,y + height);	//µ×²¿ºáÏß
		roundRectPath.AddArc(x,y,2 * halfWidth,halfHeight,0,90);	//×óÏÂÔ²½Ç
		roundRectPath.AddArc(x,y,2 * halfWidth,halfHeight,90,90);	//×óÏÂÔ²½Ç
	}
	else if(4 == rountType)	//×óÓÒ°ëÔ²½Ç
	{
		roundRectPath.AddLine(x + halfWidth,y,x + width - halfWidth,y);	//¶¥²¿ºáÏß
		roundRectPath.AddArc(x + width - 2 * halfWidth,y,2 * halfWidth,height,270,180);	//ÓÒ²à°ëÔ²½Ç
		roundRectPath.AddLine(x + width - halfWidth,y + height,x + halfWidth,y + height);	//µ×²¿ºáÏß
		roundRectPath.AddArc(x,y,2 * halfWidth,height,270,-180); //×ó²à°ëÔ²½Ç
	}
	else
	{
		roundRectPath.AddLine(x + arcSizeLT,y,x + width - arcSizeRT,y);	//¶¥²¿ºáÏß
		roundRectPath.AddArc(x + width - 2 * arcSizeRT,y,2 * arcSizeRT,2 * arcSizeRT,270,90); //ÓÒÉÏÔ²½Ç

		roundRectPath.AddLine(x + width,y + arcSizeRT,x + width,y + height - arcSizeRB);	//ÓÒ²àÊúÏß
		roundRectPath.AddArc(x + width - 2 * arcSizeRB,y + height - 2 * arcSizeRB,2 * arcSizeRB,2 * arcSizeRB,0,90); //ÓÒÏÂÔ²½Ç

		roundRectPath.AddLine(x + width - arcSizeRB,y + height,x + arcSizeLB,y + height);	//µ×²¿ºáÏß
		roundRectPath.AddArc(x,y + height - 2 * arcSizeLB,2 * arcSizeLB,2 * arcSizeLB,90,90);	//×óÏÂÔ²½Ç

		roundRectPath.AddLine(x,y + height - arcSizeLB,x,y + arcSizeLT);	//×ó²àÊúÏß
		roundRectPath.AddArc(x,y,2 * arcSizeLT,2 * arcSizeLT,180,90);	//×óÉÏÔ²½Ç
	}

	if(bFillPath)
	{
		if(fillColor.GetAlpha() == 0)
		{
			fillColor = lineColor;	//ÈôÎ´Ö¸¶¨Ìî³äÉ«£¬ÔòÓÃÏßÌõÉ«Ìî³ä
		}
		SolidBrush brush(fillColor);
		graphics.FillPath(&brush,&roundRectPath);
	}

	if(bShowLine)
	{
		Pen pen(lineColor,lineWidth);
		graphics.DrawPath(&pen,&roundRectPath);
	}

	graphics.ReleaseHDC(pDC->m_hDC);
	return;
}

void DrawGradientRoundRectangle(CDC* pDC,float x,float y,float width,float height,float arcSizeLT,float arcSizeRT,float arcSizeRB,float arcSizeLB,BOOL bShowLine,Color lineColor,float lineWidth,BOOL bFillPath,Color clrFillStart,Color clrFillEnd,int rountType /*= 1*/)
{
	if(NULL == pDC->GetSafeHdc())
	{
		return;
	}

	float halfWidth = arcSizeLT;
	float halfHeight = arcSizeLT;

	Graphics graphics(pDC->GetSafeHdc());
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	//»æÍ¼Â·¾¶
	GraphicsPath roundRectPath;

	//±£´æ»æÍ¼Â·¾¶
	if(2 == rountType)	//ÉÏÏÂÔ²½Ç
	{
		roundRectPath.AddLine(x + width,y + halfHeight,x + width,y + height - halfHeight);	//ÓÒ²àÊúÏß
		roundRectPath.AddArc(x,y + height - 2 * halfHeight,halfWidth,2 * halfHeight,0,90); //ÓÒÏÂÔ²½Ç
		roundRectPath.AddArc(x,y + height - 2 * halfHeight,halfWidth,2 * halfHeight,90,90); //ÓÒÏÂÔ²½Ç

		roundRectPath.AddLine(x,y + height - halfHeight,x,y + halfHeight);	//×ó²àÊúÏß
		roundRectPath.AddArc(x,y,halfWidth,2 * halfHeight,180,90);	//×óÉÏÔ²½Ç
		roundRectPath.AddArc(x,y,halfWidth,2 * halfHeight,180,180);	//×óÉÏÔ²½Ç
	}
	else if(3 == rountType)	//×óÓÒÔ²½Ç 
	{
		roundRectPath.AddLine(x + halfWidth,y,x + width - halfWidth,y);	//¶¥²¿ºáÏß
		roundRectPath.AddArc(x + width - 2 * halfWidth,y,2 * halfWidth,halfHeight,270,90); //ÓÒÉÏÔ²½Ç
		roundRectPath.AddArc(x + width - 2 * halfWidth,y,2 * halfWidth,halfHeight,270,270); //ÓÒÉÏÔ²½Ç

		roundRectPath.AddLine(x + width - halfWidth,y + height,x + halfWidth,y + height);	//µ×²¿ºáÏß
		roundRectPath.AddArc(x,y,2 * halfWidth,halfHeight,0,90);	//×óÏÂÔ²½Ç
		roundRectPath.AddArc(x,y,2 * halfWidth,halfHeight,90,90);	//×óÏÂÔ²½Ç
	}
	else if(4 == rountType)	//×óÓÒ°ëÔ²½Ç
	{
		roundRectPath.AddLine(x + halfWidth,y,x + width - halfWidth,y);	//¶¥²¿ºáÏß
		roundRectPath.AddArc(x + width - 2 * halfWidth,y,2 * halfWidth,height,270,180);	//ÓÒ²à°ëÔ²½Ç
		roundRectPath.AddLine(x + width - halfWidth,y + height,x + halfWidth,y + height);	//µ×²¿ºáÏß
		roundRectPath.AddArc(x,y,2 * halfWidth,height,270,-180); //×ó²à°ëÔ²½Ç
	}
	else
	{
		roundRectPath.AddLine(x + arcSizeLT,y,x + width - arcSizeRT,y);	//¶¥²¿ºáÏß
		roundRectPath.AddArc(x + width - 2 * arcSizeRT,y,2 * arcSizeRT,2 * arcSizeRT,270,90); //ÓÒÉÏÔ²½Ç

		roundRectPath.AddLine(x + width,y + arcSizeRT,x + width,y + height - arcSizeRB);	//ÓÒ²àÊúÏß
		roundRectPath.AddArc(x + width - 2 * arcSizeRB,y + height - 2 * arcSizeRB,2 * arcSizeRB,2 * arcSizeRB,0,90); //ÓÒÏÂÔ²½Ç

		roundRectPath.AddLine(x + width - arcSizeRB,y + height,x + arcSizeLB,y + height);	//µ×²¿ºáÏß
		roundRectPath.AddArc(x,y + height - 2 * arcSizeLB,2 * arcSizeLB,2 * arcSizeLB,90,90);	//×óÏÂÔ²½Ç

		roundRectPath.AddLine(x,y + height - arcSizeLB,x,y + arcSizeLT);	//×ó²àÊúÏß
		roundRectPath.AddArc(x,y,2 * arcSizeLT,2 * arcSizeLT,180,90);	//×óÉÏÔ²½Ç
	}

	if(bFillPath)
	{
		if(clrFillStart.GetAlpha() == 0)
		{
			clrFillStart = lineColor;	//ÈôÎ´Ö¸¶¨Ìî³äÉ«£¬ÔòÓÃÏßÌõÉ«Ìî³ä
		}
		if(clrFillEnd.GetAlpha() == 0)
		{
			clrFillEnd = lineColor;	
		}

		LinearGradientBrush linGrBrush(RectF(0,0,width,height),clrFillStart,clrFillEnd,LinearGradientModeVertical);
		graphics.FillPath(&linGrBrush,&roundRectPath);
	}

	if(bShowLine)
	{
		Pen pen(lineColor,lineWidth);
		graphics.DrawPath(&pen,&roundRectPath);
	}

	graphics.ReleaseHDC(pDC->m_hDC);
	return;
}

void DrawRound(CDC* pDC,CRect rect,BOOL bLine,Color lineColor,float lineWidth,BOOL bFillPath,Color fillColor)
{
	if(NULL == pDC->GetSafeHdc())
	{
		return;
	}

	Graphics graphics(pDC->GetSafeHdc());
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	//»æÍ¼Â·¾¶
	GraphicsPath roundRectPath;

	//±£´æ»æÍ¼Â·¾¶
	roundRectPath.AddEllipse(rect.left,rect.top,rect.Width() - 1,rect.Height() - 1);
	
	if(bFillPath)
	{
		if(fillColor.GetAlpha() == 0)
		{
			fillColor = lineColor;	//ÈôÎ´Ö¸¶¨Ìî³äÉ«£¬ÔòÓÃÏßÌõÉ«Ìî³ä
		}
		SolidBrush brush(fillColor);
		graphics.FillPath(&brush,&roundRectPath);
	}

	if(bLine)
	{
		Pen pen(lineColor,lineWidth);
		graphics.DrawPath(&pen,&roundRectPath);
	}

	graphics.ReleaseHDC(pDC->m_hDC);
	return;
}

int GetSystemComNames(vector<CString>& vecComName)
{
	vecComName.clear();
	HKEY hKey;
	LSTATUS retValue = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("HARDWARE\\DEVICEMAP\\SERIALCOMM\\"),NULL,KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | KEY_READ,&hKey);
	if(ERROR_SUCCESS == retValue)
	{
		TCHAR portName[256];
		memset(portName,0,sizeof(portName));
		DWORD dwLong,dwSize,dwType;
		int dwIndex = 0;
		TCHAR comName[20] = {0};
		CString tempStr(_T(""));
		while(1)
		{ 
			dwLong = sizeof(portName); //Ã¿±éÀúÒ»´Î£¬dwLongºÍdwSize¶¼Òª³õÊ¼»¯£¬·ñÔò¿ÉÄÜ»á³öÏÖ×¢²á±íÖÐµÄ²¿·ÖCOMºÅÎÞ·¨¶Á³ö
			dwSize = 14;  //COM255×Ö·û´®×î¶àÕ¼ÓÃ14¸ö×Ö½Ú

			if(ERROR_NO_MORE_ITEMS == ::RegEnumValueW(hKey,dwIndex++,portName,&dwLong,NULL,&dwType,(PUCHAR)comName,&dwSize))
			{
				return 1;  //ÕÒ²»µ½ºÏÊÊµÄ´®¿Ú£¬ÍË³öÑ­»·
			}
			else
			{
				tempStr = comName;
				vecComName.push_back(tempStr);
			}
		}
	}
	else
	{
		return 0;
	}
} 

int SiteStrToSamId(CString strSite)
{
	int siteId = 0;
	if(strSite.GetLength() < 2 || strSite.GetLength() > 3)
	{
		return siteId;
	}

	TCHAR charRowId = strSite.GetBuffer()[0]; 
	int rowId = charRowId - _T('A');
	CString strColId = strSite.Mid(1);
	int colId = _ttoi(strColId);

	siteId = rowId * 12 + colId - 1;
	return siteId;
}

int CheckAndCreateFileOrDirPath(wstring fileDirPath)
{
	if(_T("") == fileDirPath)
	{
		return -1;
	}

	vector<int> slashIndex,dotIndex,iterantSlashIndex;
	slashIndex.clear();
	dotIndex.clear();
	const wchar_t* pCWPathData = fileDirPath.c_str();

	if(!ValidateFileOrDirWChar(*pCWPathData))
	{
		return -2;		
	}

	if(L'\\' == *pCWPathData || L'/' == *pCWPathData)
	{
		return -3;
	}

	int index = 0;
	while(0 != *(++pCWPathData))
	{
		++index;
		if(!ValidateFileOrDirWChar(*pCWPathData))
		{
			return -2;		
		}

		if(L'\\' == *pCWPathData || L'/' == *pCWPathData)
		{
			if(slashIndex.size() > 0)
			{
				if(index - 1 == slashIndex[slashIndex.size() - 1])
				{
					iterantSlashIndex.push_back(index);
					continue;
				}
			}
			slashIndex.push_back(index);
		}
		else if(L'.' == *pCWPathData)
		{
			dotIndex.push_back(index);
		}
	}

	wstring subWStr = _T("");

	if(0 == slashIndex.size())
	{
		wstring defaultWStr = GetExeFileDirectory().GetBuffer();
		defaultWStr += (_T("\\") + fileDirPath);
		ifstream ifs(defaultWStr);
		if(!ifs)
		{
			ofstream ofs(defaultWStr);
			ofs.close();
		}
		else
		{
			ifs.close();
		}
		return 1;
	}
	else if(0 == dotIndex.size())
	{
		for(int i = 0;i < slashIndex.size();++i)
		{
			subWStr = fileDirPath.substr(0,slashIndex[i] + 1);

			if(!PathIsDirectoryW(subWStr.c_str()))
			{
				if(!CreateDirectoryW(subWStr.c_str(),NULL))
				{
					return -4;
				}
			} 
		}
		if(fileDirPath.size() - 1 != slashIndex[slashIndex.size() - 1])
		{
			if(!PathIsDirectoryW(fileDirPath.c_str()))
			{
				if(!CreateDirectoryW(fileDirPath.c_str(),NULL))
				{
					return -5;
				}
			} 
		}
		return 2;
	}
	else
	{
		for(int i = 0;i < slashIndex.size();++i)
		{
			subWStr = fileDirPath.substr(0,slashIndex[i] + 1);

			if(!PathIsDirectoryW(subWStr.c_str()))
			{
				if(!CreateDirectoryW(subWStr.c_str(),NULL))
				{
					return -6;
				}
			} 
		}

		if(dotIndex[dotIndex.size() - 1] > slashIndex[slashIndex.size() - 1])
		{
			ifstream ifs(fileDirPath);
			if(!ifs)
			{
				ofstream ofs(fileDirPath);
				ofs.close();
			}
			else
			{
				ifs.close();
			}
			return 3;
		}
		else
		{
			if(fileDirPath.size() - 1 != slashIndex[slashIndex.size() - 1])
			{
				if(!PathIsDirectoryW(fileDirPath.c_str()))
				{
					if(!CreateDirectoryW(fileDirPath.c_str(),NULL))
					{
						return -7;
					}
				} 
			}
			return 4;
		}
	}
}

BOOL ValidateFileOrDirWChar(const wchar_t& fileDirWChar)
{
	if(L'*' == fileDirWChar || L'\?' == fileDirWChar || L'\"' == fileDirWChar || L'<' == fileDirWChar || L'>' == fileDirWChar || L'|' == fileDirWChar)
	{
		return FALSE;
	}
	return TRUE;
}

CString SampleIdToHoleInfo(int samId)
{
	int row = samId / 12;
	int col = samId % 12;
	return arrstrRowLable[row] + arrstrColLable[col];
}

void IndexToRowColId(int index,int& rowId,int& colId)
{
	rowId = index / 12;
	colId = index % 12;
	return;
}

CString SecondToHourMinuteSecond(int iTotalSec)
{
	if(iTotalSec < 0)
	{
		iTotalSec = 0;
	}
	int iHour = iTotalSec / 3600;
	iTotalSec = iTotalSec % 3600;
	int iMinute = iTotalSec / 60;
	int iSec = iTotalSec % 60;
	CString strTemp(_T(""));
	strTemp.Format(_T("%02d:%02d:%02d"),iHour,iMinute,iSec);

	return strTemp;
}

int DrawLegend(CDC* pDc,CRect rcLegend,CLegendInfo& lgdInfo)
{
	if(NULL == pDc)
	{
		return -1;
	}

	if(rcLegend.IsRectEmpty())
	{
		return -2;
	}

	int iLgdCount = lgdInfo.GetSingleLegendCount();
	if(iLgdCount < 1)
	{
		return -3;
	}

	CFont* pOldFont = pDc->SelectObject(&lgdInfo.m_fontSet);
	pDc->SetTextColor(lgdInfo.m_txClr1);
	pDc->SetBkMode(TRANSPARENT);

	DrawGradientRoundRectangle(pDc,rcLegend.left,rcLegend.top,rcLegend.Width() - 1,rcLegend.Height() - 1,0,0,8,8,TRUE,lgdInfo.m_clrFrame,1,TRUE,lgdInfo.m_clrBk,lgdInfo.m_clrBkEnd);
	pDc->FillSolidRect(rcLegend.left,rcLegend.top,rcLegend.Width(),1,RGB(lgdInfo.m_clrBk.GetR(),lgdInfo.m_clrBk.GetG(),lgdInfo.m_clrBk.GetB()));

	int iX = rcLegend.left + lgdInfo.m_iLeftIndent;
	int iDiam = lgdInfo.m_iRoundRadius * 2;

	CSingleLgdInfo* pSingleLgdInfo = NULL;
	CRect rcTemp;
	for(int i = 0;i < iLgdCount;++i)
	{
		pSingleLgdInfo = lgdInfo.GetSingleLegendInfo(i);
		if(NULL == pSingleLgdInfo)
		{
			continue;
		}

		rcTemp.left = iX;
		rcTemp.right = rcTemp.left + iDiam;
		rcTemp.top = rcLegend.top + (rcLegend.Height() - iDiam) / 2;
		rcTemp.bottom = rcTemp.top + iDiam;
		DrawRound(pDc,rcTemp,TRUE,pSingleLgdInfo->m_clrFrame,pSingleLgdInfo->m_pxFrameW,TRUE,pSingleLgdInfo->m_clrFill);

		iX += iDiam;
		iX += lgdInfo.m_iInrIconToTx;
		rcTemp.left = iX;
		rcTemp.right = rcTemp.left + pSingleLgdInfo->m_strTx.GetLength() * lgdInfo.m_iOneCharW;
		rcTemp.top = rcLegend.top;
		rcTemp.bottom = rcLegend.bottom;
		pDc->DrawText(pSingleLgdInfo->m_strTx,&rcTemp,DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		iX += rcTemp.Width();
		iX += lgdInfo.m_iInrTxToIcon;
	}

	pDc->SelectObject(pOldFont);

	return 0;
}

void KdsptForwardMBackN_New(int n, double *pY, double *pReturn,int forwardM,int backwardN)
{
	if(NULL == pY || NULL == pReturn)
	{
		return;
	}

	if(forwardM < 0 || backwardN < 0)
	{
		memcpy(pReturn,pY,n * sizeof(double));
		return;
	}

	if(n < forwardM + backwardN + 1)
	{
		memcpy(pReturn,pY,n * sizeof(double));
		return;
	}

	int start = 0;
	int end = 0;
	int count = 0;
	double dSum = 0;
	for(int i = 0;i < n;++i)
	{
		dSum = 0;
		count = 0;
		if(i < forwardM)
		{
			start = 0;
			end = i + backwardN;
		}
		else if(i >= forwardM && i < n - backwardN)
		{
			 start = i - forwardM;
			 end = i + backwardN;
		}
		else
		{
			start = i - forwardM;
			end = n - 1;
		}

		if(forwardM == backwardN)
		{
			int nPre = i - start ;
			int nBehind = end - i;
			(nPre > nBehind) ?(start = i - nBehind):(end = i + nPre);
		}
		else
		{
			//if(i < forwardM)
			//{
			//	int nPre = i - start ;
			//	int nBehind = end - i;
			//	(nPre > nBehind) ?(start = i - nBehind):(end = i + nPre);
			//}
			//else if(i > (n - 1 - backwardN))
			//{
			//	int nPre = i - start ;
			//	int nBehind = end - i;
			//	(nPre > nBehind) ?(start = i - nBehind):(end = i + nPre);
			//	//start = end = i;
			//}

			/*if(i == n - 1)
			{
			start = end = i;
			}*/
		}

		for(int k = start;k <= end;++k)
		{
			dSum += pY[k];
			++count;
		}

		//Ê¹ÓÃ×îºóÒ»¸öÊý¾ÝÀ©³äÊý¾Ý¶ÓÁÐÓÃÓÚÂË²¨
		if(forwardM != backwardN)
		{
			if(i > (n - 1 - backwardN))
			{
				for(int k = i + backwardN;k > n - 1;--k)
				{
					dSum += pY[n - 1];
					++count;
				}
			}
		}

		if(0 != count)
		{
			pReturn[i] = dSum / count;
		}
		else
		{
			pReturn[i] = pY[i];
		}
	}

	return;
}

int LeastSquare(double *pDx,double *pDy,int dataNum,double &dSlope,double &dIntercept)
{
	if(NULL == pDx)
	{
		return -1;
	}
	if(NULL == pDy)
	{
		return -2;
	}

	if(dataNum < 2)
	{
		return -3;
	}

	double s0 = dataNum, s1 = 0, s2 = 0, t0 = 0, t1 = 0;
	for(int i = 0;i < dataNum;++i)
	{
		s1 += pDx[i];
		s2 += (pDx[i] * pDx[i]);
		t0 += pDy[i];
		t1 += (pDx[i] * pDy[i]);
	}
	double d = s0 * s2 - s1 * s1;

	if(0 == d)
	{
		return -4;
	}

	dIntercept = (s2 * t0 - s1 * t1) / d;
	dSlope = (s0 * t1 - s1 * t0) / d;
	return 0;
}

CString GetFileNameFromPath(CString strFilePath)
{
	if(strFilePath.IsEmpty())
	{
		return _T("");
	}

	int iSlantingBarIndex = strFilePath.ReverseFind(_T('\\'));
	int iDotIndex = strFilePath.ReverseFind(_T('.'));

	if(-1 == iDotIndex)
	{
		if(-1 == iSlantingBarIndex)
		{
			return strFilePath;
		}
		else
		{
			return strFilePath.Mid(iSlantingBarIndex + 1);
		}
	}
	else
	{
		if(-1 == iSlantingBarIndex)
		{
			return strFilePath.Left(iDotIndex);
		}
		else
		{
			return strFilePath.Mid(iSlantingBarIndex + 1,iDotIndex - iSlantingBarIndex - 1);
		}
	}
}

int GetIDPD(CString strData)
{
	if(strData.IsEmpty())
	{
		return 0;
	}

	int iIndex = strData.Find(_T('.'));
	if(-1 == iIndex)
	{
		return 0;
	}

	return strData.GetLength() - iIndex - 1;
}


