#pragma once
#include <vector>
#include "shlwapi.h"
#import "msxml3.dll"

class CGeneralOperator
{
public:
	CGeneralOperator(void){};
	~CGeneralOperator(void){};

	static void ExtractSubString(CString strSource, char ch, std::vector<CString>& vString)
	{
		int nBegin = 0;
		int nEnd = 0;

		nEnd = strSource.Find(ch, nBegin);

		while (nEnd != -1)
		{
			vString.push_back(strSource.Mid(nBegin, nEnd - nBegin));
			nBegin = nEnd + 1;
			nEnd = strSource.Find(ch, nBegin);
		}

		if (nBegin < strSource.GetLength())
		{
			vString.push_back(strSource.Right(strSource.GetLength() - nBegin));
		}		
	}

	///////////////////////////////////////////////////////////////////////
	/**	
	*	得到可执行程序的目录名
	*
	*	@return  可执行程序的目录名
	*	@author thh		@date 20050830
	*/
	static CString GetAppHome()
	{
		TCHAR pFileName[256]; 
		GetModuleFileName(NULL,pFileName,255);
		CString appHome;
		appHome = CString(pFileName);
		int nPos = appHome.ReverseFind( _T('\\') ); 
		if( nPos < 0 ) 
			appHome = CString(""); 
		else 
			appHome = appHome.Left(nPos);
		return appHome;
	}

	///////////////////////////////////////////////////////////////////////
	/**	
	*	数据转换为字符串
	*
	*	@param	 Type value			--  要转换的数据
	*	@param	 CString strFormat  --  要转换的格式字符串
	*	@return  转换后的字符串
	*	@author thh		@date 20050830
	*/
	template<class Type>
	static CString ExchangeValueToStr(Type value, CString strFormat)
	{
		CString strResult;
		strResult.Format(strFormat, value);
		return strResult;
	}

	template<class Type>
	static BOOL DeleteVectorItem(std::vector<Type> &strVect, int nIndex)
	{
		if(nIndex < 0 || strVect.size()-1 < static_cast<unsigned>(nIndex))
		{
			return FALSE;
		}
		for(unsigned int i = nIndex; i < strVect.size()-1; i++)
		{
			strVect[i] = strVect[i+1];
		}
		strVect.pop_back();
		return TRUE;
	}

	static CString ShowBrowseDlg(HWND wnd, CString strTitle)
	{
		CString str;
		str.Empty();
		BROWSEINFO bi;
		TCHAR name[MAX_PATH];
		ZeroMemory(&bi,sizeof(BROWSEINFO));
		bi.hwndOwner = wnd;
		bi.pszDisplayName = name;
		bi.lpszTitle = strTitle;
		bi.ulFlags = BIF_DONTGOBELOWDOMAIN |BIF_RETURNONLYFSDIRS ;
		LPITEMIDLIST idl=SHBrowseForFolder(&bi);
		if(idl==NULL)
			return str;
		SHGetPathFromIDList(idl,str.GetBuffer(MAX_PATH));
		str.ReleaseBuffer();
		return str;
	}

	static BOOL IsNumString(CString str)
	{
		for(int i = 0; i < str.GetLength(); i++)
		{
			if(!::isdigit(str.GetAt(i)))
				return FALSE;
		}
		return TRUE;
	}

	static CString GetPathLastName(CString strPath, BOOL bContainExt = FALSE)
	{
		std::vector<CString> vStr;
		CGeneralOperator::ExtractSubString(strPath, _T('\\'), vStr);
		CString strLastName = vStr.at(vStr.size() - 1);

		if(!bContainExt)
		{
			int nPos = -1;
			while( strLastName.Find(_T('.'), nPos + 1) != -1 )
			{
				nPos = strLastName.Find(_T('.'), nPos + 1);
			}
			strLastName = strLastName.Left(nPos);
		}
		return strLastName;
	}

	static BOOL IsFileExist(CString& strFile)
	{
		CFileFind ff;
		if(!ff.FindFile(strFile))
		{
			strFile = GetAppHome() + _T("\\") + strFile;
			if(!ff.FindFile(strFile))
				return FALSE;
		}
		return TRUE;
	}

	static BOOL CreateFolder(CString& strFolder)
	{
		int nPos = strFolder.ReverseFind(_T('\\'));
		strFolder = strFolder.Left(nPos);
		if(!PathFileExists(strFolder))
		{
			if(ERROR_SUCCESS != ::SHCreateDirectory(NULL, CStringW(strFolder)))
			{
				strFolder = GetAppHome() + _T("\\") + strFolder;
				if(!PathFileExists(strFolder))
				{
					if(ERROR_SUCCESS != ::SHCreateDirectory(NULL, CStringW(strFolder)))
						return FALSE;
				}
			}
		}
		return TRUE;
	}

	static CString LoadString(int idText)
	{
		CString str;
		if(str.LoadString(idText))
			return str;
		return _T("");
	}
};

#define ValueToStr(value, strformat) ( CGeneralOperator::ExchangeValueToStr(value, strformat) )