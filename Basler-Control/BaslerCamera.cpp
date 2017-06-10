#include "stdafx.h"
#include "BaslerCamera.h"
#include "PictureControl.h"
#include <algorithm>
#include <functional>
#include "BaslerControlWindow.h"
#include "BaslerControlApp.h"
#include "stdint.h"
#include "constants.h"

void onImageGrabbedProcedure(Pylon::CInstantCamera& camera, const Pylon::CGrabResultPtr& grabResult, HWND* parent)
{
}

// important constructor;
// Create an instant camera object with the camera device found first.
BaslerCameras::BaslerCameras(HWND* parent)
{
	Pylon::PylonInitialize();
	Pylon::CDeviceInfo info;
	info.SetDeviceClass( cameraType::DeviceClass() );
	if (!BASLER_ACE_SAFEMODE)
	{
		cameraType* temp;
		temp = new BaslerWrapper( Pylon::CTlFactory::GetInstance().CreateFirstDevice( info ) );
		camera = dynamic_cast<BaslerWrapper*>(temp);
	}
	camera->init(parent);
	
}

// send a software trigger to the camera after waiting to make sure it's ready to recieve said trigger.
void BaslerCameras::softwareTrigger()
{
	camera->waitForFrameTriggerReady( 5000 );
	camera->executeSoftwareTrigger();
}

// important deconstructor.
BaslerCameras::~BaslerCameras()
{
	Pylon::PylonTerminate();
	delete camera;
}

// get some information about the camera from the camera itself through pylon.
std::string BaslerCameras::getCameraInfo()
{
	// Get camera device information.
	return "Camera Device Information\n=========================\nVendor           : " + std::string( camera->DeviceVendorName.GetValue() )
			+ "\nModel            : " + std::string( camera->DeviceModelName.GetValue() ) + "\nFirmware version : "
			+ std::string( camera->DeviceFirmwareVersion.GetValue() ) ;
}


// Can change this for nicer defaults.
baslerSettings BaslerCameras::getDefaultSettings()
{
	baslerSettings defaultSettings;

	defaultSettings.dimensions.leftBorder = camera->getMinOffsetX();
	defaultSettings.dimensions.horBinNumber = camera->getMaxWidth();
	defaultSettings.dimensions.rightBorder = defaultSettings.dimensions.leftBorder + defaultSettings.dimensions.horBinNumber;
	defaultSettings.dimensions.horPixelsPerBin = 1;
	defaultSettings.dimensions.topBorder = camera->getMinOffsetY();
	defaultSettings.dimensions.vertBinNumber = camera->getMaxHeight();
	defaultSettings.dimensions.bottomBorder = camera->getMinOffsetY() + camera->getMaxHeight();
	defaultSettings.dimensions.vertPixelsPerBin = 1;
	defaultSettings.exposureMode = "Auto Exposure Continuous";
	// doesn't matter for continuous.
	defaultSettings.exposureTime = 0; 
	defaultSettings.frameRate = 30;
	defaultSettings.rawGain = camera->getMinGain();

	//defaultSettings.triggerMode = ???
	return defaultSettings;
}

// set the default parameters defined in the function above.
void BaslerCameras::setDefaultParameters()
{
	setParameters( getDefaultSettings() );
}

