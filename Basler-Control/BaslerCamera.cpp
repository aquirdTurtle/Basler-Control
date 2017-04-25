#include "stdafx.h"
#include "BaslerCamera.h"
#include "PictureControl.h"
#include <algorithm>
#include <functional>
#include "BaslerControlWindow.h"
#include "BaslerControlApp.h"
#include "stdint.h"
// important constructor;
// Create an instant camera object with the camera device found first.
BaslerCameras::BaslerCameras(HWND* parent)
{
	Pylon::PylonInitialize();
	Pylon::CDeviceInfo info;
	info.SetDeviceClass( Pylon::CBaslerUsbInstantCamera::DeviceClass() );
	camera = new Pylon::CBaslerUsbInstantCamera( Pylon::CTlFactory::GetInstance().CreateFirstDevice( info ) );
	camera->Open();
	// prepare the image event handler
	//camera->RegisterConfiguration( new Pylon::CSoftwareTriggerConfiguration, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_Delete );
	//camera->RegisterImageEventHandler( new ImageEventHandler( parent ), Pylon::RegistrationMode_Append, Pylon::Cleanup_Delete );
	camera->RegisterImageEventHandler( new ImageEventHandler( parent ), Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_Delete );
	camera->TriggerMode.SetValue( Basler_UsbCameraParams::TriggerMode_On );
}


void BaslerCameras::softwareTrigger()
{
	camera->WaitForFrameTriggerReady( 500, Pylon::TimeoutHandling_ThrowException );
	camera->ExecuteSoftwareTrigger();
}


BaslerCameras::~BaslerCameras()
{
	Pylon::PylonTerminate();
	delete camera;
}


std::string BaslerCameras::getCameraInfo()
{
	// Get camera device information.
	return "Camera Device Information\n=========================\nVendor           : " + std::string( camera->DeviceVendorName.GetValue() )
			+ "\nModel            : " + std::string( camera->DeviceModelName.GetValue() ) + "\nFirmware version : "
			+ std::string( camera->DeviceFirmwareVersion.GetValue() ) ;
}


baslerSettings BaslerCameras::getDefaultSettings()
{
	baslerSettings defaultSettings;

	defaultSettings.dimensions.leftBorder = camera->OffsetX.GetMin();
	defaultSettings.dimensions.horBinNumber = camera->WidthMax();
	defaultSettings.dimensions.rightBorder = camera->OffsetX.GetMin() + camera->WidthMax() - 5;
	defaultSettings.dimensions.horPixelsPerBin = 1;
	defaultSettings.dimensions.topBorder = camera->OffsetY.GetMin();
	defaultSettings.dimensions.vertBinNumber = camera->HeightMax();
	defaultSettings.dimensions.bottomBorder = camera->OffsetY.GetMin() + camera->HeightMax() - 5;
	defaultSettings.dimensions.vertPixelsPerBin = 1;
	defaultSettings.exposureMode = "Auto Exposure Continuous";
	defaultSettings.exposureTime = 0; // doesn't matter for continuous.

	defaultSettings.rawGain = camera->Gain.GetMin();

	//defaultSettings.triggerMode = ???
	return defaultSettings;
}

void BaslerCameras::setDefaultParameters()
{
	setParameters( getDefaultSettings() );
}



