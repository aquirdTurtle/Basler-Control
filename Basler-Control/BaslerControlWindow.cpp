
// Basler-ControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BaslerControlApp.h"
#include "BaslerControlWindow.h"
#include "afxdialogex.h"
#include "constants.h"

// the message map. Allows me to handle various events in the system using functions I write myself.
BEGIN_MESSAGE_MAP( BaslerControlWindow, CDialogEx )
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_COMMAND( IDC_ARM_BASLER_BUTTON, BaslerControlWindow::handleArmPress )
	ON_COMMAND( IDC_DISARM_BASLER_BUTTON, BaslerControlWindow::handleDisarmPress )
	ON_COMMAND( ID_SOFTWARE_TRIGGER, BaslerControlWindow::handleSoftwareTrigger )
	ON_REGISTERED_MESSAGE( ACE_PIC_READY, &BaslerControlWindow::handleNewPics )
	ON_CBN_SELENDOK( IDC_EXPOSURE_MODE_COMBO, BaslerControlWindow::passExposureMode )
	ON_CBN_SELENDOK(IDC_CAMERA_MODE_COMBO, BaslerControlWindow::passCameraMode)
	ON_CONTROL_RANGE( EN_CHANGE, IDC_BASLER_MIN_SLIDER_MIN_EDIT, IDC_BASLER_MIN_SLIDER_MIN_EDIT, &BaslerControlWindow::handlePictureRangeEditChange )
	ON_CONTROL_RANGE( EN_CHANGE, IDC_BASLER_MIN_SLIDER_MAX_EDIT, IDC_BASLER_MIN_SLIDER_MAX_EDIT, &BaslerControlWindow::handlePictureRangeEditChange )
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// this is suppose see where the mouse is at a given time so that if it is hovering over a pixel I can display the pixel count.
void BaslerControlWindow::OnMouseMove( UINT flags, CPoint point )
{
	try
	{
		picture.handleMouse( point );
	}
	catch (Error& err)
	{
		errBox( "Error! " + err.whatStr() );
	}
}


void BaslerControlWindow::handleSoftwareTrigger()
{
	try
	{
		cameraController->softwareTrigger();
	}
	catch (Pylon::TimeoutException& err)
	{
		errBox( "Software trigger timed out!" );
	}
}


void BaslerControlWindow::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* scrollbar )
{
	if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK)
	{
		int id = scrollbar->GetDlgCtrlID();
		try
		{
			picture.handleScroll( id, nPos );
			picture.redrawImage(this);
		}
		catch (Error& err)
		{
			errBox( "Error! " + err.whatStr() );
		}
	}
}


void BaslerControlWindow::handlePictureRangeEditChange( UINT id )
{
	try
	{
		picture.handleEditChange( id );
	}
	catch (Error& err)
	{
		errBox( "Error! " + err.whatStr() );
	}
}


void BaslerControlWindow::handleDisarmPress()
{
	try
	{
		cameraController->disarm();
		isRunning = false;
	}
	catch (Error& err)
	{
		errBox( "Error! " + err.whatStr() );
	}
}


LRESULT BaslerControlWindow::handleNewPics( WPARAM wParam, LPARAM lParam )
{
	std::vector<long>* image = (std::vector<long>*) lParam;
 	long size = long( wParam );
 	try
	{
		currentRepNumber++;
		CDC* tempDc = GetDC();
		picture.drawBitmap( tempDc, *image );
		ReleaseDC( tempDc );
		picture.setValue();
		if (runExposureMode == "Auto Exposure Continuous")
		{
			settings.updateExposure( cameraController->getCurrentExposure() );
		}
		
		stats.update( image, 0, { 0,0 }, settings.getCurrentSettings().dimensions.horBinNumber, currentRepNumber, 
						cameraController->getRepCounts() );

		if (currentRepNumber == 1 || cameraController->isContinuous())
		{
			saver.save( image, imageWidth );
		}
		else
		{
			saver.append( image, imageWidth );
			if (currentRepNumber == cameraController->getRepCounts())
			{
				cameraController->disarm();
				saver.close();
				isRunning = false;
			}
		}
	}
	catch (Error* err)
	{
		errBox( err->what() );
	}
	// always delete
	delete image;
	return 0;
}

void BaslerControlWindow::passCameraMode()
{
	try
	{
		settings.handleCameraMode();
	}
	catch (Error& err)
	{
		errBox( "Error! " + err.whatStr() );
	}
}


void BaslerControlWindow::passExposureMode()
{
	try
	{
		settings.handleExposureMode();
	}
	catch (Error& err)
	{
		errBox( "Error! " + err.whatStr() );
	}
}


void BaslerControlWindow::handleArmPress()
{
	try
	{
		currentRepNumber = 0;
		baslerSettings tempSettings = settings.loadCurrentSettings();
		cameraController->setParameters( tempSettings );
		picture.updateGridSpecs( tempSettings.dimensions );
		runExposureMode = tempSettings.exposureMode;
		imageWidth = tempSettings.dimensions.horBinNumber;
		HWND* win = new HWND;
		win = &m_hWnd;
		cameraController->armCamera( tempSettings.frameRate, win );
		isRunning = true;
	}
	catch (Error& err)
	{
		errBox( err.what() );
	}
}


