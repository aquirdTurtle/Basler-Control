#pragma once
#include "picam.h"

class PicamFlume
{
	public:
		PicamFlume ( bool safemode_option );
		
		void InitializeLibrary ( );
		void UninitializeLibrary ( );
		PicamHandle OpenFirstCamera ( );
		PicamCameraID GetCameraID (  );
		void CloseCamera ( );
		void ConnectAndOpenDemoCamera ( PicamCameraID& id );
		PicamAvailableData Aquire ( );
		PicamHandle getCamera ( );
		void StopAquisition ( );
		double getExposure ( );
		int getReadoutStride ( );
		void setExposure ( double time_Miliseconds );
		void turnOffTrigger ( );
		void setStandardTrigger ( );
		void commitParams ( );
		const PicamRois* getRois ( );
		void setRois ( const PicamRois* );
	private:
		bool safemode;
		PicamHandle camera;
		std::string getErrMsg ( PicamError err );
		int GetParameterIntegerValue ( PicamParameter picam );
		void SetParameterIntegerValue ( PicamParameter picam, int val );
		double GetParameterFloatingPointValue ( PicamParameter param );
		void SetParameterFloatingPointValue ( PicamParameter param, double val );
};