void BaslerCameras::setParameters( baslerSettings settings )
{
	// Set the AOI:
	// On some cameras the Offsets are read-only,
	// so we check whether we can write a value. Otherwise, we would get an exception.
	// GenApi has some convenience predicates to check this easily.
	//errBox( str( camera->OffsetX.GetMin() ) );
	//errBox( str( camera->OffsetX.GetMax() ) );
	
	camera->OffsetX.SetValue( settings.dimensions.leftBorder );
	camera->OffsetY.SetValue( settings.dimensions.topBorder );
	
	camera->Width.SetValue( settings.dimensions.horRawPixelNumber );
	camera->Height.SetValue( settings.dimensions.vertRawPixelNumber );
	
	camera->BinningHorizontal.SetValue( settings.dimensions.horPixelsPerBin );
	camera->BinningVertical.SetValue( settings.dimensions.vertPixelsPerBin );

	camera->PixelFormat.SetValue( Basler_UsbCameraParams::PixelFormat_Mono10 );
	//camera->AcquisitionMode.SetValue( Basler_UsbCameraParams::AcquisitionModeEnums::AcquisitionMode_Continuous );

	// Set the new gain to 50% ->  Min + ((Max-Min) / 2)
	//
	// Note: Some newer camera models may have auto functions enabled.
	//       To be able to set the gain value to a specific value
	//       the Gain Auto function must be disabled first.
	if (GenApi::IsWritable( camera->GainAuto ))
	{
		camera->GainAuto.FromString( "Off" );
	}
	camera->Gain.SetValue( camera->Gain.GetMin() );

	// exposure mode
	if (settings.exposureMode == "Auto Exposure Continuous")
	{
		camera->ExposureAuto.SetValue( Basler_UsbCameraParams::ExposureAutoEnums::ExposureAuto_Continuous );
	}
	else if (settings.exposureMode == "Auto Exposure Off")
	{
		camera->ExposureAuto.SetValue( Basler_UsbCameraParams::ExposureAutoEnums::ExposureAuto_Off );
		if (!(settings.exposureTime >= camera->ExposureTime.GetMin() && settings.exposureTime <= camera->ExposureTime.GetMax()))
		{
			thrower( "ERROR: exposure time must be between " + str( camera->ExposureTime.GetMin() ) + " and " 
					 + str( camera->ExposureTime.GetMax()) );
		}
		camera->ExposureTime.SetValue( settings.exposureTime );
	}
	else if (settings.exposureMode == "Auto Exposure Once")
	{
		camera->ExposureAuto.SetValue( Basler_UsbCameraParams::ExposureAutoEnums::ExposureAuto_Once );
	}

	if (settings.cameraMode == "Finite Acquisition")
	{
		continuousImaging = false;
		repCounts = settings.repCount;
	}
	else
	{
		continuousImaging = true;
		repCounts = SIZE_MAX;
	}

	if (settings.triggerMode == "External Trigger")
	{
		camera->TriggerSource.SetValue( Basler_UsbCameraParams::TriggerSource_Line4 );
		autoTrigger = false;
	}
	else if (settings.triggerMode == "Automatic Software Trigger")
	{
		camera->TriggerSource.SetValue( Basler_UsbCameraParams::TriggerSource_Software );
		autoTrigger = true;
	}
	else if (settings.triggerMode == "Manual Software Trigger")
	{
		camera->TriggerSource.SetValue( Basler_UsbCameraParams::TriggerSource_Software );
		autoTrigger = false;
	}
}

// i can potentially use this to reopen the camera if e.g. the user disconnects.
void BaslerCameras::reOpenCamera()
{
	Pylon::CDeviceInfo info;
	info.SetDeviceClass( Pylon::CBaslerUsbInstantCamera::DeviceClass() );
	delete camera;
	camera = new Pylon::CBaslerUsbInstantCamera( Pylon::CTlFactory::GetInstance().CreateFirstDevice( info ) );
	camera->Open();
}

POINT BaslerCameras::getCameraDimensions()
{
	POINT dim;
	// record the original offsets and bins.
	POINT offsets, bins;
	offsets.x = camera->OffsetX.GetValue();
	offsets.y = camera->OffsetY.GetValue();
	bins.x = camera->BinningHorizontal.GetValue();
	bins.y = camera->BinningVertical.GetValue();
	
	camera->BinningHorizontal.SetValue( camera->BinningHorizontal.GetMin() );
	camera->BinningVertical.SetValue( camera->BinningVertical.GetMin() );
	camera->OffsetX.SetValue( camera->OffsetX.GetMin() );
	camera->OffsetY.SetValue( camera->OffsetY.GetMin() );

	dim.x = camera->Width.GetMax();
	dim.y = camera->Height.GetMax();

	camera->OffsetX.SetValue( offsets.x );
	camera->OffsetY.SetValue( offsets.y );
	camera->BinningHorizontal.SetValue( bins.x );
	camera->BinningVertical.SetValue( bins.y );

	return dim;
}


