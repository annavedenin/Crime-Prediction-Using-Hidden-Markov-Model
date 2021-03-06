/*********************************************************************************************/
// Name: HmmDll.cpp : Defines the exported functions for the DLL application.
//
/*********************************************************************************************/

#include "stdafx.h"
#include "CSVReader.h"
#include "HmmDll.h"
#include "SeqCreator.h"
#include "HmmMachine.h"
#include "StatLearner.h"
#include <vector>
//! load file of crimes
/*
\ return 1 if succseded and 0 otherwise.
*/
int startLoad()
{
	CSVReader csv(true);

	if (csv.loadAndSort())
		return 1;
	else
		return 0;
}

//! creates learning sequance for spesific comunity area and IUSR
/*
\ area integer - requasted comunity area.
\ str char* - iusr of 4 charectars.
*/
void createSeq(int area, const char* str)
{
	string iusr = "";
	for (int i = 0; i < 4; i++)
		iusr += str[i];
	SeqCreator sc;
	sc.createLearningSequnce(area, iusr, 1);
}
//! perform learning and return prediction by HMM emission and transmision matrixes
/*
\ area integer - requasted comunity area.
\ str char* - iusr of 4 charectars.
\ crimeResulution integer - Resolution of crimes per day ( 5 if by default if not resived )
\ returns double - prediction by HMM matrix's
*/
double getPrediction(int area, const char* str, int crimeResulution = 5)
{
	double prediction;
	int t = 1;
	int m = 1440;
	string iusr = "";
	for (int i = 0; i < 4; i++)
		iusr += str[i];
	prediction = startLearnig(area, iusr, t, m, crimeResulution);
	return prediction;
}

//! perform statistical prediction and returns tomorow prediction by mode 
/*
\ startingYear integer - year of first crime.
\ area integer - requasted comunity area.
\ str char* - iusr of 4 charectars.
\ crimeResulution integer - Resolution of crimes per day ( 5 if by default if not resived )
\ mode integer - number of mode from 0 to 2
\ str2 char* - the today's date.
\ returns statistical prediction if mode = 0.
\ returns improved prediction if mode = 1.
\ returns gang probability if mode = 2.
*/
double getStatPrediction(int startingYear, int area, const char* str, int crimeResolution, int mode, const char* str2)
{
	double b;
	int t = 1;
	int m = 1440;
	string iusr = "";
	string date = "";
	//convert char* to strings
	for (int i = 0; i < 4; i++)
		iusr += str[i];
	for (int i = 0; i < 10; i++)
		date += str2[i];

	//perform statistical learning
	StatLearner st(area, iusr);
	vector<vector<Month>> stat = st.createByMounthStatistic(startingYear, area, iusr, crimeResolution);

	// get states probability from final model
	string line = "";
	std::ifstream prob("fsrc/finalModel.hmm");
	for (int i = 0; i < 9; i++)
	{
		std::getline(prob, line);
	}
	double p0,p1;
	std::getline(prob, line, ' ');
	p0 = std::stod(line);
	std::getline(prob, line);
	p1 = std::stod(line);
	
	//convert today's date
	int day = std::stoi(date.substr(3, 2)) - 1;
	int month = std::stoi(date.substr(0, 2)) - 1;
	int year = std::stoi(date.substr(6, 4)) - 2000;

	//find tha date of tomorrow
	day++;// increse day
	if ((month <= 6 && month % 2 == 0) || (month > 6 && month % 2 == 1)) 
	{
		if (day == 31) // cheack month
		{
			day = 0;
			month++;
			if (month == 12)
				month = 0;
		}
	}
	else
	{
		if (year % 4 == 0 && month == 1) // cheack year
			if (day == 29)
			{
				day = 0;
				month++;
			}
		else if (month == 1)
			if (day == 28)
			{
				day = 0;
				month++;
			}
		else
			if (day == 30)
			{
				day = 0;
				month++;
			}
	}

	switch (mode)
	{
	case 0:
		return  stat[mode][month].crimePerDay[day]; // return statistical prediction
		break;
	case 1:
		return  p1*stat[1][month].crimePerDay[day] + p0*stat[2][month].crimePerDay[day]; // return improved prediction
		break;
	case 2:
		return  p1; // return gang probability
		break;
	default:
		return -1.0;
	}
}

//! cheack if program loaded data before
/*
\return 1 if date was once loded
\return 0 if date never was loaded
*/
int checkProgrameState()
{
	CSVReader csv(false);
	return csv.checkstatus();
}

//! write uploadDataDate from setings file to myString
/*
\myString char* - string to return to caller.
\length integer - maxsize of myString.
*/
void getLastDate(char* myString, int length)
{
	CSVReader csv(false);
	string toReturn;
	toReturn = csv.checkDate();

	char * writable = new char[toReturn.size() + 1];
	std::copy(toReturn.begin(), toReturn.end(), writable);
	writable[toReturn.size()] = '\0'; //terminating 

	strcpy_s(myString, length, writable);
}

