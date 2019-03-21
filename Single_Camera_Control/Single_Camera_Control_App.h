// Created by Mark Brown
// Single_Camera_Control_App.h : main header file for the Single Camera Controlapplication
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// some hap-hazardly placed globals.
extern UINT PIC_READY;
extern long num;

class CSingle_Camera_Control_App : public CWinApp
{
	public:
		CSingle_Camera_Control_App();
		BOOL ExitInstance( );
		ULONG_PTR gdip_token;
		// Overrides
		virtual BOOL InitInstance();
	// 
	DECLARE_MESSAGE_MAP()
};

extern CSingle_Camera_Control_App theApp;
