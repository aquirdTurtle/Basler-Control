#include "stdafx.h"

#include "BaslerSettingsControl.h"
#include "constants.h"


void BaslerSettingsControl::rearrange(int width, int height, fontMap fonts)
{
	// arm
	armCameraButton.rearrange("", "", width, height, fonts);
	disarmCameraButton.rearrange("", "", width, height, fonts);
	// exposure
	exposureText.rearrange("", "", width, height, fonts);
	exposureModeCombo.rearrange("", "", width, height, fonts);
	exposureEdit.rearrange("", "", width, height, fonts);
	setExposure.rearrange("", "", width, height, fonts);
	// trigger
	triggerText.rearrange("", "", width, height, fonts);
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

}

void BaslerSettingsControl::initialize( POINT& pos, int& id, CWnd* parent, int picWidth, int picHeight )
{
	int width = 300;
	// arm
	armCameraButton.sPos = { pos.x, pos.y, pos.x + 150, pos.y + 25 };
	armCameraButton.ID = id++;
	if (armCameraButton.ID != IDC_ARM_BASLER_BUTTON)
	{
		throw;
	}
	armCameraButton.Create( "Arm Camera", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, armCameraButton.sPos, parent, armCameraButton.ID );
	armCameraButton.fontType = Normal;

	disarmCameraButton.sPos = { pos.x + 150, pos.y, pos.x + 300, pos.y += 25 };
	disarmCameraButton.ID = id++;
	if (disarmCameraButton.ID != IDC_DISARM_BASLER_BUTTON)
	{
		throw;
	}
	disarmCameraButton.Create( "Disarm Camera", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, disarmCameraButton.sPos, parent, 
							   disarmCameraButton.ID );
	disarmCameraButton.fontType = Normal;

	repText.sPos = { pos.x, pos.y, pos.x + 200, pos.y + 25 };
	repText.ID = id++;
	repText.Create( "Total Picture Number:", WS_CHILD | WS_VISIBLE, repText.sPos, parent, repText.ID );
	repText.fontType = Normal;
	repEdit.sPos = { pos.x + 200, pos.y, pos.x + 300, pos.y += 25 };
	repEdit.ID = id++;
	if (repEdit.ID != IDC_REPETITIONS_EDIT)
	{
		throw;
	}
	repEdit.Create( WS_CHILD | WS_VISIBLE, repEdit.sPos, parent, repEdit.ID );
	repEdit.SetWindowTextA( "---" );

	cameraMode.sPos = { pos.x, pos.y, pos.x + 300, pos.y + 100 };
	cameraMode.ID = id++;
	if (cameraMode.ID != IDC_CAMERA_MODE_COMBO)
	{
		throw;
	}
	cameraMode.Create( WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_SORT, cameraMode.sPos, parent,
					   cameraMode.ID );
	pos.y += 25;
	cameraMode.AddString( "Finite Acquisition" );
	cameraMode.AddString( "Continuous Acquisition" );
	cameraMode.SelectString( 0, "Continuous Acquisition" );
	repEdit.EnableWindow( false );
	
	exposureText.sPos = { pos.x, pos.y, pos.x + 200, pos.y + 25 };
	exposureText.ID = id++;
	exposureText.Create( "Exposure Time (us):", WS_CHILD | WS_VISIBLE, exposureText.sPos, parent, exposureText.ID );

	exposureEdit.sPos = { pos.x + 200, pos.y, pos.x + 300, pos.y += 25 };
	exposureEdit.ID = id++;
	exposureEdit.Create( WS_CHILD | WS_VISIBLE, exposureEdit.sPos, parent, exposureEdit.ID );
	exposureEdit.SetWindowTextA( "---" );

	exposureModeCombo.sPos = { pos.x, pos.y, pos.x + 300, pos.y + 100 };
	exposureModeCombo.ID = id++;
	if (exposureModeCombo.ID != IDC_EXPOSURE_MODE_COMBO)
	{
		throw;
	}
	exposureModeCombo.Create( WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_SORT, exposureModeCombo.sPos, parent, 
							  exposureModeCombo.ID );

	pos.y += 25;
	exposureModeCombo.AddString( "Auto Exposure Continuous" );
	exposureModeCombo.AddString( "Auto Exposure Off" );
	exposureModeCombo.AddString( "Auto Exposure Once" );
	exposureModeCombo.SelectString( 0, "Auto Exposure Continuous" );
	exposureEdit.EnableWindow( false );

	/// image dimensions
	//
	leftText.ID = id++;
	leftText.sPos = { pos.x, pos.y, pos.x + width/3, pos.y + 25 };
	leftText.Create( "Left", WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY, leftText.sPos, parent, leftText.ID );
	
	//
	rightText.ID = id++;
	rightText.sPos = { pos.x + width/3, pos.y, pos.x + 2* width/3, pos.y + 25 };
	rightText.Create( "Right", WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY, rightText.sPos, parent, rightText.ID );
	
	//
	horizontalBinningText.ID = id++;
	horizontalBinningText.sPos = { pos.x + 2* width/3, pos.y, pos.x + width, pos.y += 25 };
	horizontalBinningText.Create( "H. Bin", WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY, horizontalBinningText.sPos, parent,
								  horizontalBinningText.ID );
	
	//
	leftEdit.ID = id++;
	leftEdit.sPos = { pos.x, pos.y, pos.x + width/3, pos.y + 25 };
	leftEdit.Create( WS_TABSTOP | WS_CHILD | WS_VISIBLE | ES_CENTER, leftEdit.sPos, parent, leftEdit.ID );
	leftEdit.fontType = Normal;
	leftEdit.SetWindowTextA( "0" );
	//
	rightEdit.ID = id++;
	rightEdit.sPos = { pos.x + width/3, pos.y, pos.x + 2* width/3, pos.y + 25 };
	rightEdit.Create( WS_TABSTOP | WS_CHILD | WS_VISIBLE | ES_CENTER, rightEdit.sPos, parent, rightEdit.ID );
	rightEdit.fontType = Normal;
	rightEdit.SetWindowTextA( cstr(picWidth-1) );
	//
	horizontalBinningEdit.ID = id++;
	horizontalBinningEdit.sPos = { pos.x + 2* width/3, pos.y, pos.x + width, pos.y += 25 };
	horizontalBinningEdit.Create( WS_TABSTOP | WS_CHILD | WS_VISIBLE | ES_CENTER, horizontalBinningEdit.sPos, parent, 
								  horizontalBinningEdit.ID );
	horizontalBinningEdit.fontType = Normal;
	horizontalBinningEdit.SetWindowTextA( "1" );
	//
	topText.ID = id++;
	topText.sPos = { pos.x, pos.y, pos.x + width/3, pos.y + 25 };
	topText.Create( "Top", WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY, topText.sPos, parent, topText.ID );
	topText.fontType = Normal;

	//
	bottomText.ID = id++;
	bottomText.sPos = { pos.x + width/3, pos.y, pos.x +2* width/3, pos.y + 25 };
	bottomText.Create( "Bottom", WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY, bottomText.sPos, parent, bottomText.ID );
	bottomText.fontType = Normal;
	//
	verticalBinningText.ID = id++;
	verticalBinningText.sPos = { pos.x + 2* width/3, pos.y, pos.x + width, pos.y += 25 };
	verticalBinningText.Create( "V. Bin", WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY, verticalBinningText.sPos, parent,
								verticalBinningText.ID );
	verticalBinningText.fontType = Normal;
	//
	topEdit.ID = id++;
	topEdit.sPos = { pos.x, pos.y, pos.x + width/3, pos.y + 25 };
	topEdit.Create( WS_TABSTOP | WS_CHILD | WS_VISIBLE | ES_CENTER, topEdit.sPos, parent, topEdit.ID );
	topEdit.fontType = Normal;
	topEdit.SetWindowTextA( "0" );
	//
	bottomEdit.ID = id++;
	bottomEdit.sPos = { pos.x + width/3, pos.y, pos.x + 2* width/3, pos.y + 25 };
	bottomEdit.Create( WS_TABSTOP | WS_CHILD | WS_VISIBLE | ES_CENTER, bottomEdit.sPos, parent, bottomEdit.ID );
	bottomEdit.fontType = Normal;
	bottomEdit.SetWindowTextA( cstr(picHeight-1) );
	//
	verticalBinningEdit.ID = id++;
	verticalBinningEdit.sPos = { pos.x + 2* width/3, pos.y, pos.x + width, pos.y += 25 };
	verticalBinningEdit.Create( WS_TABSTOP | WS_CHILD | WS_VISIBLE | ES_CENTER, verticalBinningEdit.sPos, parent, verticalBinningEdit.ID );
	verticalBinningEdit.fontType = Normal;
	verticalBinningEdit.SetWindowTextA( "1" );

	triggerText.ID = id++;
	triggerText.sPos = { pos.x, pos.y, pos.x + 100, pos.y + 25 };
	triggerText.Create( "Trigger Mode", WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY, triggerText.sPos, parent, triggerText.ID );

	triggerCombo.ID = id++;
	if (triggerCombo.ID != IDC_TRIGGER_MODE_COMBO)
	{
		throw;
	}
	triggerCombo.sPos = { pos.x + 100, pos.y, pos.x + 300, pos.y + 100 };
	triggerCombo.Create( WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_SORT, triggerCombo.sPos, parent, triggerCombo.ID );
	triggerCombo.AddString( "External Trigger" );
	triggerCombo.AddString( "Automatic Software Trigger" );
	triggerCombo.AddString( "Manual Software Trigger" );
	triggerCombo.SelectString( 0, "Automatic Software Trigger" );
	pos.y += 25;

	frameRateText.sPos = { pos.x, pos.y, pos.x + 100, pos.y + 25 };
	frameRateText.ID = id++;
	frameRateText.Create( "Frame Rate (pics/s): ", WS_CHILD | WS_VISIBLE, frameRateText.sPos, parent, frameRateText.ID );

	frameRateEdit.sPos = { pos.x + 100, pos.y, pos.x + 200, pos.y + 25 };
	frameRateEdit.ID = id++;
	frameRateEdit.Create( WS_CHILD | WS_VISIBLE, frameRateEdit.sPos, parent, frameRateEdit.ID );
	frameRateEdit.SetWindowTextA( "30" );


	realFrameRate.sPos = { pos.x + 200, pos.y, pos.x + 300, pos.y += 25 };
	realFrameRate.ID = id++;
	realFrameRate.Create( "", WS_CHILD | WS_VISIBLE, realFrameRate.sPos, parent, realFrameRate.ID );

	gainCombo.sPos = { pos.x, pos.y, pos.x + 300, pos.y + 100 };
	gainCombo.ID = id++;
	gainCombo.Create( WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_SORT, gainCombo.sPos, parent, gainCombo.ID );
	gainCombo.AddString( "Auto Gain Continuous" );
	gainCombo.AddString( "Auto Gain Once" );
	gainCombo.AddString( "Auto Gain Off" );
	gainCombo.SelectString( 0, "Auto Gain Off" );
	pos.y += 25;

	gainText.sPos = { pos.x, pos.y, pos.x + 150, pos.y + 25 };
	gainText.ID = id++;
	gainText.Create( "Raw Gain: ", WS_CHILD | WS_VISIBLE, gainText.sPos, parent, gainText.ID );

	gainEdit.sPos = { pos.x + 150, pos.y, pos.x + 300, pos.y += 25 };
	gainEdit.ID = id++;
	gainEdit.Create( WS_CHILD | WS_VISIBLE, gainEdit.sPos, parent, gainEdit.ID );
	gainEdit.SetWindowTextA( "260" );

	realGainText.sPos = { pos.x, pos.y, pos.x + 150, pos.y + 25 };
	realGainText.ID = id++;
	realGainText.Create( "Real Gain: ", WS_CHILD | WS_VISIBLE, realGainText.sPos, parent, realGainText.ID );

	realGainStatus.sPos = { pos.x + 150, pos.y, pos.x + 300, pos.y += 25 };
	realGainStatus.ID = id++;
	realGainStatus.Create( "", WS_CHILD | WS_VISIBLE, realGainStatus.sPos, parent, realGainStatus.ID );

	/*
	// gain
	Control<CStatic> gainText;
	Control<CSliderCtrl> gainSlider;
	Control<CEdit> rawGainEdit;
	Control<CEdit> realGainText;
	*/
}

