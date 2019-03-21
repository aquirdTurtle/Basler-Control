#pragma once
#include "picam.h"

class PicamFlume
{
	public:
		PicamFlume ( bool safemode_option );
		void setRepetitions(long long reps);
		void displayTrigParams();
		void startAcquisition();
		void InitializeLibrary ( );
		void UninitializeLibrary ( );
		void OpenFirstCamera ( );
		void unlockTemperature();
		PicamCameraID GetCameraID (  );
		PicamAvailableData acquisitionUpdate();
		void CloseCamera ( );
		std::string getTemperatureStatus ( );
		void ConnectAndOpenDemoCamera ( PicamCameraID& id );
		void setTemperatureSetPoint ( double temp );
		PicamAvailableData Aquire ( );
		PicamHandle getCamera ( );
		double getCurrentTemperature ( );
		void StopAquisition ( );
		double getExposure ( );
		int getReadoutStride ( );
		double getTemperatureSetPoint ( );
		void setExposure ( double time_Miliseconds );
		void turnOffTrigger ( );
		void setStandardTrigger ( );
		void setAmplifierSettings ( );
		void setReadoutSettings();
		void commitParams ( );
		std::vector<PicamRoi> getRois ( );
		void setRois ( std::vector<PicamRoi> regions );
		const PicamRoisConstraint* getCameraRoiConstraints ( );
	private:
		bool safemode;
		PicamHandle camera;
		std::string getErrMsg ( PicamError err );
		int GetParameterIntegerValue ( PicamParameter picam );
		void SetParameterIntegerValue ( PicamParameter picam, int val );
		void SetParameterLargeIntegerValue(PicamParameter picam, long long val);
		double GetParameterFloatingPointValue ( PicamParameter param );
		void SetParameterFloatingPointValue ( PicamParameter param, double val );
};