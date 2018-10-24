/*********************************************************************************************/
// Name: StatLearner.cpp 
//
// Class for Calculating statistical prediction using normal calculation
// and integrated HMM posterior probabilities for "gang" and "No gang" states.
/*********************************************************************************************/
#include "stdafx.h"
#include "StatLearner.h"

//! A constructor.
/*
\area int - comunity area to calculate statistic for.
\iusr string - crime IUSR to calculate statistic for.
\counting and setting "years" as the number of years of information
*/
StatLearner::StatLearner(int area, string iusr)
{
	string path = "fsrc/crimesPerDayArea" + convertInt(area) + "IUSR" + iusr + ".csv";
	std::ifstream ifile(path);
	string line = "";
	int counter = 0;
	std::getline(ifile, line);
	//count number of days
	while (std::getline(ifile, line))
	{
		counter++;
	}
	ifile.close();
	//calculate number of years
	years = counter/365;
}

//! A distractor.
StatLearner::~StatLearner()
{
}

//! Calculating statistical prediction using normal calculation
// and integrated HMM posterior probabilities for "gang" and "No gang" states.
/*
\startingYear integer - the years of the first crime. 
\area int - comunity area to calculate statistic for.
\iusr string - crime IUSR to calculate statistic for.
\crimeResolution integer - Resolution of crimes per day.
\returns - a vector of size 3 containing nomal statistics statistic considering gang and
\statistic whitount gang.
*/
vector<vector<Month>> StatLearner::createByMounthStatistic(int startingYear, int area , string iusr,int crimeResolution)
{
	vector<vector<Month>> yearCrimeData;
	vector<Month> yearStatistic;
	vector<Month> withGangStat;
	vector<Month> noGangStat;
	int year = startingYear - 2000; // set starting year

	/*initialization of months in for all years to read data*/
	for (int j = 0; j < years; j++)
	{
		vector<Month> tempYear;

		for (int i = 0; i < 12; i++)
		{
			Month tempMonth;
			if ((i <= 6 && i % 2 == 0) || (i > 6 && i % 2 == 1))
			{
				tempMonth.numberOFdays = 31;
			}
			else if (i <= 6)
			{
				if (year % 4 == 0 && i == 1)
					tempMonth.numberOFdays = 29;
				else if (i == 1)
					tempMonth.numberOFdays = 28;
				else
					tempMonth.numberOFdays = 30;
			}
			tempYear.emplace_back(tempMonth);
		}
		yearCrimeData.emplace_back(tempYear);
		year++;
	}
	/*initialization of months in a year to 3 statistic tipes*/
	for (int i = 0; i < 12; i++)
	{
		Month temp1;
		Month temp2;
		Month temp3;
		if ((i <= 6 && i % 2 == 0) || (i > 6 && i % 2 == 1))
		{
			temp1.numberOFdays = 31;
			temp2.numberOFdays = 31;
			temp3.numberOFdays = 31;
		}
		else if (i <= 6)
		{
			if (i == 1)
			{
				temp1.numberOFdays = 29;
				temp2.numberOFdays = 29;
				temp3.numberOFdays = 29;
			}
			else
			{
				temp1.numberOFdays = 30;
				temp2.numberOFdays = 30;
				temp3.numberOFdays = 30;
			}
		}
		yearStatistic.emplace_back(temp1);
		withGangStat.emplace_back(temp2);
		noGangStat.emplace_back(temp3);
	}

	/*set input file of crimes per day*/
	string path = "fsrc/crimesPerDayArea" + convertInt(area) + "IUSR" + iusr + ".csv";
	std::ifstream ifile(path);
	string line = "";

	/*reading crime data per day*/
	std::getline(ifile, line); // skip header
	int _yearNum = 0;
	int month = 0;
	int day = 0;
	for (int j = 0; j < years; j++)
	{
		for (int i = 0; i < 12; i++)
		{
			while (day < yearCrimeData[j][i].numberOFdays)
			{
				if (std::getline(ifile, line, ','))
				{
					std::getline(ifile, line, ',');

					yearCrimeData[j][i].crimePerDay[day] = std::stod(line);

					std::getline(ifile, line, ','); // skip day of weak

					std::getline(ifile, line);
					yearCrimeData[j][i].gangProbability[day] = std::stod(line);
				}
				day++;
			}
			day = 0;
		}
	}

	/*calculate statistic for each month withount gang consideration*/
	double statCounter[12][31] = { 0 };
	for (int m = 0; m < 12; m++)
	{
		for (int y = 0; y < years; y++)
		{
			for (int d = 0; d < yearCrimeData[y][m].numberOFdays; d++)
			{
				/*add 2 days if posible*/
				if (d - 7 >= 0)
				{
					if (yearCrimeData[y][m].crimePerDay[d - 7] > crimeResolution)
						yearStatistic[m].crimePerDay[d] ++;
					statCounter[m][d]++;
				}
				if (d + 7 < yearCrimeData[y][m].numberOFdays)
				{
					if (yearCrimeData[y][m].crimePerDay[d + 7] > crimeResolution)
						yearStatistic[m].crimePerDay[d] ++;
					statCounter[m][d]++;
				}
				if (yearCrimeData[y][m].crimePerDay[d] > crimeResolution)
					yearStatistic[m].crimePerDay[d] ++;
				statCounter[m][d]++;

			}
		}

	}

	/*calculate statistic for each month with gang efect*/
	double gangCounter[12][31] = { 0 };
	double noGangCounter[12][31] = { 0 };

	for (int m = 0; m < 12; m++)
	{
		for (int y = 0; y < years; y++)
		{
			for (int d = 0; d < yearCrimeData[y][m].numberOFdays; d++)
			{
				/*add 2 days if posible*/
				if (d - 7 >= 0)
				{
					if (yearCrimeData[y][m].gangProbability[d - 7] > 0.5)
					{
						gangCounter[m][d]++;
						if (yearCrimeData[y][m].crimePerDay[d - 7] > crimeResolution)
							withGangStat[m].crimePerDay[d] ++;
					}
					else
					{
						noGangCounter[m][d]++;
						if (yearCrimeData[y][m].crimePerDay[d - 7] > crimeResolution)
							noGangStat[m].crimePerDay[d] ++;
					}
				}
				if (d + 7 < yearCrimeData[y][m].numberOFdays)
				{
					if (yearCrimeData[y][m].gangProbability[d + 7] > 0.5)
					{
						gangCounter[m][d]++;
						if (yearCrimeData[y][m].crimePerDay[d + 7] > crimeResolution)
							withGangStat[m].crimePerDay[d] ++;
					}
					else
					{
						noGangCounter[m][d]++;
						if (yearCrimeData[y][m].crimePerDay[d + 7] > crimeResolution)
							noGangStat[m].crimePerDay[d] ++;
					}
				}
				if (yearCrimeData[y][m].gangProbability[d] > 0.5)
				{
					gangCounter[m][d]++;
					if (yearCrimeData[y][m].crimePerDay[d] > crimeResolution)
						withGangStat[m].crimePerDay[d] ++;
				}
				else
				{
					noGangCounter[m][d]++;
					if (yearCrimeData[y][m].crimePerDay[d] > crimeResolution)
						noGangStat[m].crimePerDay[d] ++;
				}
			}
		}

	}

	/*calculat probability*/
	for (int m = 0; m < 12; m++)
		for (int d = 0; d < yearStatistic[m].numberOFdays; d++)
		{
			yearStatistic[m].crimePerDay[d] /= statCounter[m][d];
			withGangStat[m].crimePerDay[d] /= gangCounter[m][d];
			noGangStat[m].crimePerDay[d] /= noGangCounter[m][d];
		}

	vector<vector<Month>> result;
	result.emplace_back(yearStatistic);
	result.emplace_back(withGangStat);
	result.emplace_back(noGangStat);

	return result;
}

//! Converts Int to String.
/*
\input number - int to convert.
\return converted number to string.
*/
string StatLearner::convertInt(int number)
{
	std::stringstream ss;//create a stringstream
	ss << number;//add number to the stream
	return ss.str();//return a string with the contents of the stream
}