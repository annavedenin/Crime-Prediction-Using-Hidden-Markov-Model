/*********************************************************************************************/
// Name: CSVREader.h
// 
// The Class responsible for reading crime reports in .csv format, sorthing and spliting them 
// to comunity areas.
//
/*********************************************************************************************/

#pragma once
#include "fileChooser.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <String>
#include <algorithm>
#include <sstream>
#include <time.h>
#include <vector>

//structure of crime reports
struct CRIME_DATA {
	std::string date = "";
	std::string time = "";
	std::string XM = "";
	std::string iusr = "";
	int comunityArea;
	int year;
//overiding < comparison operator for sorting
	bool operator < (const CRIME_DATA& str) const
	{
		if (year < str.year)
			return true;
		else if (year == str.year)
		{
			if (date.compare(str.date) < 0)
				return true;
			else if (date.compare(str.date) == 0)
			{
				if (XM.compare(str.XM) < 0)
					return true;
				else if (XM.compare(str.XM) == 0)
				{
					if (time.compare(str.time) < 0)
						return true;
					else 
						return false;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	}
};

class CSVReader
{
	std::string fileName; // file path for getData().

public:

	CSVReader(bool);
	~CSVReader();
	bool loadAndSort();
	int checkstatus();
	string checkDate();
private:
	std::vector<CRIME_DATA> getData();
	void sortDataByDate(std::vector<CRIME_DATA>&);
	void splitDataByArea(std::vector<CRIME_DATA>);
	string convertInt(int number);
};
