#include "stdafx.h"
#include "PictureSaver.h"
#include "constants.h"
#include <ctime>

void PictureSaver::rearrange(int width, int height, fontMap fonts)
{
	saveLocationText.rearrange("", "", width, height, fonts);
	saveLocationEdit.rearrange("", "", width, height, fonts);
	saveCheckButton.rearrange("", "", width, height, fonts);
	fileNumberText.rearrange("", "", width, height, fonts);
	fileNumberEdit.rearrange("", "", width, height, fonts);
}
		

void PictureSaver::initialize( POINT& pos, int& id, CWnd* parent )
{
	/// note: the rest of the save location is hard-set in the constants file. might want to change this.
	saveLocationText.ID = id++;
	saveLocationText.sPos = { pos.x, pos.y, pos.x + 100, pos.y + 25 };
	saveLocationText.Create( "Save Date:", WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY , 
							 saveLocationText.sPos, parent, saveLocationText.ID );

	time_t t = time(0);   // get time now
	struct tm now;
	localtime_s(&now, &t);
	std::string dateText;
	if (now.tm_mon + 1 < 10)
	{
		dateText = str(now.tm_year + 1900 - 2000) + "0" + str(now.tm_mon + 1) + str(now.tm_mday);
	}
	else
	{
		dateText = str(now.tm_year + 1900 - 2000) + str(now.tm_mon + 1) + str(now.tm_mday);
	}

	saveLocationEdit.ID = id++;
	saveLocationEdit.sPos = { pos.x + 100, pos.y, pos.x + 300, pos.y += 25 };
	saveLocationEdit.Create( WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, saveLocationEdit.sPos, parent, saveLocationEdit.ID );
	saveLocationEdit.SetWindowTextA( dateText.c_str() );

	saveCheckButton.ID = id++;
	saveCheckButton.sPos = { pos.x, pos.y, pos.x + 100, pos.y + 20 };
	saveCheckButton.Create( "Save?", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, saveCheckButton.sPos, parent, saveCheckButton.ID );
	saveCheckButton.EnableWindow(0);

	fileNumberText.ID = id++;
	fileNumberText.sPos = { pos.x + 100, pos.y, pos.x + 200, pos.y + 20 };
	fileNumberText.Create( "File #: ", WS_CHILD | WS_VISIBLE, fileNumberText.sPos, parent, fileNumberText.ID );

	fileNumberEdit.ID = id++;
	fileNumberEdit.sPos = { pos.x + 200, pos.y, pos.x + 300, pos.y + 20 };
	fileNumberEdit.Create( WS_CHILD | WS_VISIBLE, fileNumberEdit.sPos, parent, fileNumberEdit.ID );
	fileNumberEdit.SetWindowTextA( "0" );
}

void PictureSaver::append( std::vector<long>* pic, int width )
{
	// check if it's actually supposed to save.
	if (!saveCheckButton.GetCheck())
	{
		return;
	}
	CString text;
	saveLocationEdit.GetWindowTextA( text );
	std::string address = DATA_SAVE_LOCATION + std::string( text ) + DATA_SAVE_LOCATION2;
	if (address == "")
	{
		thrower( "ERROR: Please enter an address for saved pictures." );
	}
	int fileNumber;
	fileNumberEdit.GetWindowTextA( text );
	try
	{
		fileNumber = std::stoi( std::string( text ) );
	}
	catch (std::invalid_argument& err)
	{
		thrower( "ERROR: Please enter a valid number in the file number edit box!" );
	}

	// file should already be open.
	if (!file.is_open())
	{
		thrower( "ERROR! Save file failed to open! Full address was: " + address + "_" + str( fileNumber ) + ".txt" );
	}
	file << ";\n";
	int count = 0;
	for (auto elem : (*pic))
	{
		file << elem;
		count++;
		if (count % width == 0)
		{
			file << "\n";
		}
		else
		{
			file << " ";
		}
	}
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
	// seperate out year month and day.
	int year = std::stoi(cstring(text.Mid(0, 2))) + 2000;
	int month = std::stoi(cstring(text.Mid(2, 2)));
	int day = std::stoi(cstring(text.Mid(4, 2)));
	// figure out the month
	std::string monthName;
	switch (month)
	{
		case 1:
			monthName = "January";
			break;
		case 2:
			monthName = "February";
			break;
		case 3:
			monthName = "March";
			break;
		case 4:
			monthName = "April";
			break;
		case 5:
			monthName = "May";
			break;
		case 6:
			monthName = "June";
			break;
		case 7:
			monthName = "July";
			break;
		case 8:
			monthName = "August";
			break;
		case 9:
			monthName = "September";
			break;
		case 10:
			monthName = "October";
			break;
		case 11:
			monthName = "November";
			break;
		case 12:
			monthName = "December";
			break;
	}

	std::string address = DATA_SAVE_LOCATION + str(year) + "\\" + monthName +"\\" + monthName + " " + str(day) + "\\"  + DATA_SAVE_LOCATION2;
	if (address == "")
	{
		thrower( "ERROR: Please enter an address for saved pictures." );
	}

	/*
	
	*/

	int fileNumber;
	fileNumberEdit.GetWindowTextA( text );
	try
	{
		fileNumber = std::stoi( std::string( text ) );
	}
	catch (std::invalid_argument& err)
	{
		thrower( "ERROR: Please enter a valid number in the file number edit box!" );
	}


	if (file.is_open())
	{
		file.close();
	}
	
	file.open( address + "_" + str(fileNumber) + ".txt");
	if (!file.is_open())
	{
		thrower( "ERROR! Save file failed to open! Full address was: " + address + "_" + str( fileNumber ) + ".txt" );
	}
	int count = 0;
	for (auto elem : (*pic))
	{
		file << elem << " ";
		count++;
		if (count % width == 0)
		{
			file << "\n";
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


void PictureSaver::handleModeChange(std::string cameraMode)
{
	if (cameraMode == "Finite Acquisition")
	{
		saveCheckButton.EnableWindow(1);
	}
	else
	{
		saveCheckButton.EnableWindow(0);
	}

}


void PictureSaver::close()
{
	if (file.is_open())
	{
		file.close();
	}
}