#pragma once

#include "PictureControl.h"
#include <pylon/PylonIncludes.h>
#include <pylon/PylonGUI.h>
#include <pylon/usb/BaslerUsbInstantCamera.h>
#include "BaslerControlApp.h"

class BaslerCameras;

struct cameraWatchThreadInput
{
	HWND* parent;
	BaslerCameras* thisObj;
};

// the object for an actual camera.  doesn't handle gui things itself, just the interface from my code to the camera object.
class BaslerCameras
{
	public:
		// important constructor to initialize camera
		BaslerCameras( HWND* parent );
		~BaslerCameras();
		void setParameters( baslerSettings settings );
		void setDefaultParameters();
		void armCamera( HWND* parent );
		void disarm();
		static void triggerThread(void* input);
		void softwareTrigger();
		//void armCamera( PictureControl* pic, CDC* dc );
		POINT getCameraDimensions();
		void reOpenCamera();
		std::string getCameraInfo();
		baslerSettings getDefaultSettings();
		double getCurrentExposure();
		// Adjust value so it complies with range and increment passed.
		//
		// The parameter's minimum and maximum are always considered as valid values.
		// If the increment is larger than one, the returned value will be: min + (n * inc).
		// If the value doesn't meet these criteria, it will be rounded down so that it does.
		int64_t Adjust( int64_t val, int64_t minimum, int64_t maximum, int64_t inc );
	private:
		Pylon::CBaslerUsbInstantCamera* camera;
		bool continuousImaging;
		bool autoTrigger;
		unsigned int repCounts;
		static void cameraWatchThread( void* inputPtr );
};


class ImageEventHandler : public Pylon::CImageEventHandler
{
	public:
		ImageEventHandler(HWND* parentHandle) : Pylon::CImageEventHandler()
		{
			parent = parentHandle;
		}

		virtual void OnImageGrabbed( Pylon::CInstantCamera& camera, const Pylon::CGrabResultPtr& grabResult )
		{
			try
			{
				// Image grabbed successfully?
				if (grabResult->GrabSucceeded())
				{
					const uint16_t *pImageBuffer = (uint16_t *)grabResult->GetBuffer();
					int width = grabResult->GetWidth();
					int vertBinNumber = grabResult->GetHeight();
					std::vector<long>* image;
					image = new std::vector<long>( pImageBuffer, pImageBuffer + width * vertBinNumber );
					for (auto& elem : *image)
					{
						elem *= 256.0 / 1024.0;
					}
					str( PostMessage( *parent, ACE_PIC_READY, grabResult->GetWidth() * grabResult->GetHeight(),
						(LPARAM)image ) );
					//pic->drawBitmap( dc, image );
				}
				else
				{
					thrower( "Error: " + str( grabResult->GetErrorCode() ) + " " + std::string( grabResult->GetErrorDescription().c_str() ) );
				}
			}
			catch (Pylon::RuntimeException& err)
			{
				thrower( "Error! " + std::string( err.what() ) );
			}
		}
	private:
		HWND* parent;
};

