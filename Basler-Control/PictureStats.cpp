#include "stdafx.h"
#include "PictureStats.h"
#include "Matrix.h"
#include <algorithm>
#include <numeric>

// as of right now, the position of this control is not affected by the mode or the trigger mode.
bool PictureStats::initialize( POINT& pos, CWnd* parent, int& id, std::unordered_map<std::string, CFont*> fonts,
							   std::vector<CToolTipCtrl*>& tooltips )
{
	int width = 300;
	pictureStatsHeader.sPos = { pos.x, pos.y, pos.x + width, pos.y + 25 };
	pictureStatsHeader.Create( "Raw Counts", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER,
							   pictureStatsHeader.sPos, parent, id++ );
	pictureStatsHeader.fontType = Heading;
	pos.y += 25;
	/// CURRENT IMAGE DATA
	// Current Accumulation Number Display
	repetitionIndicator.sPos = { pos.x, pos.y, pos.x + width, pos.y += 25 };
	repetitionIndicator.Create( "Repetition ?/?", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER, 
								repetitionIndicator.sPos,
								parent, id++ );

	updateButton.sPos = { pos.x, pos.y, pos.x + width, pos.y += 25 };
	updateButton.Create( "Update Statistics?", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_BORDER, updateButton.sPos, 
						 parent, id++ );
	/// Picture labels ///

	// 
	collumnHeaders[0].sPos = { pos.x, pos.y, pos.x + width/5, pos.y + 25 };
	collumnHeaders[0].Create( "Pic:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, collumnHeaders[0].sPos, parent, 
							  id++ );
	collumnHeaders[0].fontType = Small;
	pos.y += 25;
	int inc = 0;
	for (auto& control : picNumberIndicators)
	{
		inc++;
		control.sPos = { pos.x, pos.y, pos.x + width/5, pos.y + 25 };
		control.Create( ("#" + str( inc ) + ":").c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, control.sPos, parent, id++ );
		control.fontType = Small;
		pos.y += 25;
	}
	pos.y -= 25 * (picNumberIndicators.size() + 1);

	/// Max Count Edits
	// Max Count Display 742 - 480 )/2 = 131 
	collumnHeaders[1].sPos = { pos.x + width/5, pos.y, pos.x + 2* width/5, pos.y + 25 };
	collumnHeaders[1].Create( "Max:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, collumnHeaders[1].sPos,
							  parent, id++ );
	collumnHeaders[1].fontType = Small;
	pos.y += 25;
	// #1
	for (auto& control : maxCounts)
	{
		control.sPos = { pos.x + width/5, pos.y, pos.x + 2* width/5, pos.y + 25 };
		control.Create( "-", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, control.sPos, parent, id++ );
		control.fontType = Small;
		pos.y += 25;
	}
	// back to top.
	pos.y -= 25 * (maxCounts.size() + 1);
	/// Min Counts
	// Min Count Display	
	collumnHeaders[2].sPos = { pos.x + long(0.4 * width), pos.y, pos.x + long(0.6 * width), pos.y + 25 };
	collumnHeaders[2].Create( "Min:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, collumnHeaders[2].sPos,
							  parent, id++ );
	collumnHeaders[2].fontType = Small;
	pos.y += 25;

	for (auto& control : minCounts)
	{
		control.sPos = { pos.x + long(0.4 * width), pos.y, pos.x + long(0.6 * width), pos.y + 25 };
		control.Create( "-", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, control.sPos, parent, id++ );
		control.fontType = Small;
		pos.y += 25;
	}
	pos.y -= 25 * (minCounts.size() + 1);
	/// Average Counts
	collumnHeaders[3].sPos = { pos.x + long(0.6 * width), pos.y, pos.x + long(0.8 * width), pos.y + 25 };
	collumnHeaders[3].Create( "Avg:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, collumnHeaders[3].sPos,
							  parent, id++ );
	collumnHeaders[3].fontType = Small;
	pos.y += 25;
	// 
	for (auto& control : avgCounts)
	{
		control.sPos = { pos.x + long(0.6 * width), pos.y, pos.x + long(0.8 * width), pos.y + 25 };
		control.Create( "-", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, control.sPos, parent, id++ );
		control.fontType = Small;
		pos.y += 25;
	}

	pos.y -= 25 * (avgCounts.size()+1);
	/// Selection Counts
	collumnHeaders[4].sPos = { pos.x + long(0.8 * width), pos.y, pos.x + width, pos.y + 25 };
	collumnHeaders[4].Create( "Sel:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, collumnHeaders[4].sPos,
							  parent, id++ );
	collumnHeaders[4].fontType = Small;
	pos.y += 25;
	// #1
	for (auto& control : selCounts)
	{
		control.sPos = { pos.x + long(0.8*width), pos.y, pos.x + width, pos.y + 25 };
		control.Create( "-", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, control.sPos, parent, id++ );
		control.fontType = Small;
		pos.y += 25;
	}
	return true;
}

