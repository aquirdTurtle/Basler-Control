#include "stdafx.h"

#include "PicamFlume.h"

PicamFlume::PicamFlume ( bool safemode_option )
{
	safemode = safemode_option;
}

void PicamFlume::CloseCamera ( )
{
	Picam_CloseCamera ( camera );
}

void PicamFlume::setTemperatureSetPoint ( double temp )
{
	SetParameterFloatingPointValue ( PicamParameter_SensorTemperatureSetPoint, temp );
}


double PicamFlume::getTemperatureSetPoint ( )
{
	return GetParameterFloatingPointValue ( PicamParameter_SensorTemperatureSetPoint );
}


void PicamFlume::UninitializeLibrary ( )
{
	if ( !safemode )
	{
		auto err = Picam_UninitializeLibrary ( );
		if ( err != PicamError_None )
		{
			thrower ( getErrMsg ( err ) );
		}
	}
}

PicamHandle PicamFlume::getCamera ( )
{
	return camera;
}


void PicamFlume::InitializeLibrary ( )
{
	if ( !safemode )
	{
		auto err = Picam_InitializeLibrary ( );
		if ( err != PicamError_None )
		{
			thrower ( getErrMsg( err ) );
		}
	}
}

PicamAvailableData PicamFlume::Aquire ( )
{
	PicamAvailableData data;
	PicamAcquisitionErrorsMask errors;
	if ( !safemode )
	{		
		auto err = Picam_Acquire ( camera, 1, -1, &data, &errors );
		if ( err != PicamError_None )
		{
			thrower ( getErrMsg ( err ) );
		}
	}
	return data;
}

void PicamFlume::OpenFirstCamera ( )
{
	if ( !safemode )
	{
		auto err = Picam_OpenFirstCamera(&camera);
		if ( err != PicamError_None )
		{
			thrower ( getErrMsg ( err ) );
		}
	}
}

PicamCameraID PicamFlume::GetCameraID ( )
{
	PicamCameraID id = PicamCameraID();
	if ( !safemode )
	{
		auto err = Picam_GetCameraID ( camera, &id );
		if ( err != PicamError_None )
		{
			thrower ( getErrMsg ( err ) );
		}
	}
	return id;
}


double PicamFlume::getCurrentTemperature ( )
{
	return GetParameterFloatingPointValue ( PicamParameter_SensorTemperatureReading );
}


std::string PicamFlume::getTemperatureStatus ( )
{
	PicamSensorTemperatureStatus status;
	if ( !safemode )
	{
		auto err = Picam_ReadParameterIntegerValue ( camera, PicamParameter_SensorTemperatureStatus,
													 reinterpret_cast<piint*>( &status ) );		
		if ( err != PicamError_None )
		{
			thrower ( getErrMsg ( err ) );
		}
		const pichar* string;
		Picam_GetEnumerationString ( PicamEnumeratedType_SensorTemperatureStatus, status, &string );
		std::string msg ( string );
		Picam_DestroyString ( string );
		return msg;
	}
	return "";
}


void PicamFlume::StopAquisition ( )
{
	if ( !safemode )
	{
		auto err = Picam_StopAcquisition ( camera );
		if ( err != PicamError_None )
		{
			thrower ( getErrMsg ( err ) );
		}
	}
	
}




void PicamFlume::ConnectAndOpenDemoCamera ( PicamCameraID& id )
{
	if ( !safemode )
	{
		auto err = Picam_ConnectDemoCamera ( PicamModel_Pixis100F, "0008675309", &id );
		if ( err != PicamError_None )
		{
			thrower ( getErrMsg ( err ) );
		}
		auto err2 = Picam_OpenCamera ( &id, &camera );
		if ( err2 != PicamError_None )
		{
			thrower ( getErrMsg ( err2 ) );
		}
	}
}


int PicamFlume::getReadoutStride ( )
{
	return GetParameterIntegerValue ( PicamParameter_ReadoutStride );
}


int PicamFlume::GetParameterIntegerValue ( PicamParameter param )
{
	int val;
	if ( !safemode )
	{
		auto err = Picam_GetParameterIntegerValue ( camera, param, &val );
		if ( err != PicamError_None )
		{
			thrower ( getErrMsg ( err ) );
		}
	}
	return val;
}


std::vector<PicamRoi> PicamFlume::getRois ( )
{
	std::vector<PicamRoi> roisV;// ( rois, rois + rois->roi_count );
	if ( !safemode )
	{
		const PicamRois* regions;
		PicamError err = Picam_GetParameterRoisValue ( camera, PicamParameter_Rois, &regions );
		if ( err != PicamError_None )
		{
			thrower ( getErrMsg ( err ) );
		}
		roisV  = std::vector<PicamRoi>( regions->roi_array, regions->roi_array + regions->roi_count );
	}
	return roisV;
}


