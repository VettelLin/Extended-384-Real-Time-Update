#include "stdafx.h"
#include "BioInstanceChecker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////// Implementation //////////////////////////////

//struct which is put into shared memory
struct CWindowInstance
{
  HWND hMainWnd;
};

//Class which be used as a static to ensure that we
//only close the file mapping at the very last chance
class _INSTANCE_DATA
{
public:
  _INSTANCE_DATA();
  ~_INSTANCE_DATA();

protected:
  HANDLE hInstanceData;
  friend class CBioInstanceChecker;
};

_INSTANCE_DATA::_INSTANCE_DATA()
{
  hInstanceData = NULL;
}

_INSTANCE_DATA::~_INSTANCE_DATA()
{
  if (hInstanceData != NULL)
  {
    ::CloseHandle(hInstanceData);
    hInstanceData = NULL;
  }
}

static _INSTANCE_DATA instanceData;


CBioInstanceChecker::CBioInstanceChecker()
{
  // Only one object of type CBioInstanceChecker should be created
  VERIFY(instanceData.hInstanceData == NULL);

  m_hPrevInstance = NULL;
}

CBioInstanceChecker::~CBioInstanceChecker()
{
  //Close the MMF if need be
  if (m_hPrevInstance)
  {
    ::CloseHandle(m_hPrevInstance);
    m_hPrevInstance = NULL;
  }
}

/** Track the first instance of our App.
 *  Call this after LoadFrame() in InitInstance(). Call PreviousInstanceRunning() first
 *  and only call this if it returns false.
 *
 *  @return TRUE on success, else FALSE - another instance is already running.
 */
BOOL CBioInstanceChecker::TrackFirstInstanceRunning()
{
  VERIFY(PreviousInstanceRunning() == NULL);

  //If this is the first instance then copy in our info into the shared memory
  if (m_hPrevInstance == NULL)
  {
    //Create the MMF
    int nMMFSize = sizeof(CWindowInstance);
    instanceData.hInstanceData = ::CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, nMMFSize, MakeMMFFilename());
    VERIFY(instanceData.hInstanceData != NULL); //Creating the MMF should work

    //Open the MMF
    CWindowInstance* pInstanceData = (CWindowInstance*) ::MapViewOfFile(instanceData.hInstanceData, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, nMMFSize);
    VERIFY(pInstanceData != NULL);   //Opening the MMF should work

    // Lock the data prior to updating it
    CSingleLock dataLock(&m_instanceDataMutex, TRUE);

    ASSERT(AfxGetMainWnd() != NULL); //Did you forget to set up the mainfrm in InitInstance ?
    ASSERT(AfxGetMainWnd()->GetSafeHwnd() != NULL);
    pInstanceData->hMainWnd = AfxGetMainWnd()->GetSafeHwnd();

    VERIFY(::UnmapViewOfFile(pInstanceData));
  }

  return (m_hPrevInstance == NULL);
}

/** Returns true if a previous instance of the App is running.
 *  Call this at the very start of InitInstance().
 *  @see ActivatePreviousInstance(), TrackFirstInstanceRunning().
 */
BOOL CBioInstanceChecker::PreviousInstanceRunning()
{
  ASSERT(m_hPrevInstance == NULL); //Trying to call PreviousInstanceRunning twice

  //Try to open the MMF first to see if we are the second instance
  m_hPrevInstance = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MakeMMFFilename());

  return (m_hPrevInstance != NULL);
}

CString CBioInstanceChecker::MakeMMFFilename()
{
  //MMF name is taken from MFC's AfxGetAppName
  LPCTSTR pszAppName = AfxGetAppName(); 
  ASSERT(pszAppName);
  ASSERT(_tcslen(pszAppName)); //Missing the Application Title ?
  CString sMMF(_T("CBioInstanceChecker_MMF_"));
  sMMF += pszAppName;
  return sMMF;
}

/** Activate the Previous Instance of our Application.
 *  @note Call PreviousInstanceRunning() before calling this function.
 *  @return hWnd of the previous instance's MainFrame if successful, else NULL.
 */
HWND CBioInstanceChecker::ActivatePreviousInstance()
{
  if (m_hPrevInstance != NULL) //Whats happened to my handle !
  {
    // Open up the MMF
    int nMMFSize = sizeof(CWindowInstance);
    CWindowInstance* pInstanceData = (CWindowInstance*) ::MapViewOfFile(m_hPrevInstance, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, nMMFSize);
    if (pInstanceData != NULL) //Opening the MMF should work
    {
      // Lock the data prior to reading from it
      CSingleLock dataLock(&m_instanceDataMutex, TRUE);

      //activate the old window
      HWND hWindow = NULL;
	  hWindow =  pInstanceData->hMainWnd;
      CWnd wndPrev;
      wndPrev.Attach(hWindow);
      CWnd* pWndChild = NULL;
	  pWndChild	= wndPrev.GetLastActivePopup();

      if (wndPrev.IsIconic())
        wndPrev.ShowWindow(SW_RESTORE);

	  if(pWndChild)
	  {
		  pWndChild->SetForegroundWindow();
	  }

      //Detach the CWnd we were using
      wndPrev.Detach();

      //Unmap the MMF we were using
      VERIFY(::UnmapViewOfFile(pInstanceData));

      //Close the file handle now that we 
      ::CloseHandle(m_hPrevInstance);
      m_hPrevInstance = NULL;

      return hWindow;
    }
    else
      VERIFY(FALSE);  //Somehow MapViewOfFile failed.

    //Close the file handle now that we 
    ::CloseHandle(m_hPrevInstance);
    m_hPrevInstance = NULL;
  }
  else
    VERIFY(FALSE); //You should only call this function if PreviousInstanceRunning returned TRUE

  return NULL;
}