void BaslerSettingsControl::updateExposure( double exposure )
{
	exposureEdit.SetWindowTextA( cstr( exposure ) );
}

void BaslerSettingsControl::handleCameraMode()
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
void BaslerSettingsControl::setSettings( baslerSettings settings )
{
	currentSettings = settings;
}


baslerSettings BaslerSettingsControl::getCurrentSettings()
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
		catch (std::invalid_argument& err)
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
		catch (std::invalid_argument& err)
		{
			thrower( "Error! Please input a valid positive integer for the rep count." );
		}
	}

	// in case called before initialized
	if (!leftEdit)
	{
		return currentSettings;
	}
	// If new dimensions are set, we don't have data for the new dimensions.
	// set all of the image parameters
	CString tempStr;
	leftEdit.GetWindowTextA( tempStr );
	try
	{
		currentSettings.dimensions.leftBorder = std::stoi( std::string( tempStr ) );
	}
	catch (std::invalid_argument&)
	{
		
		thrower( "Left border argument not an integer!\r\n" );
	}
	leftEdit.RedrawWindow();
	rightEdit.GetWindowTextA( tempStr );
	try
	{
		currentSettings.dimensions.rightBorder = std::stoi( std::string( tempStr ) );
		// round up to nearest multiple of 16
		int width = currentSettings.dimensions.rightBorder - currentSettings.dimensions.leftBorder + 1;
		width = ((width + 16 - 1) / 16) * 16;
		currentSettings.dimensions.rightBorder = currentSettings.dimensions.leftBorder + width - 1;
		rightEdit.SetWindowTextA( cstr( currentSettings.dimensions.rightBorder ) );
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
		currentSettings.dimensions.topBorder = std::stoi( std::string( tempStr ) );
	}
	catch (std::invalid_argument&)
	{
		thrower( "Top border argument not an integer!\r\n" );
	}
	topEdit.RedrawWindow();
	//
	bottomEdit.GetWindowTextA( tempStr );
	try
	{
		currentSettings.dimensions.bottomBorder = std::stoi( std::string( tempStr ) );
		// round up to nearest multiple of 16
		int height = currentSettings.dimensions.bottomBorder - currentSettings.dimensions.topBorder + 1;
		height = ((height + 16 - 1) / 16) * 16;
		currentSettings.dimensions.bottomBorder = currentSettings.dimensions.topBorder + height - 1;
		bottomEdit.SetWindowTextA( cstr(currentSettings.dimensions.bottomBorder) );
	}
	catch (std::invalid_argument&)
	{
		thrower( "Bottom border argument not an integer!\r\n" );
	}
	bottomEdit.RedrawWindow();
	horizontalBinningEdit.GetWindowTextA( tempStr );
	try
	{
		currentSettings.dimensions.horPixelsPerBin = std::stoi( std::string( tempStr ) );
	}
	catch (std::invalid_argument&)
	{
		thrower( "Horizontal binning argument not an integer!\r\n" );
	}
	horizontalBinningEdit.RedrawWindow();
	verticalBinningEdit.GetWindowTextA( tempStr );
	try
	{
		currentSettings.dimensions.vertPixelsPerBin = std::stoi( std::string( tempStr ) );
	}
	catch (std::invalid_argument&)
	{
		thrower( "Vertical binning argument not an integer!\r\n" );
	}
	verticalBinningEdit.RedrawWindow();
	
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
	if (currentSettings.dimensions.horRawPixelNumber % 16 != 0 || currentSettings.dimensions.vertRawPixelNumber % 16 != 0)
	{
		thrower( "ERROR: In a basler camera, the number of pixels in each dimension must be a multiple of 16!\r\n" );
	}
	if (currentSettings.dimensions.horPixelsPerBin > 4 || currentSettings.dimensions.vertPixelsPerBin > 4)
	{
		thrower( "ERROR: Binning on a camera cannot exceed 4 pixels per bin!\r\n" );
	}

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

void BaslerSettingsControl::handleGain()
{

}



void BaslerSettingsControl::handleExposureMode()
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


