
// Basler-Control.cpp : Defines the class behaviors for the application.
//
/// Important Author's note:
//		Most of the code in this file was created programatically when I started this project.  A lot of it is probably
//		superfluous, I've created much simpler apps before, but it does no harm. For a lot of the stuff in this file 
//		however, I don't really know what it does.


#include "stdafx.h"
#include "BaslerControlApp.h"
#include "BaslerControlWindow.h"

// something windows programs often do for debugging purposes.
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// a few globals... could probably do this better...
UINT ACE_PIC_READY = ::RegisterWindowMessageA( "ACE_PIC_READY" );

// CBaslerControlApp
// TODO: Remove this. I think I should be able to remove the following couple lines.
BEGIN_MESSAGE_MAP(CBaslerControlApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// constructor for the main app.
CBaslerControlApp::CBaslerControlApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CBaslerControlApp object. The entire program runs from the construction of this object.
CBaslerControlApp theApp;
long num;

// this initialization function is called after the constructor.
BOOL CBaslerControlApp::InitInstance()
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

	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Mark Brown's control code"));

	// create the main window object.
	BaslerControlWindow dlg;
	// the app class gets a reference.
	m_pMainWnd = &dlg;

	// create the main dialog. Most of the code sits inside this function, including the main gui looping.
	INT_PTR nResponse = dlg.DoModal();
	
	// at the point the program is ending.

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
		ControlBarCleanUp();
	#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than armCamera the application's message pump.
	return FALSE;
}
