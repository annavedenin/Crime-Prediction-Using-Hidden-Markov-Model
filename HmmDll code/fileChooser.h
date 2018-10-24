/*********************************************************************************************/
// Name: fileChooser.h
// 
// The claas open a windows filechooser and return path to choosen file
//
/*********************************************************************************************/
#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <iostream>
#include <string>
#include <Commdlg.h>

using namespace std;

class fileChooser
{
	OPENFILENAME ofn;
	char filename[MAX_PATH];

public:
	fileChooser();
	~fileChooser();
	string startFileChooser();
};