// general function you should use for setting camera settings based on the input.
void BaslerCameras::setParameters( baslerSettings settings )
{
	// Set the AOI:

	/* 
		the basler API is very picky about the order of these things. Ways for this to crash:
		- Setting offset large (before making width smaller) pushes the right border past max width
		- setting width large (before making offset smaller) pushes right border past max width
	*/

	// if width is getting larger, set offset first. else set width first.
	if (settings.dimensions.horRawPixelNumber > runSettings.dimensions.horRawPixelNumber)
	{
		camera->setOffsetX( settings.dimensions.leftBorder );
		camera->setWidth( settings.dimensions.horRawPixelNumber );
	}
	else
	{
		camera->setWidth( settings.dimensions.horRawPixelNumber );
		camera->setOffsetX( settings.dimensions.leftBorder );
	}
	
	if (settings.dimensions.vertRawPixelNumber > runSettings.dimensions.vertRawPixelNumber)
	{
		camera->setOffsetY( settings.dimensions.topBorder );
		camera->setHeight( settings.dimensions.vertRawPixelNumber );
	}
	else
	{
		camera->setHeight( settings.dimensions.vertRawPixelNumber );
		camera->setOffsetY( settings.dimensions.topBorder );
	}

	camera->setHorBin( settings.dimensions.horPixelsPerBin );
	camera->setVertBin( settings.dimensions.vertPixelsPerBin );

	camera->setPixelFormat( cameraParams::PixelFormat_Mono10 );
	camera->setGainMode("Off");
	camera->setGain( camera->getMinGain() );

	// exposure mode
	if (settings.exposureMode == "Auto Exposure Continuous")
	{
		camera->setExposureAuto( cameraParams::ExposureAutoEnums::ExposureAuto_Continuous );
	}
	else if (settings.exposureMode == "Auto Exposure Off")
	{
		camera->setExposureAuto( cameraParams::ExposureAutoEnums::ExposureAuto_Off );

		if (!(settings.exposureTime >= camera->getExposureMin() && settings.exposureTime <= camera->getExposureMax()))
		{
			thrower( "ERROR: exposure time must be between " + str( camera->getExposureMin() ) + " and " 
					 + str( camera->getExposureMax()) );
		}
		camera->setExposure( settings.exposureTime );
	}
	else if (settings.exposureMode == "Auto Exposure Once")
	{
		camera->setExposureAuto( cameraParams::ExposureAutoEnums::ExposureAuto_Once );
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
		camera->setTriggerSource( cameraParams::TriggerSource_Line3 );
		autoTrigger = false;
	}
	else if (settings.triggerMode == "Automatic Software Trigger")
	{
		camera->setTriggerSource( cameraParams::TriggerSource_Software );
		autoTrigger = true;
	}
	else if (settings.triggerMode == "Manual Software Trigger")
	{
		camera->setTriggerSource( cameraParams::TriggerSource_Software );
		autoTrigger = false;
	}
	runSettings = settings;
}


// I can potentially use this to reopen the camera if e.g. the user disconnects. Don't think this is really implemented
// yet.
void BaslerCameras::reOpenCamera(HWND* parent)
{
	Pylon::CDeviceInfo info;
	info.SetDeviceClass( cameraType::DeviceClass() );
	if (!BASLER_ACE_SAFEMODE)
	{
		delete camera;
		cameraType* temp;
		temp = new cameraType( Pylon::CTlFactory::GetInstance().CreateFirstDevice( info ) );
		camera = dynamic_cast<BaslerWrapper*>(temp);
	}
	camera->init(parent);
}

// get the dimensions of the camera. This is tricky because while I can get info about each parameter easily through
// pylon, several of the parameters, such as the width, are context-sensitive and return the max values as possible 
// given other camera settings at the moment (e.g. the binning).
POINT BaslerCameras::getCameraDimensions()
{
	if (BASLER_ACE_SAFEMODE)
	{
		return { 672, 512 };
	}
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


// get the camera "armed" (ready for aquisition). Actual start of camera taking pictures depends on things like the 
// trigger mode.
void BaslerCameras::armCamera( double frameRate, HWND* parent )
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
	camera->startGrabbing( repCounts, grabStrat );
	triggerThreadInput* input = new triggerThreadInput;
	input->camera = camera;
	input->frameRate = frameRate;
	input->parent = parent;
	if (autoTrigger)
	{
		_beginthread( triggerThread, NULL, input );
	}
}


// 
double BaslerCameras::getCurrentExposure()
{
	return camera->getCurrentExposure();
}

//
unsigned int BaslerCameras::getRepCounts()
{
	return repCounts;
}

//
bool BaslerCameras::isContinuous()
{
	return continuousImaging;
}


