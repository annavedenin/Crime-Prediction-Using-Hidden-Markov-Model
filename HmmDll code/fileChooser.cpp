/*********************************************************************************************/
// Name: fileChooser.cpp
// 
// The claas open a windows filechooser and return path to choosen file
/*********************************************************************************************/
#include "stdafx.h"
#include "fileChooser.h"

//! A constructor.
/*
\opens window file chooser and save selected path in filename.
*/
fileChooser::fileChooser()
{
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = (LPCWSTR)"Csv Files\0*.csv\0Any File\0*.*\0";
	ofn.lpstrFile = (LPWSTR)filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = (LPCWSTR)"Please Select Crime Data CSV File";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA((LPOPENFILENAMEA)&ofn))
	{
		std::cout << "You chose the file \"" << filename << "\"\n";
	}

}

//! A distractor.
/*
*/
fileChooser::~fileChooser()
{
}

//! filename getter.
/*
\return file name string.
*/
std::string fileChooser::startFileChooser()
{

	return filename;
}