/*********************************************************************************************/
// Name: HmmMachine.h  
//
// perfoms most of the learning sequance, may be looked at as sort of main for
// most of the learning proces operashions.
/*********************************************************************************************/
#pragma once
#include "SeqCreator.h"
#include <String>
#include "seq.h"
#include "hmm.h"
#include "Posterior.h"
#include "forward.h"
#include "backward.h"
#include "viterbi.h"
#include "baumwelch.h"

using namespace std;

//! combines all the other clases and performs most of the learning proces. 
/*
\area integer - chicago comunity area number.
\IUSR String - iusr of the requested crime.
\timeStep - time represented by each bit in input file.
\maxSize - size in minutes to be devided by timeStep to represented by each bit.
\ crimeResulution integer - Resolution of crimes per day 
\returns double - prediction by HMM matrix's
*/
double startLearnig(int area, string iusr, int timeStep, int maxSize, int crimeResulution)
{
	//create seq resolution
	SeqCreator sc;
	vector<Seq> seq = sc.createSeqResolution(area, iusr, timeStep, maxSize, crimeResulution);

	// Load model file
	fstream hf("fsrc/initialModel.hmm");
	HMM h(hf);

	//perform loarning
	baumWelch(h, seq, 10000, 0.000000000000000001);

	//wrtie final model
	std::ofstream of("fsrc/finalModel.hmm");
	h.print(of);

	//calculate postirior probability matrix
	vector<double> post = posterior(h, seq, 1);
	//create crimes per day file for statistical learning
	sc.createCrimesPerDay(area, iusr, timeStep, maxSize, post); // prepare input for statistical learning

	int year = 2003; // learning always from the first year, must upload firt data from 2003
	//get HMM matrix's prediction
	double result = 100 * h.givePrediction();

	//write state history to file
	std::ofstream ofstates("fsrc/stateHistory.txt");
	ofstates << "Y - No Gang state at current day in year" << std::endl;
	ofstates << "X - Gang state at current day in year" << std::endl;

	//use veterbi algorithm for each year sequance
	for (int i = 0; i < seq.size(); i++)
	{
		ofstates << "year : " << year << std::endl;
		int *path = viterbi(h, seq[i]);

		for (int j = 0; j < seq[i].T; j++)
			if (*(path + j) == 0)
				ofstates << "Y";
			else
				ofstates << "X";
		ofstates << std::endl;
		year++;
	}
	return result;
}