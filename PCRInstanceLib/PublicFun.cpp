#include "StdAfx.h"
#include "PublicFun.h"
#include "math.h"

int CPublicFun::MyRound(double r)
{
	return int((r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5));
}


void CPublicFun::SplitCombineStringByFlag(CString strCombine, CStringArray* pArray, CString strFlag)
{
	pArray->RemoveAll();

	while (!strCombine.IsEmpty())
	{
		int nPosition = strCombine.Find(strFlag);
		if (nPosition > 0)
		{
			CString stCol = strCombine.Left(nPosition);
			strCombine = strCombine.Right(strCombine.GetLength() - nPosition -1);
			pArray->Add(stCol.Trim());
		}
		else
		{
			break;
		}
	}

	if (!strCombine.IsEmpty())
	{
		pArray->Add(strCombine.Trim());
	}
}

CString CPublicFun::GetCurrentDateTime(BOOL bHasMS)
{
	CString strTime;
	SYSTEMTIME st;
	GetLocalTime(&st);
	if (bHasMS)
	{
		strTime.Format(_T("%4d-%02d-%02d %02d:%02d:%02d:%03d"),st.wYear, st.wMonth, st.wDay, st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	}
	else
	{
		strTime.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"),st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	}

	return strTime;
}


CString CPublicFun::GetExeFileDirectory()
{
	TCHAR filePath[MAX_PATH];
	CString fileDirectory(_T(""));
	memset(filePath,0,MAX_PATH);
	::GetModuleFileName(NULL,filePath,MAX_PATH);
	(_tcsrchr(filePath,_T('\\')))[1] = 0;
	fileDirectory += filePath;
	return fileDirectory;  
}

BOOL CPublicFun::ValidateFileOrDirWChar(const wchar_t& fileDirWChar)
{
	if(L'*' == fileDirWChar || L'\?' == fileDirWChar || L'\"' == fileDirWChar || L'<' == fileDirWChar || L'>' == fileDirWChar || L'|' == fileDirWChar)
	{
		return FALSE;
	}
	return TRUE;
}

int CPublicFun::CheckAndCreateFileOrDirPath(wstring fileDirPath)
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
		for(UINT i = 0;i < slashIndex.size();++i)
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
		for(UINT i = 0;i < slashIndex.size();++i)
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

void CPublicFun::WriteStringToFile(CFile* pFile, CString strInput)
{
	int nStrLength = strInput.GetLength();
	pFile->Write((const char *)&nStrLength, sizeof(int));
	if(nStrLength > 0)
	{
		pFile->Write((const char*)(strInput.GetBuffer()), sizeof(TCHAR)*nStrLength);
	}
}

CString CPublicFun::ReadStringFromFile(CFile* pFile)
{
	CString strRead;
	strRead.Empty();
	int nStrLength = 0;
	pFile->Read((char*)&nStrLength, sizeof(int));
	if(nStrLength > 0)
	{
		TCHAR lpTStr[1024] = {0};
		pFile->Read((char*)lpTStr, sizeof(TCHAR) * nStrLength);
		lpTStr[nStrLength] = 0; 
		strRead = lpTStr;
	}

	return strRead;
}

COLORREF CPublicFun::ConverToColor(CString strColor)
{
	COLORREF clrReturn = RGB(0, 0, 0);
	CStringArray arrayColor;
	SplitCombineStringByFlag(strColor, &arrayColor, _T(","));
	ASSERT(arrayColor.GetCount() == 3);
	int r = _ttoi(arrayColor.GetAt(0));
	int g = _ttoi(arrayColor.GetAt(1));
	int b = _ttoi(arrayColor.GetAt(2));

	clrReturn = RGB(r, g, b);

	return clrReturn;
}

double CPublicFun::GetDoublePrecision(double dValue,int nPres)
{
	double factor = pow(10.0,nPres);
	dValue = (double)((/*int*/__int64)(dValue*factor)/factor);
	return dValue;
}