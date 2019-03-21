// Created by Mark Brown
#include "stdafx.h"
#include "CameraSettingsControl.h"
#include "constants.h"
#include "PixisCamera.h"
#include <boost/lexical_cast.hpp>

void CameraSettingsControl::rearrange(int width, int height, fontMap fonts)
{
	statusText.rearrange("", "", width, height, fonts);
	// exposure
	exposureText.rearrange("", "", width, height, fonts);
	exposureModeCombo.rearrange("", "", width, height, fonts);
	exposureEdit.rearrange("", "", width, height, fonts);
	setExposure.rearrange("", "", width, height, fonts);
	// trigger
	triggerCombo.rearrange("", "", width, height, fonts);

	// Dimensions & Binning
	leftText.rearrange("", "", width, height, fonts);
	rightText.rearrange("", "", width, height, fonts);
	horizontalBinningText.rearrange("", "", width, height, fonts);
	topText.rearrange("", "", width, height, fonts);
	bottomText.rearrange("", "", width, height, fonts);
	verticalBinningText.rearrange("", "", width, height, fonts);
	leftEdit.rearrange("", "", width, height, fonts);
	rightEdit.rearrange("", "", width, height, fonts);
	horizontalBinningEdit.rearrange("", "", width, height, fonts);
	topEdit.rearrange("", "", width, height, fonts);
	bottomEdit.rearrange("", "", width, height, fonts);
	verticalBinningEdit.rearrange("", "", width, height, fonts);

	// camera mode: continuous or set #
	cameraMode.rearrange("", "", width, height, fonts);
	repText.rearrange("", "", width, height, fonts);
	repEdit.rearrange("", "", width, height, fonts);

	frameRateText.rearrange("", "", width, height, fonts);
	frameRateEdit.rearrange("", "", width, height, fonts);
	realFrameRate.rearrange("", "", width, height, fonts);

	gainText.rearrange("", "", width, height, fonts);
	gainCombo.rearrange("", "", width, height, fonts);
	gainEdit.rearrange("", "", width, height, fonts);
	realGainText.rearrange("", "", width, height, fonts);
	realGainStatus.rearrange("", "", width, height, fonts);

	setTemperatureButton.rearrange ( "", "", width, height, fonts );
	temperatureOffButton.rearrange ( "", "", width, height, fonts );
	temperatureEdit.rearrange ( "", "", width, height, fonts );
	temperatureDisplay.rearrange ( "", "", width, height, fonts );
	temperatureMsg.rearrange ( "", "", width, height, fonts );
}


void CameraSettingsControl::handleTimer ( PixisCamera* cam )
{
	// This case displays the current temperature in the main window. When the temp stabilizes at the desired 
	// level the appropriate message is displayed.
	// initial value is only relevant for safemode.
	int currentTemperature = INT_MAX;
	int setTemperature = INT_MAX;
	// in this case you expect it to throw.
	setTemperature = cam->getSetTemperature ( );
	std::string msg;
	try
	{
		currentTemperature = cam->getCurrentTemperature();
		msg = cam->getTemperatureStatus();
		mostRecentTemp = currentTemperature;
	}
	catch (Error& err)
	{
		currentTemperature = mostRecentTemp;
		msg = "Aquiring, no temperature updates";
	}

	temperatureDisplay.SetWindowTextA ( cstr ( setTemperature ) );
	temperatureMsg.SetWindowTextA ( cstr ( "Current Temperature: " + str ( currentTemperature ) + " (C)\r\n" + msg ) );
}


// assumes called on every 10 pics.
void CameraSettingsControl::handleFrameRate()
{
	ULONG currentTime = GetTickCount();
	ULONG timePerPic = (currentTime - lastTime)/10.0;
	if (timePerPic == 0)
	{
		// avoid dividing by 0.
		timePerPic++;
	}
	realFrameRate.SetWindowTextA(cstr(1000.0 / timePerPic));
	lastTime = currentTime;
}


void CameraSettingsControl::setStatus(std::string status)
{
	statusText.SetWindowTextA(status.c_str());
}

