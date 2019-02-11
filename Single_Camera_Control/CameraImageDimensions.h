#pragma once

#include "Control.h"

class BaslerCameraWindow;

struct cameraPositions;

struct imageParameters
{
	int leftBorder;
	int rightBorder;
	int topBorder;
	int bottomBorder;
	int horPixelsPerBin;
	int vertPixelsPerBin;
	int horBinNumber;
	int vertBinNumber;
};

class CameraImageDimensionsControl
{
	public:
		CameraImageDimensionsControl();
		//void initialize( cameraPositions& pos, CWnd* parentWindow, bool isTriggerModeSensitive, int& id );
		imageParameters readImageParameters( BaslerCameraWindow* camWin );
		void setImageParametersFromInput( imageParameters param, BaslerCameraWindow* camWin );
		bool checkReady();
		imageParameters getImageParameters();
		void rearrange( std::string cameraMode, std::string triggerMode, int width,
						int vertBinNumber, std::unordered_map<std::string, CFont*> fonts );
		//HBRUSH colorEdits( HWND window, UINT message, WPARAM wParam, LPARAM lParam, BaslerCameraWindow* mainWin );
		//void drawBackgrounds( BaslerCameraWindow* camWin );
		void cameraIsOn( bool state );
	// TODO:
	// bool drawPicture();
	// bool drawSelectionCircle();
	// bool drawAnalysisSquares();
	private:
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
		Control<CButton> setImageDimensionsButton;
		bool isReady;
		imageParameters currentImageParameters;
};