bool PictureStats::rearrange(std::string cameraMode, std::string trigMode, int width, int height, std::unordered_map<std::string, CFont*> fonts)
{
	pictureStatsHeader.rearrange(cameraMode, trigMode, width, height, fonts);
	repetitionIndicator.rearrange(cameraMode, trigMode, width, height, fonts);
	for (auto& control : collumnHeaders)
	{
		control.rearrange(cameraMode, trigMode, width, height, fonts);
	}
	for (auto& control : maxCounts)
	{
		control.rearrange(cameraMode, trigMode, width, height, fonts);
	}
	for (auto& control : minCounts)
	{
		control.rearrange(cameraMode, trigMode, width, height, fonts);
	}
	for (auto& control : picNumberIndicators)
	{
		control.rearrange(cameraMode, trigMode, width, height, fonts);
	}
	for (auto& control : selCounts)
	{
		control.rearrange(cameraMode, trigMode, width, height, fonts);
	}
	for (auto& control : avgCounts)
	{
		control.rearrange(cameraMode, trigMode, width, height, fonts);
	}
	updateButton.rearrange(cameraMode, trigMode, width, height, fonts);
	return true;
}


bool PictureStats::reset()
{
	for (auto& control : maxCounts)
	{
		control.SetWindowText("-");
	}
	for (auto& control : minCounts)
	{
		control.SetWindowText("-");
	}
	for (auto& control : avgCounts)
	{
		control.SetWindowText("-");
	}
	for (auto& control : selCounts)
	{
		control.SetWindowText("-");
	}
	repetitionIndicator.SetWindowTextA( "Repetition ---/---" );
	return true;
}


bool PictureStats::updateType(std::string typeText)
{
	pictureStatsHeader.SetWindowText(typeText.c_str());
	return true;
}

 
void PictureStats::update( Matrix<long> image, unsigned int imageNumber, POINT selectedPixel, int pictureWidth, 
						   int currentRepetitionNumber, int totalRepetitionCount)
{
	// always update the picture number, it's very low cost compared to the other stuff.
	repetitionIndicator.SetWindowTextA(("Repetition " + str(currentRepetitionNumber) + "/"
									   + str(totalRepetitionCount)).c_str());
	if (!updateButton.GetCheck())
	{
		return;
	}
	long selCounts = image(selectedPixel);
	//long selCounts = image[selectedPixel.first + selectedPixel.second * pictureWidth];
	long maxCounts = 1;
	long minCounts = 65536;
	double avgCounts;
	// for all pixels... find the max and min of the picture.
	for ( const auto& pixel : image )
	{
		if ( pixel > maxCounts )
		{
			maxCounts = pixel;
		}
		if ( pixel < minCounts )
		{
			minCounts = pixel;
		}
	}
	avgCounts = std::accumulate( image.begin(), image.end(), 0.0 ) / image.size();

	this->maxCounts[imageNumber].SetWindowTextA( cstr( maxCounts ) );
	this->minCounts[imageNumber].SetWindowTextA( cstr( minCounts ) );
	//this->selCounts[imageNumber].SetWindowTextA( str( selCounts ).c_str() );
	this->avgCounts[imageNumber].SetWindowTextA( cstr( avgCounts, 1 ) );
}