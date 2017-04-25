#include "stdafx.h"
#include "miscellaneousCommonFunctions.h"
#include <Windows.h>
#include <string>

void errBox( std::string msg )
{
	MessageBox( 0, msg.c_str(), "ERROR!", MB_ICONERROR );
}

// for mfc edits
void appendText(std::string newText, CEdit& edit)
{
	// get the initial text length
	int nLength = edit.GetWindowTextLength();
	// put the selection at the end of text
	edit.SetSel(nLength, nLength);
	// replace the selection
	edit.ReplaceSel(newText.c_str());
	return;
}

// for mfc rich edits
void appendText(std::string newText, Control<CRichEditCtrl>& edit)
{
	// get the initial text length
	int nLength = edit.GetWindowTextLength();
	// put the selection at the end of text
	edit.SetSel(nLength, nLength);
	// replace the selection
	edit.ReplaceSel(newText.c_str());
	return;
}

std::string doubleToString( double number, long precision )
{
	std::stringstream stream;
	stream << std::fixed << std::setprecision( precision ) << number;
	return stream.str();
}