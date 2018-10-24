/*********************************************************************************************/
// Name: seq.cpp
// 
// class of the learning sequance structur
// T - size of sequance
// O - pointer to sequance
/*********************************************************************************************/
#include "stdafx.h"
#include"seq.h"
#include<string>
#include<iostream>

using std::cout;
using std::endl;
using std::string;

//! A constructor.
/*
\input fileName - path to input file.
\loading parameters from input file.
*/
Seq::Seq(string fileName)
{
	fstream f(fileName);
	string skip;
	f >> skip >> T;	// skip = "T= "
	O = new int[T];
	for (int i = 0; i < T; ++i)
	{
		f >> O[i];
	}
	f.close();
}
//! A distractor.
Seq::~Seq()
{
}
//! printing sequance to screen.
void Seq::print()
{
	cout << "T= " << T << endl;
	for (int i = 0; i < T; ++i)
	{
		cout << O[i] << " ";
	}
	cout << endl;
}