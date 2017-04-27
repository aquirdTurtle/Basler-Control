#pragma once
#include "stdafx.h"
#include <fstream>

class PictureSaver
{
	public:
		void initialize( POINT& pos, int& id, CWnd* parent );
		void save(std::vector<long>* pic, int width);
		void save( std::vector<std::vector<long>> pics, int width );
		void append( std::vector<long>* pic, int width );
		void close();
	private:
		std::ofstream file;
		Control<CStatic> saveLocationText;
		Control<CEdit> saveLocationEdit;
		Control<CButton> saveCheckButton;
		Control<CStatic> fileNumberText;
		Control<CEdit> fileNumberEdit;
};
