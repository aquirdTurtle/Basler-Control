#pragma once

#include <pylon/PylonIncludes.h>
#include <pylon/usb/BaslerUsbInstantCamera.h>
#include <pylon/1394/Basler1394InstantCamera.h>

#define BASLER_ACE_SAFEMODE true
// which type of camera
//#define FIREWIRE_CAMERA 
#define USB_CAMERA 

// The code compiles fairly differently for Firewire (1384) cameras vs. USB cameras.
#ifdef FIREWIRE_CAMERA 
	typedef Pylon::CBasler1394InstantCamera cameraType;
	typedef Pylon::CBasler1394GrabResultPtr grabPtr;
	namespace cameraParams = Basler_IIDC1394CameraParams;
#elif defined USB_CAMERA
	typedef Pylon::CBaslerUsbInstantCamera cameraType;
	typedef Pylon::CBaslerUsbGrabResultPtr grabPtr;
	namespace cameraParams = Basler_UsbCameraParams;
#endif

const std::string DATA_SAVE_LOCATION = "C:\\Users\\Mark\\Documents\\Basler-Control\\Data\\";

// constants for various controls that I need to reference in the basler window message map.
#define IDC_BASLER_MIN_SLIDER_MIN_EDIT 1049
#define IDC_BASLER_MIN_SLIDER_MAX_EDIT 1052
#define IDC_CAMERA_MODE_COMBO 1004
#define IDC_EXPOSURE_MODE_COMBO 1007
#define IDC_REPETITIONS_EDIT 1003
#define IDC_ARM_BASLER_BUTTON 1000
#define IDC_DISARM_BASLER_BUTTON 1001
#define IDC_TRIGGER_MODE_COMBO 1021
// the basler has a 10 bit dac, but the data is compressed to 256 for visualization (visualization only!)
#define PICTURE_PALETTE_SIZE 256