// this is the thread that programatically software-triggers the camera when triggering internally.
void BaslerCameras::triggerThread( void* rawInput )
{
	triggerThreadInput* input = (triggerThreadInput*)rawInput;
	try
	{
		while (input->camera->isGrabbing())
		{
			// Execute the software trigger. The call waits up to 100 ms for the camera
			// to be ready to be triggered.
			input->camera->waitForFrameTriggerReady( 5000 );
			Sleep( int(1000.0 / input->frameRate ));
			input->camera->executeSoftwareTrigger();
			if (BASLER_ACE_SAFEMODE)
			{
				// simulate successful grab
				// need some way to communicate the width and height of the pic to this function...
				std::vector<long>* image;
				image = new std::vector<long>(672*512);
				for (auto& elem : *image)
				{
					// picture comes in as 10-Bit number.
					elem = rand() % 1024;
				}
				for (auto& elem : *image)
				{
					elem *= 256.0 / 1024.0;
				}
				PostMessage(*input->parent, ACE_PIC_READY, 672 * 512, (LPARAM)(image));
				//pic->drawBitmap( dc, image );
			}
		}
	}
	catch (Pylon::TimeoutException& timeoutErr)
	{
		errBox( "Trigger Thread Timeout Error!" + std::string(timeoutErr.what()) );
	}
	catch (Pylon::RuntimeException& err)
	{
		// aborted, that's fine
	}
}


// stop the camera from taking any pictures, even if triggered afterwards.
void BaslerCameras::disarm()
{
	camera->stopGrabbing();
}

/*
 * Adjust value so it complies with range and increment passed.
 *
 * The parameter's minimum and maximum are always considered as valid values.
 * If the increment is larger than one, the returned value will be: min + (n * inc).
 * If the value doesn't meet these criteria, it will be rounded down so that it does.

 * this function comes from basler example code. Not sure if I'm using it right now.
 */
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


// initialize the camera using the fundamental settings I use for all cameras. 
void BaslerWrapper::init( HWND* parent )
{
	if (!BASLER_ACE_SAFEMODE)
	{
		try
		{
			Open();
			// prepare the image event handler
			RegisterImageEventHandler( new ImageEventHandler( parent ), Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_Delete );
			TriggerMode.SetValue( cameraParams::TriggerMode_On );
		}
		catch (Pylon::GenericException& err)
		{
			thrower( err.what() );
		}

	}
}


/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
///					Pylon Wrappers
/// 
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * The rest of the functions in this file are simple wrappers for pylon functinos. They serve several purposes. 
 *		- they allow easy handling of errors.
		- More specifically, they allow me to standardize error handling. All my API wrappers handle errors by throwing
			an "Error" Object of my own making.
			- Pylon's error handling is actually already pretty close to my own error handling, in that it throws
				a specialized exception. However, I oftentimes here just make it return /my/ specialized exception 
				instead so that I can catch all errors simultaneously in my larger programs.
		- They allow easy bypass of the function call if the program is being run in safemode, oftentimes returning 
			dummy values that should allow the code to continue sensibly.
		- For the basler code specifically, they allow me to taylor the call to the situations when I'm using a 
			firewire camera or a usb camera without interupting the logical flow above. 
 */


int BaslerWrapper::getMinOffsetX()
{
	if (BASLER_ACE_SAFEMODE)
	{
		return 0;
	}
	try
	{
		return OffsetX.GetMin();
	}
	catch (Pylon::GenericException& err)
	{
		thrower( err.what() );
	}
}

int BaslerWrapper::getMinOffsetY()
{
	if (BASLER_ACE_SAFEMODE)
	{
		return 0;
	}
	try
	{
		return OffsetY.GetMin();
	}
	catch (Pylon::GenericException& err)
	{
		thrower( err.what() );
	}
}

int BaslerWrapper::getMaxWidth()
{
	if (BASLER_ACE_SAFEMODE)
	{
		return 672;
	}
	try
	{
		return OffsetX.GetMax();
	}
	catch (Pylon::GenericException& err)
	{
		thrower( err.what() );
	}
}

int BaslerWrapper::getMaxHeight()
{
	if (BASLER_ACE_SAFEMODE)
	{
		return 512;
	}
	try
	{
		return OffsetY.GetMin();
	}
	catch (Pylon::GenericException& err)
	{
		thrower( err.what() );
	}
}

int BaslerWrapper::getMinGain()
{
	if (BASLER_ACE_SAFEMODE)
	{
		return 260;
	}
	try
	{
		#ifdef USB_CAMERA
			return Gain.GetMin();
		#elif defined FIREWIRE_CAMERA
			return GainRaw.GetMin();
		#endif
	}
	catch (Pylon::GenericException& err)
	{
		thrower( err.what() );
	}
}


