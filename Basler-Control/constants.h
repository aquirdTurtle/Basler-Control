#pragma once

#include <pylon/PylonIncludes.h>
#include <pylon/usb/BaslerUsbInstantCamera.h>
#include <pylon/1394/Basler1394InstantCamera.h>

#define BASLER_SAFEMODE false
// which type of camera
// #define FIREWIRE_CAMERA
#define USB_CAMERA

// The code compiles fairly differently for Firewire (1384) cameras vs. USB cameras.
#ifdef FIREWIRE_CAMERA 
	typedef Pylon::CBasler1394InstantCamera cameraType;
	typedef Pylon::CBasler1394GrabResultPtr grabPtr;
	namespace cameraParams = Basler_IIDC1394CameraParams;
	const std::string mainColor = "Dark Orange";
#elif defined USB_CAMERA
	typedef Pylon::CBaslerUsbInstantCamera cameraType;
	typedef Pylon::CBaslerUsbGrabResultPtr grabPtr;
	namespace cameraParams = Basler_UsbCameraParams;
	const std::string mainColor = "Dark Indigo";
#endif

const std::string DATA_SAVE_LOCATION = "J:\\Data Repository\\New Data Repository\\";

#ifdef FIREWIRE_CAMERA
	const std::string DATA_SAVE_LOCATION2 = "\\Raw Data\\ScoutData";
#elif defined USB_CAMERA
	const std::string DATA_SAVE_LOCATION2 = "\\Raw Data\\AceData";
#endif

// constants for various controls that I need to reference in the basler window message map.
#define IDC_MIN_SLIDER_EDIT 1047
#define IDC_MAX_SLIDER_EDIT 1050
#define IDC_CAMERA_MODE_COMBO 1003
#define IDC_EXPOSURE_MODE_COMBO 1006
#define IDC_REPETITIONS_EDIT 1002
#define IDC_TRIGGER_MODE_COMBO 1019
// the basler has a 10 bit dac, but the data is compressed to 256 for visualization (visualization only!)
#define PICTURE_PALETTE_SIZE 256