const PicamRoisConstraint* PicamFlume::getCameraRoiConstraints ( )
{
	const PicamRoisConstraint* constraint = NULL;
	if ( !safemode )
	{
		auto err = Picam_GetParameterRoisConstraint ( camera, PicamParameter_Rois, PicamConstraintCategory_Required,
													  &constraint );
		if ( err != PicamError_None )
		{
			thrower ( getErrMsg ( err ) );
		}
	}
	return constraint;
}



void PicamFlume::setRois ( std::vector<PicamRoi> regions )
{
	PicamRois rois;
	rois.roi_array = regions.data ( );
	rois.roi_count = regions.size ( );
	if ( !safemode )
	{
		auto err = Picam_SetParameterRoisValue ( camera, PicamParameter_Rois, &rois );
		if ( err != PicamError_None )
		{
			thrower ( getErrMsg ( err ) );
		}
	}
}


void PicamFlume::commitParams ( )
{
	if ( !safemode )
	{
		const PicamParameter* failedParamArray;
		int failureCount = 0;
		PicamError err = Picam_CommitParameters ( camera, &failedParamArray, &failureCount );
		if ( err != PicamError_None )
		{
			thrower ( getErrMsg ( err ) );
		}
	}
}


void PicamFlume::SetParameterIntegerValue ( PicamParameter param, int val )
{
	if ( !safemode )
	{
		auto err = Picam_SetParameterIntegerValue ( camera, param, val );
		if ( err != PicamError_None )
		{
			thrower ( getErrMsg ( err ) );
		}
	}
}


void PicamFlume::turnOffTrigger ( )
{
	Picam_SetParameterIntegerValue ( camera, PicamParameter_TriggerResponse, PicamTriggerResponse_NoResponse );
}


void PicamFlume::setStandardTrigger ( )
{
	Picam_SetParameterIntegerValue ( camera, PicamParameter_TriggerResponse, PicamTriggerResponse_ReadoutPerTrigger );
	Picam_SetParameterIntegerValue ( camera, PicamParameter_TriggerDetermination, PicamTriggerDetermination_NegativePolarity );
}


double PicamFlume::GetParameterFloatingPointValue ( PicamParameter param )
{
	piflt val;
	if ( !safemode )
	{
		auto err = Picam_GetParameterFloatingPointValue ( camera, param, &val );
		if ( err != PicamError_None )
		{
			thrower ( getErrMsg ( err ) );
		}
	}
	return val;
}


double PicamFlume::getExposure ( )
{
	return GetParameterFloatingPointValue ( PicamParameter_ExposureTime );
}


void PicamFlume::SetParameterFloatingPointValue ( PicamParameter param, double val )
{
	if ( !safemode )
	{
		auto err = Picam_SetParameterFloatingPointValue ( camera, param, val );
		if ( err != PicamError_None )
		{
			thrower ( getErrMsg ( err ) );
		}
	}
}


void PicamFlume::setExposure ( double time_Miliseconds )
{
	SetParameterFloatingPointValue ( PicamParameter_ExposureTime, time_Miliseconds );
}


