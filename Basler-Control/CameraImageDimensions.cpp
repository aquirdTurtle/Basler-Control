/// This file contains all functiosn for the CameraImageParametersControlParameters singleton class.

#include "stdafx.h"
#include "CameraImageDimensions.h"
#include "BaslerSettingsControl.h"
#include "BaslerWindow.h"

void CameraImageDimensionsControl::cameraIsOn( bool state )
{
	setImageDimensionsButton.EnableWindow( !state );
}


CameraImageDimensionsControl::CameraImageDimensionsControl()
{
	isReady = false;
}

imageParameters CameraImageDimensionsControl::readImageParameters( BaslerCameraWindow* camWin )
{
	// in case called before initialized
	if (!leftEdit)
	{
		return currentImageParameters;
	}
	//drawBackgrounds( camWin );
	// If new dimensions are set, we don't have data for the new dimensions.
	// eDataExists = false;
	// set all of the image parameters
	CString tempStr;
	leftEdit.GetWindowTextA( tempStr );
	try
	{
		currentImageParameters.leftBorder = std::stoi( std::string( tempStr ) );
	}
	catch (std::invalid_argument&)
	{
		isReady = false;
		thrower( "Left border argument not an integer!\r\n" );
	}
	leftEdit.RedrawWindow();
	rightEdit.GetWindowTextA( tempStr );
	try
	{
		currentImageParameters.rightBorder = std::stoi( std::string( tempStr ) );
	}
	catch (std::invalid_argument&)
	{
		isReady = false;
		thrower( "Right border argument not an integer!\r\n" );
	}
	rightEdit.RedrawWindow();
	//
	topEdit.GetWindowTextA( tempStr );
	try
	{
		currentImageParameters.topBorder = std::stoi( std::string( tempStr ) );
	}
	catch (std::invalid_argument&)
	{
		isReady = false;
		thrower( "Top border argument not an integer!\r\n" );
	}
	topEdit.RedrawWindow();
	//
	bottomEdit.GetWindowTextA( tempStr );
	try
	{
		currentImageParameters.bottomBorder = std::stoi( std::string( tempStr ) );
	}
	catch (std::invalid_argument&)
	{
		isReady = false;
		thrower( "Bottom border argument not an integer!\r\n" );
	}
	bottomEdit.RedrawWindow();
	horizontalBinningEdit.GetWindowTextA( tempStr );
	try
	{
		currentImageParameters.horPixelsPerBin = std::stoi( std::string( tempStr ) );
	}
	catch (std::invalid_argument&)
	{
		isReady = false;
		thrower( "Horizontal binning argument not an integer!\r\n" );
	}
	horizontalBinningEdit.RedrawWindow();
	verticalBinningEdit.GetWindowTextA( tempStr );
	try
	{
		currentImageParameters.vertPixelsPerBin = std::stoi( std::string( tempStr ) );
	}
	catch (std::invalid_argument&)
	{
		isReady = false;
		thrower( "Vertical binning argument not an integer!\r\n" );
	}
	verticalBinningEdit.RedrawWindow();
	// reset this. There must be at least one pixel...
	/// TODO
	/*
	eCurrentlySelectedPixel.first = 0;
	eCurrentlySelectedPixel.second = 0;
	*/
	// Calculate the number of actual pixels in each dimension.
	currentImageParameters.horBinNumber = (currentImageParameters.rightBorder - currentImageParameters.leftBorder + 1) / currentImageParameters.horPixelsPerBin;
	currentImageParameters.vertBinNumber = (currentImageParameters.bottomBorder - currentImageParameters.topBorder + 1) / currentImageParameters.vertPixelsPerBin;

	// Check Image parameters
	if (currentImageParameters.leftBorder > currentImageParameters.rightBorder || currentImageParameters.topBorder > currentImageParameters.bottomBorder)
	{
		isReady = false;
		thrower( "ERROR: Image start positions must not be greater than end positions\r\n" );
	}
	if (currentImageParameters.leftBorder < 1 || currentImageParameters.rightBorder > 512)
	{
		isReady = false;
		thrower( "ERROR: Image horizontal borders must be greater than 0 and less than the detector width\r\n" );
	}
	if (currentImageParameters.topBorder < 1 || currentImageParameters.bottomBorder > 512)
	{
		isReady = false;
		thrower( "ERROR: Image verttical borders must be greater than 0 and less than the detector height\r\n" );
	}
	if ((currentImageParameters.rightBorder - currentImageParameters.leftBorder + 1) % currentImageParameters.horPixelsPerBin != 0)
	{
		isReady = false;
		thrower( "ERROR: Image width must be a multiple of Horizontal Binning\r\n" );
	}
	if ((currentImageParameters.bottomBorder - currentImageParameters.topBorder + 1) % currentImageParameters.vertPixelsPerBin != 0)
	{
		isReady = false;
		thrower( "ERROR: Image height must be a multiple of Vertical Binning\r\n" );
	}
	// made it through successfully.
	isReady = true;
	return currentImageParameters;
}


/*
 * I forget why I needed a second function for this.
 */
