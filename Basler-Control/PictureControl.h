#pragma once
#include "Control.h"
#include "BaslerSettingsControl.h"
#include <array>
#include "Matrix.h"
#include "CameraImageDimensions.h"


class PictureControl
{
	public:
		PictureControl();
		void initialize(POINT& loc, CWnd* parent, int& id, int width, int height, CBrush* defaultGridBrush);
		void recalculateGrid( imageDimensions newParameters );
		void setPictureArea( POINT loc, int width, int height );
		void drawBitmap( CDC* deviceContext, const Matrix<long>& picData );
		void drawBackground( CDC* parentCdc );
		void drawGrid(CDC* parentCdc, CBrush* brush);
		void drawIntegratingCircles( CDC* parentCdc, double circleRadius );
		long integrateRegion(const Matrix<long>& picData, POINT selectedLocation, double circleSize);
		void rearrange( std::string cameraMode, std::string triggerMode, int width, int height, fontMap fonts );
		void handleScroll( int id, UINT nPos );
		void handleEditChange( int id );
		void updatePalette( HPALETTE pallete );
		void redrawImage( CDC* parentCdc );
		void setActive( bool activeState );
		bool isActive();
		void handleButtonClick();
		POINT checkClickLocation( CPoint clickLocation );
		void createPalettes( CDC* dc );
		void handleRightClick( CPoint clickLocation, CDC* parentCdc );
		void handleMouse( CPoint point );
		void setHoverValue();
		void drawDongles( CDC* parentCdc, const Matrix<long>& pic );
		void addIntegrationText( const Matrix<long>& pic, CDC* parentCdc );
		long getIntegrationSize();
	private:
		CBrush* gridBrush;
		CFont pictureTextFont;
		POINT mouseCoordinates;
		UINT maxWidth, maxHeight;
		// for replotting.
		Matrix<long> mostRecentImage;
		//std::vector<long> mostRecentImage;
		// stores info as to whether the control is currently being used in plotting camera data or was used 
		// in the most recent run.
		bool active = true;
		bool currentlySettingLocations;
		std::vector<POINT> analysisLocations;
		// Arguably I should make these static controls instead of keeping track explicitly of these things.
		RECT unscaledBackgroundArea;
		// scaled for the size of the window
		RECT scaledBackgroundArea;
		// scaled for the dimensions of the picture
		RECT pictureArea;

		// 
		int maxSliderPosition;
		int minSliderPosition;
		int colorIndicator;
		HPALETTE imagePalette;
		std::vector<POINT> locations;
		// grid data
		std::vector<std::vector<RECT>> grid;
		// Picture location data
		Control<CSliderCtrl> sliderMax;
		Control<CSliderCtrl> sliderMin;
		//
		Control<CStatic> labelMax;
		Control<CStatic> labelMin;
		//
		Control<CEdit> editMax;
		Control<CEdit> editMin;

		Control<CStatic> coordinatesText;
		Control<CStatic> coordinatesDisp;
		Control<CStatic> valueText;
		Control<CStatic> valueDisp;
		Control<CButton> setLocationsButton;
		Control<CStatic> circleSizeText;
		Control<CEdit> circleSizeEdit;
		std::array<HPALETTE, 3> palettes;

		// unofficial; these are just parameters this uses to keep track of grid size on redraws.
		imageDimensions unofficialImageParameters;
};