void CameraSettingsControl::initialize( POINT& pos, int& id, CWnd* parent, int picWidth, int picHeight, POINT cameraDims )
{
	int width = 300;
	lastTime = 0;
	statusText.sPos = { pos.x, pos.y, pos.x + width, pos.y += 50 };
	statusText.Create("Camera Status: IDLE", WS_CHILD | WS_VISIBLE | WS_BORDER, statusText.sPos, parent, id++);
	////
	repText.sPos = { pos.x, pos.y, pos.x + 200, pos.y + 25 };
	repText.Create( "Total Picture Number:", WS_CHILD | WS_VISIBLE, repText.sPos, parent, id++ );
	//
	repEdit.sPos = { pos.x + 200, pos.y, pos.x + 300, pos.y += 25 };
	repEdit.Create( WS_CHILD | WS_VISIBLE, repEdit.sPos, parent, IDC_REPETITIONS_EDIT );
	repEdit.SetWindowTextA( "100" );

	cameraMode.sPos = { pos.x, pos.y, pos.x + 300, pos.y + 100 };
	cameraMode.Create( WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_SORT, cameraMode.sPos, parent,
					   IDC_CAMERA_MODE_COMBO );
	pos.y += 25;
	cameraMode.AddString( "Finite Acquisition" );
	cameraMode.AddString( "Continuous Acquisition" );
	cameraMode.SelectString( 0, "Continuous Acquisition" );
	repEdit.EnableWindow( false );
	
	exposureText.sPos = { pos.x, pos.y, pos.x + 200, pos.y + 25 };
	std::string exposureTimeText;
	exposureText.Create(exposureTimeText.c_str(), WS_CHILD | WS_VISIBLE, exposureText.sPos, parent, id++ );

	exposureEdit.sPos = { pos.x + 200, pos.y, pos.x + 300, pos.y += 25 };
	exposureEdit.Create( WS_CHILD | WS_VISIBLE, exposureEdit.sPos, parent, id++ );
	exposureEdit.SetWindowTextA( "100" );

	exposureModeCombo.sPos = { pos.x, pos.y, pos.x + 300, pos.y + 100 };
	exposureModeCombo.Create( WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_SORT, exposureModeCombo.sPos, parent, 
							  IDC_EXPOSURE_MODE_COMBO );

	pos.y += 25;
	exposureModeCombo.AddString( "Auto Exposure Continuous" );
	exposureModeCombo.AddString( "Auto Exposure Off" );
	exposureModeCombo.AddString( "Auto Exposure Once" );
	exposureModeCombo.SelectString( 0, "Auto Exposure Off" );

	/// image dimensions
	//
	leftText.sPos = { pos.x, pos.y, pos.x + width/3, pos.y + 25 };
	leftText.Create( "Left", WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY, leftText.sPos, parent, id++ );
	
	//
	rightText.sPos = { pos.x + width/3, pos.y, pos.x + 2* width/3, pos.y + 25 };
	rightText.Create( std::string("Right (/" + str(cameraDims.x-1) + ")").c_str(), WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY, rightText.sPos,
					 parent, id++ );
	
	//
	horizontalBinningText.sPos = { pos.x + 2* width/3, pos.y, pos.x + width, pos.y += 25 };
	horizontalBinningText.Create( "H. Bin", WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY, horizontalBinningText.sPos,
								  parent, id++ );
	
	//
	leftEdit.sPos = { pos.x, pos.y, pos.x + width/3, pos.y + 25 };
	leftEdit.Create( WS_TABSTOP | WS_CHILD | WS_VISIBLE | ES_CENTER, leftEdit.sPos, parent, id++);
	leftEdit.SetWindowTextA( "0" );
	//
	rightEdit.sPos = { pos.x + width/3, pos.y, pos.x + 2* width/3, pos.y + 25 };
	rightEdit.Create( WS_TABSTOP | WS_CHILD | WS_VISIBLE | ES_CENTER, rightEdit.sPos, parent, id++ );
	rightEdit.SetWindowTextA(str(cameraDims.x-1).c_str() );
	//
	horizontalBinningEdit.sPos = { pos.x + 2* width/3, pos.y, pos.x + width, pos.y += 25 };
	horizontalBinningEdit.Create( WS_TABSTOP | WS_CHILD | WS_VISIBLE | ES_CENTER, horizontalBinningEdit.sPos, parent, 
								  id++ );
	horizontalBinningEdit.SetWindowTextA( "1" );
	//
	topText.sPos = { pos.x, pos.y, pos.x + width/3, pos.y + 25 };
	topText.Create( "Top", WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY, topText.sPos, parent, id++ );

	//
	bottomText.sPos = { pos.x + width/3, pos.y, pos.x +2* width/3, pos.y + 25 };
	bottomText.Create( std::string("Bottom (/" + str(cameraDims.y-1) + ")").c_str(), WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY, bottomText.sPos, 
					  parent, id++ );
	//
	verticalBinningText.sPos = { pos.x + 2* width/3, pos.y, pos.x + width, pos.y += 25 };
	verticalBinningText.Create( "V. Bin", WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY, verticalBinningText.sPos, parent,
								id++ );
	//
	topEdit.sPos = { pos.x, pos.y, pos.x + width/3, pos.y + 25 };
	topEdit.Create( WS_TABSTOP | WS_CHILD | WS_VISIBLE | ES_CENTER, topEdit.sPos, parent, id++ );
	topEdit.SetWindowTextA( "0" );
	//
	bottomEdit.sPos = { pos.x + width/3, pos.y, pos.x + 2* width/3, pos.y + 25 };
	bottomEdit.Create( WS_TABSTOP | WS_CHILD | WS_VISIBLE | ES_CENTER, bottomEdit.sPos, parent, id++ );
	bottomEdit.SetWindowTextA(str(cameraDims.y - 1).c_str() );
	//
	verticalBinningEdit.sPos = { pos.x + 2* width/3, pos.y, pos.x + width, pos.y += 25 };
	verticalBinningEdit.Create( WS_TABSTOP | WS_CHILD | WS_VISIBLE | ES_CENTER, verticalBinningEdit.sPos, parent, id++ );
	verticalBinningEdit.SetWindowTextA( "1" );

	triggerCombo.sPos = { pos.x, pos.y, pos.x + 300, pos.y + 100 };
	triggerCombo.Create( WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_SORT, triggerCombo.sPos, parent, 
						 IDC_TRIGGER_MODE_COMBO );
	triggerCombo.AddString( "External Trigger" );
	triggerCombo.AddString( "Automatic Software Trigger" );
	//triggerCombo.AddString( "Manual Software Trigger" );
	triggerCombo.SelectString( 0, "Automatic Software Trigger" );
	pos.y += 25;

	frameRateText.sPos = { pos.x, pos.y, pos.x + 150, pos.y + 25 };
	frameRateText.Create( "Frame Rate (pics/s): ", WS_CHILD | WS_VISIBLE, frameRateText.sPos, parent, id++ );

	frameRateEdit.sPos = { pos.x + 150, pos.y, pos.x + 225, pos.y + 25 };
	frameRateEdit.Create( WS_CHILD | WS_VISIBLE, frameRateEdit.sPos, parent, id++ );
	#ifdef _DEBUG
		frameRateEdit.SetWindowTextA( "0.1" );
	#else
		frameRateEdit.SetWindowTextA( "30" );
	#endif

	realFrameRate.sPos = { pos.x + 225, pos.y, pos.x + 300, pos.y += 25 };
	realFrameRate.Create( "", WS_CHILD | WS_VISIBLE, realFrameRate.sPos, parent, id++ );

	gainCombo.sPos = { pos.x, pos.y, pos.x + 300, pos.y + 100 };
	gainCombo.Create( WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_SORT, gainCombo.sPos, parent, id++ );
	gainCombo.AddString( "Auto Gain Continuous" );
	gainCombo.AddString( "Auto Gain Once" );
	gainCombo.AddString( "Auto Gain Off" );
	gainCombo.SelectString( 0, "Auto Gain Off" );
	pos.y += 25;

	gainText.sPos = { pos.x, pos.y, pos.x + 150, pos.y + 25 };
	gainText.Create( "Raw Gain (260-?): ", WS_CHILD | WS_VISIBLE, gainText.sPos, parent, id++ );

	gainEdit.sPos = { pos.x + 150, pos.y, pos.x + 300, pos.y += 25 };
	gainEdit.Create( WS_CHILD | WS_VISIBLE, gainEdit.sPos, parent, id++ );
	gainEdit.SetWindowTextA( "260" );

	realGainText.sPos = { pos.x, pos.y, pos.x + 150, pos.y + 25 };
	realGainText.Create( "Real Gain: ", WS_CHILD | WS_VISIBLE, realGainText.sPos, parent, id++ );

	realGainStatus.sPos = { pos.x + 150, pos.y, pos.x + 300, pos.y += 25 };
	realGainStatus.Create( "", WS_CHILD | WS_VISIBLE, realGainStatus.sPos, parent, id++ );
	// temperature controls
	setTemperatureButton.sPos = { pos.x, pos.y, pos.x + 150, pos.y + 25 };
	setTemperatureButton.Create ( "Set Cam. Temp. (C)", NORM_PUSH_OPTIONS, setTemperatureButton.seriesPos,
								  parent, IDC_SET_TEMPERATURE_BUTTON );
	temperatureEdit.sPos = { pos.x + 150, pos.y, pos.x + 200, pos.y + 25 };
	temperatureEdit.Create ( NORM_EDIT_OPTIONS, temperatureEdit.seriesPos, parent, id++ );
	temperatureEdit.SetWindowTextA ( "0" );
	temperatureDisplay.sPos = { pos.x + 200, pos.y, pos.x + 250, pos.y + 25 };
	temperatureDisplay.Create ( "", NORM_STATIC_OPTIONS, temperatureDisplay.seriesPos, parent, id++ );
	temperatureOffButton.sPos = { pos.x + 250, pos.y, pos.x + 300, pos.y += 25 };
	temperatureOffButton.Create ( "OFF", NORM_PUSH_OPTIONS, temperatureOffButton.seriesPos, parent, id++ );
	temperatureMsg.sPos = { pos.x, pos.y, pos.x + 300, pos.y += 50 };
	temperatureMsg.Create ( "Temperature control is disabled", NORM_STATIC_OPTIONS, temperatureMsg.seriesPos, parent, id++ );
}

