#pragma once
#include "stdafx.h"
#include <fstream>
#include "commonTypes.h"

class PictureSaver
{
	public:
		void initialize( POINT& pos, int& id, CWnd* parent );
		void save(std::vector<long>* pic, int width);
		void save( std::vector<std::vector<long>> pics, int width );
		void append( std::vector<long>* pic, int width );
		void close();
		void rearrange(int width, int height, fontMap fonts);
		void handleModeChange(std::string cameraMode);
	private:
		std::ofstream file;
		Control<CStatic> saveLocationText;
		Control<CEdit> saveLocationEdit;
		Control<CButton> saveCheckButton;
		Control<CStatic> fileNumberText;
		Control<CEdit> fileNumberEdit;
};
