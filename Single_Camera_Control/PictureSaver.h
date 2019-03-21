#pragma once
// Created by Mark Brown
#include "stdafx.h"
#include "Matrix.h"
#include "commonTypes.h"
#include <fstream>



class PictureSaver
{
	public:
		ULONG getNextFileIndex( std::string fileBase, std::string ext );
		void initialize( POINT& pos, int& id, CWnd* parent );
		void save(const Matrix<long>& pic, int width);
		//void save( const std::vector<Matrix<long>>& pics, int width );
		void append( const Matrix<long>& pic, int width );
		void close();
		void rearrange(int width, int height, fontMap fonts);
		void handleModeChange(std::string cameraMode);
	private:
		std::ofstream file;
		Control<CStatic> saveLocationText;
		Control<CEdit> saveLocationEdit;
		Control<CButton> saveCheckButton;
		Control<CStatic> fileNumberText;
		Control<CStatic> fileNumberDisp;
};
