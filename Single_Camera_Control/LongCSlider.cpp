// created by Mark Brown
#include "stdafx.h"
#include "LongCSlider.h"
#include "boost/lexical_cast.hpp"

void LongCSlider::initialize ( POINT& loc, CWnd* parent, int& id, int width, int height, int editID, std::string headerText )
{
	header.sPos = { loc.x, loc.y, loc.x + 25, loc.y += 20 };
	header.Create ( headerText.c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER, header.sPos, parent, id++ );
	header.fontType = fontTypes::Small;

	edit.sPos = { loc.x, loc.y, loc.x + 25, loc.y += 20 };
	edit.Create ( WS_CHILD | WS_VISIBLE | SS_LEFT | ES_AUTOHSCROLL, edit.sPos, parent, editID );
	edit.fontType = fontTypes::Small;

	if ( height < 40 )
	{
		thrower ( "ERROR: Must initialize LongCSLider with a height greater than 40 to have room for edit and header controls!" );
	}
	slider.sPos = { loc.x, loc.y, loc.x + 25, loc.y += (height-40) };
	slider.Create ( WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_VERT, slider.sPos, parent, id++ );
	slider.SetRange ( 0, 1024 );
	slider.SetPageSize ( 0 );
}

int LongCSlider::getSliderId ( )
{
	return slider.GetDlgCtrlID ( );
}

void LongCSlider::rearrange ( std::string cameraMode, std::string triggerMode, int width, int height, fontMap fonts )
{
	edit.rearrange ( cameraMode, triggerMode, width, height, fonts );
	slider.rearrange ( cameraMode, triggerMode, width, height, fonts );
	header.rearrange ( cameraMode, triggerMode, width, height, fonts );
}

double LongCSlider::getValue ( )
{
	return currentValue;
}

void LongCSlider::setValue ( double value, bool updateEdit )
{
	if ( value < minVal )
	{
		thrower ( "Tried to set slider value below minimum value of " + str ( minVal ) + "!" );
	}
	else if ( value > maxVal )
	{
		thrower ( "Tried to set slider value above maximum value of " + str ( maxVal ) + "!" );
	}
	currentValue = value; 
	if ( updateEdit )
	{
		edit.SetWindowText ( cstr ( value ) );
	}
	double p = ( value - minVal ) / ( maxVal - minVal );
	int setP = int(p * 1024);
	slider.SetPos ( setP );
	
}

void LongCSlider::handleEdit ()
{
	int val;
	CString txt;
	edit.GetWindowTextA ( txt );
	try
	{
		val = int(boost::lexical_cast<double>( std::string ( txt ) ));
	}
	catch ( boost::bad_lexical_cast& )
	{
		thrower( "Please enter a number." );
	}
	if ( val < minVal || val > maxVal )
	{
		thrower( "Please enter a number within the slider's range ("+str(minVal) + "," + str(maxVal) + ")" );
	}
	if ( val != currentValue )
	{
		setValue ( val, false );
	}
}

void LongCSlider::handleSlider ( UINT nPos )
{
	double p = nPos / 1024.0;
	int value = int(p * ( maxVal - minVal ) + minVal);
	setValue ( value );
}

