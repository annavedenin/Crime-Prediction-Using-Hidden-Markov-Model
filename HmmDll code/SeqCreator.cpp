/*********************************************************************************************/
// Name: SeqCreator.cpp
// 
// The Class responsible for creating learning sequances and crime per day file for learning.
/*********************************************************************************************/
#include "stdafx.h"
#include "SeqCreator.h"
#include <time.h>
#include <windows.h>

using namespace std;

//! A constructor.
SeqCreator::SeqCreator()
{
}
//! A distractor.
SeqCreator::~SeqCreator()
{
}

void SeqCreator::createLearningSequnce(int area, string iusr, int timeStep)
{
	//convert csv files to binary txt
	convertToSeq(area, iusr, timeStep, dataFrom);
}
//! Converts area crimes report to  binary sequance for each minuet of the period.
/*
\area integer - chicago comunity area number.
\IUSR String - iusr of the requested crime.
\timeStep - time represented by each bit.
\initialDate - date and time to start zero pading from
\output - sequance writen into "CommunityArea/ ***.txt"
*/
void SeqCreator::convertToSeq(int area, string iusr, int timeStep, string initialDate)
{
	CRIME_DATA fromCrime;
	CRIME_DATA untilCrime;
	
	int zeroIsnertNumber;
	//set input and output files
	string ifilePath = "CommunityArea/area";
	string ofilePath = "CommunityArea/area";
	ifilePath += convertInt(area) + ".csv";
	ofilePath += convertInt(area) + "IUSR" + iusr + ".txt";
	std::ifstream ifile(ifilePath);
	std::ofstream ofile(ofilePath);

	std::string line = "";
	//set initial date
	fromCrime.date = initialDate.substr(0, 10);
	fromCrime.time = initialDate.substr(11, 8);
	fromCrime.XM = initialDate.substr(20, 2);
	//read .csv file of comunity area 'area' and create binary txt file.
	while (std::getline(ifile, line))
	{
		if (line.substr(23, 27) == iusr)
		{
				untilCrime.date = line.substr(0, 10);
				untilCrime.time = line.substr(11, 8);
				untilCrime.XM = line.substr(20, 2);

				//count and build sequance

				if (fromCrime.date == untilCrime.date && fromCrime.time.substr(0, 5) == untilCrime.time.substr(0, 5) && fromCrime.XM == untilCrime.XM)
				{
					fromCrime = untilCrime;
				}
				else // fill 0 betwen two crimes
				{
					zeroIsnertNumber = findTimeDiffrence(fromCrime, untilCrime, timeStep);

					for (int i = 1; i < zeroIsnertNumber; i++)
						ofile << "0 ";
					ofile << "1 ";
					fromCrime = untilCrime;
				}
				

			
		}//if iusr
	}//while

	std::ifstream inDate("fsrc/setings.txt"); // update date
	std::getline(inDate, line);
	std::getline(inDate, line, ' ');
	std::getline(inDate, line);
	ofile.close();

	// get last learned date
	untilCrime.date = line;
	untilCrime.time = "00:00:00";
	untilCrime.XM = "AM";
	// fill 0 up to last learned date
	if (fromCrime.date == untilCrime.date && fromCrime.time.substr(0, 5) == untilCrime.time.substr(0, 5) && fromCrime.XM == untilCrime.XM)
	{
		fromCrime = untilCrime;
	}
	else
	{
		zeroIsnertNumber = findTimeDiffrence(fromCrime, untilCrime, timeStep);
		for (int i = 0; i < zeroIsnertNumber; i++)
			ofile << "0 ";
	}

}

// !Creates new representation of convertToSeq() output.
/*
\area integer - chicago comunity area number.
\IUSR String - iusr of the requested crime.
\timeStep - time represented by each bit in input file.
\maxSize - size in minutes to be devided by timeStep to represented by each bit.
\cr - the size of the resolution to be created
\return - vector of sequances splited by splitSeq to sizes of year"
*/
vector<Seq> SeqCreator::createSeqResolution(int area, string iusr, int timeStep, int maxSize, int cr)
{
	int seqMaxSize = maxSize / timeStep;
	int seqTempSize = 0;
	int crimeResolution = 0;
	int crimePoint = cr;
	string line;
	string seq = "";
	std::ifstream ifile("CommunityArea/area" + convertInt(area) + "IUSR" + iusr + ".txt");

	std::ofstream ofile("fsrc/tempSeq.txt");
	// read seqMaxSize of bits while posible
	while (std::getline(ifile, line, ' '))
	{
		//read 1 bit
		if (line.compare("1") == 0)
			crimeResolution++;
		//read seqMaxSize - 1 bits
		for (int i = 1; i < seqMaxSize && std::getline(ifile, line, ' '); i++)
		{
			if (line.compare("1") == 0)
				crimeResolution++;
		}
		//check if resolution were reached
		if (crimeResolution > crimePoint) 
			seq += convertInt(1) + " ";
		else
			seq += convertInt(0) + " ";

		crimeResolution = 0;
		seqTempSize++;

	}

	ofile << seq;
	ofile.close();
	return splitSeq(365); //return splited sequance vector
}