void CameraSettingsControl::updateExposure( double exposure )
{
	exposureEdit.SetWindowTextA( cstr( exposure ) );
}

void CameraSettingsControl::handleCameraMode()
{
	int sel = cameraMode.GetCurSel();
	CString text;
	cameraMode.GetLBText( sel, text );
	if (text == "Finite Acquisition")
	{
		repEdit.EnableWindow();
	}
	else
	{
		repEdit.EnableWindow( false );
	}
}

// care, this only sets the settings that this control sees, not the settings that the camera itself sees. 
// (see BaslerCamera class for the latter)
void CameraSettingsControl::setSettings( CameraSettings settings )
{
	currentSettings = settings;
}


CameraSettings CameraSettingsControl::getCurrentSettings()
{
	return currentSettings;
}

CameraSettings CameraSettingsControl::loadCurrentSettings(POINT cameraDims)
{	
	isReady = false;
	int selection = exposureModeCombo.GetCurSel();
	CString text;
	exposureModeCombo.GetLBText(selection, text );
	currentSettings.exposureMode = std::string( text );
	if (currentSettings.exposureMode == "Auto Exposure Off")
	{
		try
		{
			exposureEdit.GetWindowTextA( text );
			currentSettings.exposureTime = std::stod( std::string( text ) );
			if (currentSettings.exposureTime <= 0)
			{
				throw std::invalid_argument( "err!" );
			}
		}
		catch (std::invalid_argument&)
		{
			thrower( "Error! Please input a valid double for the exposure time." );
		}
	}

	int sel = cameraMode.GetCurSel();
	cameraMode.GetLBText( sel, text );
	currentSettings.cameraMode = std::string( text );
	if (currentSettings.cameraMode == "Finite Acquisition")
	{
		try
		{
			repEdit.GetWindowTextA( text );
			currentSettings.repCount = std::stoi( std::string( text ) );
			if (currentSettings.repCount == 0)
			{
				thrower( "ERROR! Repetition count must be strictly positive." );
			}
		}
		catch (std::invalid_argument&)
		{
			thrower( "Error! Please input a valid positive integer for the rep count." );
		}
	}

	CString tempStr;
	temperatureEdit.GetWindowTextA ( tempStr );
	try
	{
		currentSettings.currentTemperatureVal = boost::lexical_cast<double>( str ( tempStr ) );
	}
	catch ( boost::bad_lexical_cast& err )
	{
		thrower ( "ERROR! Failed to convert temperature text to double!" );
	}

	// in case called before initialized
	if (!leftEdit)
	{
		return currentSettings;
	}
	// If new dimensions are set, we don't have data for the new dimensions.
	// set all of the image parameters
	leftEdit.GetWindowTextA( tempStr );
	try
	{
		int val = currentSettings.dimensions.leftBorder = std::stoi( std::string( tempStr ) );
	}
	catch (std::invalid_argument&)
	{		
		thrower( "Left border argument not an integer!\r\n" );
	}
	leftEdit.RedrawWindow();

	horizontalBinningEdit.GetWindowTextA(tempStr);
	try
	{
		currentSettings.dimensions.horPixelsPerBin = std::stoi(std::string(tempStr));
	}
	catch (std::invalid_argument&)
	{
		thrower("Horizontal binning argument not an integer!\r\n");
	}
	horizontalBinningEdit.RedrawWindow();

	rightEdit.GetWindowTextA( tempStr );
	try
	{
		currentSettings.dimensions.rightBorder = std::stoi( std::string( tempStr ) );
	}
	catch (std::invalid_argument&)
	{
		thrower( "Right border argument not an integer!\r\n" );
	}
	rightEdit.RedrawWindow();
	//
	topEdit.GetWindowTextA( tempStr );
	try
	{
		int val = currentSettings.dimensions.topBorder = std::stoi(std::string(tempStr));
	}
	catch (std::invalid_argument&)
	{
		thrower( "Top border argument not an integer!\r\n" );
	}
	topEdit.RedrawWindow();
	//
	verticalBinningEdit.GetWindowTextA(tempStr);
	try
	{
		currentSettings.dimensions.vertPixelsPerBin = std::stoi(std::string(tempStr));
	}
	catch (std::invalid_argument&)
	{
		thrower("Vertical binning argument not an integer!\r\n");
	}
	verticalBinningEdit.RedrawWindow();
	//
	bottomEdit.GetWindowTextA( tempStr );
	try
	{
		currentSettings.dimensions.bottomBorder = std::stoi( std::string( tempStr ) );
	}
	catch (std::invalid_argument&)
	{
		thrower( "Bottom border argument not an integer!\r\n" );
	}
	bottomEdit.RedrawWindow();

	currentSettings.dimensions.horRawPixelNumber = currentSettings.dimensions.rightBorder - currentSettings.dimensions.leftBorder + 1;
	currentSettings.dimensions.vertRawPixelNumber = currentSettings.dimensions.bottomBorder - currentSettings.dimensions.topBorder + 1;

	currentSettings.dimensions.horBinNumber = currentSettings.dimensions.horRawPixelNumber / currentSettings.dimensions.horPixelsPerBin;
	currentSettings.dimensions.vertBinNumber = currentSettings.dimensions.vertRawPixelNumber / currentSettings.dimensions.vertPixelsPerBin;
	
	// Check Image parameters
	if (currentSettings.dimensions.leftBorder > currentSettings.dimensions.rightBorder 
		 || currentSettings.dimensions.topBorder > currentSettings.dimensions.bottomBorder)
	{
		thrower( "ERROR: Image start positions must not be greater than end positions\r\n" );
	}
	if (currentSettings.dimensions.leftBorder < 0)
	{
		thrower( "ERROR: Image horizontal borders must be greater than 0 and less than the detector width\r\n" );
	}
	if (currentSettings.dimensions.topBorder < 0)
	{
		thrower( "ERROR: Image verttical borders must be greater than 0 and less than the detector height\r\n" );
	}
	if ((currentSettings.dimensions.rightBorder - currentSettings.dimensions.leftBorder + 1) % currentSettings.dimensions.horPixelsPerBin != 0)
	{
		thrower( "ERROR: Image width must be a multiple of Horizontal Binning\r\n" );
	}
	if ((currentSettings.dimensions.bottomBorder - currentSettings.dimensions.topBorder + 1) % currentSettings.dimensions.vertPixelsPerBin != 0)
	{
		thrower( "ERROR: Image height must be a multiple of Vertical Binning\r\n" );
	}
	//if (currentSettings.dimensions.horPixelsPerBin > 4 || currentSettings.dimensions.vertPixelsPerBin > 4)
	//{
	//	thrower( "ERROR: Binning on a camera cannot exceed 4 pixels per bin!\r\n" );
	//}

	selection = triggerCombo.GetCurSel();
	triggerCombo.GetLBText( selection, text );
	currentSettings.triggerMode = std::string( text );

	frameRateEdit.GetWindowTextA( text );
	try
	{
		currentSettings.frameRate = std::stod( std::string( text ) );
	}
	catch (std::invalid_argument& err)
	{
		thrower( std::string("ERROR! Please enter a valid float for the frame rate. ") + err.what() );
	}
	isReady = true;

	return currentSettings;
}

void CameraSettingsControl::handleGain()
{

}


void CameraSettingsControl::handleExposureMode()
{
	int sel = exposureModeCombo.GetCurSel();
	CString text;
	exposureModeCombo.GetLBText(sel, text);
	if (text == "Auto Exposure Off")
	{
		exposureEdit.EnableWindow();
	}
	else
	{
		exposureEdit.EnableWindow(false);
	}
}


