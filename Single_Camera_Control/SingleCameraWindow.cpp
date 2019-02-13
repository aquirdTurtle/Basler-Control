
// Basler-ControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Single_Camera_Control_App.h"
#include "SingleCameraWindow.h"
#include "afxdialogex.h"
#include "constants.h"


SingleCameraWindow::SingleCameraWindow( CWnd* pParent /*=NULL*/ ) : CDialogEx( IDD_BASLERCONTROL_DIALOG, pParent ), 
																	cameraController ( PIXIS_SAFEMODE )
{
	for ( auto elem : GIST_RAINBOW_RGB )
	{
		Gdiplus::Color c( 50, BYTE( elem[0] ), BYTE( elem[1] ), BYTE( elem[2] ) );
		Gdiplus::SolidBrush* b = new Gdiplus::SolidBrush( c );
		Gdiplus::Pen* p = new Gdiplus::Pen( b );
		Gdiplus::Color c_bright( 255, BYTE( elem[0] ), BYTE( elem[1] ), BYTE( elem[2] ) );
		Gdiplus::SolidBrush* b_bright = new Gdiplus::SolidBrush( c_bright );
		Gdiplus::Pen* p_bright = new Gdiplus::Pen( b_bright );
		plotBrushes.push_back( b );
		plotPens.push_back( p );
		brightPlotBrushes.push_back( b_bright );
		brightPlotPens.push_back( p_bright );
	}
	(plotfont = new CFont)->CreateFontA( 12/*20*/, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
										 CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Arial" ) );

	m_hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );

	mainRGBs["Dark Grey"] = RGB( 15, 15, 15 );
	mainRGBs["Dark Grey Red"] = RGB( 20, 12, 12 );
	mainRGBs["Medium Grey"] = RGB( 30, 30, 30 );
	mainRGBs["Light Grey"] = RGB( 60, 60, 60 );
	mainRGBs["Green"] = RGB( 50, 200, 50 );
	mainRGBs["Red"] = RGB( 200, 50, 50 );
	mainRGBs["Blue"] = RGB( 50, 50, 200 );
	mainRGBs["Gold"] = RGB( 218, 165, 32 );
	mainRGBs["White"] = RGB( 255, 255, 255 );
	mainRGBs["Light Red"] = RGB( 255, 100, 100 );
	mainRGBs["Dark Red"] = RGB( 150, 0, 0 );
	mainRGBs["Light Blue"] = RGB( 100, 100, 255 );
	mainRGBs["Forest Green"] = RGB( 34, 139, 34 );
	mainRGBs["Dark Green"] = RGB( 0, 50, 0 );
	mainRGBs["Dull Red"] = RGB( 107, 35, 35 );
	mainRGBs["Dark Lavender"] = RGB( 100, 100, 205 );
	mainRGBs["Teal"] = RGB( 0, 255, 255 );
	mainRGBs["Tan"] = RGB( 210, 180, 140 );
	mainRGBs["Purple"] = RGB( 147, 112, 219 );
	mainRGBs["Orange"] = RGB( 255, 165, 0 );
	mainRGBs["Brown"] = RGB( 139, 69, 19 );
	mainRGBs["Black"] = RGB( 0, 0, 0 );
	mainRGBs["Dark Blue"] = RGB( 0, 0, 75 );
	mainRGBs["Indigo"] = RGB( 75, 0, 130 );
	mainRGBs["Dark Indigo"] = RGB( 18, 0, 32 );
	mainRGBs["Dark Orange"] = RGB( 31, 17, 0 );
	// there are less brushes because these are only used for backgrounds.
	(mainBrushes["Black"] = new CBrush)->CreateSolidBrush( mainRGBs["Black"] );
	mainBrushes["Dark Red"] = new CBrush;
	mainBrushes["Dark Red"]->CreateSolidBrush( mainRGBs["Dark Red"] );
	mainBrushes["Gold"] = new CBrush;
	mainBrushes["Gold"]->CreateSolidBrush( mainRGBs["Gold"] );
	mainBrushes["Dark Grey"] = new CBrush;
	mainBrushes["Dark Grey"]->CreateSolidBrush( mainRGBs["Dark Grey"] );
	mainBrushes["Dark Grey Red"] = new CBrush;
	mainBrushes["Dark Grey Red"]->CreateSolidBrush( mainRGBs["Dark Grey Red"] );
	mainBrushes["Medium Grey"] = new CBrush;
	mainBrushes["Medium Grey"]->CreateSolidBrush( mainRGBs["Medium Grey"] );
	mainBrushes["Light Grey"] = new CBrush;
	mainBrushes["Light Grey"]->CreateSolidBrush( mainRGBs["Light Grey"] );
	mainBrushes["Green"] = new CBrush;
	mainBrushes["Green"]->CreateSolidBrush( mainRGBs["Green"] );
	mainBrushes["Red"] = new CBrush;
	mainBrushes["Red"]->CreateSolidBrush( mainRGBs["Red"] );
	mainBrushes["White"] = new CBrush;
	mainBrushes["White"]->CreateSolidBrush( mainRGBs["White"] );
	mainBrushes["Dull Red"] = new CBrush;
	mainBrushes["Dull Red"]->CreateSolidBrush( mainRGBs["Dull Red"] );
	mainBrushes["Dark Blue"] = new CBrush;
	mainBrushes["Dark Blue"]->CreateSolidBrush( mainRGBs["Dark Blue"] );
	mainBrushes["Dark Green"] = new CBrush;
	mainBrushes["Dark Green"]->CreateSolidBrush( mainRGBs["Dark Green"] );
	// these are equivalent to the two-lines per used above.
	(mainBrushes["Indigo"] = new CBrush)->CreateSolidBrush( mainRGBs["Indigo"] );
	(mainBrushes["Dark Indigo"] = new CBrush)->CreateSolidBrush( mainRGBs["Dark Indigo"] );
	(mainBrushes["Dark Orange"] = new CBrush)->CreateSolidBrush( mainRGBs["Dark Orange"] );
	/// the following is all equivalent to:
	// mainFonts["Font name"] = new CFont;
	// mainFonts["Font name"].CreateFontA(...);
	(mainFonts["Smaller Font Max"] = new CFont)
		->CreateFontA( 27, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Arial" ) );
	(mainFonts["Normal Font Max"] = new CFont)
		->CreateFontA( 34, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Arial" ) );
	(mainFonts["Heading Font Max"] = new CFont)
		->CreateFontA( 42, 0, 0, 0, FW_DONTCARE, TRUE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Old Sans Black" ) );
	(mainFonts["Code Font Max"] = new CFont)
		->CreateFontA( 32, 0, 0, 0, 700, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Consolas" ) );
	(mainFonts["Larger Font Max"] = new CFont)
		->CreateFontA( 40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Arial" ) );
	(mainFonts["Smaller Font Large"] = new CFont)
		->CreateFontA( 14, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Arial" ) );
	(mainFonts["Normal Font Large"] = new CFont)
		->CreateFontA( 20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Arial" ) );
	(mainFonts["Heading Font Large"] = new CFont)
		->CreateFontA( 28, 0, 0, 0, FW_DONTCARE, TRUE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Old Sans Black" ) );
	(mainFonts["Code Font Large"] = new CFont)
		->CreateFontA( 16, 0, 0, 0, 700, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Consolas" ) );
	(mainFonts["Larger Font Large"] = new CFont)
		->CreateFontA( 40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Arial" ) );
	(mainFonts["Smaller Font Med"] = new CFont)
		->CreateFontA( 8, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Arial" ) );
	(mainFonts["Normal Font Med"] = new CFont)
		->CreateFontA( 12, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Arial" ) );
	(mainFonts["Heading Font Med"] = new CFont)
		->CreateFontA( 16, 0, 0, 0, FW_DONTCARE, TRUE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Old Sans Black" ) );
	(mainFonts["Code Font Med"] = new CFont)
		->CreateFontA( 10, 0, 0, 0, 700, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Consolas" ) );
	(mainFonts["Larger Font Med"] = new CFont)
		->CreateFontA( 22, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Arial" ) );
	(mainFonts["Smaller Font Small"] = new CFont)
		->CreateFontA( 6, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Arial" ) );
	(mainFonts["Normal Font Small"] = new CFont)
		->CreateFontA( 8, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Arial" ) );
	(mainFonts["Heading Font Small"] = new CFont)
		->CreateFontA( 12, 0, 0, 0, FW_DONTCARE, TRUE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Old Sans Black" ) );
	(mainFonts["Code Font Small"] = new CFont)
		->CreateFontA( 7, 0, 0, 0, 700, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Consolas" ) );
	(mainFonts["Larger Font Small"] = new CFont)
		->CreateFontA( 16, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT( "Arial" ) );
}

// the message map. Allows me to handle various events in the system using functions I write myself.
BEGIN_MESSAGE_MAP( SingleCameraWindow, CDialogEx )
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE()

	ON_COMMAND( ID_SOFTWARE_TRIGGER, SingleCameraWindow::handleSoftwareTrigger )
	ON_COMMAND( ID_FILE_ARMCAMERA, SingleCameraWindow::handleArmPress)
	ON_COMMAND( ID_FILE_DISARMCAMERA, SingleCameraWindow::handleDisarmPress)
	ON_COMMAND( IDOK, &SingleCameraWindow::handleEnter )
	ON_COMMAND( IDC_SET_ANALYSIS_LOCATIONS, &SingleCameraWindow::passSetLocationsButton)
	ON_COMMAND ( IDC_SET_TEMPERATURE_BUTTON, &SingleCameraWindow::handleSetTemperature )

	ON_CONTROL_RANGE(EN_CHANGE, IDC_MIN_SLIDER_EDIT, IDC_MIN_SLIDER_EDIT, &SingleCameraWindow::pictureRangeEditChange)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_MAX_SLIDER_EDIT, IDC_MAX_SLIDER_EDIT, &SingleCameraWindow::pictureRangeEditChange)

	ON_REGISTERED_MESSAGE( PIC_READY, &SingleCameraWindow::handleNewPics )
	
	ON_CBN_SELENDOK( IDC_EXPOSURE_MODE_COMBO, SingleCameraWindow::passExposureMode )
	ON_CBN_SELENDOK( IDC_CAMERA_MODE_COMBO, SingleCameraWindow::passCameraMode)

	ON_COMMAND( IDCANCEL, &SingleCameraWindow::handleClose )

	ON_WM_RBUTTONUP()	
	ON_WM_TIMER ( )

END_MESSAGE_MAP()


void SingleCameraWindow::handleSetTemperature ( )
{
	try
	{
		settings.loadCurrentSettings ( cameraController.getCameraDimensions ( ) );
		auto currSettings = settings.getCurrentSettings ( );
		cameraController.setTemperatureSetPoint ( currSettings.currentTemperatureVal );
	}
	catch ( Error& err )
	{
		errBox ( err.what ( ) );
	}
}


void SingleCameraWindow::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange( pDX );
}


void SingleCameraWindow::handleClose( )
{
	auto res = promptBox("Close the Camera Application?", MB_OKCANCEL );
	if ( res == IDOK )
	{
		CDialog::OnCancel( );
	}
}


void SingleCameraWindow::passSetLocationsButton()
{
	picture.handleButtonClick();
}


void SingleCameraWindow::OnTimer ( UINT_PTR id )
{
	settings.handleTimer ( &cameraController );
}


void SingleCameraWindow::OnRButtonUp( UINT stuff, CPoint clickLocation )
{
	CDC* cdc = GetDC( );
	picture.handleRightClick(clickLocation, cdc);
	ReleaseDC( cdc );
}


void SingleCameraWindow::handleEnter( ) { errBox( "hi!" ); }


// this is suppose see where the mouse is at a given time so that if it is hovering over a pixel I can display the pixel count.
void SingleCameraWindow::OnMouseMove( UINT flags, CPoint point )
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


void SingleCameraWindow::handleSoftwareTrigger()
{
	try
	{
		//cameraController.softwareTrigger();
	}
	catch (Pylon::TimeoutException& err)
	{
		errBox( "Software trigger timed out!" );
	}
}


void SingleCameraWindow::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* scrollbar )
{
	if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK)
	{
		int id = scrollbar->GetDlgCtrlID();
		try
		{
			picture.handleScroll( id, nPos );
			CDC* cdc = GetDC( );
			picture.redrawImage(cdc);
			ReleaseDC( cdc );
		}
		catch (Error& err)
		{
			errBox( "Error! " + err.whatStr() );
		}
	}
}


void SingleCameraWindow::pictureRangeEditChange( UINT id )
{
	try
	{
		picture.handleEditChange( id );
		CDC* cdc = GetDC ( );
		picture.redrawImage ( cdc );
		ReleaseDC ( cdc );
	}
	catch (Error& err)
	{
		errBox( "Error! " + err.whatStr() );
	}
}


void SingleCameraWindow::handleDisarmPress()
{
	try
	{
		cameraController.disarm();
		triggerThreadFlag = false;
		isRunning = false;
		settings.setStatus("Camera Status: Idle");
	}
	catch (Error& err)
	{
		errBox( "Error! " + err.whatStr() );
		settings.setStatus("Camera Status: ERROR?!?!");
	}
}


LRESULT SingleCameraWindow::handleNewPics( WPARAM wParam, LPARAM lParam )
{
	Matrix<long>* imageMatrix(reinterpret_cast<Matrix<long>*>(lParam));
	//std::vector<long>* image = (std::vector<long>*) lParam;
 	long size = long( wParam );
 	try
	{
		currentRepNumber++;
		CDC* cdc = GetDC();
		picture.drawBitmap( cdc, *imageMatrix );
		picture.updatePlotData( );
		picture.drawDongles( cdc, *imageMatrix );
		ReleaseDC( cdc );
		picture.setHoverValue();
		if (runExposureMode == "Auto Exposure Continuous")
		{
			settings.updateExposure( cameraController.getExposure() );
		}
		
		stats.update( *imageMatrix, 0, { 0,0 }, settings.getCurrentSettings().dimensions.horBinNumber, currentRepNumber,
					  cameraController.getRepCounts() );
		settings.setStatus("Camera Status: Acquiring Pictures.");
		if (currentRepNumber % 10 == 0)
		{
			settings.handleFrameRate();
		}
		if (currentRepNumber == 1)// && !cameraController.isContinuous())
		{
			saver.save( *imageMatrix, imageWidth );
		}
		else// if (!cameraController.isContinuous())
		{
			saver.append( *imageMatrix, imageWidth );
		}
		if (currentRepNumber == cameraController.getRepCounts())
		{
			cameraController.disarm();
			saver.close();
			isRunning = false;
			settings.setStatus("Camera Status: Finished finite acquisition.");
		}
	}
	catch (Error& err)
	{
		errBox( err.what() );
		settings.setStatus("Camera Status: ERROR?!?!?");
	}
	OnPaint( );
	// always delete
	delete imageMatrix;
	return 0;
}

void SingleCameraWindow::passCameraMode()
{
	try
	{
		settings.handleCameraMode();
		saver.handleModeChange(settings.loadCurrentSettings(cameraController.getCameraDimensions()).cameraMode);
	}
	catch (Error& err)
	{
		errBox( "Error! " + err.whatStr() );
	}
}


void SingleCameraWindow::passExposureMode()
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


void SingleCameraWindow::handleArmPress()
{
	try
	{
		currentRepNumber = 0;
		CameraSettings tempSettings = settings.loadCurrentSettings(cameraController.getCameraDimensions());
		cameraController.setParameters( tempSettings );
		picture.recalculateGrid( tempSettings.dimensions );
		auto* dc = GetDC( );
		picture.drawBackground( dc );
		ReleaseDC( dc );
		runExposureMode = tempSettings.exposureMode;
		imageWidth = tempSettings.dimensions.horBinNumber;
		triggerThreadFlag = true;

		/*triggerThreadInput* input = new triggerThreadInput;
		input->width = tempSettings.dimensions.horBinNumber;
		input->height = tempSettings.dimensions.vertBinNumber;
		input->frameRate = tempSettings.frameRate;
		input->parent = this;
		input->runningFlag = &triggerThreadFlag;*/

		cameraController.armCamera();
		settings.setStatus("Camera Status: Armed...");
		isRunning = true;
	}
	catch (Error& err)
	{
		errBox( err.what() );
	}
}


void SingleCameraWindow::OnSize( UINT nType, int cx, int cy )
{
	picture.rearrange("", "", cx, cy, mainFonts);
	settings.rearrange(cx, cy, mainFonts);
	stats.rearrange("", "", cx, cy, mainFonts);
	saver.rearrange(cx, cy, mainFonts);
	if ( horGraph )
	{
		horGraph->rearrange( cx, cy, mainFonts );
	}
	if ( vertGraph )
	{
		vertGraph->rearrange( cx, cy, mainFonts );
	}
}


HBRUSH SingleCameraWindow::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
			return *mainBrushes[mainColor];
		}
	}
}


BOOL SingleCameraWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow( SW_SHOWMAXIMIZED );
	try
	{
		initializeControls();
	}
	catch (Error& err)
	{
		errBox( err.what() );
		EndDialog(-1);
		return FALSE;
	}
	CRect rect;
	GetWindowRect(&rect);
	OnSize(0, rect.right - rect.left, rect.bottom - rect.top);
	SetTimer ( NULL, 1000, NULL );
	return TRUE;  // return TRUE  unless you set the focus to a control
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void SingleCameraWindow::OnPaint()
{
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
		auto* dc = GetDC( );
		CRect size;
		GetClientRect( &size );
		picture.paint( dc, size, mainBrushes["Black"] );
		ReleaseDC( dc );
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR SingleCameraWindow::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void SingleCameraWindow::initializeControls()
{
	#ifdef PIXIS_CAMERA
		SetWindowText ( "Pixis Camera Control" );
	#endif

	CMenu menu;
	menu.LoadMenu( IDR_MENU1 );
	SetMenu( &menu );
	cameraController.initialize ( this );
	int id = 1000;
	POINT pos = { 0,0 };
	POINT cameraDims = cameraController.getCameraDimensions();
	settings.initialize( pos, id, this, cameraDims.x, cameraDims.y, cameraDims);
	settings.setSettings( cameraController.getDefaultSettings() );
	std::unordered_map<std::string, CFont*> fontDummy;
	std::vector<CToolTipCtrl*> toolTipDummy;
	stats.initialize( pos, this, id, fontDummy, toolTipDummy );
	saver.initialize( pos, id, this );

	POINT picPos = { 300, 0 };
	//POINT dims = cameraController.getCameraDimensions();

	// scale to fill the window (approximately).
	cameraDims.x *= 0.8;
	cameraDims.y *= 0.8;

	picture.initialize( picPos, this, id, cameraDims.x + picPos.x + 115, cameraDims.y + picPos.y, mainBrushes["Red"],
						brightPlotPens, plotfont, brightPlotBrushes );
	picture.recalculateGrid( cameraController.getDefaultSettings().dimensions );
	CDC* cdc = GetDC( );
	picture.drawBackground( cdc );
	ReleaseDC( cdc );
}

