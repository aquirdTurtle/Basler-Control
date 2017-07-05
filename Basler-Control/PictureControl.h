#pragma once
#include "Control.h"
#include "BaslerSettingsControl.h"
#include <array>

class PictureControl
{
	public:
		PictureControl::PictureControl();
		void initialize(POINT& loc, CWnd* parent, int& id, int width, int height);
		void updateGridSpecs( imageDimensions newParameters );
		void setPictureArea( POINT loc, int width, int height );
		void drawBitmap( CDC* deviceContext, std::vector<long> picData );
		void drawBackground(CWnd* parent);
		void drawGrid(CWnd* parent, CBrush* brush);
		void drawRectangles(CWnd* parent, CBrush* brush);
		void drawPixelCircle(CWnd* parent, std::pair<int, int> selectedLocation );
		void drawIntegratingCircle(CWnd* parent, std::pair<int, int> selectedLocation, double circleSize);
		void integrateRegion(std::vector<long> picData, std::pair<int, int> selectedLocation, double circleSize);
		void rearrange( std::string cameraMode, std::string triggerMode, int width, int height, fontMap fonts );
		void handleScroll( int id, UINT nPos );
		void handleEditChange( int id );
		void updatePalette( HPALETTE pallete );
		void redrawImage( CWnd* parent );
		void setActive( bool activeState );
		bool isActive();
		std::pair<int, int> checkClickLocation( CPoint clickLocation );
		void createPalettes( CDC* dc );
		void handleMouse( CPoint point );
		void setValue();

	private:
		POINT mouseCoordinates;
		// for replotting.
		std::vector<long> mostRecentImage;
		// stores info as to whether the control is currently being used in plotting camera data or was used 
		// in the most recent run.
		bool active = true;
		// Arguably I should make these static controls instead of keeping track explicitly of these things. 
		RECT originalBackgroundArea;
		RECT currentBackgroundArea;
		// 
		int maxSliderPosition;
		int minSliderPosition;
		int colorIndicator;
		HPALETTE imagePalette;
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

		std::array<HPALETTE, 3> palettes;
};