std::string PicamFlume::getErrMsg ( PicamError err )
{
	switch ( err )
	{
		//* Success ---------------------------------------------------------------*/
		case PicamError_None://= 0,
			return "";
			//* General Errors --------------------------------------------------------*/
		case PicamError_UnexpectedError:// = 4,
			return "UnexpectedError";
		case PicamError_UnexpectedNullPointer:// = 3,
			return "UnexpectedNullPointer";
		case PicamError_InvalidPointer:// = 35,
			return "InvalidPointer";
		case PicamError_InvalidCount:// = 39,
			return "InvalidCount";
		case PicamError_EnumerationValueNotDefined://= 17,
			return "EnumerationValueNotDefined";
		case PicamError_InvalidOperation:// = 42,
			return "InvalidOperation";
		case PicamError_OperationCanceled:// = 43,
			return "OperationCanceled";
			//* Library Initialization Errors -----------------------------------------*/
		case PicamError_LibraryNotInitialized:// = 1,
			return "LibraryNotInitialized";
		case PicamError_LibraryAlreadyInitialized:// = 5,
			return "LibraryAlreadyInitialized";
			//* General String Handling Errors ----------------------------------------*/
		case PicamError_InvalidEnumeratedType:// = 16,
			return "InvalidEnumeratedType";
			//* Camera/Accessory Plug 'n Play Discovery Errors ------------------------*/
		case PicamError_NotDiscoveringCameras:// = 18,
			return "NotDiscoveringCameras";
		case PicamError_AlreadyDiscoveringCameras://= 19,
			return "AlreadyDiscoveringCameras";
		case PicamError_NotDiscoveringAccessories:// = 48,
			return "NotDiscoveringAccessories";
		case PicamError_AlreadyDiscoveringAccessories:// = 49,
			return "AlreadyDiscoveringAccessories";
			//* Camera/Accessory Access Errors ----------------------------------------*/
		case PicamError_NoCamerasAvailable:// = 34,
			return "NoCamerasAvailable";
		case PicamError_CameraAlreadyOpened:// = 7,
			return "CameraAlreadyOpened";
		case PicamError_InvalidCameraID:// = 8,
			return "InvalidCameraID";
		case PicamError_NoAccessoriesAvailable:// = 45,
			return "NoAccessoriesAvailable";
		case PicamError_AccessoryAlreadyOpened:// = 46,
			return "AccessoryAlreadyOpened";
		case PicamError_InvalidAccessoryID:// = 47,
			return "InvalidAccessoryID";
		case PicamError_InvalidHandle:// = 9,
			return "InvalidHandle";
		case PicamError_DeviceCommunicationFailed:// = 15,
			return "DeviceCommunicationFailed";
		case PicamError_DeviceDisconnected:// = 23,
			return "DeviceDisconnected";
		case PicamError_DeviceOpenElsewhere:// = 24,
			return "DeviceOpenElsewhere";
			//* Demo Camera Errors ----------------------------------------------------*/
		case PicamError_InvalidDemoModel:// = 6,
			return "InvalidDemoModel";
		case PicamError_InvalidDemoSerialNumber:// = 21,
			return "InvalidDemoSerialNumber";
		case PicamError_DemoAlreadyConnected:// = 22,
			return "DemoAlreadyConnected";
		case PicamError_DemoNotSupported://= 40,
			return "DemoNotSupported";
			//* Camera/Accessory Parameter Access Errors ------------------------------*/
		case PicamError_ParameterHasInvalidValueType:// = 11,
			return "ParameterHasInvalidValueType";
		case PicamError_ParameterHasInvalidConstraintType:// = 13,
			return "ParameterHasInvalidConstraintType";
		case PicamError_ParameterDoesNotExist:// = 12,
			return "ParameterDoesNotExist";
		case PicamError_ParameterValueIsReadOnly:// = 10,
			return "ParameterValueIsReadOnly";
		case PicamError_InvalidParameterValue:// = 2,
			return "InvalidParameterValue";
		case PicamError_InvalidConstraintCategory:// = 38,
			return "InvalidConstraintCategory";
		case PicamError_ParameterValueIsIrrelevant:// = 14,
			return "ParameterValueIsIrrelevant";
		case PicamError_ParameterIsNotOnlineable:// = 25,
			return "ParameterIsNotOnlinable";
		case PicamError_ParameterIsNotReadable:// = 26,
			return "ParameterIsNotReadable";
		case PicamError_ParameterIsNotWaitableStatus:// = 50,
			return "ParameterIsNotWaitableStatus";
		case PicamError_InvalidWaitableStatusParameterTimeOut:// = 51,
			return "InvalidWaitableStatusParameterTimeOut";
			//* Camera Data Acquisition Errors ----------------------------------------*/			
		case PicamError_InvalidParameterValues:// = 28,
			return "InvalidParameterValues";
		case PicamError_ParametersNotCommitted:// = 29,
			return "ParametersNotCommitted";
		case PicamError_InvalidAcquisitionBuffer:// = 30,
			return "InvalidAcquisitionBuffer";
		case PicamError_InvalidReadoutCount:// = 36,
			return "InvalidReadoutCount";
		case PicamError_InvalidReadoutTimeOut:// = 37,
			return "InvalidReadoutTimeOut";
		case PicamError_InsufficientMemory:// = 31,
			return "InsufficientMemory";
		case PicamError_AcquisitionInProgress:// = 20,
			return "AcquisitionInProgress";
		case PicamError_AcquisitionNotInProgress:// = 27,
			return "AcquisitionNotInProgress";
		case PicamError_TimeOutOccurred:// = 32,
			return "TimeOutOccurred";
		case PicamError_AcquisitionUpdatedHandlerRegistered:// = 33,
			return "AcquisitionUpdatedHandlerRegistered";
		case PicamError_InvalidAcquisitionState:// = 44,
			return "InvalidAcquisitionState";
		case PicamError_NondestructiveReadoutEnabled:// = 41,
			return "NondestructiveReadoutEnabled";
		case PicamError_ShutterOverheated:// = 52,
			return "ShutterOverheated";
		case PicamError_CenterWavelengthFaulted:// = 54,
			return "CenterWavelengthFaulted";
		case PicamError_CameraFaulted:// = 53			
			return "CameraFaulted";
	}
	return "Error not Recognized!!!";
}