void CameraImageDimensionsControl::setImageParametersFromInput( imageParameters param, BaslerCameraWindow* camWin )
{
	//drawBackgrounds( camWin );
	//eDataExists = false;
	// set all of the image parameters
	currentImageParameters.leftBorder = param.leftBorder;
	leftEdit.SetWindowText( std::to_string( currentImageParameters.leftBorder ).c_str() );
	currentImageParameters.rightBorder = param.rightBorder;
	rightEdit.SetWindowText( std::to_string( currentImageParameters.rightBorder ).c_str() );
	currentImageParameters.topBorder = param.topBorder;
	topEdit.SetWindowText( std::to_string( currentImageParameters.topBorder ).c_str() );
	currentImageParameters.bottomBorder = param.bottomBorder;
	bottomEdit.SetWindowText( std::to_string( currentImageParameters.bottomBorder ).c_str() );
	currentImageParameters.horPixelsPerBin = param.horPixelsPerBin;
	horizontalBinningEdit.SetWindowText( std::to_string( currentImageParameters.horPixelsPerBin ).c_str() );
	currentImageParameters.vertPixelsPerBin = param.vertPixelsPerBin;
	verticalBinningEdit.SetWindowText( std::to_string( currentImageParameters.vertPixelsPerBin ).c_str() );
	// reset this. There must be at least one pixel...
	/*
	eCurrentlySelectedPixel.first = 0;
	eCurrentlySelectedPixel.second = 0;
	*/
	// Calculate the number of actual pixels in each dimension.
	currentImageParameters.horBinNumber = (currentImageParameters.rightBorder - currentImageParameters.leftBorder + 1) / currentImageParameters.horPixelsPerBin;
	currentImageParameters.vertBinNumber = (currentImageParameters.bottomBorder - currentImageParameters.topBorder + 1) / currentImageParameters.vertPixelsPerBin;

	// Check Image parameters
	if (currentImageParameters.leftBorder > currentImageParameters.rightBorder || currentImageParameters.topBorder > currentImageParameters.bottomBorder)
	{
		isReady = false;
		thrower( "ERROR: Image start positions must not be greater than end positions\r\n" );
	}
	if (currentImageParameters.leftBorder < 1 || currentImageParameters.rightBorder > 512)
	{
		isReady = false;
		thrower( "ERROR: Image horizontal borders must be greater than 0 and less than the detector width\r\n" );
	}
	if (currentImageParameters.topBorder < 1 || currentImageParameters.bottomBorder > 512)
	{
		isReady = false;
		thrower( "ERROR: Image verttical borders must be greater than 0 and less than the detector height\r\n" );
	}
	if ((currentImageParameters.rightBorder - currentImageParameters.leftBorder + 1) % currentImageParameters.horPixelsPerBin != 0)
	{
		isReady = false;
		thrower( "ERROR: Image width must be a multiple of Horizontal Binning\r\n" );
	}
	if ((currentImageParameters.bottomBorder - currentImageParameters.topBorder + 1) % currentImageParameters.vertPixelsPerBin != 0)
	{
		isReady = false;
		thrower( "ERROR: Image height must be a multiple of Vertical Binning\r\n" );
	}
	isReady = true;
}


bool CameraImageDimensionsControl::checkReady()
{
	if (isReady)
	{
		return true;
	}
	else
	{
		return false;
	}
}


imageParameters CameraImageDimensionsControl::getImageParameters()
{
	return currentImageParameters;
}


void CameraImageDimensionsControl::rearrange( std::string cameraMode, std::string triggerMode, int horBinNumber, 
											 int vertBinNumber, std::unordered_map<std::string, CFont*> fonts )
{
	leftText.rearrange( cameraMode, triggerMode, horBinNumber, vertBinNumber, fonts );
	rightText.rearrange( cameraMode, triggerMode, horBinNumber, vertBinNumber, fonts );
	horizontalBinningText.rearrange( cameraMode, triggerMode, horBinNumber, vertBinNumber, fonts );
	topText.rearrange( cameraMode, triggerMode, horBinNumber, vertBinNumber, fonts );
	bottomText.rearrange( cameraMode, triggerMode, horBinNumber, vertBinNumber, fonts );
	verticalBinningText.rearrange( cameraMode, triggerMode, horBinNumber, vertBinNumber, fonts );
	leftEdit.rearrange( cameraMode, triggerMode, horBinNumber, vertBinNumber, fonts );
	rightEdit.rearrange( cameraMode, triggerMode, horBinNumber, vertBinNumber, fonts );
	horizontalBinningEdit.rearrange( cameraMode, triggerMode, horBinNumber, vertBinNumber, fonts );
	topEdit.rearrange( cameraMode, triggerMode, horBinNumber, vertBinNumber, fonts );
	bottomEdit.rearrange( cameraMode, triggerMode, horBinNumber, vertBinNumber, fonts );
	verticalBinningEdit.rearrange( cameraMode, triggerMode, horBinNumber, vertBinNumber, fonts );
	setImageDimensionsButton.rearrange( cameraMode, triggerMode, horBinNumber, vertBinNumber, fonts );
}

