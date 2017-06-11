
// Basler-Control.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// some hap-hazardly placed globals.
extern UINT ACE_PIC_READY;
extern long num;

class CBaslerControlApp : public CWinApp
{
	public:
		CBaslerControlApp();
		// Overrides
		virtual BOOL InitInstance();
	// 
	DECLARE_MESSAGE_MAP()
};

extern CBaslerControlApp theApp;