void BaslerWrapper::setOffsetX( int offset )
{
	if (!BASLER_ACE_SAFEMODE)
	{
		try
		{
			OffsetX.SetValue( offset );
		}
		catch (Pylon::GenericException& err)
		{
			thrower( err.what() );
		}

	}
}

void BaslerWrapper::setOffsetY( int offset )
{
	if (!BASLER_ACE_SAFEMODE)
	{
		OffsetY.SetValue( offset );
	}
}

void BaslerWrapper::setWidth( int width )
{
	if (!BASLER_ACE_SAFEMODE)
	{
		try
		{
			Width.SetValue( width );
		}
		catch (Pylon::GenericException& err)
		{
			thrower( err.what() );
		}
	}
}


void BaslerWrapper::setHeight( int height )
{
	if (!BASLER_ACE_SAFEMODE)
	{
		try
		{	
			Height.SetValue( height );
		}
		catch (Pylon::GenericException& err)
		{
			thrower( err.what() );
		}
	}
}


void BaslerWrapper::setHorBin( int binning )
{
	if (!BASLER_ACE_SAFEMODE)
	{
		try
		{
			BinningHorizontal.SetValue( binning );
		}
		catch (Pylon::GenericException& err)
		{
			thrower( err.what() );
		}
	}
}


void BaslerWrapper::setVertBin( int binning )
{
	if (!BASLER_ACE_SAFEMODE)
	{
		try
		{			
			BinningVertical.SetValue( binning );
		}
		catch (Pylon::GenericException& err)
		{
			thrower( err.what() );
		}
	}
}

void BaslerWrapper::stopGrabbing()
{
	if (!BASLER_ACE_SAFEMODE)
	{
		try
		{
			StopGrabbing();
		}
		catch (Pylon::GenericException& err)
		{
			thrower( err.what() );
		}
	}
}

bool BaslerWrapper::isGrabbing()
{
	if (BASLER_ACE_SAFEMODE)
	{
		return true;
	}
	try
	{
		return IsGrabbing();
	}
	catch (Pylon::GenericException& err)
	{
		thrower( err.what() );
	}
}

// not being used???
std::vector<long> BaslerWrapper::retrieveResult( unsigned int timeout )
{
	Pylon::CGrabResultPtr resultPtr;
	RetrieveResult( timeout, resultPtr, Pylon::TimeoutHandling_ThrowException );
	if (!resultPtr->GrabSucceeded())
	{
		thrower( "Error: " + str( resultPtr->GetErrorCode() ) + " " + std::string( resultPtr->GetErrorDescription().c_str() ) );
	}
	const uint16_t *pImageBuffer = (uint16_t *)resultPtr->GetBuffer();
	int width = resultPtr->GetWidth();
	int height = resultPtr->GetHeight();
	std::vector<long> image( pImageBuffer, pImageBuffer + width * height );
	return image;
}


int BaslerWrapper::getCurrentHeight()
{
	if (BASLER_ACE_SAFEMODE)
	{
		return 512;
	}
	try
	{
		return Height.GetValue();
	}
	catch (Pylon::GenericException& err)
	{
		thrower( err.what() );
	}
}


int BaslerWrapper::getCurrentWidth()
{
	if (BASLER_ACE_SAFEMODE)
	{
		return 672;
	}
	try
	{
		return Width.GetValue();
	}
	catch (Pylon::GenericException& err)
	{
		thrower( err.what() );
	}
}


int BaslerWrapper::getCurrentOffsetX()
{
	if (BASLER_ACE_SAFEMODE)
	{
		return 0;
	}
	try
	{
		return OffsetX.GetValue();
	}
	catch (Pylon::GenericException& err)
	{
		thrower( err.what() );
	}
}


int BaslerWrapper::getCurrentOffsetY()
{
	if (BASLER_ACE_SAFEMODE)
	{
		return 0;
	}
	try
	{
		return OffsetY.GetValue();
	}
	catch (Pylon::GenericException& err)
	{
		thrower( err.what() );
	}
}


void BaslerWrapper::setPixelFormat( cameraParams::PixelFormatEnums pixelFormat)
{
	if (!BASLER_ACE_SAFEMODE)
	{
		try
		{
			PixelFormat.SetValue( pixelFormat );
		}
		catch (Pylon::GenericException& err)
		{
			thrower( err.what() );
		}
	}
}

