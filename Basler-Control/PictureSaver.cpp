#include "stdafx.h"
#include "PictureSaver.h"
#include <fstream>

void PictureSaver::initialize( POINT& pos, int& id, CWnd* parent )
{
	saveLocationText.ID = id++;
	saveLocationText.sPos = { pos.x, pos.y, pos.x + 100, pos.y + 25 };
	saveLocationText.Create( "Save Location:", WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY, saveLocationText.sPos, parent,
							 saveLocationText.ID );
	saveLocationEdit.ID = id++;
	saveLocationEdit.sPos = { pos.x + 100, pos.y, pos.x + 300, pos.y += 25 };
	saveLocationEdit.Create( WS_CHILD | WS_VISIBLE, saveLocationEdit.sPos, parent, saveLocationEdit.ID );
	
	saveCheckButton.ID = id++;
	saveCheckButton.sPos = { pos.x, pos.y, pos.x + 100, pos.y + 20 };
	saveCheckButton.Create( "Save?", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, saveCheckButton.sPos, parent, saveCheckButton.ID );

	fileNumberText.ID = id++;
	fileNumberText.sPos = { pos.x + 100, pos.y, pos.x + 200, pos.y + 20 };
	fileNumberText.Create( "File #: ", WS_CHILD | WS_VISIBLE, fileNumberText.sPos, parent, fileNumberText.ID );

	fileNumberEdit.ID = id++;
	fileNumberEdit.sPos = { pos.x + 200, pos.y, pos.x + 300, pos.y + 20 };
	fileNumberEdit.Create( WS_CHILD | WS_VISIBLE, fileNumberEdit.sPos, parent, fileNumberEdit.ID );
	fileNumberEdit.SetWindowTextA( "0" );
}

// save a picture with width width.
void PictureSaver::save( std::vector<long>* pic, int width )
{
	// check if it's actually supposed to save.
	if (!saveCheckButton.GetCheck())
	{
		return;
	}
	CString text;
	saveLocationEdit.GetWindowTextA( text );
	std::string address = std::string( text );
	if (address == "")
	{
		thrower( "ERROR: Please enter an address for saved pictures." );
	}
	int fileNumber;
	text;
	fileNumberEdit.GetWindowTextA( text );
	try
	{
		fileNumber = std::stoi( std::string( text ) );
	}
	catch (std::invalid_argument& err)
	{
		thrower( "ERROR: Please enter a valid number in the file number edit box!" );
	}

	std::ofstream saveFile( address + "_" + str(fileNumber) + ".txt");
	if (!saveFile.is_open())
	{
		thrower( "ERROR! Save file failed to open! Full address was: " + address + "_" + str( fileNumber ) + ".txt" );
	}
	int count = 0;
	for (auto elem : (*pic))
	{
		saveFile << elem;
		count++;
		if (count % width == 0)
		{
			saveFile << "\n";
		}
	}
	fileNumber++;
	fileNumberEdit.SetWindowTextA( std::to_string( fileNumber ).c_str() );
}


// save an array of pictures. Each picture has width of width.
void PictureSaver::save( std::vector<std::vector<long>> pics, int width )
{
	// check if it's actually supposed to save.
	if (!saveCheckButton.GetCheck())
	{
		return;
	}

	CString text;
	saveLocationEdit.GetWindowTextA( text );
	std::string address = std::string( text );

	int fileNumber;
	text;
	fileNumberEdit.GetWindowTextA( text );
	try
	{
		fileNumber = std::stoi( std::string( text ) );
	}
	catch (std::invalid_argument& err)
	{
		thrower( "ERROR: Please enter a valid number in the file number edit box!" );
	}
	std::ofstream saveFile( address + "_" + str( fileNumber ) + ".txt" );
	if (!saveFile.is_open())
	{
		thrower( "ERROR! Save file failed to open! Full address was: " + address + "_" + str( fileNumber ) + ".txt" );
	}
	int count = 0;
	for (auto pic : pics)
	{
		for (auto elem : pic)
		{
			saveFile << elem;
			count++;
			if (count % width == 0)
			{
				saveFile << "\n";
			}
		}
		saveFile << "\n,\n";
	}
	fileNumber++;
	fileNumberEdit.SetWindowTextA( std::to_string( fileNumber ).c_str() );

}

