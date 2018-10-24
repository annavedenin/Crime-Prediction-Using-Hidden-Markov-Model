/*********************************************************************************************/
// Name: StatLearner.cpp 
//
// Class for Calculating statistical prediction using normal calculation
// and integrated HMM posterior probabilities for "gang" and "No gang" states.
//
/*********************************************************************************************/
#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct Month
{
	int numberOFdays;
	double crimePerDay[31] = { 0 };
	double gangProbability[31] = { 0 };
};

class StatLearner
{
	int years = 0;
public:
	StatLearner(int,string);
	~StatLearner();
	vector<vector<Month>> createByMounthStatistic( int, int ,string,int); //return vector of 3*12
private:
	string convertInt(int number);

};