void BaslerControlWindow::OnSize( UINT nType, int cx, int cy )
{
	//CDialog::OnSize( nType, cx, cy );
	picture.rearrange("", "", cx, cy, mainFonts);
	settings.rearrange(cx, cy, mainFonts);
	stats.rearrange("", "", cx, cy, mainFonts);
	saver.rearrange(cx, cy, mainFonts);
}


HBRUSH BaslerControlWindow::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
		case CTLCOLOR_STATIC:
		{
			pDC->SetTextColor(mainRGBs["White"]);
			pDC->SetBkColor(mainRGBs["Medium Grey"]);
			return *mainBrushes["Medium Grey"];
		}
		case CTLCOLOR_EDIT:
		{
			pDC->SetTextColor(mainRGBs["White"]);
			pDC->SetBkColor(mainRGBs["Dark Grey"]);
			return *mainBrushes["Dark Grey"];
		}
		case CTLCOLOR_LISTBOX:
		{
			pDC->SetTextColor(mainRGBs["White"]);
			pDC->SetBkColor(mainRGBs["Dark Grey"]);
			return *mainBrushes["Dark Grey"];
		}
		default:
		{
			return *mainBrushes["Dark Indigo"];
		}
	}
}

BaslerControlWindow::BaslerControlWindow( CWnd* pParent /*=NULL*/ ) : CDialogEx( IDD_BASLERCONTROL_DIALOG, pParent )
{
	m_hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );

	mainRGBs["Dark Grey"] = RGB(15, 15, 15);
	mainRGBs["Dark Grey Red"] = RGB(20, 12, 12);
	mainRGBs["Medium Grey"] = RGB(30, 30, 30);
	mainRGBs["Light Grey"] = RGB(60, 60, 60);
	mainRGBs["Green"] = RGB(50, 200, 50);
	mainRGBs["Red"] = RGB(200, 50, 50);
	mainRGBs["Blue"] = RGB(50, 50, 200);
	mainRGBs["Gold"] = RGB(218, 165, 32);
	mainRGBs["White"] = RGB(255, 255, 255);
	mainRGBs["Light Red"] = RGB(255, 100, 100);
	mainRGBs["Dark Red"] = RGB(150, 0, 0);
	mainRGBs["Light Blue"] = RGB(100, 100, 255);
	mainRGBs["Forest Green"] = RGB(34, 139, 34);
	mainRGBs["Dark Green"] = RGB(0, 50, 0);
	mainRGBs["Dull Red"] = RGB(107, 35, 35);
	mainRGBs["Dark Lavender"] = RGB(100, 100, 205);
	mainRGBs["Teal"] = RGB(0, 255, 255);
	mainRGBs["Tan"] = RGB(210, 180, 140);
	mainRGBs["Purple"] = RGB(147, 112, 219);
	mainRGBs["Orange"] = RGB(255, 165, 0);
	mainRGBs["Brown"] = RGB(139, 69, 19);
	mainRGBs["Black"] = RGB(0, 0, 0);
	mainRGBs["Dark Blue"] = RGB(0, 0, 75);
	mainRGBs["Indigo"] = RGB(75, 0, 130);
	mainRGBs["Dark Indigo"] = RGB(18, 0, 32);
	// there are less brushes because these are only used for backgrounds.
	mainBrushes["Dark Red"] = new CBrush;
	mainBrushes["Dark Red"]->CreateSolidBrush(mainRGBs["Dark Red"]);
	mainBrushes["Gold"] = new CBrush;
	mainBrushes["Gold"]->CreateSolidBrush(mainRGBs["Gold"]);
	mainBrushes["Dark Grey"] = new CBrush;
	mainBrushes["Dark Grey"]->CreateSolidBrush(mainRGBs["Dark Grey"]);
	mainBrushes["Dark Grey Red"] = new CBrush;
	mainBrushes["Dark Grey Red"]->CreateSolidBrush(mainRGBs["Dark Grey Red"]);
	mainBrushes["Medium Grey"] = new CBrush;
	mainBrushes["Medium Grey"]->CreateSolidBrush(mainRGBs["Medium Grey"]);
	mainBrushes["Light Grey"] = new CBrush;
	mainBrushes["Light Grey"]->CreateSolidBrush(mainRGBs["Light Grey"]);
	mainBrushes["Green"] = new CBrush;
	mainBrushes["Green"]->CreateSolidBrush(mainRGBs["Green"]);
	mainBrushes["Red"] = new CBrush;
	mainBrushes["Red"]->CreateSolidBrush(mainRGBs["Red"]);
	mainBrushes["White"] = new CBrush;
	mainBrushes["White"]->CreateSolidBrush(mainRGBs["White"]);
	mainBrushes["Dull Red"] = new CBrush;
	mainBrushes["Dull Red"]->CreateSolidBrush(mainRGBs["Dull Red"]);
	mainBrushes["Dark Blue"] = new CBrush;
	mainBrushes["Dark Blue"]->CreateSolidBrush(mainRGBs["Dark Blue"]);
	mainBrushes["Dark Green"] = new CBrush;
	mainBrushes["Dark Green"]->CreateSolidBrush(mainRGBs["Dark Green"]);

	(mainBrushes["Indigo"] = new CBrush)->CreateSolidBrush(mainRGBs["Indigo"]);
	(mainBrushes["Dark Indigo"] = new CBrush)->CreateSolidBrush(mainRGBs["Dark Indigo"]);
	
	/// the following is all equivalent to:
	// mainFonts["Font name"] = new CFont;
	// mainFonts["Font name"].CreateFontA(...);
	(mainFonts["Smaller Font Max"] = new CFont)
		->CreateFontA(27, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
	(mainFonts["Normal Font Max"] = new CFont)
		->CreateFontA(34, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
	(mainFonts["Heading Font Max"] = new CFont)
		->CreateFontA(42, 0, 0, 0, FW_DONTCARE, TRUE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Old Sans Black"));
	(mainFonts["Code Font Max"] = new CFont)
		->CreateFontA(32, 0, 0, 0, 700, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Consolas"));
	(mainFonts["Larger Font Max"] = new CFont)
		->CreateFontA(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
	(mainFonts["Smaller Font Large"] = new CFont)
		->CreateFontA(14, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
	(mainFonts["Normal Font Large"] = new CFont)
		->CreateFontA(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
	(mainFonts["Heading Font Large"] = new CFont)
		->CreateFontA(28, 0, 0, 0, FW_DONTCARE, TRUE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Old Sans Black"));
	(mainFonts["Code Font Large"] = new CFont)
		->CreateFontA(16, 0, 0, 0, 700, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Consolas"));
	(mainFonts["Larger Font Large"] = new CFont)
		->CreateFontA(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
	(mainFonts["Smaller Font Med"] = new CFont)
		->CreateFontA(8, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
	(mainFonts["Normal Font Med"] = new CFont)
		->CreateFontA(12, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
	(mainFonts["Heading Font Med"] = new CFont)
		->CreateFontA(16, 0, 0, 0, FW_DONTCARE, TRUE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Old Sans Black"));
	(mainFonts["Code Font Med"] = new CFont)
		->CreateFontA(10, 0, 0, 0, 700, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Consolas"));
	(mainFonts["Larger Font Med"] = new CFont)
		->CreateFontA(22, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
	(mainFonts["Smaller Font Small"] = new CFont)
		->CreateFontA(6, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
	(mainFonts["Normal Font Small"] = new CFont)
		->CreateFontA(8, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
	(mainFonts["Heading Font Small"] = new CFont)
		->CreateFontA(12, 0, 0, 0, FW_DONTCARE, TRUE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Old Sans Black"));
	(mainFonts["Code Font Small"] = new CFont)
		->CreateFontA(7, 0, 0, 0, 700, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Consolas"));
	(mainFonts["Larger Font Small"] = new CFont)
		->CreateFontA(16, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
}

void BaslerControlWindow::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
}

BOOL BaslerControlWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow( SW_SHOWMAXIMIZED );
	initializeControls();
	CRect rect;
	GetWindowRect(&rect);
	OnSize(0, rect.right - rect.left, rect.bottom - rect.top);
	return TRUE;  // return TRUE  unless you set the focus to a control
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void BaslerControlWindow::OnPaint()
{
	//picture.drawBackground( this );
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR BaslerControlWindow::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void BaslerControlWindow::initializeControls()
{
	try
	{
		CMenu menu;
		menu.LoadMenu( IDR_MENU1 );
		SetMenu( &menu );
		HWND* temp = new HWND( GetSafeHwnd() );
		cameraController = new BaslerCameras( temp );
		int id = 1000;
		POINT pos = { 0,0 };
		POINT cameraDims = cameraController->getCameraDimensions();
		settings.initialize( pos, id, this, cameraDims.x, cameraDims.y );
		settings.setSettings( cameraController->getDefaultSettings() );
		std::unordered_map<std::string, CFont*> fontDummy;
		std::vector<CToolTipCtrl*> toolTipDummy;
		stats.initialize( pos, this, id, fontDummy, toolTipDummy );
		saver.initialize( pos, id, this );

		POINT picPos = { 300, 0 };
		POINT dims = cameraController->getCameraDimensions();

		// scale to fill the window (approximately).
		dims.x *= 1.7;
		dims.y *= 1.7;
		picture.initialize( picPos, this, id, dims.x + picPos.x + 100, dims.y + picPos.y );
		picture.updateGridSpecs( cameraController->getDefaultSettings().dimensions );
		picture.drawBackground( this );
	}
	catch (Error& err)
	{
		errBox( "Initialization Error! " + err.whatStr() );
	}
}

