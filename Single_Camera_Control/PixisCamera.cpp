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
	auto constraint = flume.getCameraRoiConstraints ( );
	//PicamRoisConstraint
	// hard coded right now, probably query-able though.
	return { (long)constraint->width_constraint.maximum, (long)constraint->height_constraint.maximum };
}


UINT PixisCamera::getRepCounts ( )
{
	return repCounts;
}


void PixisCamera::armCamera ( )
{
	repCounts = 0;
	auto stride = flume.getReadoutStride ( );
	auto rois = flume.getRois ( );	
	for ( auto i : range(1) )
	{
		auto data = flume.Aquire ( );
		Matrix<long>* dataM;
		std::vector<long> dataV ( (USHORT*) data.initial_readout, (USHORT*) data.initial_readout + stride / sizeof ( USHORT ) );
		dataM = new Matrix<long> ( int(rois[0].height / rois[ 0 ].y_binning), int(rois[ 0 ].width / rois[0].x_binning), dataV );
		parentWindow->PostMessageA ( PIC_READY, dataM->size(), reinterpret_cast<LPARAM> (dataM) );
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
		region[ 0 ].height = settings.dimensions.vertRawPixelNumber;
		region[ 0 ].width = settings.dimensions.horRawPixelNumber;

		region[ 0 ].x = settings.dimensions.leftBorder;
		region[ 0 ].y = settings.dimensions.topBorder;

		region[ 0 ].x_binning = settings.dimensions.horPixelsPerBin;
		region[ 0 ].y_binning = settings.dimensions.vertPixelsPerBin;
		flume.setRois ( region );		
		flume.commitParams ( );
	}
	catch ( Error& err )
	{
		errBox ( err.what ( ) );
	}
}

void PixisCamera::setTemperatureSetPoint ( double temperature )
{
	flume.setTemperatureSetPoint ( temperature );
}

double PixisCamera::getCurrentTemperature ( )
{
	return flume.getCurrentTemperature ( );
}

std::string PixisCamera::getTemperatureStatus ( )
{
	return flume.getTemperatureStatus ( );
}

double PixisCamera::getSetTemperature ( )
{
	return flume.getTemperatureSetPoint ( );
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