void BaslerCameras::cameraWatchThread( void* inputPtr )
{
	cameraWatchThreadInput* input = (cameraWatchThreadInput*) inputPtr;
	try
	{
		// This smart pointer will receive the grab result data.
		Pylon::CGrabResultPtr grabResult;

		if (input->thisObj->continuousImaging)
		{
			int count = 0;
			while (true)
			{
				input->thisObj->camera->GrabOne( 5000, grabResult );
				// Image grabbed successfully?
				if (grabResult->GrabSucceeded())
				{
					count++;
					const uint16_t *pImageBuffer = (uint16_t *)grabResult->GetBuffer();
					// for continuous, frontload the anaylsis here. This is typically the limiting factor in this mode.
					int width = grabResult->GetWidth();
					int vertBinNumber = grabResult->GetHeight();
					std::vector<long> image( pImageBuffer, pImageBuffer + grabResult->GetWidth() * grabResult->GetHeight() );
					for (auto& elem : image)
					{
						elem *= 256.0 / 1024.0;
					}
					PostMessage( *input->parent, ACE_PIC_READY, grabResult->GetWidth() * grabResult->GetHeight(), (LPARAM)image.data() );
					//pic->drawBitmap( dc, image );
				}
				else
				{
					thrower( "Error: " + str( grabResult->GetErrorCode() ) + " " + std::string( grabResult->GetErrorDescription().c_str() ) );
				}
			}
		}
		else
		{
			input->thisObj->camera->StartGrabbing( input->thisObj->repCounts, Pylon::GrabStrategy_OneByOne, Pylon::GrabLoop_ProvidedByUser );
			int count = 0;
			while (input->thisObj->camera->IsGrabbing())
			{
				// Wait for an image and then retrieve it. A timeout of 5000 ms is used.
				input->thisObj->camera->RetrieveResult( 5000, grabResult, Pylon::TimeoutHandling_ThrowException );
				// Image grabbed successfully?
				if (grabResult->GrabSucceeded())
				{
					count++;
					const uint8_t *pImageBuffer = (uint8_t *)grabResult->GetBuffer();
					int width = grabResult->GetWidth();
					int vertBinNumber = grabResult->GetHeight();
					std::vector<long> image( pImageBuffer, pImageBuffer + width * vertBinNumber );
					for (auto& elem : image)
					{
						elem *= 256.0 / 1024.0;
					}
					PostMessage( *input->parent, ACE_PIC_READY, grabResult->GetWidth() * grabResult->GetHeight(), (LPARAM)image.data() );
					//pic->drawBitmap( dc, image );
				}
				else
				{
					thrower( "Error: " + str( grabResult->GetErrorCode() ) + " " + std::string( grabResult->GetErrorDescription().c_str() ) );
				}
			}
		}
	}
	catch (Pylon::RuntimeException& err)
	{
		delete inputPtr;
		thrower( "Error! " + std::string( err.what() ) );
	}
}


double BaslerCameras::getCurrentExposure()
{
	return camera->ExposureTime.GetValue();
}


void BaslerCameras::armCamera( HWND* parent )
//void BaslerCameras::armCamera(PictureControl* pic, CDC* dc)
{
	Pylon::EGrabStrategy grabStrat;
	if (continuousImaging)
	{
		grabStrat = Pylon::GrabStrategy_LatestImageOnly;
	}
	else
	{
		grabStrat = Pylon::GrabStrategy_OneByOne;
	}
	// grab stuff.
	camera->StartGrabbing( repCounts, grabStrat, Pylon::GrabLoop_ProvidedByInstantCamera );
	if (autoTrigger)
	{
		_beginthread( triggerThread, NULL, camera );
	}
}


void BaslerCameras::triggerThread( void* input )
{
	Pylon::CBaslerUsbInstantCamera* camera = (Pylon::CBaslerUsbInstantCamera*)input;
	try
	{
		while (camera->IsGrabbing())
		{
			// Execute the software trigger. The call waits up to 100 ms for the camera
			// to be ready to be triggered.
			camera->WaitForFrameTriggerReady( 500, Pylon::TimeoutHandling_ThrowException );
			Sleep( 10 );
			camera->ExecuteSoftwareTrigger();
			//camera.RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException );
		}
	}
	catch (Pylon::TimeoutException& timeoutErr)
	{
		errBox( "Timeout!" + std::string(timeoutErr.what()) );
	}
	catch (Pylon::RuntimeException& err)
	{
		// aborted.
	}
}


void BaslerCameras::disarm()
{
	camera->StopGrabbing();
}

// Adjust value so it complies with range and increment passed.
//
// The parameter's minimum and maximum are always considered as valid values.
// If the increment is larger than one, the returned value will be: min + (n * inc).
// If the value doesn't meet these criteria, it will be rounded down so that it does.
int64_t BaslerCameras::Adjust( int64_t val, int64_t minimum, int64_t maximum, int64_t inc )
{
	// Check the input parameters.
	if (inc <= 0)
	{
		// Negative increments are invalid.
		throw LOGICAL_ERROR_EXCEPTION( "Unexpected increment %d", inc );
	}
	if (minimum > maximum)
	{
		// Minimum must not be bigger than or equal to the maximum.
		throw LOGICAL_ERROR_EXCEPTION( "minimum bigger than maximum." );
	}

	// Check the lower bound.
	if (val < minimum)
	{
		return minimum;
	}

	// Check the upper bound.
	if (val > maximum)
	{
		return maximum;
	}

	// Check the increment.
	if (inc == 1)
	{
		// Special case: all values are valid.
		return val;
	}
	else
	{
		// The value must be min + (n * inc).
		// Due to the integer division, the value will be rounded down.
		return minimum + (((val - minimum) / inc) * inc);
	}
}
