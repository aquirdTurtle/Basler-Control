#pragma once
#include "picam.h"
#include "PicamFlume.h"
#include "CameraSettingsControl.h"
#include <atomic>

struct pictureWatcherInput
{
	CWnd* parent;
	PicamFlume* flume;
	std::atomic<bool>* running;
	std::atomic<UINT>* repCounts;
};

class PixisCamera
{
	public:
		PixisCamera ( bool safemode );
		~PixisCamera ( );
		void initialize ( CWnd* parent );
		POINT getCameraDimensions ( );
		double getSetTemperature ( );
		void setTemperatureSetPoint ( double temperature );
		double getCurrentTemperature ( );
		static UINT __stdcall pictureWatcherProcedure ( void* inputPtr );
		std::string getTemperatureStatus ( );
		CameraSettings getDefaultSettings ( );
		void setParameters ( CameraSettings settings );
		void armCamera ( );
		void disarm ( );
		double getExposure ( );
		UINT getRepCounts ( );
	private:
		PicamFlume flume;
		//PicamHandle camera;
		PicamCameraID camID;
		CWnd* parentWindow;
		std::atomic<UINT> repCounts = 0;
		std::atomic<bool> runningFlag;
};
