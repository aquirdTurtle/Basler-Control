#pragma once
#include <array>
#include "Control.h"
#include "Matrix.h"

// contains experimentally calibrated conversion factors. See the onenote section on the camera
// for more conversion factors and notes on the calibration.
/*
const double countToCameraPhoton = 0.697798;
const double countToScatteredPhoton = 0.697798 / 0.07;
// for em gain of 200 only! Could add more arbitrary approximation, but I specifically calibrated this value (I
// also calibrated X50, just didn't include that here.
const double countToCameraPhotonEM200 = 0.018577;
const double countToScatteredPhotonEM200 = 0.018577 / 0.07;
const double conventionalBackgroundCount = 88;
const double EMGain200BackgroundCount = 105;
*/
struct conversions
{
	const double countToCameraPhoton = 0.697798;
	const double countToScatteredPhoton = 0.697798 / 0.07;
	// for em gain of 200 only! Could add more arbitrary approximation, but I specifically calibrated this value (I 
	// also calibrated X50, just didn't include that here.
	const double countToCameraPhotonEM200 = 0.018577;
	const double countToScatteredPhotonEM200 = 0.018577 / 0.07;
	const double conventionalBackgroundCount = 88;
	const double EMGain200BackgroundCount = 105;
};

class PictureStats
{
	public:
		PictureStats::PictureStats()
		{
			//this->displayDataType = RAW_COUNTS;
		}
		bool initialize( POINT& pos, CWnd* parent, int& id, std::unordered_map<std::string, CFont*> fonts,
						 std::vector<CToolTipCtrl*>& tooltips );
		bool rearrange( std::string cameraMode, std::string trigMode, int width, int height,
						std::unordered_map<std::string, CFont*> fonts );
		void update( Matrix<long> image, unsigned int imageNumber, POINT selectedPixel, int pictureWidth, 
					 int currentRepetitionNumbar, int totalRepetitionCount );
		bool reset();
		bool updateType( std::string typeText );

	private:
		std::string displayDataType;
		conversions convs;
		
		Control<CStatic> pictureStatsHeader;
		Control<CStatic> repetitionIndicator;
		Control<CButton> updateButton;

		std::array<Control<CStatic>, 5> collumnHeaders;
		std::array<Control<CStatic>, 1> picNumberIndicators;
		std::array<Control<CStatic>, 1> maxCounts;
		std::array<Control<CStatic>, 1> minCounts;
		// average counts in the picture.
		std::array<Control<CStatic>, 1> avgCounts;
		std::array<Control<CStatic>, 1> selCounts;
		
};



