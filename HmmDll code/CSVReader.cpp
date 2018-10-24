/*********************************************************************************************/
// Name: CSVReader.cpp
// 
// The Class responsible for reading crime reports in .csv format, sorthing and spliting them 
// to comunity areas.
/*********************************************************************************************/
#pragma once
#include "stdafx.h"
#include "CSVReader.h"

using namespace std;

//! A constructor.
/*
\input loading - identify if file chooser needed.
\seting fileName to chossen file path.
*/
CSVReader::CSVReader(bool loading)
{
	if (loading)
	{
		const int bufferSize = MAX_PATH;
		char oldDir[bufferSize]; // store the current directory

		// get the current directory, and store it
		if (!GetCurrentDirectory(bufferSize, (LPWSTR)oldDir))
			std::cerr << "Error getting current directory: #" << GetLastError();

		// select .csv file
		fileChooser f;
		fileName = f.startFileChooser();
		f.~fileChooser();

		//restor old directory of project
		if (!SetCurrentDirectory((LPCWSTR)oldDir))
			std::cerr << "Error resetting current directory: #" << GetLastError();
	}
}

//! A distractor.
CSVReader::~CSVReader()
{
}

//! Read crime reports file in fileName path and create CRIME_DATA vector .
/*
\return a vector of CRIME_DATA from fileName file.
*/
std::vector<CRIME_DATA> CSVReader::getData()
{
	std::ifstream file(fileName);

	std::vector<CRIME_DATA> dataList;
	int number_of_lines = 0;
	std::string line = "";

	CRIME_DATA temp_data;

	//count line number and reserve vector size in memory.
	std::getline(file, line);// read header
	while (std::getline(file, line))
		number_of_lines++;
	dataList.reserve(number_of_lines);

	//return to begining of file.
	file.clear();
	file.seekg(0, ios::beg);
	std::getline(file, line);// read header.

	//read line and create CRIME_DATA object for vector datalist.
	while ( std::getline(file, line) )
	{
		temp_data.date = line.substr(0,10);
		temp_data.time = line.substr(11,8);
		temp_data.XM = line.substr(20,2);
		temp_data.iusr = line.substr(23,4);

		if (line.size() == 35)
		{
			temp_data.comunityArea = std::stoi(line.substr(28,2));
			temp_data.year = std::stoi(line.substr(31,4));
		}
		else
		{
			temp_data.comunityArea = std::stoi(line.substr(28,1));
			temp_data.year = std::stoi(line.substr(30,4));
		}

		if (temp_data.time.substr(0, 2).compare("12") == 0)
			temp_data.time = "00" + temp_data.time.substr(2, 8);

		dataList.emplace_back(temp_data);

	}
	// Close the File
	file.close();

	return dataList;
}

//!sort vector of CRIME_DATA by date and time.
/*
\input data - vector of CRIME_DATA to sort by referance.
*/
void CSVReader::sortDataByDate(std::vector<CRIME_DATA>& data)
{
	std::sort(data.begin(), data.end());
}

//! Splits vector of CRIME_DATA to 77 .csv files .
/*
\input data - sorted vector of CRIME_DATA.
*/
void CSVReader::splitDataByArea(std::vector<CRIME_DATA> data)
{
	// open 77 out put files to writing
	std::ofstream area[77];
	for (int i = 0; i < 77; i++)
	{
		std::string filePath = "CommunityArea/";
		filePath += "area" + convertInt(i+1);
		filePath += ".csv";
		area[i].open(filePath.c_str(), std::ios::app);
		if (!area[i])
			perror("Stream Failed to open because: ");
	}
	string upto = checkDate();

	//write vector data to suitable file by area
	for (auto i : data)
	{

		if (i.comunityArea > 0 && i.comunityArea <= 77)
			if (upto.substr(6, 4).compare(i.date.substr(6, 4)) < 0)
			{
				area[i.comunityArea - 1] << i.date << "," << i.time << " " << i.XM << "," << i.iusr << std::endl;
			}
			else if (upto.substr(6, 4).compare(i.date.substr(6, 4)) == 0)
			{
				if (upto.compare(i.date) < 0)
					area[i.comunityArea - 1] << i.date << "," << i.time << " " << i.XM << "," << i.iusr << std::endl;
			}
	}

	//close splited files
	for(int i=0;i<77;i++)
		area[i].close();
}

//! Converts Int to String.
/*
\input number - int to convert.
\return converted number to string.
*/
string CSVReader::convertInt(int number)
{
	std::stringstream ss;//create a stringstream
	ss << number;//add number to the stream
	return ss.str();//return a string with the contents of the stream
}

//! Loading, sorting and spliting data by areas.
/*
\return bool - true if sucseded , false if gile name is empty.
*/
bool CSVReader::loadAndSort()
{
	if (fileName != "")
	{
		//load data
		std::vector<CRIME_DATA> cData = this->getData();
		//sort data vector
		this->sortDataByDate(cData);
		// save last date
		CRIME_DATA lastCrime = cData.at(cData.size() - 1); 

		//split to 77 difrent files no date or year
		this->splitDataByArea(cData);
		cData.clear(); // relase vector data;

		//update setings.txt file
		std::ofstream ofile("fsrc/setings.txt"); // update date
		ofile << "#lerning: 1" << std::endl << "#uploadDataDate: " << lastCrime.date;
		ofile.close();
		return true;
	}
	return false;
}

//! Return data loading status from setings.
/*
\ 1- data was loaded previusly.
\ 0- 1- data never were loaded.
*/
int CSVReader::checkstatus()
{
	int status;
	string line = "";
	std::ifstream ifile("fsrc/setings.txt");
	std::getline(ifile, line,' '); //skip name
	std::getline(ifile, line);
	status = std::stoi(line);
	ifile.close();
	return status;
}

//! Return date of system last crime from setings.
/*
\ 1- data was loaded previusly.
\ 0- 1- data never were loaded.
*/
string CSVReader::checkDate()
{
	string line = "";
	std::ifstream ifile("fsrc/setings.txt");
	std::getline(ifile, line); //skip status
	std::getline(ifile, line,' '); // skip name
	std::getline(ifile, line);
	ifile.close();
	return line;
}


