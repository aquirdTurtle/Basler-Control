#include "stdafx.h"
#include "PixisCamera.h"
#include "Matrix.h"
#include "Single_Camera_Control_App.h"
#include <memory> 

PixisCamera::PixisCamera ( bool safemode ) : flume ( safemode )
{
	flume.InitializeLibrary ( );
}


PixisCamera::~PixisCamera ( )
{
	flume.CloseCamera ( );
	flume.UninitializeLibrary ( );
}


double PixisCamera::getExposure ( )
{
	return flume.getExposure ( );
}


POINT PixisCamera::getCameraDimensions ( )
{
	// hard coded right now, probably query-able though.
	return { 1024, 1024 };
}


UINT PixisCamera::getRepCounts ( )
{
	return repCounts;
}


void PixisCamera::armCamera ( )
{
	repCounts = 0;
	auto stride = flume.getReadoutStride ( );
	for ( auto i : range(1) )
	{
		auto data = flume.Aquire ( );
		Matrix<long>* dataM;
		std::vector<long> dataV ( (USHORT*) data.initial_readout, (USHORT*) data.initial_readout + stride / sizeof ( USHORT ) );
		dataM = new Matrix<long> ( 1024, 1024, dataV );
		parentWindow->PostMessageA ( PIC_READY, 1024*1024, reinterpret_cast<LPARAM> (dataM) );
	}
	// todo properly...
}

void PixisCamera::disarm ( )
{
	repCounts = 0;
	flume.StopAquisition ( );
}


void PixisCamera::setParameters ( CameraSettings settings )
{
	// todo
	try
	{
		flume.setExposure ( settings.exposureTime );
		//
		if ( settings.triggerMode == "External Trigger" )
		{
			flume.setStandardTrigger ( );
		}
		else
		{
			flume.turnOffTrigger ( );
		}
		// Handle Region of Interest
		auto region = flume.getRois ( );
		if ( region->roi_count == 1 )
		{
			region->roi_array[ 0 ].height = settings.dimensions.vertRawPixelNumber;
			region->roi_array[ 0 ].width = settings.dimensions.horRawPixelNumber;

			region->roi_array[ 0 ].x = settings.dimensions.leftBorder;
			region->roi_array[ 0 ].y = settings.dimensions.topBorder;

			region->roi_array[ 0 ].x_binning = settings.dimensions.horPixelsPerBin;
			region->roi_array[ 0 ].y_binning = settings.dimensions.vertPixelsPerBin;
		}
		flume.setRois ( region );		
		flume.commitParams ( );
	}
	catch ( Error& err )
	{
		errBox ( err.what ( ) );
	}
}


CameraSettings PixisCamera::getDefaultSettings ( )
{
	// todo
	return CameraSettings ( );
}


void PixisCamera::initialize ( CWnd* parent )
{
	parentWindow = parent;
	try
	{
		flume.OpenFirstCamera ( );
		camID = flume.GetCameraID ( );
	}
	catch ( Error& err )
	{
		errBox ( "No Pixis Camera Detected (" + err.whatStr() + "), opening demo camera!" );
		flume.ConnectAndOpenDemoCamera ( camID );
	}
}

