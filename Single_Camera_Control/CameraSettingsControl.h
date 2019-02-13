#pragma once

#include <string>
#include "Control.h"
#include "commonTypes.h"

class PixisCamera;

struct imageDimensions
{
	// in raw pixels, not binned pixels. These area meant to refer to the spatial extent of the camera being used.
	int leftBorder;
	int rightBorder;
	int topBorder;
	int bottomBorder;

	// extent of hardware binning
	int horPixelsPerBin;
	// total number of binned pixels
	int horBinNumber;
	// total number of pixels (not considering binning), i.e. rightBorder-leftBorder.
	int horRawPixelNumber;

	// etc
	int vertPixelsPerBin;
	int vertBinNumber;
	int vertRawPixelNumber;
};


struct CameraSettings
{
	double currentTemperatureVal;
	unsigned int rawGain;
	std::string exposureMode;
	double exposureTime;
	std::string cameraMode;
	unsigned int repCount;
	std::string triggerMode;
	double frameRate;
	imageDimensions dimensions;
};


class CameraSettingsControl
{
	public:
		void initialize(POINT& pos, int& id, CWnd* parent, int picWidth, int picHeight, POINT cameraDims);
		void handleGain();
		void setStatus(std::string status);
		void handleExposureMode();
		void handleCameraMode();
		void handleFrameRate();
		CameraSettings loadCurrentSettings(POINT cameraDims);
		CameraSettings getCurrentSettings();		
		// change all the settings.
		void setSettings( CameraSettings settings);
		void updateExposure( double exposure );
		void rearrange(int width, int height, fontMap fonts);
		void handleTimer ( PixisCamera* cam );
	private:
		ULONG lastTime;
		CameraSettings currentSettings;
		Control<CStatic> statusText;
		// exposure
		Control<CStatic> exposureText;
		Control<CComboBox> exposureModeCombo;
		Control<CEdit> exposureEdit;
		Control<CButton> setExposure;
		// trigger
		Control<CComboBox> triggerCombo;
		// Dimensions & Binning
		Control<CStatic> leftText;
		Control<CStatic>  rightText;
		Control<CStatic>  horizontalBinningText;
		Control<CStatic>  topText;
		Control<CStatic>  bottomText;
		Control<CStatic>  verticalBinningText;
		Control<CEdit>  leftEdit;
		Control<CEdit> rightEdit;
		Control<CEdit> horizontalBinningEdit;
		Control<CEdit> topEdit;
		Control<CEdit> bottomEdit;
		Control<CEdit> verticalBinningEdit;

		// camera mode: continuous or set #
		Control<CComboBox> cameraMode;
		Control<CStatic> repText;
		Control<CEdit> repEdit;

		Control<CStatic> frameRateText;
		Control<CEdit> frameRateEdit;
		Control<CStatic> realFrameRate;

		Control<CStatic> gainText;
		Control<CComboBox> gainCombo;
		Control<CEdit> gainEdit;
		Control<CStatic> realGainText;
		Control<CStatic> realGainStatus;
		// temperature
		Control<CButton> setTemperatureButton;
		Control<CButton> temperatureOffButton;
		Control<CEdit> temperatureEdit;
		Control<CStatic> temperatureDisplay;
		Control<CStatic> temperatureMsg;

		bool isReady;
};
