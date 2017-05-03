#pragma once

#include <string>
#include "Control.h"

struct imageDimensions
{
	int leftBorder;
	int rightBorder;
	int topBorder;
	int bottomBorder;
	
	int horPixelsPerBin;
	int horBinNumber;
	int horRawPixelNumber;

	int vertPixelsPerBin;
	int vertBinNumber;
	int vertRawPixelNumber;
};


struct baslerSettings
{
	unsigned int rawGain;
	std::string exposureMode;
	double exposureTime;
	std::string cameraMode;
	unsigned int repCount;
	std::string triggerMode;
	double frameRate;
	imageDimensions dimensions;
};


class BaslerSettingsControl
{
	public:
		void initialize( POINT& pos, int& id, CWnd* parent, int width, int height );
		void handleGain();
		void handleExposureMode();
		void handleCameraMode();
		baslerSettings getCurrentSettings();
		// change all the settings.
		void setSettings( baslerSettings settings);
		void updateExposure( double exposure );

	private:
		baslerSettings currentSettings;
		// arm
		Control<CButton> armCameraButton;
		Control<CButton> disarmCameraButton;
		// exposure
		Control<CStatic> exposureText;
		Control<CComboBox> exposureModeCombo;
		Control<CEdit> exposureEdit;
		Control<CButton> setExposure;
		// trigger
		Control<CStatic> triggerText;
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

		bool isReady;
};