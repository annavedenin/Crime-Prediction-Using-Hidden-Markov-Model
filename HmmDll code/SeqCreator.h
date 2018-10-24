/*********************************************************************************************/
// Name: SeqCreator.cpp
// 
// The Class responsible for creating learning sequances and crime per day file for learning.
/*********************************************************************************************/
#pragma once
#include"seq.h"
#include "CSVReader.h"
#include <String>
#include <vector>
#include <iostream>
using namespace std;

class SeqCreator
{
	string dataFrom = "01/01/2003 00:00:00 AM"; // first uploade must be from these date

public:
	SeqCreator();
	~SeqCreator();
	void createLearningSequnce(int, string, int);
	vector<Seq> createSeqResolution(int, string, int, int, int);
	void createCrimesPerDay(int area, string iusr, int timeStep, int maxSize, vector<double>);
private:
	void convertToSeq(int, string, int, string);
	vector<Seq> splitSeq(int);
	string convertInt(int number);
	int findTimeDiffrence(CRIME_DATA, CRIME_DATA, int);
};

