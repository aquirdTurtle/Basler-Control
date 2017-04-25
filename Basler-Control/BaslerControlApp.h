
// Basler-Control.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

extern UINT ACE_PIC_READY;

// CBaslerControlApp:
// See Basler-Control.cpp for the implementation of this class
//

class CBaslerControlApp : public CWinApp
{
public:
	CBaslerControlApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CBaslerControlApp theApp;
