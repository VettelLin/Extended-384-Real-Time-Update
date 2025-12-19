#pragma once

#include <string>
#include <tchar.h>
#include <fstream>
#include <vector>

using namespace std;

// 公用函数，一般是Static
class AFX_EXT_CLASS CPublicFun
{
public:
	CPublicFun(void) {}
	~CPublicFun(void){}

public:
	static int MyRound(double r);
	static void SplitCombineStringByFlag(CString strCombine, CStringArray* pArray, CString strFlag=_T(";"));
	static CString GetCurrentDateTime(BOOL bHasMS=TRUE);

	static CString GetExeFileDirectory();
	static BOOL ValidateFileOrDirWChar(const wchar_t& fileDirWChar);
	static int CheckAndCreateFileOrDirPath(wstring fileDirPath);

	static void WriteStringToFile(CFile* pFile, CString strInput);
	static CString ReadStringFromFile(CFile* pFile);

	static COLORREF ConverToColor(CString strColor);

	static double GetDoublePrecision(double dValue,int nPres=3);
};