// !Splits seqance out of tempSeq.txt to smaller porshions.
/*
\area integer - chicago comunity area number.
\maxSize int - size of max sequnace size.
\return - vector of sequances splited to maxSize, last one may be smaller"
*/
vector<Seq> SeqCreator::splitSeq(int maxSize)
{
	int seqMaxSize = 0;
	int seqTempSize = 0;
	vector<Seq> seqVec;
	string line;
	string seq = "";
	int year = 3; //date of initial data year 2003 - User can't change.
	std::ifstream ifile("fsrc/tempSeq.txt");
	
	//read bit by bit out of tempSeq.txt
	while (std::getline(ifile, line, ' '))
	{
		std::ofstream ofile("fsrc/tempSeq2.txt");

		//insert first bit to temporal file.
		seqTempSize++;
		seq += line + " ";
		//cheack numbers of days in year.
		if (year % 4 == 0)
			seqMaxSize = 366;
		else
			seqMaxSize = 365;
		// continue reading until max size reached.
		for (int i = 1; i < seqMaxSize && std::getline(ifile, line, ' '); i++)
		{
			seqTempSize++;
			seq += line + " ";
		}
		// prepare out input file format for sequance constractor.
		ofile << "T= " << seqTempSize << std::endl;
		ofile << seq;
		ofile.close();
		//create new sequance
		Seq t("fsrc/tempSeq2.txt");
		seqVec.emplace_back(t); // add sequance to vector.
		seqTempSize = 0;
		seq = "";
		year++;
	}
	return seqVec; //return vector of sequnaces.
}

//! Converts Int to String.
/*
\input number - int to convert.
\return converted number to string.
*/
string SeqCreator::convertInt(int number)
{
	std::stringstream ss;//create a stringstream
	ss << number;//add number to the stream
	return ss.str();//return a string with the contents of the stream
}

// !Calculates and return the number of minues divided by size betwen two crimes.
/*
\from  CRIME_DATA- data and time of first crime.
\to  CRIME_DATA- date and time of second crime.
\size integer - time represented by each bit.
\return integer- number of minues divided by size betwen two crimes"
*/
int SeqCreator::findTimeDiffrence(CRIME_DATA from, CRIME_DATA to, int size)
{
	int minut1, minut2;
	int hour1, hour2;
	int day1, day2;
	int month1, month2;
	int year1, year2;
	string XM1, XM2;
	int difrenceInMinut = 0;

	//set start time
	minut1 = std::stoi(from.time.substr(3, 5));
	hour1 = std::stoi(from.time.substr(0, 2));
	day1 = std::stoi(from.date.substr(3, 5));
	month1 = std::stoi(from.date.substr(0, 2));
	year1 = std::stoi(from.date.substr(6, 10));
	XM1 = from.XM;
	//set end time
	minut2 = std::stoi(to.time.substr(3, 5));
	hour2 = std::stoi(to.time.substr(0, 2));
	day2 = std::stoi(to.date.substr(3, 5));
	month2 = std::stoi(to.date.substr(0, 2));
	year2 = std::stoi(to.date.substr(6, 10));
	XM2 = to.XM;

	time_t timer;
	struct tm y1k = { 0 };
	struct tm y2k = { 0 };
	double seconds1;
	double seconds2;

	//convert time if needed
	if (XM1 == "PM")
		hour1 += 12;
	if (XM2 == "PM")
		hour2 += 12;
	
	//set first crime in struct
	y1k.tm_hour = hour1; y1k.tm_min = minut1; y1k.tm_sec = 0;
	y1k.tm_year = 100 + year1 - 2000; y1k.tm_mon = month1 - 1; y1k.tm_mday = day1;
	//set second crime in struct
	y2k.tm_hour = hour2;   y2k.tm_min = minut2; y2k.tm_sec = 0;
	y2k.tm_year = 100 + year2 - 2000; y2k.tm_mon = month2 - 1; y2k.tm_mday = day2;

	time(&timer);  /* get current time; same as: timer = time(NULL)  */
	// calculate time diffrance from two dates to now in seconds
	seconds1 = difftime(timer, mktime(&y1k));
	seconds2 = difftime(timer, mktime(&y2k));

	difrenceInMinut = (int)((seconds1 - seconds2) / 60.0); // calculate the time diffrance in minues

	return difrenceInMinut / size;
}

// !Creates csv file of number of crimes per day with posterior probabilities.
/*
\area integer - chicago comunity area number.
\IUSR String - iusr of the requested crime.
\timeStep integer- time represented by each bit in input file.
\maxSize integer- size in minutes to be devided by timeStep to represented by each bit.
\posterior vector<double> - vector of posterior probabilities for each day"
*/
void SeqCreator::createCrimesPerDay(int area, string iusr, int timeStep, int maxSize, vector<double> posterior)
{
	int day = 1;
	int _numberOfCrimes = 0;
	string line;
	int number_of_days = 1; //the number of days we count crimes for
	int _dayOfWeak = 4;
	//set input and out put files
	std::ifstream ifile("CommunityArea/area" + convertInt(area) + "IUSR" + iusr + ".txt");

	string path = "fsrc/crimesPerDayArea"+ convertInt(area) +"IUSR"+ iusr +".csv";
	std::ofstream ofile(path);
	ofile << "day,crimes,dat of weak, Gang Posability" << endl; //write header
	
	//read number of 1 in defined amount of bits
	while (std::getline(ifile, line, ' '))
	{
		if (line.compare("1") == 0)
			_numberOfCrimes++;
		//read up to 1440 minutes that equal to number of minuets in 1 day
		for (int i = 1; i < number_of_days*1440 && std::getline(ifile, line, ' '); i++)
		{
			if (line.compare("1") == 0)
				_numberOfCrimes++;
		}
		//write new line to file
		ofile << day <<","<< _numberOfCrimes << "," << _dayOfWeak << "," << posterior[day-1] <<endl;
		
		_numberOfCrimes = 0;
		day++;
		//update day of weak
		_dayOfWeak++;
		if (_dayOfWeak == 8)
			_dayOfWeak = 1;

	}
	
	ofile.close();
}