void BaslerWrapper::setGainMode( std::string mode )
{
	if (!BASLER_ACE_SAFEMODE)
	{
		try
		{
			GainAuto.FromString( mode.c_str() );
		}
		catch (Pylon::GenericException& err)
		{
			thrower( err.what() );
		}
	}
}

void BaslerWrapper::setGain( int gainValue )
{
	if (!BASLER_ACE_SAFEMODE)
	{
		try
		{
			#ifdef USB_CAMERA
				Gain.SetValue( gainValue );
			#elif defined FIREWIRE_CAMERA
				GainRaw.SetValue( gainValue );
			#endif
		}
		catch (Pylon::GenericException& err)
		{
			thrower( err.what() );
		}
	}
	
}


void BaslerWrapper::waitForFrameTriggerReady( unsigned int timeout )
{
	if (!BASLER_ACE_SAFEMODE)
	{
		try
		{
			WaitForFrameTriggerReady( timeout, Pylon::TimeoutHandling_ThrowException );
		}
		catch (Pylon::GenericException& err)
		{
			thrower( err.what() );
		}
	}
}



void BaslerWrapper::executeSoftwareTrigger()
{
	if (!BASLER_ACE_SAFEMODE)
	{
		try
		{
			ExecuteSoftwareTrigger();
		}
		catch (Pylon::GenericException& err)
		{
			thrower( err.what() );
		}
	}
}

void BaslerWrapper::setTriggerSource(Basler_UsbCameraParams::TriggerSourceEnums mode)
{
	if (!BASLER_ACE_SAFEMODE)
	{
		try
		{
			TriggerSource.SetValue(mode);
		}
		catch (Pylon::GenericException& err)
		{
			thrower(err.what());
		}
	}
}

void BaslerWrapper::startGrabbing( unsigned int picturesToGrab, Pylon::EGrabStrategy grabStrat )
{
	if (!BASLER_ACE_SAFEMODE)
	{
		try
		{
			StartGrabbing( picturesToGrab, grabStrat, Pylon::GrabLoop_ProvidedByInstantCamera );
		}
		catch (Pylon::GenericException& err)
		{
			thrower( err.what() );
		}
	}
}

// returns in us
double BaslerWrapper::getExposureMax()
{
	if (!BASLER_ACE_SAFEMODE)
	{
		try
		{
			#ifdef USB_CAMERA
				return ExposureTime.GetMax();
			#elif defined FIREWIRE_CAMERA
				return ExposureTimeRaw.GetMax();
			#endif
		}
		catch (Pylon::GenericException& err)
		{
			thrower( err.what() );
		}
	}
	return 1000000;
}

double BaslerWrapper::getExposureMin()
{
	if (!BASLER_ACE_SAFEMODE)
	{
		try
		{
			#ifdef USB_CAMERA
				return ExposureTime.GetMin();
			#elif defined FIREWIRE_CAMERA
				return ExposureTimeRaw.GetMin();
			#endif
		}
		catch (Pylon::GenericException& err)
		{
			thrower( err.what() );
		}
	}
	return 0;
}

double BaslerWrapper::getCurrentExposure()
{
	if (!BASLER_ACE_SAFEMODE)
	{
		try
		{
			#ifdef USB_CAMERA
				return ExposureTime.GetValue();
			#elif defined FIREWIRE_CAMERA
				return ExposureTimeRaw.GetValue();
			#endif
		}
		catch (Pylon::GenericException& err)
		{
			thrower( err.what() );
		}
	}
	return 10000;
}

void BaslerWrapper::setExposure( double exposureTime )
{
	if (!BASLER_ACE_SAFEMODE)
	{
		try
		{
			#ifdef USB_CAMERA
				ExposureTime.SetValue( exposureTime );
			#elif defined FIREWIRE_CAMERA
				ExposureTimeRaw.SetValue( exposureTime );
			#endif
		}
		catch (Pylon::GenericException& err)
		{
			thrower( err.what() );
		}
	}
}


void BaslerWrapper::setExposureAuto(cameraParams::ExposureAutoEnums mode)
{
	if (!BASLER_ACE_SAFEMODE)
	{
		try
		{
			ExposureTime.SetValue(mode);
		}
		catch (Pylon::GenericException& err)
		{
			thrower(err.what());
		}
	}
}