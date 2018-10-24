/*********************************************************************************************/
// Name: seq.h
// 
// header of the learning sequance structur
// T - size of sequance
// O - pointer to sequance
/*********************************************************************************************/
#pragma once

#include<fstream>
#include<string>
using namespace std;
using std::fstream;

class Seq
{
public:
	Seq(string file);
	~Seq();
	void print();
	int T;
	int* O;
};