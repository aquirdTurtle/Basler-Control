#pragma once
#include "picam.h"
#include "PicamFlume.h"
#include "CameraSettingsControl.h"

class PixisCamera
{
	public:
		PixisCamera ( bool safemode );
		~PixisCamera ( );
		void initialize ( CWnd* parent );
		POINT getCameraDimensions ( );
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
		UINT repCounts = 0;
};
